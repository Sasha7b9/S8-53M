#include "defines.h"
#include "device.h"
#include "common/Log_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Display/Display.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/EPROM.h"
#include "LAN/LAN.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"
#include "Utils/ProcessingSignal.h"
#include "VCP/VCP.h"


bool Device::needResetSettings = false;

//static void ProcessingSignal();


void Device::Init()
{
    HAL::Init();
    Panel::Init();
    Settings::Load(false);
    FPGA::Init();
    HAL_TIM2::Delay(250);
    Display::Init();
    FPGA::Start();

    //    FPGA::OnPressStartStop();
    //    LAN::Init();
    //    FDrive::Init();
    //    VCP::Init();
}


static void WriteFPS()
{
    static uint timePrev = 0;
    static int counter = 0;

    counter++;

    if (TIME_MS - timePrev >= 1000)
    {
        LOG_WRITE("fps = %d", counter);

        counter = 0;
        timePrev = TIME_MS;
    }
}


void Device::Update()
{
//    WriteFPS();

    HAL_TIM2::StartMultiMeasurement();
    FPGA::Update();
    Panel::Update();
    Display::Update();
    Menu::Update();
}


//static void ProcessingSignal()
//{
//    uint8 **data0 = &Storage::dataA;
//    uint8 **data1 = &Storage::dataB;
//    DataSettings **ds = &Storage::set;
//
//    int first = 0;
//    int last = 0;
//    SettingsDisplay::PointsOnDisplay(&first, &last);
//
//    if (MODE_WORK_IS_DIRECT)
//    {
//        Storage::GetDataFromEnd(0, &Storage::set, &Storage::dataA, &Storage::dataB);
//        if (ENumAveraging::NumAverages() != 1 || sTime_RandomizeModeEnabled())
//        {
//            Storage::dataA = Storage::GetAverageData(ChA);
//            Storage::dataB = Storage::GetAverageData(ChB);
//        }
//    }
//    else if (MODE_WORK_IS_LATEST)
//    {
//        data0 = &Storage::dataLastA;
//        data1 = &Storage::dataLastB;
//        ds = &Storage::dsLast;
//        Storage::GetDataFromEnd(PageMemory::PageLatest::currentSignal, &Storage::dsLast, &Storage::dataLastA, &Storage::dataLastB);
//    }
//    else if (MODE_WORK_IS_MEMINT)
//    {
//        data0 = &Storage::dataIntA;
//        data1 = &Storage::dataIntB;
//        ds = &Storage::dsInt;
//        EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::dsInt, &Storage::dataIntA, &Storage::dataIntB);
//    }
//
//    if (MODE_WORK_IS_MEMINT)
//    {
//        if (!MODE_SHOW_MEMINT_IS_SAVED)
//        {
//            Processing::SetSignal(Storage::dataA, Storage::dataB, Storage::set, first, last);
//        }
//    }
//    else
//    {
//        Processing::SetSignal(*data0, *data1, *ds, first, last);
//    }
//
//    Cursors_Update();    // В случае, если находимся в режиме курсорных измерений, обновляем их положение, если нужно.
//}
