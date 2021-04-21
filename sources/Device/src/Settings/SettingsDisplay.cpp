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

    return fps[value];
}


uint Smoothing::NumPoints()
{
    if (set.display.smoothing.IsDisable())
    {
        return 0;
    }
    return (uint)set.display.smoothing + 1;          // WARN Здесь очень коряво реализовано
}


BitSet64 SettingsDisplay::PointsOnDisplay()
{
    BitSet64 result;

    int num_points_on_screen = 281;

    result.first = set.display.shift_in_memory_in_points;
    result.second = set.display.shift_in_memory_in_points + num_points_on_screen;

    return result;
}


int MenuAutoHide::Time()
{
    static const int times[] =
    {
        0, 5, 10, 15, 30, 60
    };
    return times[value] * 1000;
}


bool SettingsDisplay::IsSeparate()
{
    return (!ModeDrawMath::IsDisabled() && ModeDrawMath::IsSeparate()) || set.math.fft.enabled;
}


ModeAveraging::E ModeAveraging::Current()
{
    if (TBase::IsRandomize())
    {
        return ModeAveraging::Around;
    }

    return set.display.mode_ave;
}


int ENumAveraging::NumAverages()
{
    if (TBase::IsRandomize() && (set.debug.num_ave_for_rand >= ENumAveraging::ToAbs()))
    {
        return set.debug.num_ave_for_rand;
    }

    return ToAbs();
}


int ENumAveraging::ToAbs()
{
    return (1 << value);
}
