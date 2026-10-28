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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t is_counting = 0; // Flag to track if the button was pressed
uint8_t counter = 0;
volatile uint8_t system_halted=0;
uint8_t numbers[10][7] = {
  {1,1,1,1,1,1,0}, // 0: Symmetric (No change)
  {0,0,0,0,1,1,0}, // 1: Lights up physical F and E (Top-Right and Bot-Right on mirror)
  {1,0,1,1,0,1,1}, // 2: Lights up A, F, G, C, D
  {1,0,0,1,1,1,1}, // 3: Lights up A, F, E, D, G
  {0,1,0,0,1,1,1}, // 4: Lights up B, F, E, G
  {1,1,0,1,1,0,1}, // 5: Lights up A, B, G, E, D
  {1,1,1,1,1,0,1}, // 6: Lights up A, B, C, D, E, G
  {1,0,0,0,1,1,0}, // 7: Lights up A, F, E
  {1,1,1,1,1,1,1}, // 8: Symmetric (No change)
  {1,1,0,1,1,1,1}  // 9: Lights up A, B, F, G, E, D
};
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
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);   // Segment A ON
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   // Segment B ON
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);   // Segment C ON
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);   // Segment D ON
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);   // Segment E ON
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);   // Segment F ON
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET); // Segment G OFF
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
    {
        // 1. Read the button on PA2. If it is pressed, set the flag to 1.
	  if (system_halted==0){
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET) {
            is_counting = 1;
        }}

        // 2. If the flag is set, start the continuous counting process
        if (is_counting == 1) {
            // Write the array values to the 7 output pins
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, numbers[counter][0]); // Segment A
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, numbers[counter][1]); // Segment B
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, numbers[counter][2]); // Segment C
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, numbers[counter][3]); // Segment D
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, numbers[counter][4]); // Segment E
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, numbers[counter][5]); // Segment F
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, numbers[counter][6]); // Segment G

            // Increment the counter. If it goes past 9, reset it to 0.
            counter++;
            if (counter > 9) {
                counter = 0;
            }

            // Wait 1000 milliseconds (1 second) before looping again
            HAL_Delay(1000);
        }
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA3 PA4 PA5 PA6
                           PA7 PA8 PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN)
{
	    // Check if the interrupt was triggered by PA0
	    if (GPIO_PIN == GPIO_PIN_0) {
	    	system_halted=1;
	        is_counting = 0; // Stop the main loop from overriding the display

	        // Display "1"
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, numbers[1][0]);
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, numbers[1][1]);
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, numbers[1][2]);
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, numbers[1][3]);
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, numbers[1][4]);
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, numbers[1][5]);
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, numbers[1][6]);
	    }

	    // Check if the interrupt was triggered by PA1
	    else if (GPIO_PIN == GPIO_PIN_1) {
	    	system_halted=1;
	        is_counting = 0; // Stop the main loop

	        // Display "9"
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, numbers[9][0]);
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, numbers[9][1]);
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, numbers[9][2]);
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, numbers[9][3]);
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, numbers[9][4]);
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, numbers[9][5]);
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, numbers[9][6]);
	    }
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
#ifdef USE_FULL_ASSERT
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
