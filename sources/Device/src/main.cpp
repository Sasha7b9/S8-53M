// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "device.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/USBH/USBH_.h"
#include "FDrive/FDrive.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"
#include <stm32f4xx_hal.h>
#include <usbh_def.h>
#include "usbh_diskio_.h"


FATFS USBDISKFatFs;
FIL MyFile;

char USBDISKPath[4];


int main()
{
    /*
    Sector::Get(Sector::_12_NRST_1).Erase();
    Sector::Get(Sector::_13_NRST_2).Erase();
   
    Sector::Get(Sector::_10_SETTINGS_1).Erase();
    Sector::Get(Sector::_11_SETTINGS_2).Erase(); 
    */
    
    setNRST.Load();

    Device::Init();

    while (1)
    {
        /* USB Host Background task */
        USBH_Process((USBH_HandleTypeDef *)USBH::handle);

        /* Mass Storage Application State Machine */
        switch (Appli_state)
        {
        case APPLICATION_START:
            Appli_state = APPLICATION_IDLE;
            break;

        case APPLICATION_IDLE:
            break;

        case APPLICATION_RUNNING:
            break;

        default:
            break;
        }
    }

//    while(1)
//    {
//        Device::Update();
//
//        set.Save();
//
//        setNRST.Save();
//    }
}
