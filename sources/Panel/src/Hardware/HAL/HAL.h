#pragma once
#include "Hardware/HAL/HAL_PINS.h"


#define TIME_MS HAL_TIM::TimeMS()       /* Количество миллисекунд с момента запуска */

extern PinSL0 pinSL0;
extern PinSL1 pinSL1;
extern PinSL2 pinSL2;
extern PinSL3 pinSL3;
extern PinSL4 pinSL4;
extern PinSL5 pinSL5;

extern PinRL0 pinRL0;
extern PinRL1 pinRL1;
extern PinRL2 pinRL2;
extern PinRL3 pinRL3;
extern PinRL4 pinRL4;
extern PinRL5 pinRL5;
extern PinRL6 pinRL6;
extern PinRL7 pinRL7;

extern PinRC0 pinRC0;
extern PinRC1 pinRC1;
extern PinRC2 pinRC2;
extern PinRC3 pinRC3;

extern PinON pinON;


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

    static void Send(uint8 *data, int size);
};

struct HAL_TIM
{
    static uint TimeMS();
};
