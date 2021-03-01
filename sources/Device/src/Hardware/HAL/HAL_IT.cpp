#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
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


void ADC_IRQHandler(void)
{
    HAL_ADC_IRQHandler(reinterpret_cast<ADC_HandleTypeDef *>(HAL_ADC3::handle)); //-V2571
}


void OTG_HS_IRQHandler(void)
{
    HAL_HCD_IRQHandler(reinterpret_cast<HCD_HandleTypeDef *>(HAL_HCD::handle)); //-V2571
}


void DMA1_Stream5_IRQHandler()
{
    HAL_DMA_IRQHandler(reinterpret_cast<DAC_HandleTypeDef *>(HAL_DAC1::handle)->DMA_Handle1); //-V2571
}


void OTG_FS_IRQHandler()
{
    HAL_PCD_IRQHandler(reinterpret_cast<PCD_HandleTypeDef *>(HAL_HCD::handle)); //-V2571
}


#ifdef __cplusplus
}
#endif
