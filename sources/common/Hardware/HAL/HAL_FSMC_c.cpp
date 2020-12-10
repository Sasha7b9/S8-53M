#include "defines.h"
#include "common/Hardware/HAL/HAL_c.h"
#include "Hardware/Hardware.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include "common/Hardware/Timer_c.h"
#include <stm32f4xx_hal.h>


static const uint ADDR_BANK = 0x60000000;


uint8 * const HAL_FSMC::ADDR_FPGA = (reinterpret_cast<uint8 *>(ADDR_BANK + 0x00c80000));  // ����� ������ � ���������� ��������. //-V566 //-V2571
uint8 * const HAL_FSMC::ADDR_NULL = (reinterpret_cast<uint8 *>(ADDR_BANK + 0x00a00000)); //-V566 //-V2571


static GPIO_InitTypeDef is =
{
    GPIO_PIN_0,
    GPIO_MODE_AF_PP,
    GPIO_PULLUP,
    GPIO_SPEED_FREQ_VERY_HIGH,
    GPIO_AF12_FMC
};


uint8 HAL_FSMC::Read(pUCHAR const address)
{
    return(*(address));
}


void HAL_FSMC::Write(uint8 * const address, uint8 value)
{
    *address = value;
}


void HAL_FSMC::Init()
{
    InitFPGA();

    InitRAM();
}


void HAL_FSMC::InitFPGA()
{
    static SRAM_HandleTypeDef gSramHandle =
    {
        FMC_NORSRAM_DEVICE, //-V2571
        FMC_NORSRAM_EXTENDED_DEVICE, //-V2571
        {
            FMC_NORSRAM_BANK1,                 // Init.NSBank
            FMC_DATA_ADDRESS_MUX_DISABLE,      // Init.DataAddressMux
            FMC_MEMORY_TYPE_NOR,               // Init.MemoryType
            FMC_NORSRAM_MEM_BUS_WIDTH_8,       // Init.MemoryDataWidth
            FMC_BURST_ACCESS_MODE_DISABLE,     // Init.BurstAccessMode
            FMC_WAIT_SIGNAL_POLARITY_LOW,      // Init.WaitSignalPolarity
            FMC_WRAP_MODE_DISABLE,             // Init.WrapMode
            FMC_WAIT_TIMING_BEFORE_WS,         // Init.WaitSignalActive
            FMC_WRITE_OPERATION_ENABLE,        // Init.WriteOperation
            FMC_WAIT_SIGNAL_DISABLE,           // Init.WaitSignal
            FMC_EXTENDED_MODE_DISABLE,         // Init.ExtendedMode
            FMC_ASYNCHRONOUS_WAIT_DISABLE,     // Init.AsynchronousWait
            FMC_WRITE_BURST_DISABLE            // Init.WriteBurst
        }
    };

    static const FMC_NORSRAM_TimingTypeDef sramTiming =
    {
        0,                 // FSMC_AddressSetupTime
        0,                 // FSMC_AddressHoldTime
        6,                 // FSMC_DataSetupTime   ��� �������� 9 32�� ������������ � RAM �� 1000���. ����������
                           // �� ���� ������ ��������� ���� �������� �� 90 ���. ���� 3 - 32�� ��������� �� 460 ���.
        0,                 // FSMC_BusTurnAroundDuration
        0,                 // FSMC_CLKDivision
        0,                 // FSMC_DataLatency
        FMC_ACCESS_MODE_C  // FSMC_AccessMode
    };

    SRAM_HandleTypeDef *hsram = &gSramHandle;
    FMC_NORSRAM_TimingTypeDef *Timing = const_cast<FMC_NORSRAM_TimingTypeDef *>(&sramTiming); //-V2567

    FMC_NORSRAM_Init(hsram->Instance, &(hsram->Init));

    FMC_NORSRAM_Timing_Init(hsram->Instance, Timing, hsram->Init.NSBank);

    hsram->Extended->BWTR[hsram->Init.NSBank] = 0x0FFFFFFFU;

    __FMC_NORSRAM_ENABLE(hsram->Instance, hsram->Init.NSBank);
}


void HAL_FSMC::InitRAM()
{
    static SRAM_HandleTypeDef gSramHandle =
    {
        FMC_NORSRAM_DEVICE, //-V2571
        FMC_NORSRAM_EXTENDED_DEVICE, //-V2571
        {
            FMC_NORSRAM_BANK2,                 // Init.NSBank
            FMC_DATA_ADDRESS_MUX_DISABLE,      // Init.DataAddressMux
            FMC_MEMORY_TYPE_NOR,               // Init.MemoryType
            FMC_NORSRAM_MEM_BUS_WIDTH_16,      // Init.MemoryDataWidth
            FMC_BURST_ACCESS_MODE_DISABLE,     // Init.BurstAccessMode
            FMC_WAIT_SIGNAL_POLARITY_LOW,      // Init.WaitSignalPolarity
            FMC_WRAP_MODE_DISABLE,             // Init.WrapMode
            FMC_WAIT_TIMING_BEFORE_WS,         // Init.WaitSignalActive
            FMC_WRITE_OPERATION_ENABLE,        // Init.WriteOperation
            FMC_WAIT_SIGNAL_DISABLE,           // Init.WaitSignal
            FMC_EXTENDED_MODE_DISABLE,         // Init.ExtendedMode
            FMC_ASYNCHRONOUS_WAIT_DISABLE,     // Init.AsynchronousWait
            FMC_WRITE_BURST_DISABLE            // Init.WriteBurst
        }
    };

    static const FMC_NORSRAM_TimingTypeDef sramTiming =
    {
        0,                 // FSMC_AddressSetupTime
        0,                 // FSMC_AddressHoldTime
        6,                 // FSMC_DataSetupTime   ��� �������� 9 32�� ������������ � RAM �� 1000���. ����������
                           // �� ���� ������ ��������� ���� �������� �� 90 ���. ���� 3 - 32�� ��������� �� 460 ���.
        0,                 // FSMC_BusTurnAroundDuration
        0,                 // FSMC_CLKDivision
        0,                 // FSMC_DataLatency
        FMC_ACCESS_MODE_C  // FSMC_AccessMode
    };

    FMC_NORSRAM_TimingTypeDef *timing = const_cast<FMC_NORSRAM_TimingTypeDef *>(&sramTiming); //-V2567

    HAL_SRAM_Init(&gSramHandle, timing, timing);
}
