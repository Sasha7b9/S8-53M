#include "defines.h"
#include "common/Utils/Math_.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"


int ENumSignalsInSec::NumSignalsInS()
{
    return Tables::ENUMtoNumSignalsInSec(set.display.enum_fps);
}


uint Smoothing::NumPoints()
{
    if (Smoothing::IsDisable())
    {
        return 0;
    }
    return (uint)Smoothing::Get() + 1;          // WARN Здесь очень коряво реализовано
}


bool Smoothing::IsDisable()
{
    return (set.display.smoothing == Smoothing::Disable);
}


Smoothing::E Smoothing::Get()
{
    return set.display.smoothing;
}


void Smoothing::Set(Smoothing::E v)
{
    set.display.smoothing = v;
}


void SettingsDisplay::PointsOnDisplay(int *firstPoint, int *lastPoint)
{
    int numPointsOnScreen = 281;
    *firstPoint = TShift::GetInMemory();
    *lastPoint = TShift::GetInMemory() + numPointsOnScreen - 1;
}


int MenuAutoHide::Time()
{
    static const int times[] =
    {
        0, 5, 10, 15, 30, 60
    };
    return times[set.display.menu_auto_hide] * 1000;
}


bool SettingsDisplay::IsSeparate()
{
    return (!ModeDrawMath::IsDisabled() && ModeDrawMath::IsSeparate()) || SettingsMath::IsEnbaledFFT();
}


ModeAveraging::E ModeAveraging::Current()
{
    if (FPGA::SET::InRandomizeMode())
    {
        return ModeAveraging::Around;
    }
    return set.display.mode_ave;
}


int ENumAveraging::NumAverages()
{
    if (FPGA::SET::InRandomizeMode() && (NUM_AVE_FOR_RAND >= NUM_AVE))
    {
        return NUM_AVE_FOR_RAND;
    }
    return NUM_AVE;
}
