#include "stm32l4xx_hal.h"
#include <stdbool.h>

#define BMP180ADDR 0x77

I2C_HandleTypeDef hI2C;
bool sensorReady = 0;
bool transmitterReady = 0;
void writeRegister(uint8_t addr, uint8_t regAddr, uint8_t value);
void readRegister(uint8_t addr, uint8_t regAddr, uint8_t *result, uint16_t size);
void I2C_Init(void);