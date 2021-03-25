#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Debug_.h"
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
    pchar file = Debug::file[0];
    int line = Debug::line[0];
    int posItem = Debug::posItem;

    while (1) //-V776
    {
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
        // HAL_ADC_Start((ADC_HandleTypeDef *)HAL_ADC1::handle);

        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);

        ADC_HandleTypeDef *hadc = (ADC_HandleTypeDef *)HAL_ADC1::handle;

        hadc->Lock = HAL_LOCKED;

        if ((hadc->Instance->CR2 & ADC_CR2_ADON) != ADC_CR2_ADON)
        {
            __HAL_ADC_ENABLE(hadc);

            __IO uint counter = (ADC_STAB_DELAY_US * (SystemCoreClock / 1000000U));

            while (counter != 0U)
            {
                counter--;
            }
        }

        ADC_STATE_CLR_SET(hadc->State,
            HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR, HAL_ADC_STATE_REG_BUSY);

        ADC_CLEAR_ERRORCODE(hadc);

        hadc->Lock = HAL_UNLOCKED;

        __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOC | ADC_FLAG_OVR);

        if ((hadc->Instance->CR2 & ADC_CR2_EXTEN) == RESET)
        {
            hadc->Instance->CR2 |= (uint32_t)ADC_CR2_SWSTART;
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
