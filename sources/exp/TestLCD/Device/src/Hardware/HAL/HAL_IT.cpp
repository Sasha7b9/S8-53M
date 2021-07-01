// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL_.h"
#include "Display/Display.h"
#include <stm32f4xx_hal.h>


#ifdef __cplusplus
extern "C" {
#endif



void NMI_Handler()
{
}


void HardFault_Handler()
{
    while (1) //-V776
    {
    }
}


void MemManage_Handler()
{
  while (1)
  {
  }
}


void BusFault_Handler()
{
  while (1)
  {
  }
}


void UsageFault_Handler()
{
  while (1)
  {
  }
}


void SVC_Handler()
{
}


void DebugMon_Handler()
{
}


void PendSV_Handler()
{
}


void SysTick_Handler()
{
    HAL_IncTick();
}


void SPI5_IRQHandler()
{
//    HAL_SPI_IRQHandler((SPI_HandleTypeDef *)HAL_SPI5::handle);
}


void EXTI4_IRQHandler()
{
}


void OTG_HS_IRQHandler(void)
{
    HAL_HCD_IRQHandler((HCD_HandleTypeDef *)HAL_HCD::handle);
}



void OTG_FS_IRQHandler()
{
    HAL_PCD_IRQHandler((PCD_HandleTypeDef *)HAL_PCD::handle);
}


void TIM6_DAC_IRQHandler()
{
//    HAL_TIM6::CallbackOnIRQ();
}

#ifdef __cplusplus
}
#endif
