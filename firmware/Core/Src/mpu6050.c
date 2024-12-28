#include "mpu6050.h"



float step;
accelMPU6050 *accel_buffer_MPU6050;
velocityMPU6050 *velocity_buffer_MPU6050;
altitudeMPU6050 *altitude_buffer_MPU6050;

size_t total_buffer_size;
size_t curr_buffer_size;

int accel_idx;
int velocity_idx;
int altitude_idx;

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
	/*writeRegister(MPU6050ADDR, PWR_MGMT_1, 0x00);
	uint8_t temp[1];
	readRegister(MPU6050ADDR, ACCEL_CONFIG, temp, 1);
	temp[0] &= 0b11100111;
	temp[0] |= (AFS_SEL << 3);
	writeRegister(MPU6050ADDR, ACCEL_CONFIG, temp);*/

	total_buffer_size = 100;
	accel_buffer_MPU6050 = (accelMPU6050*)malloc(sizeof(accelMPU6050)*total_buffer_size);
	velocity_buffer_MPU6050 = (velocityMPU6050*)malloc(sizeof(velocityMPU6050)*total_buffer_size);
	altitude_buffer_MPU6050 = (altitudeMPU6050*)malloc(sizeof(altitudeMPU6050)*total_buffer_size);
	curr_buffer_size = 0;
	accel_idx = 0;
	velocity_idx = 0;
	altitude_idx = 0;
}

void getAccelMPU6050(int s){
	uint8_t accelReg[6];
	readRegister(MPU6050ADDR,ACCEL_XOUT_H, accelReg, 6);

	int16_t X = (accelReg[0] << 8) | (accelReg[1]);
	int16_t Y = (accelReg[2] << 8) | (accelReg[3]);
	int16_t Z = (accelReg[4] << 8) | (accelReg[5]);

	accelMPU6050 a;
	a.accelX = X*step;
	a.accelY = Y*step;
	a.accelZ = Z*step;
	a.seconds = s;

	if(curr_buffer_size + 1 >= total_buffer_size){
		total_buffer_size*=2;
		accel_buffer_MPU6050 = (accelMPU6050*)realloc(accel_buffer_MPU6050, total_buffer_size*sizeof(accelMPU6050));
		velocity_buffer_MPU6050 = (velocityMPU6050*)realloc(velocity_buffer_MPU6050, total_buffer_size*sizeof(velocityMPU6050));
		altitude_buffer_MPU6050 = (altitudeMPU6050*)realloc(altitude_buffer_MPU6050, total_buffer_size*sizeof(altitudeMPU6050));
	}

	accel_buffer_MPU6050[accel_idx++] = a;
}

void getVelocityMPU6050(){
	velocity_buffer_MPU6050[0].velX = 0;
	velocity_buffer_MPU6050[0].velY = 0;
	velocity_buffer_MPU6050[0].velZ = 0;
	velocity_buffer_MPU6050[0].velX = accel_buffer_MPU6050[0].seconds;
	for(int i = 1; i < curr_buffer_size; i++){
		int dt = accel_buffer_MPU6050[i].seconds - accel_buffer_MPU6050[i-1].seconds;
		velocity_buffer_MPU6050[i].velX = (accel_buffer_MPU6050[i-1].accelX+accel_buffer_MPU6050[i].accelX)*dt*(1/2);
		velocity_buffer_MPU6050[i].velY = (accel_buffer_MPU6050[i-1].accelY+accel_buffer_MPU6050[i].accelY)*dt*(1/2);
		velocity_buffer_MPU6050[i].velZ = (accel_buffer_MPU6050[i-1].accelZ+accel_buffer_MPU6050[i].accelZ)*dt*(1/2);
		velocity_buffer_MPU6050[i].seconds = accel_buffer_MPU6050[i].seconds;
	}
}

void getAltitudeMPU6050(){
	altitude_buffer_MPU6050[0].altitudeX = 0;
	altitude_buffer_MPU6050[0].altitudeY = 0;
	altitude_buffer_MPU6050[0].altitudeZ = 0;
	altitude_buffer_MPU6050[0].seconds = velocity_buffer_MPU6050[0].seconds;
	for(int i = 1; i < curr_buffer_size; i++){
		int dt = velocity_buffer_MPU6050[i].seconds - velocity_buffer_MPU6050[i-1].seconds;
		altitude_buffer_MPU6050[i].altitudeX = (velocity_buffer_MPU6050[i-1].velX+velocity_buffer_MPU6050[i].velX)*dt*(1/2);
		altitude_buffer_MPU6050[i].altitudeY = (velocity_buffer_MPU6050[i-1].velY+velocity_buffer_MPU6050[i].velY)*dt*(1/2);
		altitude_buffer_MPU6050[i].altitudeZ = (velocity_buffer_MPU6050[i-1].velZ+velocity_buffer_MPU6050[i].velZ)*dt*(1/2);
		altitude_buffer_MPU6050[i].seconds = velocity_buffer_MPU6050[i].seconds;
	}

}
