#include "neom9n.h"

UART_HandleTypeDef huart3;
uint8_t buffer[100];
uint8_t buffer_idx;
NEOM9N_DATA_struct NEOM9N_data;


//TX = 11, RX = 10
void calculate_checksum(uint8_t *ck1, uint8_t *ck2, uint8_t* buff, uint8_t size);
void UART_Init();
void UART_Init(){
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 38400;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart3) != HAL_OK)
	{
	  while(1);
	}
}

void NEOM9N_Init(){
  HAL_UART_Receive_IT(&huart3, &buffer[buffer_idx], 1);
  buffer_idx = 0;
  uint8_t c1;
  uint8_t c2;
  uint8_t temp[] = {0x06, 0x08, 0x00, 0x00};
  calculate_checksum(&c1, &c2, temp, 4);
  uint8_t arr[] = {0xB5, 0x62, 0x06, 0x08, 0x00, 0x00, c1, c2};
  HAL_UART_Transmit(&huart3, arr, sizeof(arr), HAL_MAX_DELAY);
  HAL_Delay(100);
  buffer_idx=0;
  uint8_t temp2[] = {0x06 ,0x00 ,0x01 ,0x00 ,0x01};
  calculate_checksum(&c1, &c2, temp2, 5);
  uint8_t arr2[] = {0xB5,0x62, 0x06 ,0x00 ,0x01 ,0x00 ,0x01 , c1, c2};
  HAL_UART_Transmit(&huart3, arr2, sizeof(arr2), HAL_MAX_DELAY);
  HAL_Delay(100);
  buffer_idx=0;
  buffer[20] = 0x01;
  HAL_UART_Transmit(&huart3, buffer, 28, HAL_MAX_DELAY);
  HAL_Delay(100);
  buffer_idx=0;
  uint8_t temp3[] = {0x06, 0x00, 0x01, 0x00, 0x00};
  calculate_checksum(&c1, &c2, temp3, 5);
  uint8_t arr3[] = {0xB5,0x62, 0x06, 0x00, 0x01, 0x00, 0x00, c1, c2};
  HAL_UART_Transmit(&huart3, arr3, sizeof(arr3), HAL_MAX_DELAY);
  HAL_Delay(100);
  buffer_idx=0;
  buffer[20] = 0x01;
  HAL_UART_Transmit(&huart3, buffer, 28, HAL_MAX_DELAY);
  HAL_Delay(100);
  buffer_idx=0;
  uint8_t temp4[] = {0x06, 0x09, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,0xFF, 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
  calculate_checksum(&c1, &c2, temp4, sizeof(temp4));
  uint8_t arr4[] = {0xB5 ,0x62 ,0x06 ,0x09 ,0x0C ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0xFF ,0xFF ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,c1,c2};
  HAL_UART_Transmit(&huart3, arr4, sizeof(arr4), HAL_MAX_DELAY);
  HAL_Delay(100);
  buffer_idx=0;
}

void calculate_checksum(uint8_t *ck1, uint8_t *ck2, uint8_t* buff, uint8_t size){
	int i;
	*(ck1) = 0;
	*(ck2) = 0;
	for(i = 0; i < size; i++){
		*ck1 = *ck1 + *(buff+i);
		*ck2 = *ck2 + *ck1;
	}
}

void NEOM9N_get_PVT(){
	HAL_UART_Transmit(&huart3, pvt_cmd, sizeof(pvt_cmd), HAL_MAX_DELAY);
	HAL_Delay(1000);
	buffer_idx = 0;
	uint8_t temp[92];
	memcpy(temp,buffer+6,92);
	NEOM9N_data.year = (temp[5]<< 8)|(temp[4]);
	NEOM9N_data.month = temp[6];
	NEOM9N_data.day = temp[7];
	NEOM9N_data.hour = temp[8];
	NEOM9N_data.min = temp[9];
	NEOM9N_data.sec = temp[10];
	NEOM9N_data.lon = ((temp[27] << 24) | (temp[26] << 16) | (temp[25] << 8) | temp[24])*pow(10,-7);
	NEOM9N_data.lat = ((temp[31] << 24) | (temp[30] << 16) | (temp[29] << 8) | (temp[28]))*pow(10,-7);
	NEOM9N_data.height = ((temp[35] << 24) | (temp[34] << 16) | (temp[33] << 8) | (temp[32]));
	NEOM9N_data.hMSL = ((temp[39] << 24) | (temp[38] << 16) | (temp[37] << 8) | (temp[36]));
	NEOM9N_data.velN = ((temp[51] << 24) | (temp[50] << 16) | (temp[49] << 8) | (temp[48]));
	NEOM9N_data.velE = ((temp[55] << 24) | (temp[54] << 16) | (temp[53] << 8) | (temp[52]));
	NEOM9N_data.velD = ((temp[59] << 24) | (temp[58] << 16) | (temp[57] << 8) | (temp[56]));
	NEOM9N_data.gspeed = ((temp[63] << 24) | (temp[62] << 16) | (temp[61] << 8) | (temp[60]));
}

