/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mk_dht11.h"
#include "lcd.h"
#include <stdio.h>
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
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
dht11_t dht;
uint8_t mode = 0; // 0: Default, 1: Temp Set, 2: Hum Set
uint8_t target_temp = 25;
uint8_t target_hum = 50;

uint32_t last_dht_read = 0;
char lcd_buf[16];
uint32_t last_lcd_update = 0;

// LCD Handler specific to your library
Lcd_HandleTypeDef hlcd;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t last_btn_press[3] = {0, 0, 0};
uint8_t button_states[3] = {1, 1, 1}; // Default HIGH due to pull-ups

// Updated debouncer with edge-detection
uint8_t button_pressed(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t btn_index) {
    uint8_t current_state = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);

    // Detect falling edge (button was just pressed)
    if (current_state == GPIO_PIN_RESET && button_states[btn_index] == GPIO_PIN_SET) {
        if (HAL_GetTick() - last_btn_press[btn_index] > 200) { // 200ms debounce
            last_btn_press[btn_index] = HAL_GetTick();
            button_states[btn_index] = current_state; // Lock state
            return 1;
        }
    }
    // Reset state when button is released
    else if (current_state == GPIO_PIN_SET) {
        button_states[btn_index] = GPIO_PIN_SET;
    }
    return 0;
}

// Function to update motor PWM duty cycle
void set_fan_speed(uint16_t duty_cycle) {
    if(duty_cycle > 999) duty_cycle = 999;
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, duty_cycle);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  init_dht11(&dht, &htim1, GPIOB, GPIO_PIN_0);

  // 2. Start PWM for the DC Fan
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

  // 3. Initialize LCD using your specific library
  // D4->PA10, D5->PA11, D6->PA12, D7->PA15
  Lcd_PortType ports[] = {GPIOA, GPIOA, GPIOA, GPIOA};
  Lcd_PinType pins[] = {GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_15};

  // RS->PA8, EN->PA9. Configure for 4-bit mode.
  hlcd = Lcd_create(ports, pins, GPIOA, GPIO_PIN_8, GPIOA, GPIO_PIN_9, LCD_4_BIT_MODE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      if (HAL_GetTick() - last_dht_read >= 2000) {
          readDHT11(&dht);
          last_dht_read = HAL_GetTick();
      }

      // 2. MENU & BUTTON STATE LOGIC
      if (button_pressed(GPIOA, GPIO_PIN_2, 2)) { // MENU Button (PA2)
          mode++;
          if (mode > 2) mode = 0;
          Lcd_clear(&hlcd);
      }

      if (button_pressed(GPIOA, GPIO_PIN_0, 0)) { // UP Button (PA0)
          if (mode == 1) target_temp++;
          if (mode == 2) target_hum++;
      }

      if (button_pressed(GPIOA, GPIO_PIN_1, 1)) { // DOWN Button (PA1)
          if (mode == 1) target_temp--;
          if (mode == 2) target_hum--;
      }

      // 3. LCD DISPLAY UPDATES
      if (HAL_GetTick() - last_lcd_update > 200) {
                last_lcd_update = HAL_GetTick();

                if (mode == 0) {
                    sprintf(lcd_buf, "T:%dC S:%dC   ", dht.temperature, target_temp);
                    Lcd_cursor(&hlcd, 0, 0);
                    Lcd_string(&hlcd, lcd_buf);

                    sprintf(lcd_buf, "H:%d%% S:%d%%   ", dht.humidty, target_hum);
                    Lcd_cursor(&hlcd, 1, 0);
                    Lcd_string(&hlcd, lcd_buf);
                }
                else if (mode == 1) {
                    Lcd_cursor(&hlcd, 0, 0);
                    Lcd_string(&hlcd, "Set Temp Menu:  ");
                    sprintf(lcd_buf, "Target Temp: %dC  ", target_temp);
                    Lcd_cursor(&hlcd, 1, 0);
                    Lcd_string(&hlcd, lcd_buf);
                }
                else if (mode == 2) {
                    Lcd_cursor(&hlcd, 0, 0);
                    Lcd_string(&hlcd, "Set Humid Menu: ");
                    sprintf(lcd_buf, "Target Hum: %d%%  ", target_hum);
                    Lcd_cursor(&hlcd, 1, 0);
                    Lcd_string(&hlcd, lcd_buf);
                }
            }

      // 4. ACTUATOR & FEEDBACK LOGIC
      // Heater (Red LED - PB1)
      if (dht.temperature < target_temp) {
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
      } else {
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
      }

      // Humidifier (Blue LED - PB2)
      if (dht.humidty < target_hum) {
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
      } else {
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
      }

      // Fan Motor (PWM - PA6)
      if (dht.temperature > target_temp) {
          // Proportional Control: the higher the difference, the faster the fan
          int diff = dht.temperature - target_temp;
          int pwm_val = diff * 200; // Multiplier to increase duty cycle scaling
          set_fan_speed(pwm_val);
      } else {
          set_fan_speed(0); // Turn fan off entirely
      }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

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

/**
  * @brief TIM1 Initialization Function
  */
static void MX_TIM1_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 71;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  */
static void MX_TIM3_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 71;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_MspPostInit(&htim3);
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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA9 PA10 PA11 PA12 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */
