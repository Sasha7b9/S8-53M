#pragma once
#include "Hardware/HAL/HAL_PINS.h"


extern Pin pinG1;
extern Pin pinG2;
extern Pin pinG3;
extern Pin pinG5;
extern Pin pinG7;

extern Pin pinLED;
extern Pin pinDisplayReady;


struct HAL
{
    static void Init();
};

struct HAL_PINS
{
    static void Init();
};
