#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32l0xx_hal.h>


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


void SVC_Handler(void)
{

}


void PendSV_Handler(void)
{

}


void SysTick_Handler(void)
{
    HAL_IncTick();
}


void SPI2_IRQHandler()
{
    HAL_SPI_IRQHandler((SPI_HandleTypeDef *)HAL_SPI2::handle);
}


#ifdef __cplusplus
}
#endif
