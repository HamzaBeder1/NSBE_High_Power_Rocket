#include "main.h"


extern DMA_HandleTypeDef hdma_i2c1_rx;
extern DMA_HandleTypeDef hdma_i2c1_tx;

void HAL_MspInit(void)
{

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c){
	__HAL_RCC_I2C1_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	__GPIOB_CLK_ENABLE();
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	hdma_i2c1_tx.Instance = DMA1_Channel6;
	hdma_i2c1_tx.Init.Request = DMA_REQUEST_3;
	hdma_i2c1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_i2c1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_i2c1_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_i2c1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_i2c1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_i2c1_tx.Init.Mode = DMA_NORMAL;
	hdma_i2c1_tx.Init.Priority = DMA_PRIORITY_LOW;

	if(HAL_DMA_Init(&hdma_i2c1_tx) != HAL_OK){
		Error_Handler();
	}

	__HAL_LINKDMA(hi2c, hdmatx, hdma_i2c1_tx);

	hdma_i2c1_rx.Instance = DMA1_Channel7;
	hdma_i2c1_rx.Init.Request = DMA_REQUEST_3;
	hdma_i2c1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_i2c1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_i2c1_rx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_i2c1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_i2c1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_i2c1_rx.Init.Mode = DMA_NORMAL;
	hdma_i2c1_rx.Init.Priority = DMA_PRIORITY_LOW;

	if(HAL_DMA_Init(&hdma_i2c1_rx) != HAL_OK){
		Error_Handler();
	}

	__HAL_LINKDMA(hi2c, hdmarx, hdma_i2c1_rx);
}
