#include "neom9n.h"

UART_HandleTypeDef huart3;
uint8_t buffer[100];
uint8_t buffer_idx;

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
