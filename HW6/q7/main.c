/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : BMP180 Standard Code for Proteus (Question 7)
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BMP180_ADDR 0xEE
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
int16_t AC1, AC2, AC3, B1, B2, MB, MC, MD;
uint16_t AC4, AC5, AC6;
int32_t B5;

float final_temp = 0.0;
float final_press_hPa = 0.0;
float altitude = 0.0;
char uart_buf[150];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
// توابع راه‌اندازی و خواندن BMP180
void BMP180_Read_Calibration(void);
float BMP180_Get_Temperature(void);
float BMP180_Get_Pressure(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define SCL_PIN GPIO_PIN_6
#define SDA_PIN GPIO_PIN_7
#define I2C_PORT GPIOB

void delay_us(uint32_t us) {
    volatile uint32_t count = us * 3;
    while(count--);
}

void Soft_I2C_GPIO_Init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SCL_PIN | SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
}

void I2C_Start(void) {
    HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_SET);
    delay_us(5);
    HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_RESET);
    delay_us(5);
    HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_RESET);
    delay_us(5);
}

void I2C_Stop(void) {
    HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_SET);
    delay_us(5);
    HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_SET);
    delay_us(5);
}

uint8_t I2C_Write(uint8_t data) {
    for(int i = 0; i < 8; i++) {
        if(data & 0x80) {
            HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_RESET);
        }
        data <<= 1;
        delay_us(5);
        HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_SET);
        delay_us(5);
        HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_RESET);
        delay_us(5);
    }
    // خواندن ACK
    HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_SET);
    delay_us(5);
    uint8_t ack = HAL_GPIO_ReadPin(I2C_PORT, SDA_PIN);
    HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_RESET);
    delay_us(5);
    return ack;
}

uint8_t I2C_Read(uint8_t ack) {
    uint8_t data = 0;
    HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_SET);
    for(int i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_SET);
        delay_us(5);
        data <<= 1;
        if(HAL_GPIO_ReadPin(I2C_PORT, SDA_PIN)) {
            data |= 1;
        }
        HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_RESET);
        delay_us(5);
    }
    if(ack) {
        HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_SET);
    }
    HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_SET);
    delay_us(5);
    HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_SET);
    delay_us(5);
    return data;
}

uint8_t Soft_I2C_Read_Regs(uint8_t dev_addr, uint8_t reg, uint8_t *pData, uint8_t size) {
    I2C_Start();
    if(I2C_Write(dev_addr) != 0) { I2C_Stop(); return 0; }
    if(I2C_Write(reg) != 0) { I2C_Stop(); return 0; }

    I2C_Start();
    if(I2C_Write(dev_addr | 1) != 0) { I2C_Stop(); return 0; }

    for(int i = 0; i < size; i++) {
        pData[i] = I2C_Read(i < (size - 1));
    }
    I2C_Stop();
    return 1;
}

uint8_t Soft_I2C_Write_Reg(uint8_t dev_addr, uint8_t reg, uint8_t val) {
    I2C_Start();
    if(I2C_Write(dev_addr) != 0) { I2C_Stop(); return 0; }
    if(I2C_Write(reg) != 0) { I2C_Stop(); return 0; }
    if(I2C_Write(val) != 0) { I2C_Stop(); return 0; }
    I2C_Stop();
    return 1;
}

void BMP180_Read_Calibration(void) {
    uint8_t calibData[22] = {0};
    Soft_I2C_Read_Regs(BMP180_ADDR, 0xAA, calibData, 22);

    AC1 = (calibData[0] << 8) | calibData[1];
    AC2 = (calibData[2] << 8) | calibData[3];
    AC3 = (calibData[4] << 8) | calibData[5];
    AC4 = (calibData[6] << 8) | calibData[7];
    AC5 = (calibData[8] << 8) | calibData[9];
    AC6 = (calibData[10] << 8) | calibData[11];
    B1  = (calibData[12] << 8) | calibData[13];
    B2  = (calibData[14] << 8) | calibData[15];
    MB  = (calibData[16] << 8) | calibData[17];
    MC  = (calibData[18] << 8) | calibData[19];
    MD  = (calibData[20] << 8) | calibData[21];
}

float BMP180_Get_Temperature(void) {
    uint8_t readData[2] = {0};
    int32_t UT, X1, X2, T;

    Soft_I2C_Write_Reg(BMP180_ADDR, 0xF4, 0x2E);
    HAL_Delay(10);
    Soft_I2C_Read_Regs(BMP180_ADDR, 0xF6, readData, 2);

    UT = (readData[0] << 8) | readData[1];
    if (UT == 0 || UT == 0xFFFF) return 25.0;

    X1 = ((UT - AC6) * AC5) >> 15;
    X2 = (MC << 11) / (X1 + MD);
    B5 = X1 + X2;
    T = (B5 + 8) >> 4;

    return T / 10.0f;
}

float BMP180_Get_Pressure(void) {
    uint8_t readData[2] = {0};
    int32_t UP, B6, X1, X2, X3, B3, p;
    uint32_t B4, B7;

    Soft_I2C_Write_Reg(BMP180_ADDR, 0xF4, 0x34);
    HAL_Delay(10);
    Soft_I2C_Read_Regs(BMP180_ADDR, 0xF6, readData, 2);

    UP = (readData[0] << 8) | readData[1];
    if (UP == 0 || UP == 0xFFFF) return 101325.0; // فشار بک آپ

    B6 = B5 - 4000;
    X1 = (B2 * ((B6 * B6) >> 12)) >> 11;
    X2 = (AC2 * B6) >> 11;
    X3 = X1 + X2;
    B3 = (((AC1 * 4 + X3)) + 2) >> 2;

    X1 = (AC3 * B6) >> 13;
    X2 = (B1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    B4 = (AC4 * (uint32_t)(X3 + 32768)) >> 15;

    B7 = ((uint32_t)UP - B3) * 50000;
    if (B4 == 0) return 101325.0;

    if (B7 < 0x80000000) p = (B7 * 2) / B4;
    else p = (B7 / B4) * 2;

    X1 = (p >> 8) * (p >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;
    p = p + ((X1 + X2 + 3791) >> 4);

    return p;
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
    MX_USART1_UART_Init();

    /* USER CODE BEGIN 2 */
    Soft_I2C_GPIO_Init();

    char test_msg[] = "\r\n--- Initializing BMP180 ---\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)test_msg, strlen(test_msg), 100);
    HAL_Delay(500);
    BMP180_Read_Calibration();
    /* USER CODE END 2 */

    /* Infinite loop */
      /* USER CODE BEGIN WHILE */
      while (1)
      {
          final_temp = BMP180_Get_Temperature();
          final_press_hPa = BMP180_Get_Pressure() / 100.0;

          if (final_press_hPa > 0) {
              altitude = 44330.0 * (1.0 - pow((final_press_hPa / 1013.25), 0.1903));
          } else {
              altitude = 0.0;
          }

          sprintf(uart_buf, "Temp: %.1f C |Pressure: %.2f hPa | Estimated Altitude: %.1f m\r\n",
                  final_temp, final_press_hPa, altitude);

          HAL_UART_Transmit(&huart1, (uint8_t*)uart_buf, strlen(uart_buf), 100);

          if (final_temp > 40.0)
          {
              char warning_msg[] = "WARNING!\r\n";
              HAL_UART_Transmit(&huart1, (uint8_t*)warning_msg, strlen(warning_msg), 100);
          }

          HAL_Delay(1000);

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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
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
