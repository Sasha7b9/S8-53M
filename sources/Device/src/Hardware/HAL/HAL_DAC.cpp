#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


static DAC_HandleTypeDef handleDAC = { DAC };
void *HAL_DAC::handle = &handleDAC;


void HAL_DAC::Init()
{
    DAC_ChannelConfTypeDef config =
    {
        DAC_TRIGGER_T7_TRGO,
        DAC_OUTPUTBUFFER_ENABLE
    };

    HAL_DAC_DeInit(&handleDAC);

    HAL_DAC_Init(&handleDAC);

    HAL_DAC_ConfigChannel(&handleDAC, &config, DAC_CHANNEL_1);
}


void HAL_DAC::StartDMA(uint8 *points, int numPoints)
{
    HAL_DAC_Start_DMA(&handleDAC, DAC_CHANNEL_1, (uint32_t *)points, (uint)numPoints, DAC_ALIGN_8B_R);
}


void HAL_DAC::StopDMA()
{
    HAL_DAC_Stop_DMA(&handleDAC, DAC_CHANNEL_1);
}
