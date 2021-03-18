#include "defines.h"
#include "common/Log_.h"
#include "FPGA/FPGA_Reader.h"
#include "Settings/Settings.h"


int sTime_TShiftInPoints(PeackDetMode::E peakDet)
{
    return TSHIFT * (peakDet == PeackDetMode::Disable ? 2 : 1);
}


int16 sTime_TShiftMin()
{
    static const int16 m[3][3] = {{-511, -441, -371},
    {-511, -383, -255},
    {-511, -255, 0}};

    ENUM_POINTS_FPGA::E numPoints = ENUM_POINTS;
    if ((int)numPoints < 3)
    {
        return m[numPoints][SET_TPOS];
    }
    LOG_ERROR("");
    return 0;
}


int16 sTime_TShiftZero()
{
    return -sTime_TShiftMin();
}


bool sTime_RandomizeModeEnabled()
{
    return SET_TBASE < TBase::_100ns;
}
