#include "sd.h"


static bool is_initialized = 0;

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

bool initialize_SD_card(){
	uint8_t cmd0[6] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95};
		  uint8_t cmd8[6] = {0x48, 0x00, 0x00, 0x01, 0xAA, 0x87};
		  uint8_t cmd55[6] = {0x77, 0x00, 0x00, 0x00, 0x00, 0xFF};
		  uint8_t acdm41[6] = {0x69, 0x40,0x00, 0x00, 0x00, 0xFF};
		  uint8_t cmd16[6] = {0x50, 0x00, 0x00, 0x02, 0x00, 0xFF};
		  bool result = 0;

		  initialize_SPI_mode();
		  while(result == 0)
			  result = send_command(cmd0, 0x01);
		  result = 0;
		  while(result == 0)
			  result = send_command(cmd8, 0x01);
		  result = 0;
		  while(result == 0){
			  bool temp = send_command(cmd55, 0x01);
			  if(!temp)
				  continue;
			  result = send_command(acdm41, 0x00);
		  }
		  result = 0;
		  while(result == 0){
			  result = send_command(cmd16, 0x00);
		  }
		  result = 0;
		  is_initialized = 1;
		  return 1;
}

bool is_SD_card_initialized(){
	return is_initialized;
}

bool write_multiple_blocks(uint32_t first_block, uint8_t* data, uint32_t num_blocks){
	uint8_t cmd25[6];
	cmd25[0] = 0x59;
	cmd25[1] = (first_block >> 24)&0xFF;
	cmd25[2] = (first_block >> 16)&0xFF;
	cmd25[3] = (first_block >> 8) & 0xFF;
	cmd25[4] = (first_block)&0xFF;
	cmd25[5] = 0xFF;

	bool result = send_command(cmd25, 0x00);
	if(!result)
		return 0;

	uint8_t response[2];
	for(int i = 0; i < num_blocks; i++){
		send_bytes_SPI((uint8_t[]){0xFC}, 1);
		send_bytes_SPI((data+i*512), 512);
		send_bytes_SPI((uint8_t[]){0xFF, 0xFF}, 2);
		send_receive_bytes_SPI((uint8_t[]){0xFF}, response, 1);
		if((uint8_t)(response[0]&0x1F) != 0x05)
			return 0;
		while(1){
			send_receive_bytes_SPI((uint8_t[]){0xFF}, response, 1);
			if(response[0] != 0x00)
				break;
		}
	}

	send_bytes_SPI((uint8_t[]){0xFD}, 1);
	while(1){
			send_receive_bytes_SPI((uint8_t[]){0xFF}, response, 1);
			if(response[0] != 0x00)
				break;
	}

	for(int i = 0; i < 100; i++)
		send_bytes_SPI((uint8_t[]){0xFF}, 1);
	return 1;
}

bool read_multiple_blocks(uint32_t first_block, uint8_t* data, uint32_t num_blocks){
	uint8_t cmd18[6];
	uint8_t cmd12[] = {0x4C, 0x00, 0x00, 0x00, 0x00, 0xFF};
	cmd18[0] = 0x52;
	cmd18[1] = (first_block >> 24)&0xFF;
	cmd18[2] = (first_block >> 16)&0xFF;
	cmd18[3] = (first_block >> 8) & 0xFF;
	cmd18[4] = (first_block)&0xFF;
	cmd18[5] = 0xFF;
	bool result = 0;
	result = send_command(cmd18, 0x00);
	if(!result)
		return 0;

	uint8_t response[2];
	for(int i = 0; i < num_blocks; i++){
		for(int i = 0; i < 1000; i++){
			send_receive_bytes_SPI((uint8_t[]){0xFF}, response, 1);
			if(response[0] == 0xFE)
				break;
		}
		if(response[0] != 0xFE)
			return 0;

		uint8_t send[512] = {0};
		memset(send, 0xFF,512);
		send_receive_bytes_SPI(send, data+i*512,512);

		send_receive_bytes_SPI((uint8_t[]){0xFF, 0xFF}, response, 2);
	}
	result = send_command(cmd12, 0x00);
	if(!result)
		return 0;

	while(1){
		send_receive_bytes_SPI((uint8_t[]){0xFF}, response, 1);
		if(response[0] != 0x00)
			break;
	}

	return 1;
}

