#include "sd.h"

bool send_command(uint8_t* cmd, uint8_t response){
	bool success = 0;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, cmd, 6, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_Delay(1);
	for(int i = 0; i < 1000; i++){
		uint8_t result[1];
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_SPI_TransmitReceive(&hspi1, (uint8_t[]){0xFF}, result, 1, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
		if(result[0] == response){
			if(cmd[0] == 0x48){
				uint8_t result_cmd8[4];
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
				HAL_SPI_TransmitReceive(&hspi1, (uint8_t[]){0xFF, 0xFF, 0xFF, 0xFF}, result_cmd8, 4, HAL_MAX_DELAY);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
				if(result_cmd8[0] != 0x00 || result_cmd8[1] != 0x00 || result_cmd8[2] != 0x01 || result_cmd8[3] != 0xAA)
					continue;
			}
			success = 1;
			break;
		}
		HAL_Delay(1);
	}
	return success;
}

void initialize_SPI_mode(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	uint8_t dummy[10];
	memset(dummy, 0xFF, sizeof(dummy));
	HAL_SPI_Transmit(&hspi1, dummy, sizeof(dummy), HAL_MAX_DELAY);
}


bool read_block(uint32_t block, uint8_t* data){
	uint8_t cmd17[6];
	cmd17[0] = 0x51;
	cmd17[1] = (block >> 24)&0xFF;
	cmd17[2] = (block >> 16)&0xFF;
	cmd17[3] = (block >> 8) & 0xFF;
	cmd17[4] = (block)&0xFF;
	cmd17[5] = 0xFF;
	bool result = 0;
	result = send_command(cmd17, 0x00);
	if(!result)
		return 0;
	uint8_t response[1];
	for(int i = 0; i < 1000; i++){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_SPI_TransmitReceive(&hspi1, (uint8_t[]){0xFF}, response, 1, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
		if(response[0] == 0xFE){
			break;
		}
	}

	if(response[0] != 0xFE)
		return 0;

	uint8_t send[512];
	memset(send, 0xFF,512);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, send, data, 512, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

	uint8_t crc[2];
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t[]){0xFF, 0xFF}, crc, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	return 1;
}

bool write_block(uint32_t block, uint8_t* data){
	uint8_t cmd24[6];
	cmd24[0] = 0x58;
	cmd24[1] = (block >> 24)&0xFF;
	cmd24[2] = (block >> 16)&0xFF;
	cmd24[3] = (block >> 8) & 0xFF;
	cmd24[4] = (block)&0xFF;
	cmd24[5] = 0xFF;
	bool result = 0;
	result = send_command(cmd24, 0x00);
	if(!result)
		return 0;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t[]){0xFE}, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

	for(int i = 0; i < 512; i++){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&hspi1, (uint8_t[]){data[i]}, 1, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	}

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t[]){0xFF, 0xFF}, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

	uint8_t response[1];
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t[]){0xFF}, response, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	if((uint8_t)(response[0]&0x1F) != 0x05)
		return 0;

	while(1){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_SPI_TransmitReceive(&hspi1, (uint8_t[]){0xFF}, response, 1, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
		if(response[0] != 0x00)
			break;
	}

	return 1;
}
