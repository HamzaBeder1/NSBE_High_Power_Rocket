#pragma once

#include "stm32l4xx_hal.h"
#include "i2c.h"
#include <stdbool.h>

#define DS3231_ADDR 0x68

extern uint8_t second;
extern uint8_t minute;
extern uint8_t hour;
extern uint8_t day;
extern uint8_t date;
extern uint8_t month;
extern uint16_t year;
extern bool AMorPM;

void initDS3231(uint16_t y, uint8_t * arr, bool PMnAM, int h);
void getDateAndTime();
uint8_t bcd2dec(uint8_t bcd);
uint8_t dec2bcd(uint8_t dec);
