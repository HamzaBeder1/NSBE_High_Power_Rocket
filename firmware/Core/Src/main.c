#include "main.h"


void SystemClock_Config(void);
void DMA_Init(void);
void I2C_Init(void);

I2C_HandleTypeDef hI2C;
DMA_HandleTypeDef hdma_i2c1_tx;
DMA_HandleTypeDef hdma_i2c1_rx;

#define BUFFERSIZE 100
#define BMP180ADDR 0b11101110
#define MPU6050ADDR 0b11010010

#define AC1_H 0xAA
#define AC1_L 0xAB


uint8_t Buffer_Dest[BUFFERSIZE];
uint8_t Buffer_Src[BUFFERSIZE];


int main(void)
{
  Buffer_Src[0] = 0xAA;
  HAL_Init();
  SystemClock_Config();
  I2C_Init();
  DMA_Init();

  if(HAL_I2C_Master_Transmit_DMA(&hI2C, BMP180ADDR, (uint8_t*)Buffer_Src, sizeof(Buffer_Src)) != HAL_OK)
  		  Error_Handler();
  if(HAL_I2C_Master_Receive_DMA(&hI2C, BMP180ADDR, Buffer_Dest , 1) != HAL_OK)
  		Error_Handler();
  while (1)
  {
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
void I2C_Init(void){
	hI2C.Instance = I2C1;
	hI2C.Init.OwnAddress2 = 0;
	hI2C.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hI2C.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hI2C.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hI2C.Init.OwnAddress1 = 0;
	hI2C.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hI2C.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hI2C.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	hI2C.Init.Timing = 0x00100D14;

	if (HAL_I2C_Init(&hI2C) != HAL_OK)
	{
		Error_Handler();
	}
}

void DMA_Init(void){
	__HAL_RCC_DMA1_CLK_ENABLE();

	HAL_NVIC_SetPriority(DMA1_Channel6_IRQn,0,0);
	HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

	HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
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
