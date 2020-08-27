#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


static uint16 adcValue = 0;


void HAL_ADC3::Init()
{
    /*
    ј÷ѕ дл€ рандомизатора
    вход - ADC3 - 18 ADC3_IN4 - PF6
    тактова€ частота 25ћ√ц
    режим работы :
    -измерение по 1 регул€рному каналу
    - одиночное измерение по фронту внешнего запуска(прерывание от 112 - EXT11 - PC11)
    */

    HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);

    handleADC.Instance = ADC3;
    handleADC.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    handleADC.Init.Resolution = ADC_RESOLUTION12b;
    handleADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    handleADC.Init.ScanConvMode = DISABLE;
    handleADC.Init.EOCSelection = ENABLE;
    handleADC.Init.ContinuousConvMode = DISABLE;
    handleADC.Init.DMAContinuousRequests = DISABLE;
    handleADC.Init.NbrOfConversion = 1;
    handleADC.Init.DiscontinuousConvMode = DISABLE;
    handleADC.Init.NbrOfDiscConversion = 0;
    handleADC.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    handleADC.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_Ext_IT11;

    if (HAL_ADC_Init(&handleADC) != HAL_OK)
    {
        HARDWARE_ERROR
    }

    ADC_ChannelConfTypeDef sConfig;

    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Offset = 0;

    if (HAL_ADC_ConfigChannel(&handleADC, &sConfig) != HAL_OK)
    {
        HARDWARE_ERROR
    }

    if (HAL_ADC_Start_IT(&handleADC) != HAL_OK)
    {
        HARDWARE_ERROR
    }
}


uint16 HAL_ADC3::GetValue(void)
{
    return adcValue;
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    adcValue = (uint16)HAL_ADC_GetValue(hadc);
}
