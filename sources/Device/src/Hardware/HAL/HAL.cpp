#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <usbh_def.h>
#include <stm32f4xx_hal.h>


static HCD_HandleTypeDef handleHCD;
void *HAL_HCD::handle = &handleHCD;

static USBH_HandleTypeDef handleUSBH;
void *HAL_USBH::handle = &handleUSBH;

static DAC_HandleTypeDef handleDAC = { DAC };
void *HAL_DAC::handle = &handleDAC;


void HAL::Init()
{
    HAL_Init();

    __SYSCFG_CLK_ENABLE();

    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOD_CLK_ENABLE();
    __GPIOE_CLK_ENABLE();
    __GPIOF_CLK_ENABLE();
    __GPIOG_CLK_ENABLE();
    __GPIOH_CLK_ENABLE();
    __GPIOI_CLK_ENABLE();

    __ADC3_CLK_ENABLE();
    __DAC_CLK_ENABLE();         // Для бикалки
    __DMA1_CLK_ENABLE();        // Для DAC1 (бикалка)
    __ETH_CLK_ENABLE();
    __PWR_CLK_ENABLE();
    __SPI1_CLK_ENABLE();
    __SYSCFG_CLK_ENABLE();
    __TIM2_CLK_ENABLE();        // Для тиков
    __TIM6_CLK_ENABLE();        // Для отсчёта миллисекунд
    __TIM7_CLK_ENABLE();        // Для DAC1 (бикалка)
    __USB_OTG_FS_CLK_ENABLE();
    __USB_OTG_HS_CLK_ENABLE();

    HAL_PINS::Init();

    HAL_ADC3::Init();

    HAL_HCD::Init();

    HAL_SPI1::Init();
}


void HAL_HCD::Init()
{
    /* Set USBHS Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(OTG_HS_IRQn, 15, 0);

    /* Enable USBHS Interrupt */
    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
}
