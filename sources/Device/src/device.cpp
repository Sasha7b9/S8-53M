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
    uint8 **data1 = &Storage::dataB;
    DataSettings **ds = &Storage::set;

    int first = 0;
    int last = 0;
    SettingsDisplay::PointsOnDisplay(&first, &last);

    if (MODE_WORK_IS_DIRECT)
    {
        Storage::GetDataFromEnd(0, &Storage::set, &Storage::dataA, &Storage::dataB);
        if (ENumAveraging::NumAverages() != 1 || sTime_RandomizeModeEnabled())
        {
            Storage::dataA = Storage::GetAverageData(Channel::A);
            Storage::dataB = Storage::GetAverageData(Channel::B);
        }
    }
    else if (MODE_WORK_IS_LATEST)
    {
        data0 = &Storage::dataLastA;
        data1 = &Storage::dataLastB;
        ds = &Storage::dsLast;
        Storage::GetDataFromEnd(PageMemory::PageLatest::currentSignal, &Storage::dsLast, &Storage::dataLastA, &Storage::dataLastB);
    }
    else if (MODE_WORK_IS_MEMINT)
    {
        data0 = &Storage::dataIntA;
        data1 = &Storage::dataIntB;
        ds = &Storage::dsInt;
        EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::dsInt, &Storage::dataIntA, &Storage::dataIntB);
    }

    if (MODE_WORK_IS_MEMINT)
    {
        if (!MODE_SHOW_MEMINT_IS_SAVED)
        {
            Processing::SetSignal(Storage::dataA, Storage::dataB, Storage::set, first, last);
        }
    }
    else
    {
        Processing::SetSignal(*data0, *data1, *ds, first, last);
    }

    Cursors_Update();    // В случае, если находимся в режиме курсорных измерений, обновляем их положение, если нужно.
}
