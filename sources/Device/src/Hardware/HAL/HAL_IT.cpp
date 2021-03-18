#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Debug_.h"
#include <stm32f4xx_hal.h>


#ifdef __cplusplus
extern "C" {
#endif



void NMI_Handler()
{
}


void HardFault_Handler()
{
    pchar file = Debug::file[0];
    int line = Debug::line[0];
    int posItem = Debug::posItem;

    while (1)
    {
        file = file;
        line = line;
        posItem = posItem;
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
    HAL_SPI_IRQHandler((SPI_HandleTypeDef *)HAL_SPI5::handle);
}


void DMA1_Stream5_IRQHandler()
{
    HAL_DMA_IRQHandler(((DAC_HandleTypeDef *)HAL_DAC2::handle)->DMA_Handle1);
}


//void ADC_IRQHandler()
//{
//    HAL_ADC_IRQHandler(reinterpret_cast<ADC_HandleTypeDef *>(HAL_ADC3::handle));
//}
//
//
//void OTG_HS_IRQHandler()
//{
//    HAL_HCD_IRQHandler(reinterpret_cast<HCD_HandleTypeDef *>(HAL_HCD::handle));
//}
//
//
//void OTG_FS_IRQHandler()
//{
//    HAL_PCD_IRQHandler(reinterpret_cast<PCD_HandleTypeDef *>(HAL_HCD::handle));
//}

#ifdef __cplusplus
}
#endif
