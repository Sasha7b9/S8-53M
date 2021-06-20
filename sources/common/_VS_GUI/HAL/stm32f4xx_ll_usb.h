// 2021/06/20 19:19:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


typedef enum
{
    URB_IDLE = 0,
    URB_DONE,
    URB_NOTREADY,
    URB_NYET,
    URB_ERROR,
    URB_STALL
} USB_OTG_URBStateTypeDef;

