#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"

#define DACx_CHANNEL2_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define DMAx_CLK_ENABLE()               __HAL_RCC_DMA1_CLK_ENABLE()  
     
#define DACx_FORCE_RESET()              __HAL_RCC_DAC_FORCE_RESET()
#define DACx_RELEASE_RESET()            __HAL_RCC_DAC_RELEASE_RESET()

#define DACx_CHANNEL2_PIN                GPIO_PIN_5
#define DACx_CHANNEL2_GPIO_PORT          GPIOA 

#define DACx_CHANNEL2                    DAC_CHANNEL_2

#define DACx_DMA_CHANNEL2                DMA_CHANNEL_7
#define DACx_DMA_STREAM1                 DMA1_Stream6

#define DACx_DMA_IRQn1                   DMA1_Stream6_IRQn
#define DACx_DMA_IRQHandler1             DMA1_Stream6_IRQHandler

#endif
