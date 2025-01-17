#pragma once

#include "stm32l4xx_hal.h"

extern UART_HandleTypeDef huart3;
extern uint8_t buffer[100];
extern uint8_t buffer_idx;

void NEOM9N_Init();
