#include "defines.h"
#include "common/Log.h"
#include "common/Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>

#ifdef LOADER
static bool SOUND_IS_BEEP = false;
#endif


void HAL_EPROM::ClearFlags()
{
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
}


void HAL_EPROM::EraseSector(uint numSector)
{
    ClearFlags();

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef flashITD;
    flashITD.TypeErase = TYPEERASE_SECTORS;
    flashITD.Sector = numSector;
    flashITD.NbSectors = 2;
    flashITD.VoltageRange = VOLTAGE_RANGE_3;

    uint error = 0;

    while (SOUND_IS_BEEP)
    {
    };

    HAL_FLASHEx_Erase(&flashITD, &error);

    HAL_FLASH_Lock();
}


uint HAL_EPROM::GetSector(uint startAddress)
{
    switch (startAddress)
    {
    case ADDR_SECTOR_DATA_MAIN:
        return FLASH_SECTOR_8;
    case ADDR_SECTOR_DATA_HELP:
        return FLASH_SECTOR_9;
    case ADDR_SECTOR_RESOURCES:
        return FLASH_SECTOR_10;
    case ADDR_SECTOR_SETTINGS:
        return FLASH_SECTOR_11;
    }
    LOG_ERROR("Недопустимый сектор");
    return FLASH_SECTOR_11;
}


void HAL_EPROM::WriteWord(uint address, uint word)
{
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

    HAL_FLASH_Unlock();
    if (HAL_FLASH_Program(TYPEPROGRAM_WORD, address, (uint64_t)word) != HAL_OK)
    {
        LOG_ERROR("Не могу записать в память");
    }
    HAL_FLASH_Lock();
}


void HAL_EPROM::WriteBufferBytes(uint address, uint8 *buffer, int size)
{
    HAL_FLASH_Unlock();
    for (int i = 0; i < size; i++)
    {
        if (HAL_FLASH_Program(TYPEPROGRAM_BYTE, address, (uint64_t)(buffer[i])) != HAL_OK)
        {
            HAL::Error();
        }
        address++;
    }
    HAL_FLASH_Lock();
}
