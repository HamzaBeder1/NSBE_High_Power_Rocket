#pragma once
#include "stm32l4xx_hal.h"
#include "spi.h"
#include "stdbool.h"

void initialize_SPI_mode();
bool send_command(uint8_t* cmd, uint8_t response);
bool read_block(uint32_t block, uint8_t* data);
bool write_block(uint32_t block, uint8_t* data);
