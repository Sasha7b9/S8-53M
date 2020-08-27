#pragma once
#include "Hardware/HAL/HAL_PINS.h"


struct HAL
{
    static void Init();
    static void ErrorHandler();
};

struct HAL_PINS
{
    static void Init();
};

struct HAL_SPI2
{
    static void Init();
};

