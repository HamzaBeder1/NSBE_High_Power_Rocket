#pragma once

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "bmp180.h"
#include "mpu6050.h"
#include "ds3231.h"

void Error_Handler(void);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
