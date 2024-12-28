#pragma once

#include "stm32l4xx_hal.h"
#include "i2c.h"
#include "math.h"

#define BMP180_ADDR 119
#define BMP180_CTRL_MEAS 0xF4
#define BMP180_OUT_MSB 0xF6
#define OSS_BMP180 0b00
#define AC1_H 0xAA
#define T0 288
#define L 0.0065
#define R 8.3144598
#define g 9.81
#define M 0.0289644
#define P0 101000

typedef struct{
	float accel;
	uint8_t seconds;
}accelBMP180;

typedef struct{
	float vel;
	uint8_t seconds;
}velocityBMP180;

typedef struct{
	float altitude;
	uint8_t seconds;
}altitudeBMP180;

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
void getAccelBMP180();
void getVelocityBMP180();
void getAltitudeBMP180(int s);

