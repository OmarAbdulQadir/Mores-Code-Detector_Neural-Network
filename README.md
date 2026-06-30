# Embedded TinyML Morse Code Decoder

This project implements an on-device Morse Code decoder using a 1D Convolutional Neural Network (CNN) optimized for the **STM32F401 (ARM Cortex-M4F)** microcontroller.

## 📚 Documentation Roadmap
For a deeper dive into specific aspects of this project, refer to:
- **[Model.md](Model-Docs/Model.md)**: Technical breakdown of the 1D-CNN architecture, training graphs, and rationale.
- **[tinyML.md](https://github.com/OmarAbdulQadir/Mores-Code-Detector_Neural-Network/blob/e723bf6b5fbaf9a3b29a6f3f5f23c35e6d87b1ed/Model-Docs/tinyML.md)**: Full MCU environment setup and detailed C firmware implementation logic.

## 1. Project Specifications
- **Target Hardware:** STM32F401 (96 KB RAM, 512 KB Flash).
- **Sampling Rate:** 20 Hz (1 sample every 50ms).
- **Input:** 100-sample sliding window (5.0 seconds of history).
- **Output:** 28 Classes (IDLE, A-Z, SPACE).

## 2. Core Logic & Character Detection
### How many characters are detected at once?
The model is trained as a single-label classifier. It analyzes the entire 100-sample window and identifies the **one most prominent character** within that 5-second context. While the window is 5 seconds long, it is optimized to recognize a single Morse sequence (e.g., 'S' or 'O') at a time.

### The Sliding Window & Memory Clearing
To ensure smooth real-time decoding, the system uses a specific management strategy:
- **Continuous Sliding:** New samples enter every 50ms while history shifts left, providing a constant real-time view.
- **Post-Detection Clearing:** Once a character is detected with high confidence (>80%), the input buffer is **cleared (reset to -128)**. 
- **Why clear it?** Because the window slides slowly, a detected character would stay inside the window for 100 consecutive ticks (5 seconds). Without clearing, the AI would repeat the same character 100 times. Clearing acts as a software reset, allowing the model to wait for the next unique signal to begin.

## 3. MCU Deployment Pipeline

### Step 1: STM32CubeMX Initialization
- **Middleware:** Enable `X-CUBE-AI`.
- **Timer (TIM11):** Configured to fire an update interrupt every 50ms (20Hz) — prescaler 83, period 49999 at 84MHz → 1MHz/50000 = 50ms.
- **GPIO:** `BTN_Pin` (PA0) as input, active-low push button.
- **UART (USART2, PA2/PA3):** 115200 baud, used for `printf` output.

### Step 2: Data Binding
1. Import `morse_model.tflite` into X-CUBE-AI.
2. Generate code to create `network.h`, `network_data.h`, and the weights/activations data.

### Step 3: Firmware Logic
Refer to `tinyML.md` for the actual C implementation covering model init (`ai_network_create_and_init`), `shift_window()`, `read_btn()`, `ai_run_inference()`, and `process_result()`.
- Inference uses an **8352-byte** activation scratch buffer; weights are resolved from flash automatically.
- `printf` is retargeted to USART2 via `Core/Src/syscalls.c` (`_write` → `__io_putchar`); `_sbrk` remains in `sysmem.c`.
