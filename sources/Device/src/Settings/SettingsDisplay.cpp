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
    return (uint)set.display.smoothing + 1;          // WARN ����� ����� ������ �����������
}


Set2Int SettingsDisplay::PointsOnDisplay()
{
    Set2Int result;

    int num_points_on_screen = 281;

    result.value1 = TShift::GetInMemory();
    result.value2 = TShift::GetInMemory() + num_points_on_screen;

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
