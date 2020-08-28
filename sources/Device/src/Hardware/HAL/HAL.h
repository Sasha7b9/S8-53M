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

// АЦП рандомизатора
struct HAL_ADC3
{
    static void Init();
    static uint16 GetValue();
    static void *handle;    // ADC_HandleTypeDef
};

struct HAL_DAC
{
    static void *handle;    // DAC_HandleTypeDef
};

struct HAL_RTC
{
public:
    static void Init();
    static bool SetTimeAndData(int8 day, int8 month, int8 year, int8 hours, int8 minutes, int8 seconds);
    static PackedTime GetPackedTime();

private:
    static void *handle;    // RTC_HandleTypeDef
};

struct HAL_SPI1
{
    static void Init();
    static void *handle;    // SPI_HandleTypeDef
};

struct HAL_HCD
{
    static void Init();
    static void *handle;    // HCD_HandleTypeDef
};

struct HAL_USBH
{
    static void *handle;    // USBH_HandleTypeDef
};
