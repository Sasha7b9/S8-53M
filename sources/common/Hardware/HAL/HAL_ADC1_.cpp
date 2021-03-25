#include "defines.h"
#include "common/Log_.h"
#include "common/Hardware/HAL/HAL_.h"
#include <stm32f4xx_hal.h>


static uint16 adc_value = 0;

static ADC_HandleTypeDef handleADC;
void *HAL_ADC1::handle = &handleADC;


void HAL_ADC1::Init()
{
    HAL_PINS::ADC1_::Init();

    HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
    
    handleADC.Instance = ADC1;
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
    handleADC.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    handleADC.Init.ExternalTrigConv = ADC_EXTERNALTRIGINJECCONV_T1_CC4;

    if (HAL_ADC_Init(&handleADC) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    ADC_ChannelConfTypeDef sConfig;

    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Offset = 0;

    if (HAL_ADC_ConfigChannel(&handleADC, &sConfig) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 1);         // По этому прерыванию нужно считывать значение рандомизатора
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}


uint16 HAL_ADC1::GetValue()
{
    ReadValue();

    LOG_WRITE("%d", adc_value);

    return adc_value;
}


void HAL_ADC1::ReadValue()
{
    if (HAL_ADC_PollForConversion(&handleADC, 1) == HAL_OK)
    {
        adc_value = (uint16)HAL_ADC_GetValue(&handleADC);
    }
}


void HAL_ADC1::StartConvertion()
{

}
