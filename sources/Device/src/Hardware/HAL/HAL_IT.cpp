// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Timer_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Settings/SettingsTypes_.h"
#include "common/Utils/Debug_.h"
#include "Display/Display.h"
#include <stm32f4xx_hal.h>

extern HCD_HandleTypeDef hhcd;

const Item *last_item = nullptr;


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
    
    while (1) //-V776
    {
        last_item = last_item; //-V570
        Display::Update();
        file = file; //-V570
        line = line; //-V570
        posItem = posItem; //-V570
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


void EXTI4_IRQHandler()
{
    HAL_ADC1::CallbackOnIRQ();
}


void OTG_HS_IRQHandler(void)
{
    HAL_HCD_IRQHandler(&hhcd);
}



void OTG_FS_IRQHandler()
{
    HAL_PCD_IRQHandler((PCD_HandleTypeDef *)HAL_PCD::handle);
}


void TIM6_DAC_IRQHandler()
{
    HAL_TIM6::CallbackOnIRQ();
}

#ifdef __cplusplus
}
#endif
