#pragma once

#include "stm32l4xx_hal.h"
#include "i2c.h"
#include <stdbool.h>

#define MPU6050ADDR 0x68
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
#define SIGNAL_PATH_RESET 0x68
#define SMPRT_DIV 0x19
#define CONFIG 0x1A

typedef struct{
	float accelX;
	float accelY;
	float accelZ;
	uint8_t seconds;
}accelMPU6050;

typedef struct{
	float velX;
	float velY;
	float velZ;
	uint8_t seconds;
}velocityMPU6050;

typedef struct{
	float altitudeX;
	float altitudeY;
	float altitudeZ;
	uint8_t seconds;
}altitudeMPU6050;

extern accelMPU6050 *accel_buffer_MPU6050;
extern velocityMPU6050 *velocity_buffer_MPU6050;
extern altitudeMPU6050 *altitude_buffer_MPU6050;
extern float step;

extern size_t total_buffer_size;
extern size_t curr_buffer_size;

extern int accel_idx;
extern int velocity_idx;
extern int altitude_idx;

enum accelRange{
	RANGE_2G = 0b00,
	RANGE_4G = 0b01,
	RANGE_8G = 0b10,
	RANGE_16G = 0b11
};

void initMPU6050(unsigned char AFS_SEL);
void getAccelMPU6050(int s);
void getVelocityMPU6050();
void getAltitudeMPU6050();

