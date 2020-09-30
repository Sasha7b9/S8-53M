#include "defines.h"
#include "common/Hardware/HAL/HAL.h"
#include <cstring>
#include <stm32f4xx_hal.h>


bool HAL_OTP::SaveSerialNumber(char *serialNumber)
{
    // ������� ������ ������ ������ ������ 16 ���� � ������� OPT, ������� � ������.
    uint8 *address = (uint8 *)FLASH_OTP_BASE;

    while ((*address) != 0xff &&                // *address != 0xff ��������, ��� ������ � ��� ������ ��� �������������
           address < (uint8 *)FLASH_OTP_END - 16)
    {
        address += 16;
    }

    if (address < (uint8 *)FLASH_OTP_END - 16)
    {
        HAL_EPROM::WriteBufferBytes((uint)address, (uint8 *)serialNumber, static_cast<int>(std::strlen(serialNumber)) + 1);
        return true;
    }

    return false;
}


int HAL_OTP::GetSerialNumber(char buffer[17])
{
    // \todo �������� - ������ �������������� �������. ������ ������ ������ �������� ������� �����, ������� � ��� ��������, � �� 16, ��� ������.

    const int allShotsMAX = 512 / 16;   // ������������ ����� ������� � OPT ��������� ������.

    uint8 *address = (uint8 *)FLASH_OTP_END - 15;

    do
    {
        address -= 16;
    } while (*address == 0xff && address > (uint8 *)FLASH_OTP_BASE);

    if (*address == 0xff)   // �� ����� ������ � �����������, ����� �� ���������� ������ OTP
    {
        buffer[0] = 0;
        return allShotsMAX;
    }

    std::strcpy(buffer, (char *)address);

    return allShotsMAX - (address - (uint8 *)FLASH_OTP_BASE) / 16 - 1;
}
