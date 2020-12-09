#include "defines.h"
#include "common/Log_c.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"


void sTime_SetTBase(TBase::E tBase)
{
    SET_TBASE = tBase;
    FPGA::ClearData();
}


void sTime_SetTShift(int16 shift)
{
    TSHIFT = shift;
    FPGA::ClearData();
}


int sTime_TPosInPoints(PeackDetMode::E peakDet, int numPoints, TPos::E tPos) //-V2506
{
    if (peakDet == PeackDetMode::Disable)
    {
        static const int m[3][3] =
        {
            {0, 140, 280},
            {0, 255, 511},
            {0, 512, 1022}
        };
        return m[sMemory_IntNumPoints2FPGA_NUM_POINTS(numPoints)][tPos];
    }
    else
    {
        static const int m[3][3] =
        {
            {0, 140, 280},
            {0, 256, 510},
            {0, 256, 510}
        };
        return m[sMemory_IntNumPoints2FPGA_NUM_POINTS(numPoints)][tPos];
    }
}


int sTime_TShiftInPoints(PeackDetMode::E peakDet)
{
    return TSHIFT * (peakDet == PeackDetMode::Disable ? 2 : 1);
}


int16 sTime_TShiftMin() //-V2506
{
    static const int16 m[3][3] = {{-511, -441, -371},
    {-511, -383, -255},
    {-511, -255, 0}};

    ENUM_POINTS_FPGA::E numPoints = ENUM_POINTS;
    if ((int)numPoints < 3) //-V2533
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
