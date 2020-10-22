#include "defines.h"
#include "device.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "FPGA/Storage.h"
#include "Hardware/EPROM.h"
#include "Hardware/Hardware.h"
#include "LAN/LAN.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "Utils/ProcessingSignal.h"
#include "VCP/VCP.h"


bool Device::needResetSettings = false;

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
    if (Device::needResetSettings)
    {
        Settings::Load(true);
        Device::needResetSettings = false;
    }

    HAL_TIM2::StartMultiMeasurement();      // Сброс таймера для замера длительности временных интервалов в течение одной итерации цикла.
    FDrive::Update();
    LAN::Update(0);
    FPGA::Update();                      // Обновляем аппаратную часть.
    ProcessingSignal();
    Display::Update();                   // Рисуем экран.
    Menu::UpdateInput();                 // Обновляем состояние меню
    //VCP::Update();
}


static void ProcessingSignal()
{
    uint8 **data0 = &Storage::dataA;
    uint8 **data1 = &gData1;
    DataSettings **ds = &gDSet;

    int first = 0;
    int last = 0;
    SettingsDisplay::PointsOnDisplay(&first, &last);

    if (MODE_WORK_IS_DIRECT)
    {
        Storage::GetDataFromEnd(0, &gDSet, &Storage::dataA, &gData1);
        if (ENumAveraging::NumAverages() != 1 || sTime_RandomizeModeEnabled())
        {
            Storage::dataA = Storage::GetAverageData(Channel::A);
            gData1 = Storage::GetAverageData(Channel::B);
        }
    }
    else if (MODE_WORK_IS_LATEST)
    {
        data0 = &gData0memLast;
        data1 = &gData1memLast;
        ds = &gDSmemLast;
        Storage::GetDataFromEnd(PageMemory::PageLatest::currentSignal, &gDSmemLast, &gData0memLast, &gData1memLast);
    }
    else if (MODE_WORK_IS_MEMINT)
    {
        data0 = &gData0memInt;
        data1 = &gData1memInt;
        ds = &gDSmemInt;
        EPROM::GetData(PageMemory::PageInternal::currentSignal, &gDSmemInt, &gData0memInt, &gData1memInt);
    }

    if (MODE_WORK_IS_MEMINT)
    {
        if (!MODE_SHOW_MEMINT_IS_SAVED)
        {
            Processing::SetSignal(Storage::dataA, gData1, gDSet, first, last);
        }
    }
    else
    {
        Processing::SetSignal(*data0, *data1, *ds, first, last);
    }

    Cursors_Update();    // В случае, если находимся в режиме курсорных измерений, обновляем их положение, если нужно.
}
