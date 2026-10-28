/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Solar Tracker Stepper Motor Controller - Manual Mode Only
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "lcd.h"
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
Lcd_HandleTypeDef lcd;

// Stepper Motor Sequence (Full-Step / Wave Drive)

const uint8_t step_sequence[4][4] = {
  {1, 0, 1, 0}, // Step 1: Coil A  and Coil B  ON
  {0, 1, 1, 0}, // Step 2: Coil A' and Coil B  ON
  {0, 1, 0, 1}, // Step 3: Coil A' and Coil B' ON
  {1, 0, 0, 1}  // Step 4: Coil A  and Coil B' ON
};

int current_step_index = 0; // Tracks which of the 4 sequences we are on
int angle_steps = 0;        // 0 to 100 (Where 1 step = 1.8 degrees)
char lcd_buffer[16];        // Buffer for text formatting
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void step_motor(int direction);
void update_display(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Function to move the stepper motor 1 step (1.8 degrees)
void step_motor(int direction) {
    if (direction == 1) { // Clockwise (West)
        current_step_index++;
        if (current_step_index > 3) current_step_index = 0;
        angle_steps++;
    } else { // Counter-Clockwise (East)
        current_step_index--;
        if (current_step_index < 0) current_step_index = 3;
        angle_steps--;
    }

    // Send the sequence to PB4, PB5, PB6, PB7
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, step_sequence[current_step_index][0]);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, step_sequence[current_step_index][1]);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, step_sequence[current_step_index][2]);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, step_sequence[current_step_index][3]);

    // Give the physical motor time to execute the step
    HAL_Delay(20);
}

// Function to update the LCD Screen
void update_display(void) {
    // Math trick: Instead of floats, multiply by 18 and divide by 10
    // Example: 1 step * 18 = 18. (18/10 = 1), (18%10 = 8) -> Prints "1.8"
    int whole_deg = (angle_steps * 18) / 10;
    int decimal_deg = (angle_steps * 18) % 10;

    sprintf(lcd_buffer, "Angle: %3d.%d  ", whole_deg, decimal_deg);
    Lcd_cursor(&lcd, 0, 0);
    Lcd_string(&lcd, lcd_buffer);

    Lcd_cursor(&lcd, 1, 0);
    Lcd_string(&lcd, "Mode: MANUAL    ");
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */

  // 1. Configure LCD Pins (D4=PA4, D5=PA5, D6=PA6, D7=PA7)
  Lcd_PortType ports[] = {GPIOA, GPIOA, GPIOA, GPIOA};
  Lcd_PinType pins[] = {GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7};

  // 2. Initialize LCD (RS=PA1, EN=PA2, 4-bit mode)
  lcd = Lcd_create(ports, pins, GPIOA, GPIO_PIN_1, GPIOA, GPIO_PIN_2, LCD_4_BIT_MODE);
  Lcd_clear(&lcd);

  // Print Initial Screen
  update_display();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    // ==========================================
    //            MANUAL MODE (Part B)
    // ==========================================

    // 1. Check CW Button (PB0)
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET) {
        HAL_Delay(30); // Debounce
        if (angle_steps < 100) { // Limit to 180 deg
            step_motor(1);
            update_display();
        }
        while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET); // Wait for release
        HAL_Delay(30);
    }

    // 2. Check CCW Button (PB1)
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET) {
        HAL_Delay(30); // Debounce
        if (angle_steps > 0) { // Limit to 0 deg
            step_motor(-1);
            update_display();
        }
        while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET); // Wait for release
        HAL_Delay(30);
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

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

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
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_AFIO_CLK_ENABLE();

  // CRITICAL: Free PB3 and PB4 from JTAG Debugger so they work as normal pins
  __HAL_AFIO_REMAP_SWJ_NOJTAG();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA1 PA2 PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 (Only the 2 Manual Buttons) */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4 PB5 PB6 PB7 (Stepper Driver Outputs) */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
