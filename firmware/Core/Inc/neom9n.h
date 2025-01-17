#pragma once

#include "stm32l4xx_hal.h"

#define PREAMBLE_1 0xB5
#define PREAMBLE_2 0x62

#define UBX_CLASS_NAV 0x01
#define UBX_NAV_PVT 0x07

extern UART_HandleTypeDef huart3;
extern uint8_t buffer[100];
extern uint8_t buffer_idx;

static const uint8_t pvt_cmd[]={0xB5,0x62,0x01,0x07,0x00,0x00,0x08,0x19};

typedef struct{
	uint32_t iTOW;
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint32_t lon;
	uint32_t lat;
	uint32_t height;
	uint32_t velocity_north;
	uint32_t velocity_east;
	uint32_t velocity_down;
	uint32_t gspeed;
}NEOM9N_DATA_struct;

extern NEOM9N_DATA_struct NEOM9N_data;

void NEOM9N_Init();
void NEOM9N_get_PVT();
