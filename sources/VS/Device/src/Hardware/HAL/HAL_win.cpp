#include "defines.h"
#include "common/Hardware/HAL/HAL_c.h"
#include <stm32f4xx_hal.h>
#include <usbh_def.h>


static USBH_HandleTypeDef handleUSBH;
void *HAL_USBH::handle = &handleUSBH;


void HAL::Init()
{

}


void HAL::Error()
{
    while (1)
    {
    }
}
