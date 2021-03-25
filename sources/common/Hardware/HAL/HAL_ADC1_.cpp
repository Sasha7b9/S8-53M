#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include <stm32f4xx_hal.h>


static uint16 adcValue = 0;

static ADC_HandleTypeDef hADC;
void *HAL_ADC1::handle = &hADC;


void HAL_ADC1::Init()
{
    HAL_PINS::ADC1_::Init();

    HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
    
    hADC.Instance = ADC1;
    hADC.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    hADC.Init.Resolution = ADC_RESOLUTION12b;
    hADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hADC.Init.ScanConvMode = DISABLE;
    hADC.Init.EOCSelection = ENABLE;
    hADC.Init.ContinuousConvMode = DISABLE;
    hADC.Init.DMAContinuousRequests = DISABLE;
    hADC.Init.NbrOfConversion = 1;
    hADC.Init.DiscontinuousConvMode = DISABLE;
    hADC.Init.NbrOfDiscConversion = 0;
    hADC.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hADC.Init.ExternalTrigConv = ADC_SOFTWARE_START;

    if (HAL_ADC_Init(&hADC) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    ADC_ChannelConfTypeDef sConfig;

    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Offset = 0;

    if (HAL_ADC_ConfigChannel(&hADC, &sConfig) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 1);         // По этому прерыванию нужно считывать значение рандомизатора
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}


uint16 HAL_ADC1::GetValue()
{
    return adcValue;
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    adcValue = (uint16)(HAL_ADC_GetValue(hadc));
}
