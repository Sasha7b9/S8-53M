#include "defines.h"
#include "ADC.h"
#include "Hardware.h"
#include <stm32f4xx_hal.h>


static uint16 adcValue = 0;


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    adcValue = (uint16)HAL_ADC_GetValue(hadc);
}


uint16 ADConverter::GetValue(void)
{
    return adcValue;
}
