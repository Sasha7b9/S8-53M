#include "defines.h"
#include "main.h"
#include "FDrive/FDrive.h"
#include "Hardware/Hardware.h"
#include "Settings/Settings.h"
#include "Display/Display.h"
#include "common/Hardware/Timer_c.h"
#include "Panel/Panel.h"
#include "common/Hardware/HAL/HAL_c.h"
#include <cstdlib>



#define FILE_NAME "S8-53.bin"

MainStruct *MainStruct::ms = nullptr;

void Upgrade(void);



int main(void)
{
    MainStruct::ms = static_cast<MainStruct *>(malloc(sizeof(MainStruct)));
    MainStruct::ms->percentUpdate = 0.0F; //-V522

    Hardware_Init();

    Settings_Load();

    HAL_TIM2::Delay(250);

    MainStruct::ms->state = State::Start;

    Display::Init();

    Timer::Enable(TypeTimer::Temp, 10, Display::Update);

    uint timeStart = HAL_TIM2::TimeMS();

    FDrive_Init();

    while (HAL_TIM2::TimeMS() - timeStart < TIME_WAIT && !FDrive_Update())
    {
    }

    if (((MainStruct::ms->drive.connection != 0) && (MainStruct::ms->drive.active == 0)) ||  // ���� ����� ����������, �� � �������� ��������� ������-�� �� �������
        ((MainStruct::ms->drive.active != 0) && (MainStruct::ms->state != State::Mount)))     // ��� ������� � �������� ���������, �� ������-�� �� ���������� ������� ������������
    {
        free(MainStruct::ms);
        HAL::SystemReset();
    }

    if (MainStruct::ms->state == State::Mount)                           // ��� ��������, ��� ���� ������ �������������
    {
        if (FDrive_FileExist(FILE_NAME))                    // ���� �� ����� ���������� ��������
        {
            MainStruct::ms->state = State::RequestAction;

            while (1) //-V2530
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

    free(MainStruct::ms);

    HAL::JumpToApplication();
    
    return 0;
}



void Upgrade(void)
{
    const int sizeSector = 1 * 1024;

    uint8 buffer[sizeSector];

    HAL_EPROM::EraseSector(5);

    int size = FDrive_OpenFileForRead(FILE_NAME);
    int fullSize = size;
    uint address = 0x08020000U;

    while (size)
    {
        int readedBytes = FDrive_ReadFromFile(sizeSector, buffer);
        HAL_EPROM::WriteBufferBytes(address, buffer, readedBytes);
        size -= readedBytes;
        address += readedBytes;

        MainStruct::ms->percentUpdate = 1.0F - static_cast<float>(size) / static_cast<float>(fullSize);
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
