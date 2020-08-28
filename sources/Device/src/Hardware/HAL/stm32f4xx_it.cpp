#include "defines.h"
#include "Hardware/HAL/HAL.h"
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
    HAL_ADC_IRQHandler(reinterpret_cast<ADC_HandleTypeDef *>(HAL_ADC3::handle));
}


/// This interrupt call soft NSS for spi (see Hardware::SPIforPanel.c::PanelInit() and HAL_GPIO_EXTI_Callback().
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}


/// See Hardware::SPIforPanel.c::HAL_SPI_RxCpltCallback().
void SPI1_IRQHandler(void)
{
    HAL_SPI_IRQHandler(reinterpret_cast<SPI_HandleTypeDef *>(HAL_SPI1::handle));
}


void OTG_HS_IRQHandler(void)
{
    HAL_HCD_IRQHandler(reinterpret_cast<HCD_HandleTypeDef *>(HAL_HCD::handle));
}


void DMA1_Stream5_IRQHandler()
{
    HAL_DMA_IRQHandler(reinterpret_cast<DAC_HandleTypeDef *>(HAL_DAC::handle)->DMA_Handle1);
}

#ifdef __cplusplus
}
#endif
