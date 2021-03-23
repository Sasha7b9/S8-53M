#include "defines.h"
#include "common/Log_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/HAL/HAL_.h"
#include <stm32f4xx_hal.h>

#ifdef LOADER
static volatile bool SOUND_IS_BEEP = false;
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

    while (Sound::isBeep)
    {
    };

    HAL_FLASHEx_Erase(&flashITD, &error);

    HAL_FLASH_Lock();
}


uint HAL_EPROM::GetSector(uint startAddress)
{
    uint result = FLASH_SECTOR_11;
    
    switch (startAddress)
    {
    case ADDR_SECTOR_DATA_MAIN: result = FLASH_SECTOR_8;  break;
    case ADDR_SECTOR_DATA_HELP: result = FLASH_SECTOR_9;  break;
    case ADDR_SECTOR_RESOURCES: result = FLASH_SECTOR_10; break;
    case ADDR_SECTOR_SETTINGS:                            break;
    }
    LOG_ERROR("������������ ������");
    
    return result;
}


void HAL_EPROM::WriteWord(uint address, uint word)
{
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

    HAL_FLASH_Unlock();
    if (HAL_FLASH_Program(TYPEPROGRAM_WORD, address, (uint64_t)(word)) != HAL_OK)
    {
        LOG_ERROR("�� ���� �������� � ������");
    }
    HAL_FLASH_Lock();
}


void HAL_EPROM::WriteBufferBytes(uint address, puchar buffer, uint size)
{
    HAL_FLASH_Unlock();

    for (uint i = 0; i < size; i++)
    {
        if (HAL_FLASH_Program(TYPEPROGRAM_BYTE, address, (uint64_t)(buffer[i])) != HAL_OK)
        {
            ERROR_HANDLER();
        }
        address++;
    }

    HAL_FLASH_Lock();
}
