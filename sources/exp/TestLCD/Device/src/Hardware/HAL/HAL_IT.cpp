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


#ifdef __cplusplus
}
#endif
