#pragma once
#include "Hardware/HAL/HAL_PINS.h"


extern Pin pinG1;
extern Pin pinG2;
extern Pin pinG3;
extern Pin pinG5;
extern Pin pinG7;

extern Pin pinLED;
extern Pin pinDisplayReady;


/**
  * gTimerTics - количество тиков, прошедших с момента последнего вызова функции Timer_StartMultiMeasurement().
  * В одной секунде 120.000.000 тиков. Максимальный отрезок времени, который можно отсчитать с её помощью - 35 сек.
  * Количество тиков, прошедших с момента последнего вызова функции Timer_StartMultiMeasurement(). Не более (1 << 32).
***/
#define gTimerTics HAL_TIM2::GetTicks()
#define gTimerMS   HAL_TIM2::TimeMS()


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

struct HAL_CRC
{
    static void Init();
    static uint Calculate(uint address = 0x08020000, uint numBytes = 128 * 1024 * 3 / 4);
};

struct HAL_DAC
{
    static void Init();
    static void StartDMA(uint8 *points, int numPoints);
    static void StopDMA();
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

struct HAL_TIM2
{
    static void Init();

    static uint GetTicks();

    static uint TimeMS();
    
    // Запускает счётчик для измерения малых отрезков времени.
    static void StartMultiMeasurement();
    
    // Задержка на timeMS миллисекунд
    static void Delay(uint timeMS);
};

struct HAL_TIM6
{
    static void Init();
};

struct HAL_TIM7
{
    static void Config(uint16 prescaler, uint16 period);
};

struct HAL_USBH
{
    static void *handle;    // USBH_HandleTypeDef
};
