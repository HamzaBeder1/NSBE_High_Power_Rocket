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
}

void get_bmp_up(){
	writeRegister(BMP180_ADDR, BMP180_CTRL_MEAS, 0x34);
	uint8_t data[3];
	readRegister(BMP180_ADDR, BMP180_OUT_MSB, data, 3);
	bmpUP = (((int32_t)(data[0]) << 16) | ((int32_t)(data[1]) << 8) | (int32_t)(data[2])) >> (8-OSS_BMP180);
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
}

void get_bmp_p(){
	get_bmp_calibration_data();
	get_bmp_ut();
	get_bmp_up();
	int16_t AC1 = (calibration_data[0] << 8) | calibration_data[1];
	int16_t AC2  = (calibration_data[2] << 8) | calibration_data[3];
	int16_t AC3 = (calibration_data[4] << 8) | calibration_data[5];
	uint16_t AC4 = (calibration_data[6] << 8) | calibration_data[7];
	uint16_t AC6 = (calibration_data[10] << 8) | calibration_data[11];
	uint16_t AC5 = (calibration_data[8] << 8) | calibration_data[9];
	int16_t B1 = (calibration_data[12] << 8) | calibration_data[13];
	int16_t B2 = (calibration_data[14] << 8) | calibration_data[15];
	int16_t MC = (calibration_data[18] << 8) | calibration_data[19];
	int16_t MD = (calibration_data[20] << 8) | calibration_data[21];
	int32_t X1 = ((bmpUT - (int32_t)AC6)*(int32_t)AC5) >> 15;
	int32_t X2 = ((int32_t)MC<<11)/(X1+(int32_t)MD);
	int32_t B5 = X1 + X2;

	int32_t B6 = B5 - 4000;
	X1 = ((int32_t)B2*((B6*B6) >> 12))>>11;
	X2 = ((int32_t)AC2*B6)>>11;
	int32_t X3 = X1+X2;
	int32_t B3 = ((((int32_t)AC1*4+X3)<<OSS_BMP180)+2) >> 2;
	X1 = ((int32_t)AC3*B6)>>13;
	X2 = ((int32_t)B1 * ((B6*B6)>>12))>>16;
	X3 = ((X1+X2)+2)>>2;
	uint32_t B4 = ((uint32_t)AC4*(X3+32768L))>>15;
	uint32_t B7 = (bmpUP - B3)*(50000UL >>OSS_BMP180);

	if(B7 < 0x80000000){
	    bmpP = (B7/B4)*2;
	  }
	  else{
	    bmpP = (B7/B4)<<1;
	  }

	X1 = (bmpP>>8)*(bmpP>>8);
	X1 = (X1*3038L) >> 16;
	X2 = (-7357L*bmpP) >> 16;
	bmpP = bmpP + ((X1+X2+3791L)>>4);
	int x = 21;
	x+=1;
}

