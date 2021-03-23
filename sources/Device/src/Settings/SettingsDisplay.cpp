#include "defines.h"
#include "common/Utils/Math_.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"


int ENumSignalsInSec::NumSignalsInS()
{
    static const int fps[Count] =
    {
        25, 10, 5, 2, 1
    };

    return fps[set.display.enum_fps];
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


Set2Int SettingsDisplay::PointsOnDisplay()
{
    Set2Int result;

    int num_points_on_screen = 281;

    result.value1 = TShift::GetInMemory();
    result.value2 = TShift::GetInMemory() + num_points_on_screen - 1;

    return result;
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
    return (!ModeDrawMath::IsDisabled() && ModeDrawMath::IsSeparate()) || set.math.fft.enabled;
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
    if (FPGA::SET::InRandomizeMode() && (set.debug.num_ave_for_rand >= ENumAveraging::ToAbs()))
    {
        return set.debug.num_ave_for_rand;
    }

    return ToAbs();
}


int ENumAveraging::ToAbs()
{
    return (1 << value);
}
