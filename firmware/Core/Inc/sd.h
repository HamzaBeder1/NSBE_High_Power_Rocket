#pragma once
#include "stm32l4xx_hal.h"
#include "spi.h"
#include "stdbool.h"

static bool is_initialized;


void initialize_SPI_mode();
bool send_command(uint8_t* cmd, uint8_t response);
bool read_block(uint32_t block, uint8_t* data);
bool write_block(uint32_t block, uint8_t* data);
bool initialize_SD_card();
bool is_SD_card_initialized();
bool read_multiple_blocks(uint32_t first_block, uint8_t* data, uint32_t num_blocks);
bool read_multiple_blocks(uint32_t first_block, uint8_t* data, uint32_t num_blocks);
