// 2021/06/17 16:05:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/USBH/USBH_.h"
#include <stm32f4xx_hal.h>
#include <usbh_def.h>


static USBH_HandleTypeDef handleUSBH;
void *USBH::handle = &handleUSBH;
