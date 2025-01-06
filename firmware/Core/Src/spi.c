#include "spi.h"

//PA5 = SCK, PA6 = MISO, PA7 = MOSI, PA9 = CS
void SPI_Init(){
	hspi1.Instance = SPI1;
	  hspi1.Init.Mode = SPI_MODE_MASTER;
	  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	  hspi1.Init.NSS = SPI_NSS_SOFT;
	  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	  hspi1.Init.CRCPolynomial = 7;
	  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
	  if (HAL_SPI_Init(&hspi1) != HAL_OK)
	  {
	    while(1);
	  }
}

void send_bytes_SPI(uint8_t *send, uint32_t num){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, send, num, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
}

void send_receive_bytes_SPI(uint8_t *send, uint8_t *receive, uint32_t num){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, send, receive, num, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
}


