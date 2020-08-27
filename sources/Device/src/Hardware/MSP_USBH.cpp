#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_hcd.h>
#include <usbh_core.h>
#include <stm32f429xx.h>

/*
    Функции инициализации и деинициализации аппаратной части USBH, используемого для работы с флеш-диском.
*/



void HAL_HCD_MspInit(HCD_HandleTypeDef *)
{
    /* Set USBHS Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(OTG_HS_IRQn, 15, 0);

    /* Enable USBHS Interrupt */
    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
}


void HAL_HCD_MspDeInit(HCD_HandleTypeDef *)
{
    __USB_OTG_HS_CLK_DISABLE();
}
