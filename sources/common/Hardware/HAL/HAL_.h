#pragma once
#include "common/Hardware/HAL/HAL_PINS_.h"
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
  * gTimerTics - ���������� �����, ��������� � ������� ���������� ������ ������� Timer_StartMultiMeasurement().
  * � ����� ������� 120.000.000 �����. ������������ ������� �������, ������� ����� ��������� � � ������� - 35 ���.
  * ���������� �����, ��������� � ������� ���������� ������ ������� Timer_StartMultiMeasurement(). �� ����� (1 << 32).
***/
#define gTimerTics HAL_TIM2::GetTicks()
#define TIME_MS    HAL_TIM::TimeMS()


// ��������� � ����������� ������
#define ADDR_FLASH_SECTOR_0     ((uint)0x08000000)  // Base @ of Sector 0, 16 Kbytes
#define ADDR_FLASH_SECTOR_1     ((uint)0x08004000)  // Base @ of Sector 1, 16 Kbytes
#define ADDR_FLASH_SECTOR_2     ((uint)0x08008000)  // Base @ of Sector 2, 16 Kbytes
#define ADDR_FLASH_SECTOR_3     ((uint)0x0800C000)  // Base @ of Sector 3, 16 Kbytes
#define ADDR_FLASH_SECTOR_4     ((uint)0x08010000)  // Base @ of Sector 4, 64 Kbytes
#define ADDR_FLASH_SECTOR_5     ((uint)0x08020000)  // Base @ of Sector 5, 128 Kbytes  |
#define ADDR_FLASH_SECTOR_6     ((uint)0x08040000)  // Base @ of Sector 6, 128 Kbytes  | �������� ��������
#define ADDR_FLASH_SECTOR_7     ((uint)0x08060000)  // Base @ of Sector 7, 128 Kbytes  /

// ����������� ������
#define ADDR_SECTOR_DATA_MAIN   ((uint)0x08080000)  // Base @ of Sector 8, 128 Kbytes
#define ADDR_SECTOR_DATA_HELP   ((uint)0x080A0000)  // Base @ of Sector 9, 128 Kbytes

// ����������� � �������� �������
#define ADDR_SECTOR_RESOURCES   ((uint)0x080C0000)  // Base @ of Sector 10, 128 Kbytes

// ���������
#define ADDR_SECTOR_SETTINGS    ((uint)0x080E0000)  // Base @ of Sector 11, 128 Kbytes
#define SIZE_SECTOR_SETTINGS    (128 * 1024)


#define ERROR_HANDLER() HAL::ErrorHandler(__FILE__, __LINE__)


struct HAL
{
    static void Init();
    static void DeInit();
    static void SystemReset();
    // ������� ������������ � ���������� ��� �������� �� �������� ���������
    static void JumpToApplication();
    static void ErrorHandler(pchar file, int line);
private:
    static void EnablePeripherals();
    static void ConfigSystemClock();
};


// ��� �������������
struct HAL_ADC1
{
    static void Init();
    static Uint16 GetValue();
    static void StartConversion();

    // ���������� ����� ������� �������
    static void ResetValue();
};


struct HAL_CRC
{
    static void Init();
    static uint Calculate(uint address = 0x08020000, uint numBytes = 128 * 1024 * 3 / 4);
};


// ������������ ��� ���������� �������� �������
struct HAL_DAC1
{
    static void Init();
    // �������� value �� 0 �� 100
    static void SetValue(int value);
};


// �������
struct HAL_DAC2
{
    static void Init();
    static void StartDMA(uint8 *points, int numPoints);
    static void StopDMA();
    static void *handle;                                        // DAC_HandleTypeDef
};


struct HAL_EPROM
{
    static void ClearFlags();
    static void EraseSector(uint numSector);
    static uint GetSector(uint startAddress);
    static void WriteWord(uint address, uint word);
    static void WriteBufferBytes(uint address, puchar buffer, uint size);
};


struct HAL_ETH
{
    static void Init();
};


// ������������ ��� ����� � FPGA � RAM
struct HAL_FMC 
{
    static void Init();
    inline static uint16 Read(uint16 * const address)               { return *address; };
    static void Write(uint16 * const address, uint16 value);

private:
    
    static void InitRAM();
    static void InitFPGA();

public:

    static uint16 *const ADDR_FPGA;                 // ����� ������ � �������

    static uint16 *const ADDR_RAM_DISPLAY_FRONT;    // ������� ����������� �������. ��������� � RAM � �� ����� ������-
                                                    // �������� ��� ������ �����
    static uint8 *const ADDR_RAM_BEGIN;             // ������ ��������� ����� � RAM.
    static uint8 *const ADDR_RAM_END;               // ��������� �� ������ ������������ ����� RAM. ������������ �����
                                                    // ������ [ADDR_RAM_BEGIN...(ADDR_RAM_END - 1)]
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

    // ������� ���������� ����� ��������� ���� ��� ������. ���� 0, �� ����� � OTP ��� �� ��������. 
    static int GetSerialNumber(char buffer[17]);
};


struct HAL_PCD
{
    static void *handle;    // PCD_HandleTypeDef
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


// ��� ����� � �������
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

    // ��������� ������� ��� ��������� ����� �������� �������.
    static void StartMultiMeasurement();
    
    // �������� �� timeMS �����������
    static void Delay(uint timeMS);
};


struct HAL_TIM6
{
    static void Init();
};


// ��������������� ������ ��� DAC2
struct HAL_TIM7
{
    static void Init();

    static void Config(uint16 prescaler, uint16 period);
};


struct HAL_USBD
{
    static void Init();
    static bool PrevSendingComplete();
    static void Transmit(uint8 *buffer, uint size);
    static void Flush(uint8 *buffer, uint sizeBuffer);
    static void Wait();
    static void *handle;    // USBD_HandleTypeDef
};

struct HAL_USBH
{
    static void *handle;    // USBH_HandleTypeDef
};
