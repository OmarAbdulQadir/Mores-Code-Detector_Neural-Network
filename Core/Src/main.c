/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "network.h"
#include "network_data.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define BUFFER_SIZE 100
#define True 1
#define False 0
#define BUTTON_PRESSED 127
#define BUTTON_RELEASED -128   /* matches the released level used during training */
#define BUTTON_NO_CHANGE 0
#define CONFIDENCE_THRESHOLD 100  /* min INT8 score to accept a prediction (tune to model output scale) */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* App Logic variables */
int8_t  input_buffer[AI_NETWORK_IN_1_SIZE];               /* model input  (S8, 100 samples) */
int8_t  output_buffer[AI_NETWORK_OUT_1_SIZE];    /* model output (S8, 28 classes)  */
uint8_t buffer_idx= 0;
uint8_t tick= False;
int8_t btn_state= BUTTON_NO_CHANGE;

/* Index 0 = IDLE/unknown, 1..26 = A..Z, 27 = SPACE */
const char LOOKUP[] = "?ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

/* AI variables */
ai_handle network = AI_HANDLE_NULL;
ai_buffer input;
ai_buffer output;
/* Scratch buffer for the network's intermediate activations (4-byte aligned). */
static uint8_t activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE] __attribute__((aligned(4)));
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void tick_handler(void);
void shift_window(void);
void read_btn(void);
void ai_run_inference(void);
void process_result(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  tick = True;
}

void tick_handler()
{
  read_btn();
  ai_run_inference();
}

void shift_window()
{
  if(buffer_idx < BUFFER_SIZE-1)
  {
    /* if index is within bounds, update the buffer */
    input_buffer[buffer_idx++] = btn_state;
  }
  else if(buffer_idx == BUFFER_SIZE-1)
  {
    /* buffer full: slide history one sample left, then append the newest one */
    memmove(input_buffer, input_buffer + 1, BUFFER_SIZE - 1);
    input_buffer[buffer_idx] = btn_state;
  }
  else
  {
    /* index is out of bounds, clear the buffer */
    memset(input_buffer, 0, BUFFER_SIZE);
    buffer_idx = 0;
  }
}

void read_btn()
{
  /* Read the button state */
  btn_state = HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin) == GPIO_PIN_SET ? BUTTON_RELEASED : BUTTON_PRESSED;
  
  /* update the input buffer */
  shift_window();

  /* Reset the button state */
  btn_state = BUTTON_NO_CHANGE;
}

void ai_run_inference()
{
  /* Only run once the 5-second window is full. The buffer descriptors
     (format/shape/size) were bound once in main(); we just point them at
     our data here. */
  if(network != AI_HANDLE_NULL && buffer_idx == BUFFER_SIZE-1)
  {
    input.data  = AI_HANDLE_PTR(input_buffer);
    output.data = AI_HANDLE_PTR(output_buffer);

    /* Run inference */
    ai_i32 n_batches = ai_network_run(network, &input, &output);

    if(n_batches != 1)
    {
      /* Inference failed: report and skip this tick. */
      ai_error err = ai_network_get_error(network);
      HAL_UART_Transmit(&huart2, (uint8_t*)"AI run error.\r\n", 15, HAL_MAX_DELAY);
      // printf("AI run error: type=%d code=%d\r\n", (int)err.type, (int)err.code);
      return;
    }

    process_result();
  }
}

void process_result()
{
  /* Argmax over the 28 INT8 class scores. */
  int8_t max_val = -128;
  int    max_idx = 0;
  for(int i = 0; i < AI_NETWORK_OUT_1_SIZE; i++)
  {
    if(output_buffer[i] > max_val)
    {
      max_val = output_buffer[i];
      max_idx = i;
    }
  }

  /* Emit only confident, non-IDLE predictions. Then reset the window to the
     released level so the same character is not reported on every subsequent
     tick while it lingers inside the 5-second window. */
  if(max_val > 75 && max_idx != 0)
  {
    HAL_UART_Transmit(&huart2, (uint8_t*)"Decoded Character: ", 19, HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, (uint8_t*)&LOOKUP[max_idx], 1, HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);

    HAL_UART_Transmit(&huart2, (uint8_t*)"Input buffer: ", 14, HAL_MAX_DELAY);
    for(uint8_t i= 0; i< 100; i++)
    {
    	uint8_t c = 0;
    	if(input_buffer[i] == BUTTON_NO_CHANGE) c= 'N';
    	else if(input_buffer[i] == BUTTON_RELEASED) c= 'D';
    	else if(input_buffer[i] == BUTTON_PRESSED) c= 'U';
    	HAL_UART_Transmit(&huart2, (uint8_t*)&c, 1, HAL_MAX_DELAY);
    	HAL_UART_Transmit(&huart2, (uint8_t*)" ", 1, HAL_MAX_DELAY);
    }
    HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);

    // printf("Decoded Character: %c\r\n", LOOKUP[max_idx]);
    memset(input_buffer, BUTTON_NO_CHANGE, BUFFER_SIZE);
    buffer_idx = 0;
  }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CRC_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Transmit(&huart2, (uint8_t*)"Starting neural network init.\r\n", 31, HAL_MAX_DELAY);
  /* Send printf output immediately instead of buffering it. */
  setvbuf(stdout, NULL, _IONBF, 0);

  /* Create and initialize the AI network. The activation buffer is supplied by
     us; weights are resolved automatically from flash (NULL -> use built-in). */
  ai_handle act_addr[] = { activations };
  ai_error ai_err = ai_network_create_and_init(&network, act_addr, NULL);
  if(ai_err.type != AI_ERROR_NONE)
  {
    HAL_UART_Transmit(&huart2, (uint8_t*)"AI init error.\r\n", 16, HAL_MAX_DELAY);
    // printf("AI init error: type=%d code=%d\r\n", (int)ai_err.type, (int)ai_err.code);
    Error_Handler();
  }

  /* Grab the model's pre-built I/O descriptors (correct format/shape/size).
     ai_run_inference() only needs to set the .data pointers afterwards. */
  input  = ai_network_inputs_get(network, NULL)[0];
  output = ai_network_outputs_get(network, NULL)[0];

  /* Prime the window as fully released, then start the 50 ms (20 Hz) sampler. */
  memset(input_buffer, BUTTON_RELEASED, BUFFER_SIZE);
  if(HAL_TIM_Base_Start_IT(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_UART_Transmit(&huart2, (uint8_t*)"Morse decoder ready.\r\n", 22, HAL_MAX_DELAY);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if(True == tick)
    {
      tick_handler();
      tick= False;
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/* Retarget printf/putchar to USART2 (PA2 TX). _write() in syscalls.c calls
   this for every character. */
int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
