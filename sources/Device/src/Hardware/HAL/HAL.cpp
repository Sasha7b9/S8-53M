#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <usbh_def.h>
#include <stm32f4xx_hal.h>


static HCD_HandleTypeDef hHCD;
void *HAL_HCD::handle = &hHCD;

static USBH_HandleTypeDef hUSBH;
void *HAL_USBH::handle = &hUSBH;


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
    __DAC_CLK_ENABLE();         // ��� �������
    __DMA1_CLK_ENABLE();        // ��� DAC1 (�������)
    __ETH_CLK_ENABLE();
    __PWR_CLK_ENABLE();
    __SPI1_CLK_ENABLE();
    __SYSCFG_CLK_ENABLE();
    __TIM2_CLK_ENABLE();        // ��� �����
    __TIM6_CLK_ENABLE();        // ��� ������� �����������
    __TIM7_CLK_ENABLE();        // ��� DAC1 (�������)
    __USB_OTG_FS_CLK_ENABLE();
    __USB_OTG_HS_CLK_ENABLE();

    HAL_PINS::Init();

    HAL_ADC3::Init();

    HAL_SPI1::Init();
}
