#include "defines.h"
#include "main.h"
#include "FDrive/FDrive.h"
#include "Hardware/Hardware.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#include "Display/Display.h"
#include "Hardware/Timer.h"
#include "Panel/Panel.h"
#include "Hardware/FLASH.h"
#include "common/Hardware/HAL/HAL.h"
#include <cstdlib>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FILE_NAME "S8-53.bin"

MainStruct *ms;

void Upgrade(void);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    ms = (MainStruct *)malloc(sizeof(MainStruct));
    ms->percentUpdate = 0.0f;

    Hardware_Init();

    Settings_Load();

    Timer_PauseOnTime(250);

    ms->state = State_Start;

    Display_Init();

    Timer_Enable(kTemp, 10, Display_Update);

    uint timeStart = HAL_TIM2::TimeMS();

    FDrive_Init();

    while (HAL_TIM2::TimeMS() - timeStart < TIME_WAIT && !FDrive_Update())
    {
    }

    if ((ms->drive.connection && ms->drive.active == 0) ||  // Если флеша подключена, но в активное состояние почему-то не перешла
        (ms->drive.active && ms->state != State_Mount))     // или перешла в активное состояние, по почему-то не запустился процесс монтирования
    {
        free(ms);
        HAL::SystemReset();
    }

    if (ms->state == State_Mount)                           // Это означает, что диск удачно примонтирован
    {
        if (FDrive_FileExist(FILE_NAME))                    // Если на диске обнаружена прошивка
        {
            ms->state = State_RequestAction;

            while (1)
            {
                PanelButton button = Panel_PressedButton();
                if (button == B_F1)
                {
                    ms->state = State_Upgrade;
                    Upgrade();
                    break;
                }
                else if (button == B_F5)
                {
                    ms->state = State_Ok;
                    break;
                }
            }
        }
        else
        {
            ms->state = State_NotFile;
        }
    }
    else if (ms->state == State_WrongFlash) // Диск не удалось примонтировать
    {
        Timer_PauseOnTime(5000);
    }

    ms->state = State_Ok;

    HAL_SPI1::DeInit();

    Timer_Disable(kTemp);

    while (Display_IsRun())
    {
    }

    Display_Update();

    HAL::DeInit();

    free(ms);

    HAL::JumpToApplication();
}



void Upgrade(void)
{
    const int sizeSector = 1 * 1024;

    uint8 buffer[sizeSector];

    FLASH_Prepare();

    int size = FDrive_OpenFileForRead(FILE_NAME);
    int fullSize = size;
    uint address = ADDR_SECTOR_PROGRAM_0;

    while (size)
    {
        int readedBytes = FDrive_ReadFromFile(sizeSector, buffer);
        FLASH_WriteData(address, buffer, readedBytes);
        size -= readedBytes;
        address += readedBytes;

        ms->percentUpdate = 1.0f - (float)size / static_cast<float>(fullSize);
    }

    FDrive_CloseOpenedFile();
}


/*! \page page1 Алгоритм работы загрузчика
    \verbatim
    По адресу 0x08000000 находится загрузчик.
    После сброса он проверяет наличие флешки.
        Если флешка есть:
            проверяет наличие файла S8-53.bin. Если таковой имеется, выводит сообщение "На USB-диске обнаружено новоое программное обеспечение. Установить?"
            Если нажато "Да":
                1. Стираются сектора:
                    5 - 0x08020000
                    6 - 0x08040000
                    7 - 0x08060000
                2. На их место записывается содержимое файла S8-53.bin
        Если флешку примонтировать не удалось:
            Вывести сообщение "Не удалось примонтировать флешку. Убедитесь, что на ней файловая система fat32"
    Далее выполняется переход по адресу, указанному в 0x0802004
    \endverbatim
*/
