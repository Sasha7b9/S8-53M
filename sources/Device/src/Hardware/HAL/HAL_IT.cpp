// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Settings/SettingsTypes_.h"
#include "common/Utils/Debug_.h"
#include "Display/Display.h"
#include <stm32f4xx_hal.h>


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
        last_item = last_item;
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


void DMA1_Stream5_IRQHandler()
{
    HAL_DMA_IRQHandler(((DAC_HandleTypeDef *)HAL_DAC2::handle)->DMA_Handle1);
}


//void ADC_IRQHandler()
//{
//    HAL_ADC_IRQHandler((ADC_HandleTypeDef *)HAL_ADC1::handle);
//}


void EXTI4_IRQHandler()
{
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);

        if (TBase::IsRandomize())
        {
            HAL_ADC1::StartConversion();
        }
    }
}


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
