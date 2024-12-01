#include "main.h"
#include "stm32l4xx_it.h"

extern DMA_HandleTypeDef hdma_i2c1_tx;
extern DMA_HandleTypeDef hdma_i2c1_rx;
extern I2C_HandleTypeDef hI2C;

void NMI_Handler(void)
{
   while (1)
  {
  }
}

void HardFault_Handler(void)
{
  while (1)
  {

  }
}


void MemManage_Handler(void)
{
  while (1)
  {

  }
}


void BusFault_Handler(void)
{
  while (1)
  {

  }
}


void UsageFault_Handler(void)
{
  while (1)
  {

  }
}


void SVC_Handler(void)
{

}


void DebugMon_Handler(void)
{

}

void PendSV_Handler(void)
{

}


void SysTick_Handler(void)
{
  HAL_IncTick();
}

void DMA1_Channel6_IRQHandler(void){
	HAL_DMA_IRQHandler(&hdma_i2c1_tx);
}

void DMA1_Channel7_IRQHandler(void){
	HAL_DMA_IRQHandler(&hdma_i2c1_rx);
}

void I2C1_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&hI2C);
}


void I2C1_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&hI2C);
}

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
