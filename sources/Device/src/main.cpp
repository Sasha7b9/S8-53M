#include "defines.h"
#include "globals.h"
#include "Ethernet/Ethernet.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "FPGA/Storage.h"
#include "Hardware/Timer.h"
#include "Hardware/Hardware.h"
#include "Hardware/EPROM.h"
#include "common/Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageCursors.h"
#include "Settings/Settings.h"
#include "Utils/ProcessingSignal.h"
#include "VCP/VCP.h"


static void ProcessingSignal();

#define TICS ((gTimerTics - time) / 120.0F)

extern void main3();    


int main(void)
{
    Hardware::Init();
    VCP::Init();
    Settings::Load(false);
    FPGA::Init();    
    HAL_TIM2::Delay(250);
    FPGA::OnPressStartStop();
    LAN::Init();
    Display::Init();

    while(1)
    {
        if(NEED_RESET_SETTINGS)
        {
            Settings::Load(true);
            NEED_RESET_SETTINGS = 0;
        }

        HAL_TIM2::StartMultiMeasurement();      // ����� ������� ��� ������ ������������ ��������� ���������� � ������� ����� �������� �����.
        FlashDrive::Update();
        LAN::Update(0);
        FPGA::Update();                      // ��������� ���������� �����.
        ProcessingSignal();
        Display::Update();                   // ������ �����.
        Menu::UpdateInput();                 // ��������� ��������� ����
        //VCP::Update();
    }
}


void ProcessingSignal() 
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

    Cursors_Update();    // � ������, ���� ��������� � ������ ��������� ���������, ��������� �� ���������, ���� �����.
}
