#include "defines.h"
#include "device.h"
#include "Ethernet/Ethernet.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "FPGA/Storage.h"
#include "Hardware/EPROM.h"
#include "Hardware/Hardware.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageCursors.h"
#include "Settings/Settings.h"
#include "VCP/VCP.h"
#include "Utils/ProcessingSignal.h"


static void ProcessingSignal();


void Device::Init()
{
    Hardware::Init();
    VCP::Init();
    Settings::Load(false);
    FPGA::Init();
    HAL_TIM2::Delay(250);
    FPGA::OnPressStartStop();
    LAN::Init();
    Display::Init();
}


void Device::Update()
{
    if (NEED_RESET_SETTINGS)
    {
        Settings::Load(true);
        NEED_RESET_SETTINGS = 0;
    }

    HAL_TIM2::StartMultiMeasurement();      // Сброс таймера для замера длительности временных интервалов в течение одной итерации цикла.
    FlashDrive::Update();
    LAN::Update(0);
    FPGA::Update();                      // Обновляем аппаратную часть.
    ProcessingSignal();
    Display::Update();                   // Рисуем экран.
    Menu::UpdateInput();                 // Обновляем состояние меню
    //VCP::Update();
}


static void ProcessingSignal()
{
    uint8 **data0 = &gData0;
    uint8 **data1 = &gData1;
    DataSettings **ds = &gDSet;

    int first = 0;
    int last = 0;
    SettingsDisplay::PointsOnDisplay(&first, &last);

    if (MODE_WORK_IS_DIRECT)
    {
        Storage::GetDataFromEnd(0, &gDSet, &gData0, &gData1);
        if (ENumAveraging::NumAverages() != 1 || sTime_RandomizeModeEnabled())
        {
            gData0 = Storage::GetAverageData(Channel::A);
            gData1 = Storage::GetAverageData(Channel::B);
        }
    }
    else if (MODE_WORK_IS_LATEST)
    {
        data0 = &gData0memLast;
        data1 = &gData1memLast;
        ds = &gDSmemLast;
        Storage::GetDataFromEnd(CURRENT_NUM_LATEST_SIGNAL, &gDSmemLast, &gData0memLast, &gData1memLast);
    }
    else if (MODE_WORK_IS_MEMINT)
    {
        data0 = &gData0memInt;
        data1 = &gData1memInt;
        ds = &gDSmemInt;
        EPROM::GetData(CURRENT_NUM_INT_SIGNAL, &gDSmemInt, &gData0memInt, &gData1memInt);
    }

    if (MODE_WORK_IS_MEMINT)
    {
        if (!MODE_SHOW_MEMINT_IS_SAVED)
        {
            Processing::SetSignal(gData0, gData1, gDSet, first, last);
        }
    }
    else
    {
        Processing::SetSignal(*data0, *data1, *ds, first, last);
    }

    Cursors_Update();    // В случае, если находимся в режиме курсорных измерений, обновляем их положение, если нужно.
}
