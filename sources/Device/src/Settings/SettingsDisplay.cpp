#include "defines.h"
#include "common/Utils/Math_.h"
#include "Settings/Settings.h"


int ENumSignalsInSec::NumSignalsInS()
{
    return Tables::ENUMtoNumSignalsInSec(ENUM_SIGNALS_IN_SEC);
}


int Smoothing::NumPoints()
{
    if (SMOOTHING_IS_DISABLE)
    {
        return 0;
    }
    return (int)SMOOTHING + 1;          // WARN Здесь очень коряво реализовано
}


void SettingsDisplay::PointsOnDisplay(int *firstPoint, int *lastPoint)
{
    int numPointsOnScreen = 281;
    *firstPoint = SHIFT_IN_MEMORY;
    *lastPoint = SHIFT_IN_MEMORY + numPointsOnScreen - 1;
}


int MenuAutoHide::Time()
{
    static const int times[] =
    {
        0, 5, 10, 15, 30, 60
    };
    return times[MENU_AUTO_HIDE] * 1000;
}


bool SettingsDisplay::IsSeparate()
{
    return (!DISABLED_DRAW_MATH && MODE_DRAW_MATH_IS_SEPARATE) || ENABLED_FFT;
}


ModeAveraging::E ModeAveraging::Current()
{
    if (FPGA_IN_RANDOMIZE_MODE)
    {
        return ModeAveraging::Around;
    }
    return MODE_AVE;
}


int ENumAveraging::NumAverages()
{
    if (FPGA_IN_RANDOMIZE_MODE && (NUM_AVE_FOR_RAND >= NUM_AVE))
    {
        return NUM_AVE_FOR_RAND;
    }
    return NUM_AVE;
}
