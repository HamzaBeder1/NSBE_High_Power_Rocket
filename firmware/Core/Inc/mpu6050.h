#pragma once

#include "stm32l4xx_hal.h"
#include "i2c.h"
#include <stdbool.h>

#define MPU6050ADDR 0x69
#define WHO_AM_I 0x75
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C
#define ACCEL_CONFIG  0x1C
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40


extern float step;
extern float accelX;
extern float accelY;
extern float accelZ;


enum accelRange{
	RANGE_2G = 0b00,
	RANGE_4G = 0b01,
	RANGE_8G = 0b10,
	RANGE_16G = 0b11
};

void initMPU6050(unsigned char AFS_SEL);
void getAccelMPU6050();

