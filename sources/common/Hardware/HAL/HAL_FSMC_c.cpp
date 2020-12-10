#include "defines.h"
#include "common/Hardware/HAL/HAL_c.h"
#include "Hardware/Hardware.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include "common/Hardware/Timer_c.h"
#include <stm32f4xx_hal.h>


static const uint ADDR_BANK = 0x60000000;


uint8 * const HAL_FSMC::ADDR_FPGA = (reinterpret_cast<uint8 *>(ADDR_BANK + 0x00c80000));  // Адрес записи в аппаратные регистры. //-V566 //-V2571
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
    InitRAM();
    InitFPGA();
}


void HAL_FSMC::InitRAM()
{
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    //           A16           A17           A18 
    is.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
    HAL_GPIO_Init(GPIOD, &is);

    //           A10          A11         A12           A13          A14         A15           NE3
    is.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOG, &is);

    //           A6            A7          A8            A9
    is.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOF, &is);

    static SRAM_HandleTypeDef gSramHandle =
    {
        FMC_NORSRAM_DEVICE,
        FMC_NORSRAM_EXTENDED_DEVICE,
        {
            FMC_NORSRAM_BANK3,                 // Init.NSBank
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
        6,                 // FSMC_DataSetupTime   При значении 9 32кБ записываются в RAM за 1000мкс. Уменьшение
                           // на одну единцу уменьшает этот параметр на 90 мкс. Если 3 - 32кБ запишутся за 460 мкс.
        0,                 // FSMC_BusTurnAroundDuration
        0,                 // FSMC_CLKDivision
        0,                 // FSMC_DataLatency
        FMC_ACCESS_MODE_C  // FSMC_AccessMode
    };

    FMC_NORSRAM_TimingTypeDef *timing = const_cast<FMC_NORSRAM_TimingTypeDef *>(&sramTiming);

    HAL_SRAM_Init(&gSramHandle, timing, timing);
}


void HAL_FSMC::InitFPGA()
{

}
