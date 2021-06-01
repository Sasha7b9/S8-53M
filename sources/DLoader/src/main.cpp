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



#define FILE_NAME "S8-53.bin"

MainStruct *MainStruct::ms = nullptr;

void Upgrade();



int main()
{
//    MainStruct::ms = (MainStruct *)(std::malloc(sizeof(MainStruct)));
//    MainStruct::ms->percentUpdate = 0.0F;

    HAL::Init();

    HAL_TIM2::Delay(250);
    
    HAL::DeInit();
    
    HAL::JumpToApplication();

    MainStruct::ms->state = State::Start;

    Display::Init();

    Timer::Enable(TypeTimer::Temp, 10, Display::Update);

    uint timeStart = TIME_MS;

    FDrive_Init();

    while (TIME_MS - timeStart < TIME_WAIT && !FDrive_Update())
    {
    }

        // ���� ����� ����������, �� � �������� ��������� ������-�� �� �������
    if (((MainStruct::ms->drive.connection != 0) && (MainStruct::ms->drive.active == 0)) ||
        // ��� ������� � �������� ���������, �� ������-�� �� ���������� ������� ������������
        ((MainStruct::ms->drive.active != 0) && (MainStruct::ms->state != State::Mount)))
    {
        std::free(MainStruct::ms);
        HAL::SystemReset();
        return 0;
    }

    if (MainStruct::ms->state == State::Mount)                           // ��� ��������, ��� ���� ������ �������������
    {
        if (FDrive_FileExist(FILE_NAME))                    // ���� �� ����� ���������� ��������
        {
            MainStruct::ms->state = State::RequestAction;

            while (1)
            {
                Key::E button = Panel::PressedButton();
                if (button == Key::F1)
                {
                    MainStruct::ms->state = State::Upgrade;
                    Upgrade();
                    break;
                }
                else if (button == Key::F5)
                {
                    MainStruct::ms->state = State::Ok;
                    break;
                }
            }
        }
        else
        {
            MainStruct::ms->state = State::NotFile;
        }
    }
    else if (MainStruct::ms->state == State::WrongFlash) // ���� �� ������� ��������������
    {
        HAL_TIM2::Delay(5000);
    }

    MainStruct::ms->state = State::Ok;

    Timer::Disable(TypeTimer::Temp);

    while (Display::IsRun())
    {
    }

    Display::Update();

    HAL::DeInit();

    std::free(MainStruct::ms);

    HAL::JumpToApplication();
    
    return 0;
}



void Upgrade()
{
    const int sizeSector = 1 * 1024;

    uint8 buffer[sizeSector];

    Sector::Get(Sector::_05_FIRM_1).Erase();

    int size = FDrive_OpenFileForRead(FILE_NAME);
    int fullSize = size;
    uint address = 0x08020000U;

    while (size)
    {
        uint readedBytes = FDrive_ReadFromFile(sizeSector, buffer);

        HAL_ROM::WriteBufferBytes(address, buffer, readedBytes);

        size -= readedBytes;
        address += readedBytes;

        MainStruct::ms->percentUpdate = 1.0F - (float)(size) / (float)(fullSize);
    }

    FDrive_CloseOpenedFile();
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
                2. �� �� ����� ������������ ���������� ����� S8-53.bin
        ���� ������ �������������� �� �������:
            ������� ��������� "�� ������� �������������� ������. ���������, ��� �� ��� �������� ������� fat32"
    ����� ����������� ������� �� ������, ���������� � 0x0802004
    \endverbatim
*/
