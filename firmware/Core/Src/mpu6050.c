#include "mpu6050.h"



float step;
float accelX;
float accelY;
float accelZ;


void initMPU6050(unsigned char AFS_SEL){
	switch(AFS_SEL){
	case RANGE_2G:
		step = 0.00059877;
		break;
	case RANGE_4G:
		step = 0.00119;
	case RANGE_8G:
		step = 0.002395;
	case RANGE_16G:
		step = 0.00479;
	default:
		break;
	}
	writeRegister(MPU6050ADDR, PWR_MGMT_1, 0x00);
	uint8_t temp[1];
	readRegister(MPU6050ADDR, ACCEL_CONFIG, temp, 1);
	temp[1] &= 0b11100111;
	temp[1] |= (AFS_SEL << 3);
	writeRegister(MPU6050ADDR, ACCEL_CONFIG, temp[1]);
}

void getAccelMPU6050(){
	uint8_t accelReg[6];
	readRegister(MPU6050ADDR,ACCEL_XOUT_H, accelReg, 6);

	    int16_t X = (accelReg[0] << 8) | (accelReg[1]);
	    int16_t Y = (accelReg[2] << 8) | (accelReg[3]);
	    int16_t Z = (accelReg[4] << 8) | (accelReg[5]);

	    accelX = (X)*step;
	    accelY = (Y)*step;
	    accelZ = (Z)*step;
}
