#include "main.h"


void SystemClock_Config(void);
void DMA_Init(void);
void I2C_Init(void);
static void MX_USART2_UART_Init(void);
void Calculate_Temp_And_Pressure(void);

//UART_HandleTypeDef huart2;

I2C_HandleTypeDef hI2C;
DMA_HandleTypeDef hdma_i2c1_tx;
DMA_HandleTypeDef hdma_i2c1_rx;

#define BUFFERSIZE 100
#define BMP180ADDR 0b11101110
#define MPU6050ADDR 0b11010010

uint8_t AC1_H = 0xAA;
uint8_t F6 = 0xF6;
uint8_t F7 = 0xF7;
uint8_t F4 = 0xF4;
uint8_t F8 = 0xF8;
uint8_t E2 = 0x2E;
uint8_t x34 = 0x23;
uint8_t WHO_AM_I;


enum State{
	READ_1,
	READ_2,
	READ_3,
	WRITE_1,
	WRITE_2,
	READ_2_SETUP,
	READ_3_SETUP
};

struct BMP180{
	uint8_t Buffer1[22];
	uint8_t  Buffer2[2];
	uint8_t  Buffer3[3];
	enum State currState;
};




struct BMP180 bmp = {.currState = READ_1};

uint8_t Buffer_BMP180[BUFFERSIZE];
uint8_t WHO_AM_I_MPU6050 = 0x75;

uint8_t currAddr;


void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *I2CHandle)
{
	if(I2CHandle->Instance == I2C1){
		if(currAddr == BMP180ADDR){
			switch(bmp.currState){
			case READ_1:
				if(HAL_I2C_Master_Receive_DMA(&hI2C, currAddr, bmp.Buffer1 ,sizeof(bmp.Buffer1)) != HAL_OK)
					Error_Handler();
				bmp.currState = WRITE_1;
				break;
			case WRITE_1:
				if(HAL_I2C_Master_Transmit_DMA(&hI2C, currAddr, &E2 ,1) != HAL_OK)
						Error_Handler();
					bmp.currState = READ_2_SETUP;
				break;
			case READ_2_SETUP:
				if(HAL_I2C_Master_Transmit_DMA(&hI2C, currAddr, &F6,1) != HAL_OK)
						Error_Handler();
					bmp.currState = READ_2;
				break;
			case READ_2:
				if(HAL_I2C_Master_Receive_DMA(&hI2C, currAddr, bmp.Buffer2 ,sizeof(bmp.Buffer2)) != HAL_OK)
										Error_Handler();
				bmp.currState = WRITE_2;
				break;
			case WRITE_2:
				if(HAL_I2C_Master_Transmit_DMA(&hI2C, currAddr, &x34 ,1) != HAL_OK)
										Error_Handler();
									bmp.currState = READ_3_SETUP;
								break;
			case READ_3_SETUP:
				if(HAL_I2C_Master_Transmit_DMA(&hI2C, currAddr, &F6,1) != HAL_OK)
										Error_Handler();
									bmp.currState = READ_3;
								break;
			case READ_3:
				if(HAL_I2C_Master_Receive_DMA(&hI2C, currAddr, bmp.Buffer3,sizeof(bmp.Buffer3)) != HAL_OK)
														Error_Handler();
				bmp.currState = READ_1;
				break;
			default:
				break;
			}
		}
	}
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef * I2CHandle){
	if(I2CHandle->Instance == I2C1){
		if(currAddr == BMP180ADDR){
			switch(bmp.currState){
			case WRITE_1:
				if(HAL_I2C_Master_Transmit_DMA(&hI2C, currAddr, &F4, 1) != HAL_OK)
							Error_Handler();
				break;
			case WRITE_2:
				if(HAL_I2C_Master_Transmit_DMA(&hI2C, currAddr, &F4, 1) != HAL_OK)
											Error_Handler();
								break;
			case READ_1:
				Calculate_Temp_And_Pressure();
			default:
							break;
			}
		}
	}
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c){

}

void readRegisters(uint8_t addr, uint8_t * startAddr){
	currAddr = addr;
	if(HAL_I2C_Master_Transmit_DMA(&hI2C, addr, startAddr, 1) != HAL_OK)
				Error_Handler();
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  DMA_Init();
  I2C_Init();

  while (1)
  {
	  readRegisters(BMP180ADDR, &AC1_H);
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
	//hI2C.Init.Timing = 0x00100D19;
	hI2C.Init.Timing = 0x00100D14;
	if (HAL_I2C_Init(&hI2C) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_I2CEx_ConfigAnalogFilter(&hI2C, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}

    if (HAL_I2CEx_ConfigDigitalFilter(&hI2C, 0) != HAL_OK)
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

void Calculate_Temp_And_Pressure(void){}

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
