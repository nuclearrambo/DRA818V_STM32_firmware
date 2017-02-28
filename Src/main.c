#include "main.h"
#include "stm32f1xx_hal.h"
//#include "cmsis_os.h"
#include "stm32f1xx.h"
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_it.h"
/* USER CODE BEGIN Includes */
#define boolean uint8_t
#define true 1
#define false 0


/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

//osThreadId defaultTaskHandle;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
void UART1SendByte(uint8_t p);
void UART2SendByte(uint8_t p);
void UART1SendString(uint8_t *data);
void UART2SendString(uint8_t *data);
extern void ftoa(char *res, float n, int afterpoint);
uint8_t *connect_cmd = "AT+DMOCONNECT\r\n";
uint8_t *config_cmd = "AT+DMOSETGROUP=1,145.3000,145.3000,0000,1,0000\r\n";
uint8_t encoderLH1, encoderLH2;
char buffer[25];
int encoderVal = 1440000;
int f_change = 0;

extern void LCD5110_init(void);
extern void LCD5110_init(void);
extern void LCD5110_write_char(unsigned char c);
extern void LCD5110_write_char_reg(unsigned char c);
extern void LCD5110_clear(void);
extern void LCD5110_set_XY(unsigned char X,unsigned char Y);
extern void LCD5110_write_string(char *s);
extern void LCD5110_write_Dec(unsigned int buffer);

int main(void)
{

  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  //MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  
  LCD5110_init();
  LCD5110_set_XY(0,1);
  LCD5110_write_string("FREQUENCY =>");
  LCD5110_set_XY(0,2);


  while (1)
  {
	  if(f_change == 1){
		ftoa(buffer, (float)encoderVal/10000.0, 4);
		UART1SendString(buffer);
		UART1SendString("\n");
//		UART2SendString(connect_cmd);
//		HAL_Delay(100);
		UART2SendString("AT+DMOSETGROUP=1,");
		UART2SendString(buffer);
		UART2SendString(",");
		UART2SendString(buffer);
		UART2SendString(",0000,1,0000\r\n");
		LCD5110_set_XY(0,2);
		LCD5110_write_string(buffer);
		f_change = 0;
	  }
	  else{
		  f_change = 0;
	  }



  }
  /* USER CODE END 3 */

}

void UART1SendByte(uint8_t p)
{
	USART1->DR = (p);
	while ((USART1->SR & USART_SR_TXE) == 0);
}

void UART1SendString(uint8_t *data){

	while(*data != 0){
		UART1SendByte(*data++);
	}
}

void UART2SendByte(uint8_t p)
{
	USART2->DR = (p);
	while ((USART2->SR & USART_SR_TXE) == 0);
}

void UART2SendString(uint8_t *data){

	while(*data != 0){
		UART2SendByte(*data++);
	}
}
/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
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

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* SPI2 init function */
static void MX_SPI2_Init(void)
{

  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

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

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}

void EXTI0_IRQHandler(){
	HAL_Delay(10);
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
	HAL_Delay(1);
	encoderLH1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
	encoderLH2 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
	if(encoderLH1 == 1 && encoderLH2 == 1){
		encoderVal += 100;
	}
	else if(encoderLH1 == 1 && encoderLH2 == 0){
		encoderVal -= 100;
	}
	f_change = 1;
}

void EXTI1_IRQHandler(){
	HAL_Delay(10);
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET)
	{
		encoderLH2 = 1;
		if(encoderLH1 == 0){
			encoderVal -= 100;
		}
	}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
	{
		encoderLH2 = 0;
	}
}

static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PB0 PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
//  HAL_NVIC_SetPriority(EXTI1_IRQn, 2, 0);
//  HAL_NVIC_EnableIRQ(EXTI1_IRQn);


  /*Configure GPIO pins : PB5 PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO Pins for SPI : PB12, PB13, PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);
  GPIOA->CRH = 0x000004B0;
  USART1->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;

  }

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
/* USER CODE BEGIN Callback 0 */

/* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
/* USER CODE BEGIN Callback 1 */

/* USER CODE END Callback 1 */
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif
