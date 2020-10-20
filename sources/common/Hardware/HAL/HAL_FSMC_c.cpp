#include "defines.h"
#include "common/Hardware/HAL/HAL_c.h"
#include "Hardware/Hardware.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include "common/Hardware/Timer_c.h"
#include <stm32f4xx_hal.h>


static const uint ADDR_BANK = 0x60000000;


uint8 * const HAL_FSMC::ADDR_DISPLAY_A0 = (reinterpret_cast<uint8 *>(ADDR_BANK + 0x00910000)); //-V566
uint8 * const HAL_FSMC::ADDR_DISPLAY_D7_D0 = (reinterpret_cast<uint8 *>(ADDR_BANK + 0x00900000)); //-V566
uint8 * const HAL_FSMC::ADDR_CDISPLAY = (reinterpret_cast<uint8 *>(ADDR_BANK + 0x00800000)); //-V566
uint8 * const HAL_FSMC::ADDR_FPGA = (reinterpret_cast<uint8 *>(ADDR_BANK + 0x00c80000));  // Адрес записи в аппаратные регистры. //-V566
uint8 * const HAL_FSMC::ADDR_NULL = (reinterpret_cast<uint8 *>(ADDR_BANK + 0x00a00000)); //-V566



/*
static const char *addrNamesForWrite[32] =
{
    "WR_START",         // 0x00
    "NONE",             // 0x01
    "NONE",             // 0x02
    "NONE",             // 0x03
    "WR_RAZV",          // 0x04
    "WR_PRED_LO",       // 0x05
    "WR_PRED_HI",       // 0x06
    "WR_POST_LO",
    "WR_POST_HI",
    "WR_TRIG_F",
    "NONE",
    "NONE",
    "WR_CAL_1",
    "WR_CAL_2",
    "WR_UPR",
    "WR_STOP",
    "RD_ADC_A2",
    "RD_ADC_A1",
    "RD_ADC_B2",
    "RD_ADC_B1",
    "RD_RAND_LOW",
    "RD_RAND_HI",
    "RD_FL",
    "RD_LAST_RECORD_LOW",       // 0x17
    "RD_LAST_RECORD_HI",        // 0x18
    "RD_ADDR_PERIOD_LOW_LOW",   // 0x19
    "RD_ADDR_PERIOD_LOW",       // 0x1a
    "RD_ADDR_PERIOD_MID_WR_ADD_RSHIFT_DAC2",    // 0x1b
    "RD_ADDR_PERIOD_HI_WR_ADD_RSHIFT_DAC1",     // 0x1c
    "RD_ADDR_FREQ_LOW",         // 0x1d
    "RD_ADDR_FREQ_MID",         // 0x1e
    "RD_ADDR_FREQ_HI"           // 0x1f
};
*/


uint8 FSMC_Read(uint8 *address);
void FSMC_Write(uint8 *address, uint8 value);



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
//    static const FSMC_NORSRAM_TimingTypeDef sramTiming =
//    {
//        5,                  // FSMC_AddressSetupTime
//        7,                  // FSMC_AddressHoldTime
//        13,                 // FSMC_DataSetupTime
//        11,                 // FSMC_BusTurnAroundDuration
//        3,                  // FSMC_CLKDivision
//        0,                  // FSMC_DataLatency
//        FSMC_ACCESS_MODE_B  // FSMC_AccessMode
//    };
//
//    static const SRAM_HandleTypeDef sramHandle =
//    {
//        FSMC_NORSRAM_DEVICE,
//        FSMC_NORSRAM_EXTENDED_DEVICE,
//        {
//            FSMC_NORSRAM_BANK1,                 // Init.NSBank
//            FSMC_DATA_ADDRESS_MUX_ENABLE,       // Init.DataAddressMux
//            FSMC_MEMORY_TYPE_NOR,               // Init.MemoryType
//            FSMC_NORSRAM_MEM_BUS_WIDTH_8,       // Init.MemoryDataWidth
//            FSMC_BURST_ACCESS_MODE_DISABLE,     // Init.BurstAccessMode
//            FSMC_WAIT_SIGNAL_POLARITY_LOW,      // Init.WaitSignalPolarity
//            FSMC_WRAP_MODE_DISABLE,             // Init.WrapMode
//            FSMC_WAIT_TIMING_BEFORE_WS,         // Init.WaitSignalActive
//            FSMC_WRITE_OPERATION_ENABLE,        // Init.WriteOperation
//            FSMC_WAIT_SIGNAL_DISABLE,           // Init.WaitSignal
//            FSMC_EXTENDED_MODE_DISABLE,         // Init.ExtendedMode
//            FSMC_ASYNCHRONOUS_WAIT_DISABLE,     // Init.AsynchronousWait
//            FSMC_WRITE_BURST_DISABLE            // Init.WriteBurst
//        }
//    };
//
//    HAL_SRAM_Init((SRAM_HandleTypeDef*)(&sramHandle), (FSMC_NORSRAM_TimingTypeDef*)(&sramTiming), (FSMC_NORSRAM_TimingTypeDef*)(&sramTiming));
}
