#pragma once

#include "stm32l4xx_hal.h"
#include "i2c.h"

typedef struct{
	int16_t bmpAC1;
	int16_t bmpAC2;
	int16_t bmpAC3;
	uint16_t bmpAC4;
	uint16_t bmpAC5;
	uint16_t bmpAC6;
	int16_t bmpB1;
	int16_t bmpB2;
	int16_t bmpMB;
	int16_t bmpMC;
	int16_t bmpMD;
	int32_t bmpUT;
	int32_t bmpUP;
	int32_t bmpB5;
	int32_t bmpT;
	int32_t bmpP;
}BMP180;

void getCalibrationData();
	void getbmpUT();
	void getbmpUP();
	void getbmpT();
	void getbmpB5();
	void getbmpP();
	int8_t getT();
	int32_t getP();

	void printCalibrationData();
	void printUT();
	void printUP();
	void printB5();
	void printT();
	void printP();

