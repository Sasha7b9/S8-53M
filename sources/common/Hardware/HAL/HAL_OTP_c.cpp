#include "defines.h"
#include "common/Hardware/HAL/HAL_c.h"
#include <cstring>
#include <stm32f4xx_hal.h>


bool HAL_OTP::SaveSerialNumber(char *serialNumber) //-V2506
{
    // ������� ������ ������ ������ ������ 16 ���� � ������� OPT, ������� � ������.
    uint8 *address = reinterpret_cast<uint8 *>(FLASH_OTP_BASE); //-V566

    while ((*address) != 0xff &&                // *address != 0xff ��������, ��� ������ � ��� ������ ��� �������������
           address < reinterpret_cast<uint8 *>(FLASH_OTP_END) - 16) //-V566 //-V2563
    {
        address += 16; //-V2563
    }

    if (address < reinterpret_cast<uint8 *>(FLASH_OTP_END) - 16) //-V566 //-V2563
    {
        HAL_EPROM::WriteBufferBytes(reinterpret_cast<uint>(address), reinterpret_cast<uint8 *>(serialNumber), static_cast<int>(std::strlen(serialNumber)) + 1); //-V2513
        return true;
    }

    return false;
}


int HAL_OTP::GetSerialNumber(char buffer[17]) //-V2506
{
    // \todo �������� - ������ �������������� �������. ������ ������ ������ �������� ������� �����, ������� � ��� ��������, � �� 16, ��� ������.

    const int allShotsMAX = 512 / 16;   // ������������ ����� ������� � OPT ��������� ������.

    uint8 *address = reinterpret_cast<uint8 *>(FLASH_OTP_END) - 15; //-V566 //-V2563

    do
    {
        address -= 16; //-V2563
    } while ((*address == 0xff) && (address > reinterpret_cast<uint8 *>(FLASH_OTP_BASE))); //-V566

    if (*address == 0xff)   // �� ����� ������ � �����������, ����� �� ���������� ������ OTP
    {
        buffer[0] = 0;
        return allShotsMAX;
    }

    std::strcpy(buffer, reinterpret_cast<char *>(address)); //-V2513

    return allShotsMAX - (address - reinterpret_cast<uint8 *>(FLASH_OTP_BASE)) / 16 - 1; //-V566
}
