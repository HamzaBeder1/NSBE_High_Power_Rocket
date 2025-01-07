#include "main.h"
void SystemClock_Config(void);
void DMA_Init(void);
void I2C_Init(void);
static void MX_USART2_UART_Init(void);
void GPIO_Init(void);
//UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_i2c1_tx;
DMA_HandleTypeDef hdma_i2c1_rx;
FATFS fs;
FIL file;
FRESULT res;
#define BUFFERSIZE 100
#define DS3231ADDR 0b1101000
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c){
}
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *I2CHandle){
	if(I2CHandle->Instance == I2C1){
		sensorReady = 1;
	}
}
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef * I2CHandle){
	if(I2CHandle->Instance == I2C1){
		transmitterReady = 1;
	}
}
int main(void)
{
 HAL_Init();
 SystemClock_Config();
 GPIO_Init();
 //DMA_Init();
 //I2C_Init();
 SPI_Init();
 uint8_t arr[] = {30, 0, 11, 28, 7, 12};
 //initDS3231(2024, arr, 1, 12);
 //get_bmp_t();
 //initMPU6050(0);
 uint8_t timeData[6] = {30, 14, 4, 3, 24, 12};
 char header[] = "Time,Acceleration";
 bool result = initialize_SD_card();
 if(!result)
	  while(1);
 result = is_SD_card_initialized();
 if(!result)
	  while(1);
 res = f_mount(&fs, "0:", 1);
 if(res != FR_OK)
	  while(1);
 res = f_open(&file, "text.txt", FA_CREATE_ALWAYS | FA_WRITE);
 if(res == FR_OK){
	  const char* text = "Hello, SD card!\n";
	  UINT bytesWritten;
	  res = f_write(&file, text, strlen(text), &bytesWritten);
	  if(res != FR_OK)
		  while(1);
	  f_close(&file);
 }
 else
	  while(1);
 res = f_open(&file, "text.txt", FA_READ);
 if(res == FR_OK){
	  char buffer[64];
	  UINT bytesRead;
	  res = f_read(&file, buffer, sizeof(buffer) - 1, &bytesRead);
	  if(res != FR_OK)
		  while(1);
	  buffer[bytesRead] = '\0';
	  f_close(&file);
 }
 else
	  while(1);

 int x = 21;
 x+=1;
 while (1)
 {
	  //getDateAndTime();
	 // getAccelMPU6050(second);
	  //getAltitudeBMP180(second);
	  int x = 21;
	  x+=1;
 }
}
void SystemClock_Config(void)
{
 RCC_OscInitTypeDef RCC_OscInitStruct = {0};
 RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
 if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
 {
   Error_Handler();
 }
 RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
 RCC_OscInitStruct.MSIState = RCC_MSI_ON;
 RCC_OscInitStruct.MSICalibrationValue = 0;
 RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
 RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
 if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
 {
   Error_Handler();
 }
 RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                             |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
 RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
 RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
 RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
 RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
 if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
 {
   Error_Handler();
 }
}
//PB6 = SCL1, PB7 = SDA1
void DMA_Init(void){
	__HAL_RCC_DMA1_CLK_ENABLE();
	HAL_NVIC_SetPriority(DMA1_Channel6_IRQn,0,0);
	HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
	HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
}
void GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */
	 /* GPIO Ports Clock Enable */
	 __HAL_RCC_GPIOA_CLK_ENABLE();
	 /*Configure GPIO pin Output Level */
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	 /*Configure GPIO pin : PA9 */
	 GPIO_InitStruct.Pin = GPIO_PIN_9;
	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
	 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}
static void MX_USART2_UART_Init(void)
{
 /*huart2.Instance = USART2;
 huart2.Init.BaudRate = 115200;
 huart2.Init.WordLength = UART_WORDLENGTH_8B;
 huart2.Init.StopBits = UART_STOPBITS_1;
 huart2.Init.Parity = UART_PARITY_NONE;
 huart2.Init.Mode = UART_MODE_TX_RX;
 huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
 huart2.Init.OverSampling = UART_OVERSAMPLING_16;
 huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
 huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
 if (HAL_UART_Init(&huart2) != HAL_OK)
 {
   Error_Handler();
 }*/
}
void Error_Handler(void)
{
 __disable_irq();
 while (1)
 {
 }
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

