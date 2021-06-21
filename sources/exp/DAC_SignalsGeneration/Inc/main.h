#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"

#define DACx_CHANNEL1_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define DMAx_CLK_ENABLE()               __HAL_RCC_DMA1_CLK_ENABLE()  
     
#define DACx_FORCE_RESET()              __HAL_RCC_DAC_FORCE_RESET()
#define DACx_RELEASE_RESET()            __HAL_RCC_DAC_RELEASE_RESET()

#define DACx_CHANNEL1_PIN                GPIO_PIN_4
#define DACx_CHANNEL1_GPIO_PORT          GPIOA 

#define DACx_CHANNEL1                    DAC_CHANNEL_1

#define DACx_DMA_CHANNEL1                DMA_CHANNEL_7
#define DACx_DMA_STREAM1                 DMA1_Stream5 

#define DACx_DMA_IRQn1                   DMA1_Stream5_IRQn
#define DACx_DMA_IRQHandler1             DMA1_Stream5_IRQHandler

#endif
