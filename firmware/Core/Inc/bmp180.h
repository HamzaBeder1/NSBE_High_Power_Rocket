#pragma once

#include "stm32l4xx_hal.h"
#include "i2c.h"

#define BMP180_ADDR 119
#define BMP180_CTRL_MEAS 0xF4
#define BMP180_OUT_MSB 0xF6
#define OSS_BMP180 0b00
#define AC1_H 0xAA


extern uint8_t calibration_data[22];
extern int32_t bmpUT;
extern int32_t bmpUP;
extern int32_t bmpB5;
extern int32_t bmpT;
extern int32_t bmpP;

void get_bmp_calibration_data();
void get_bmp_ut();
void get_bmp_up();
void get_bmp_t();
void get_bmp_p();

