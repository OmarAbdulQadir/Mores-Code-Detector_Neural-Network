# Full MCU Implementation & Env Setup

## 1. Environment Installation
1. **STM32CubeIDE:** Primary C/C++ development environment.
2. **X-CUBE-AI:** Install via `Help -> Manage Software Packs`. This tool converts the `.tflite` model into optimized C code (`network.c/.h`, `network_data*.c/.h`).

## 2. Firmware Implementation Logic

The firmware lives in `Core/Src/main.c`. A 50 ms timer (TIM11, 20 Hz) sets a `tick` flag; the main loop drains it and runs one sample-and-infer cycle.

### Data Structures
The model is **INT8** quantized, so all I/O buffers are signed bytes. The input is a 100-sample window; the output is 28 class scores.
```c
#define BUFFER_SIZE 100
#define BUTTON_PRESSED   127
#define BUTTON_RELEASED -128   // matches the released level used during training
#define CONFIDENCE_THRESHOLD 100

int8_t  input_buffer[BUFFER_SIZE];                // model input  (S8, 100 samples)
int8_t  output_buffer[AI_NETWORK_OUT_1_SIZE];     // model output (S8, 28 classes)

// Index 0 = IDLE/unknown, 1..26 = A..Z, 27 = SPACE
const char LOOKUP[] = "?ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

// AI runtime objects
ai_handle network = AI_HANDLE_NULL;
ai_buffer input;
ai_buffer output;
// Scratch buffer for intermediate activations (AI_NETWORK_DATA_ACTIVATIONS_SIZE = 8352 bytes)
static uint8_t activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE] __attribute__((aligned(4)));
```

### Model Initialization (in `main()`, USER CODE 2)
The caller supplies only the activation scratch buffer; weights are resolved automatically from flash (pass `NULL`). The pre-built I/O descriptors are fetched once so they carry the correct format/shape/size — only the `.data` pointers are set later, per inference.
```c
ai_handle act_addr[] = { activations };
ai_error ai_err = ai_network_create_and_init(&network, act_addr, NULL);
if (ai_err.type != AI_ERROR_NONE) {
    printf("AI init error: type=%d code=%d\r\n", (int)ai_err.type, (int)ai_err.code);
    Error_Handler();
}

input  = ai_network_inputs_get(network, NULL)[0];
output = ai_network_outputs_get(network, NULL)[0];

memset(input_buffer, BUTTON_RELEASED, BUFFER_SIZE);   // prime window as "released"
HAL_TIM_Base_Start_IT(&htim11);                       // start the 20 Hz sampler
```

### Function: shift_window()
Manages the sliding window. While filling, samples append at the head; once full, history slides one sample left (use `memmove`, not `memcpy` — the regions overlap) and the newest sample is appended at the end.
```c
void shift_window(void) {
    if (buffer_idx < BUFFER_SIZE - 1) {
        input_buffer[buffer_idx++] = btn_state;
    } else if (buffer_idx == BUFFER_SIZE - 1) {
        memmove(input_buffer, input_buffer + 1, BUFFER_SIZE - 1);
        input_buffer[buffer_idx] = btn_state;
    } else {
        memset(input_buffer, 0, BUFFER_SIZE);
        buffer_idx = 0;
    }
}
```

### Function: read_btn()
Samples the hardware state and maps it to the INT8 training range. The button is active-low (pressed pulls the pin low / `RESET`).
```c
void read_btn(void) {
    btn_state = HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin) == GPIO_PIN_SET
                ? BUTTON_RELEASED : BUTTON_PRESSED;
    shift_window();
    btn_state = BUTTON_NO_CHANGE;
}
```

### Function: ai_run_inference()
Interface between the application and the X-CUBE-AI engine. Runs only when the 5-second window is full; binds the data pointers, runs, and checks the batch count.
```c
void ai_run_inference(void) {
    if (network != AI_HANDLE_NULL && buffer_idx == BUFFER_SIZE - 1) {
        input.data  = AI_HANDLE_PTR(input_buffer);
        output.data = AI_HANDLE_PTR(output_buffer);

        ai_i32 n_batches = ai_network_run(network, &input, &output);
        if (n_batches != 1) {
            ai_error err = ai_network_get_error(network);
            printf("AI run error: type=%d code=%d\r\n", (int)err.type, (int)err.code);
            return;
        }
        process_result();
    }
}
```

### Function: process_result()
Extracts the predicted character (argmax over 28 INT8 scores) and outputs it over UART. On a confident, non-IDLE prediction it clears the window to prevent the same character being re-emitted on every subsequent tick.
```c
void process_result(void) {
    int8_t max_val = -128;
    int    max_idx = 0;
    for (int i = 0; i < AI_NETWORK_OUT_1_SIZE; i++) {
        if (output_buffer[i] > max_val) {
            max_val = output_buffer[i];
            max_idx = i;
        }
    }
    if (max_val > CONFIDENCE_THRESHOLD && max_idx != 0) {
        printf("Decoded Character: %c\r\n", LOOKUP[max_idx]);
        memset(input_buffer, BUTTON_RELEASED, BUFFER_SIZE);  // software reset
    }
}
```

## 3. printf over UART (retargeting)
`printf` is retargeted to **USART2 (PA2 TX)** at 115200 baud. `Core/Src/syscalls.c` provides the newlib `_write()` stub, which calls `__io_putchar()` per character; `main.c` implements `__io_putchar`:
```c
int __io_putchar(int ch) {
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
```
> Note: `_sbrk()` lives in `sysmem.c`, so `syscalls.c` must **not** redefine it. If your console is on USART1 (PA9), use `&huart1` instead.

## 4. Notes
- `CONFIDENCE_THRESHOLD` is on the INT8 output scale; tune it to the model's actual output quantization once live scores are observed.
- The TIM11 update interrupt (`TIM1_TRG_COM_TIM11_IRQHandler` -> `HAL_TIM_IRQHandler`) drives `HAL_TIM_PeriodElapsedCallback`, which sets the `tick` flag.
