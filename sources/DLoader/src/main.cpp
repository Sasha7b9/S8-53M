// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Memory/Sector_.h"
#include "common/Utils/Containers/String_.h"
#include "main.h"
#include "FDrive/FDrive.h"
#include "Display/Display.h"
#include "common/Hardware/Timer_.h"
#include "Panel/Panel.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Settings/Settings.h"
#include <cstdlib>
#include <usbh_def.h>



#define FILE_NAME "S8-53M.bin"

USBH_HandleTypeDef hUSBHost;

void Upgrade();


float MainStruct::percentUpdate = 0.0f;
State::E MainStruct::state = State::NoDrive;


int main()
{
    Settings::Load();

    HAL::Init();
    
    Display::Init();

    Timer::Enable(TypeTimer::Temp, 1000 / 30, Display::Update);
    
    while(true)
    {
    }


    FDrive::Init();

    uint timeStart = TIME_MS;

    while (TIME_MS - timeStart < TIME_WAIT && (MainStruct::state != State::Mounted) && (MainStruct::state != State::WrongFlash))
    {
        FDrive::Update();
    }

    if (MainStruct::state == State::Mounted)                    // ��� ��������, ��� ���� ������ �������������
    {
        if (FDrive::FileExist(FILE_NAME))                       // ���� �� ����� ���������� ��������
        {
            Upgrade();

//            MainStruct::state = State::RequestAction;
//
//            while (1)
//            {
//                Key::E button = Panel::PressedButton();
//
//                if (button == Key::F1)
//                {
//                    MainStruct::state = State::Upgrade;
//                    Upgrade();
//                    break;
//                }
//                else if (button == Key::F5)
//                {
//                    MainStruct::state = State::Ok;
//                    break;
//                }
//            }
        }
        else
        {
            MainStruct::state = State::NotFile;
        }
    }
    else if (MainStruct::state == State::WrongFlash) // ���� �� ������� ��������������
    {
        HAL_TIM2::Delay(5000);
    }

    MainStruct::state = State::Ok;

//    Timer::Disable(TypeTimer::Temp);

    while (Display::IsRunning())
    {
    }

    HAL::DeInit();

    HAL::JumpToApplication();
    
    return 0;
}



void Upgrade()
{
    MainStruct::state = State::Upgrade;

    const int sizeSector = 1 * 1024;

    uint8 buffer[sizeSector];

    for (int sector = Sector::_05_FIRM_1; sector <= Sector::_09_FIRM_5; sector++)
    {
        Sector::Get((Sector::E)sector).Erase();
    }

    int size = FDrive::OpenFileForRead(FILE_NAME);
    int fullSize = size;
    uint address = 0x08020000U;

    while (size)
    {
        uint readedBytes = FDrive::ReadFromFile(sizeSector, buffer);

        HAL_ROM::WriteBufferBytes(address, buffer, readedBytes);

        size -= readedBytes;
        address += readedBytes;

        MainStruct::percentUpdate = 1.0F - (float)(size) / (float)(fullSize);
        
        Display::Update();
    }

    FDrive::CloseOpenedFile();
}


/*! \page page1 �������� ������ ����������
    \verbatim
    �� ������ 0x08000000 ��������� ���������.
    ����� ������ �� ��������� ������� ������.
        ���� ������ ����:
            ��������� ������� ����� S8-53.bin. ���� ������� �������, ������� ��������� "�� USB-����� ���������� ������ ����������� �����������. ����������?"
            ���� ������ "��":
                1. ��������� �������:
                    5 - 0x08020000
                    6 - 0x08040000
                    7 - 0x08060000
                2. �� �� ����� ������������ ���������� ����� S8-53M.bin
        ���� ������ �������������� �� �������:
            ������� ��������� "�� ������� �������������� ������. ���������, ��� �� ��� �������� ������� fat32"
    ����� ����������� ������� �� ������, ���������� � 0x0802004
    \endverbatim
*/
