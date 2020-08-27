#pragma once
#include "Hardware/HAL/HAL_PINS.h"


#define TIME_MS HAL_TIM::TimeMS()


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


struct HAL_TIM
{
    static uint TimeMS();
};


struct HAL_TIM2
{
    static void Init(void (*func)());
};

