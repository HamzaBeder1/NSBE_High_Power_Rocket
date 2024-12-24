#include "bmp180.h"

uint8_t calibration_data[22];
int32_t bmpUT;
int32_t bmpUP;
int32_t bmpB5;
int32_t bmpT;
int32_t bmpP;

void get_bmp_calibration_data(){
	readRegister(BMP180_ADDR, AC1_H, calibration_data, 22);
}

void get_bmp_ut(){
	writeRegister(BMP180_ADDR, BMP180_CTRL_MEAS, 0x2E);
	HAL_Delay(5);
	uint8_t data[2];
	readRegister(BMP180_ADDR, BMP180_OUT_MSB, data, 2);
	bmpUT = (data[0] << 8) | data[1];
	int x = 21;
	x+=1;
}

void get_bmp_up(){
	writeRegister(BMP180_ADDR, BMP180_CTRL_MEAS, 0x34|(OSS_BMP180 << 6));
	uint8_t data[3];
	readRegister(BMP180_ADDR, BMP180_OUT_MSB, data, 3);
	bmpUP = ((data[0] << 16) | (data[1] << 8) | (data[2])) >> (8-OSS_BMP180);
}

void get_bmp_t(){
	get_bmp_calibration_data();
	get_bmp_ut();
	uint16_t AC6 = (calibration_data[10] << 8) | calibration_data[11];
	uint16_t AC5 = (calibration_data[8] << 8) | calibration_data[9];
	int16_t MC = (calibration_data[18] << 8) | calibration_data[19];
	int16_t MD = (calibration_data[20] << 8) | calibration_data[21];
	int32_t X1 = ((bmpUT - (int32_t)AC6)*(int32_t)AC5) >> 15;
	int32_t X2 = ((int32_t)MC<<11)/(X1+(int32_t)MD);
	int32_t B5 = X1 + X2;
	bmpT = (B5+8) >> 4;
	bmpT /= 10;
	bmpT = (9.0/5.0)*(bmpT)+32;
	int x = 21;
	x+=1;
}
