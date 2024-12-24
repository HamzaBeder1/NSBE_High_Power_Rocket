#include "ds3231.h"

uint8_t second;
uint8_t minute;
uint8_t hour;
uint8_t day;
uint8_t date;
uint8_t month;
uint16_t year;
bool AMorPM;

void initDS3231(uint16_t y, uint8_t * arr, bool PMnAM, int h){
	uint8_t data[7];
	data[6] = dec2bcd(y%100);
	for(int i = 0; i < 6; i++)
		data[i] = dec2bcd(arr[i]);
	if(PMnAM)
		data[2] |= 0b00100000;
	else
		data[2] &= 0b11011111;
	switch(h){
		case 12:
			data[2] |= 0b01000000;
			break;
		case 24:
			data[2] &= 0b10111111;
		default:
			break;
	}
	writeRegisters(DS3231_ADDR, 0x00, data, 7);
	//readRegister(DS3231_ADDR, 0x00, ds3231_data, 7);
}

void getDateAndTime(){
	uint8_t data[7];
	readRegister(DS3231_ADDR, 0x00, data, 7);
	second = bcd2dec(data[0]);
	minute = bcd2dec(data[1]);
	hour = bcd2dec(data[2]&0b00011111);
	AMorPM = (hour >> 3)&1;
	hour &= 0b00011111;
	day = bcd2dec(data[3]);
	date = bcd2dec(data[4]);
	month = bcd2dec(data[5]);
	year = 2000+bcd2dec(data[6]);
}





uint8_t bcd2dec(uint8_t bcd){
  return bcd/16*10 + bcd%16;
}

uint8_t dec2bcd(uint8_t dec){
  return dec/10*16 + dec%10;
}
