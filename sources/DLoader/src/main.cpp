#include "defines.h"
#include "main.h"
#include "FDrive/FDrive.h"
#include "Hardware/Hardware.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#include "Display/Display.h"
#include "Hardware/Timer.h"
#include "Panel/Panel.h"
#include "common/Hardware/HAL/HAL_c.h"
#include <cstdlib>



#define FILE_NAME "S8-53.bin"

MainStruct *ms; //-V707

void Upgrade(void);



int main(void)
{
    ms = static_cast<MainStruct *>(malloc(sizeof(MainStruct)));
    ms->percentUpdate = 0.0F; //-V522

    Hardware_Init();

    Settings_Load();

    Timer_PauseOnTime(250);

    ms->state = State::Start;

    Display::Init();

    Timer_Enable(TypeTimer::Temp, 10, Display::Update);

    uint timeStart = HAL_TIM2::TimeMS();

    FDrive_Init();

    while (HAL_TIM2::TimeMS() - timeStart < TIME_WAIT && !FDrive_Update())
    {
    }

    if (((ms->drive.connection != 0) && (ms->drive.active == 0)) ||  // ���� ����� ����������, �� � �������� ��������� ������-�� �� �������
        ((ms->drive.active != 0) && (ms->state != State::Mount)))     // ��� ������� � �������� ���������, �� ������-�� �� ���������� ������� ������������
    {
        free(ms);
        HAL::SystemReset();
    }

    if (ms->state == State::Mount)                           // ��� ��������, ��� ���� ������ �������������
    {
        if (FDrive_FileExist(FILE_NAME))                    // ���� �� ����� ���������� ��������
        {
            ms->state = State::RequestAction;

            while (1) //-V2530
            {
                Key::E button = Panel::PressedButton();
                if (button == Key::F1)
                {
                    ms->state = State::Upgrade;
                    Upgrade();
                    break;
                }
                else if (button == Key::F5)
                {
                    ms->state = State::Ok;
                    break;
                }
            }
        }
        else
        {
            ms->state = State::NotFile;
        }
    }
    else if (ms->state == State::WrongFlash) // ���� �� ������� ��������������
    {
        Timer_PauseOnTime(5000);
    }

    ms->state = State::Ok;

    HAL_SPI1::DeInit();

    Timer_Disable(TypeTimer::Temp);

    while (Display::IsRun())
    {
    }

    Display::Update();

    HAL::DeInit();

    free(ms);

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

        ms->percentUpdate = 1.0F - static_cast<float>(size) / static_cast<float>(fullSize);
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
