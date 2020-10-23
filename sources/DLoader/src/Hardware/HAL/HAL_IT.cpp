#include "defines.h"
#include "common/Hardware/HAL/HAL_c.h"
#include <stm32f4xx_hal.h>


#ifdef __cplusplus
extern "C" {
#endif

void NMI_Handler(void)
{
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

void OTG_HS_IRQHandler(void)
{
    HAL_HCD_IRQHandler(reinterpret_cast<HCD_HandleTypeDef *>(HAL_HCD::handle));
}

#ifdef __cplusplus
}
#endif
