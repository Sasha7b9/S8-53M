// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Hardware/HAL/HAL_PINS_.h"
#include "common/Hardware/Memory/Sector_.h"
#include "common/Utils/Containers/Values_.h"


struct PackedTime
{
    uint hours : 5;
    uint minutes : 6;
    uint seconds : 6;
    uint year : 7;
    uint month : 4;
    uint day : 5;
};

/**
  * TIME_TICKS - количество тиков, прошедших с момента последнего вызова функции Timer_StartMultiMeasurement().
  * В одной секунде 120.000.000 тиков. Максимальный отрезок времени, который можно отсчитать с её помощью - 35 сек.
  * Количество тиков, прошедших с момента последнего вызова функции Timer_StartMultiMeasurement(). Не более (1 << 32).
***/
#define TIME_TICKS HAL_TIM2::GetTicks()
#define TIME_MS    HAL_TIM::TimeMS()


#define ERROR_HANDLER() HAL::ErrorHandler(__FILE__, __LINE__)


struct HAL
{
    static void Init();
    static void DeInit();
    static void SystemReset();
    // Функция используется в загрузчике для перехода на основную программу
    static void JumpToApplication();
    static void ErrorHandler(pchar file, int line);
private:
    static void EnablePeripherals();
    static void ConfigSystemClock();
};


// АЦП рандомизатора
struct HAL_ADC1
{
    static void Init();
    static Uint16 GetValue();
    static void StartConversion();

    // Вызывается после каждого запуска
    static void ResetValue();
};


struct HAL_CRC
{
    static void Init();

    static uint Calculate(uint address = 0x08020000, uint numBytes = 128 * 1024 * 3 / 4);

    // Расчитывает контрольную сумму последовательности байт из buffer. Если size не кратно четырём, последние байты не
    // участвуют в вычислении контрольной суммы
    static uint Calculate8bit(uint8 *buffer, int size);
};


// Используется для управления яркостью дисплея
struct HAL_DAC1
{
    static void Init();
    // Значение value от 0 до 100
    static void SetValue(int value);
};


// Динамик
struct HAL_DAC2
{
    static void Init();
    static void StartDMA(uint8 *points, int numPoints);
    static void StopDMA();
    static void *handle;                                        // DAC_HandleTypeDef
};


struct HAL_ETH
{
    static void Init();
};


// Используется для связи с FPGA и RAM
struct HAL_FMC 
{
    static void Init();
    inline static uint16 Read(uint16 * const address)               { return *address; };

    static void Write(uint16 *const address, uint16 value);

private:
    
    static void InitRAM();
    static void InitFPGA();

public:

    static uint16 *const ADDR_FPGA;                 // Адрес записи в альтеру

    static uint16 *const ADDR_RAM_DISPLAY_FRONT;    // Видимое изображение дисплея. Находится в RAM и не может исполь-
                                                    // зоваться для других целей
    static uint8 *const ADDR_RAM_BEGIN;             // Первый свободный адрес в RAM.
    static uint8 *const ADDR_RAM_END;               // Указывает на первый недопустимый адрес RAM. Использовать можно
                                                    // адреса [ADDR_RAM_BEGIN...(ADDR_RAM_END - 1)]
};


struct HAL_HCD
{
    static void Init();
    static void *handle;    // HCD_HandleTypeDef
};


struct HAL_LTDC
{
    static void Init(uint8 *frontBuffer, uint8 *backBuffer);

    static void LoadPalette();

    static void ToggleBuffers();

private:

    static void SetBuffers(uint8 *frontBuffer, uint8 *backBuffer);
};


struct HAL_OTP
{
    static bool SaveSerialNumber(char *serialNumber);

    // Функция возвращает число свободных мест для записи. Если 0, то места в OTP уже не осталось. 
    static int GetSerialNumber(char buffer[17]);
};


struct HAL_PCD
{
    static void *handle;    // PCD_HandleTypeDef
};


struct HAL_ROM
{
    static const Sector sectors[Sector::Count];

    static void Init();
    static void WriteBytes(uint address, const uint8 *data, uint size);
    static void WriteBufferBytes(uint address, const void *data, uint size);
    static void Fill(uint address, uint8 value, int size);
};


struct HAL_RTC
{
    static void Init();

    static bool SetTimeAndData(int8 day, int8 month, int8 year, int8 hours, int8 minutes, int8 seconds);

    static PackedTime GetPackedTime();

private:
    static void *handle;    // RTC_HandleTypeDef
};


struct HAL_SPI1
{
public:

    static void Init();
};


// Для связи с панелью
struct HAL_SPI5
{
public:

    static void Init();

    static void *handle;       // SPI_HandleTypeDef
};


struct HAL_TIM
{
    static uint TimeMS();
};


struct HAL_TIM2
{
    static void Init();

    static uint GetTicks();

    // Запускает счётчик для измерения малых отрезков времени.
    static void StartMultiMeasurement();
    
    // Задержка на timeMS миллисекунд
    static void Delay(uint timeMS);
};


struct HAL_TIM6
{
    static void Init();
};


// Вспомогательный таймер для DAC2
struct HAL_TIM7
{
    static void Init();

    static void Config(uint16 prescaler, uint16 period);
};


struct HAL_USBH
{
    static void *handle;    // USBH_HandleTypeDef
};
