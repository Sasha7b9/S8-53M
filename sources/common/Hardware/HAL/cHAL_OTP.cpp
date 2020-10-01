#include "defines.h"
#include "common/Hardware/HAL/HAL.h"
#include <cstring>
#include <stm32f4xx_hal.h>


bool HAL_OTP::SaveSerialNumber(char *serialNumber)
{
    // Ќаходим первую пустую строку длиной 16 байт в области OPT, начина€ с начала.
    uint8 *address = reinterpret_cast<uint8 *>(FLASH_OTP_BASE); //-V566

    while ((*address) != 0xff &&                // *address != 0xff означает, что запись в эту строку уже производилась
           address < reinterpret_cast<uint8 *>(FLASH_OTP_END) - 16) //-V566
    {
        address += 16;
    }

    if (address < reinterpret_cast<uint8 *>(FLASH_OTP_END) - 16) //-V566
    {
        HAL_EPROM::WriteBufferBytes(reinterpret_cast<uint>(address), reinterpret_cast<uint8 *>(serialNumber), static_cast<int>(std::strlen(serialNumber)) + 1);
        return true;
    }

    return false;
}


int HAL_OTP::GetSerialNumber(char buffer[17])
{
    // \todo улучшить - нельз€ разбрасыватьс€ байтами.  ажда€ запись должна занимать столько места, сколько в ней символов, а не 16, как сейчас.

    const int allShotsMAX = 512 / 16;   // ћаксимальное число записей в OPT серийного номера.

    uint8 *address = reinterpret_cast<uint8 *>(FLASH_OTP_END) - 15; //-V566

    do
    {
        address -= 16;
    } while ((*address == 0xff) && (address > reinterpret_cast<uint8 *>(FLASH_OTP_BASE))); //-V566

    if (*address == 0xff)   // Ќе нашли строки с информацией, дойд€ до начального адреса OTP
    {
        buffer[0] = 0;
        return allShotsMAX;
    }

    std::strcpy(buffer, reinterpret_cast<char *>(address));

    return allShotsMAX - (address - reinterpret_cast<uint8 *>(FLASH_OTP_BASE)) / 16 - 1; //-V566
}
