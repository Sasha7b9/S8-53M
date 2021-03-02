#pragma once
#include "common/Hardware/HAL/HAL_PINS_.h"


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
  * gTimerTics - количество тиков, прошедших с момента последнего вызова функции Timer_StartMultiMeasurement().
  * В одной секунде 120.000.000 тиков. Максимальный отрезок времени, который можно отсчитать с её помощью - 35 сек.
  * Количество тиков, прошедших с момента последнего вызова функции Timer_StartMultiMeasurement(). Не более (1 << 32).
***/
#define gTimerTics HAL_TIM2::GetTicks()
#define gTimerMS   HAL_TIM2::TimeMS()


// Программа и константные данные
#define ADDR_FLASH_SECTOR_0     ((uint)0x08000000)  // Base @ of Sector 0, 16 Kbytes
#define ADDR_FLASH_SECTOR_1     ((uint)0x08004000)  // Base @ of Sector 1, 16 Kbytes
#define ADDR_FLASH_SECTOR_2     ((uint)0x08008000)  // Base @ of Sector 2, 16 Kbytes
#define ADDR_FLASH_SECTOR_3     ((uint)0x0800C000)  // Base @ of Sector 3, 16 Kbytes
#define ADDR_FLASH_SECTOR_4     ((uint)0x08010000)  // Base @ of Sector 4, 64 Kbytes
#define ADDR_FLASH_SECTOR_5     ((uint)0x08020000)  // Base @ of Sector 5, 128 Kbytes  |
#define ADDR_FLASH_SECTOR_6     ((uint)0x08040000)  // Base @ of Sector 6, 128 Kbytes  | Основная прошивка
#define ADDR_FLASH_SECTOR_7     ((uint)0x08060000)  // Base @ of Sector 7, 128 Kbytes  /

// Сохранённые данные
#define ADDR_SECTOR_DATA_MAIN   ((uint)0x08080000)  // Base @ of Sector 8, 128 Kbytes
#define ADDR_SECTOR_DATA_HELP   ((uint)0x080A0000)  // Base @ of Sector 9, 128 Kbytes

// Графические и звуковые ресурсы
#define ADDR_SECTOR_RESOURCES   ((uint)0x080C0000)  // Base @ of Sector 10, 128 Kbytes

// Настройки
#define ADDR_SECTOR_SETTINGS    ((uint)0x080E0000)  // Base @ of Sector 11, 128 Kbytes
#define SIZE_SECTOR_SETTINGS    (128 * 1024)


#define ERROR_HANDLER() HAL::ErrorHandler(__FILE__, __LINE__)


struct HAL
{
    static void Init();
    static void DeInit();
    static void SystemReset();
    // Функция используется в загрузчике для перехода на основную программу
    static void JumpToApplication();
    static void ErrorHandler(const char *file, int line);
private:
    static void EnablePeripherals();
    static void ConfigSystemClock();
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


// Используется для управления яркостью дисплея
struct HAL_DAC1
{
    static void Init();
    // Значение value от 0 до 100
    static void SetValue(int value);
};


// Для звука
struct HAL_DAC2
{
    static void Init();
    static void StartDMA(uint8 *points, int numPoints);
    static void StopDMA();
    static void *handle;    // DAC_HandleTypeDef
};


struct HAL_EPROM
{
    static void ClearFlags();
    static void EraseSector(uint numSector);
    static uint GetSector(uint startAddress);
    static void WriteWord(uint address, uint word);
    static void WriteBufferBytes(uint address, pUCHAR buffer, int size);
};


struct HAL_ETH
{
    static void Init();
};


struct HAL_FMC 
{
    static uint8 * const ADDR_FPGA;     // Адрес записи в альтеру
    static uint8 * const ADDR_RAM;      // Адрес записи в RAM
    static uint8 * const ADDR_NULL;

    static void    Init(void);

    static uint8   Read(pUCHAR const address);

    static void    Write(uint8 * const address, uint8 value);

private:
    
    static void InitRAM();
    static void InitFPGA();
};


struct HAL_LTDC
{
    static void Init(uint8 *frontBuffer, uint8 *backBuffer);

    static void LoadPalette();

    static void ToggleBuffers();

    static void *handle;            // LTDC_HandleTypeDef

private:

    static void SetBuffers(uint8 *frontBuffer, uint8 *backBuffer);
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


struct HAL_HCD
{
    static void Init();
    static void *handle;    // HCD_HandleTypeDef
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

struct HAL_USBD
{
    static void Init();
    static bool PrevSendingComplete();
    static void Transmit(uint8 *buffer, int size);
    static void Flush(uint8 *buffer, int sizeBuffer);
    static void Wait();
    static void *handle;    // USBD_HandleTypeDef
};

struct HAL_USBH
{
    static void *handle;    // USBH_HandleTypeDef
};
