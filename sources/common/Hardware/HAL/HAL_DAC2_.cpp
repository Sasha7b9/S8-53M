#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include <stm32f4xx_hal.h>


static DAC_HandleTypeDef handleDAC = { DAC }; //-V2571
void *HAL_DAC2::handle = &handleDAC;


void HAL_DAC2::Init()
{
    __DAC_CLK_ENABLE();

    HAL_PINS::DAC2_::Init();

    static DMA_HandleTypeDef hdmaDAC1 =
    {
        DMA1_Stream5, //-V2571
        {
            DMA_CHANNEL_7,
            DMA_MEMORY_TO_PERIPH,
            DMA_PINC_DISABLE,
            DMA_MINC_ENABLE,
            DMA_PDATAALIGN_BYTE,
            DMA_MDATAALIGN_BYTE,
            DMA_CIRCULAR,
            DMA_PRIORITY_HIGH,
            DMA_FIFOMODE_DISABLE,
            DMA_FIFO_THRESHOLD_HALFFULL,
            DMA_MBURST_SINGLE,
            DMA_PBURST_SINGLE
        }
    };

    HAL_DMA_Init(&hdmaDAC1);

    __HAL_LINKDMA(&handleDAC, DMA_Handle1, hdmaDAC1);

    HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

    DAC_ChannelConfTypeDef config =
    {
        DAC_TRIGGER_T7_TRGO,
        DAC_OUTPUTBUFFER_ENABLE
    };

    HAL_DAC_DeInit(&handleDAC);

    HAL_DAC_Init(&handleDAC);

    HAL_DAC_ConfigChannel(&handleDAC, &config, DAC_CHANNEL_2);
}


void HAL_DAC2::StartDMA(uint8 *points, int numPoints)
{
    HAL_DAC_Start_DMA(&handleDAC, DAC_CHANNEL_2, reinterpret_cast<uint32_t *>(points), static_cast<uint>(numPoints), DAC_ALIGN_8B_R);
}


void HAL_DAC2::StopDMA()
{
    HAL_DAC_Stop_DMA(&handleDAC, DAC_CHANNEL_2);
}
