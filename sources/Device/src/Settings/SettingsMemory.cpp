#include "defines.h"
#include "FPGA/FPGA_types.h"
#include "Settings/Settings.h"


int sMemory_GetNumPoints(bool forCalculate) //-V2506
{
    static const int numPoints[3][3] =
    {
        {281, 512, 1024},
        {281, 512, 512},
        {281, 512, 512}
    };

    if (ENUM_POINTS_IS_1024 && forCalculate)
    {
        return FPGA_MAX_POINTS_FOR_CALCULATE;
    }

    return numPoints[PEAKDET][ENUM_POINTS];
}


ENUM_POINTS_FPGA::E sMemory_IntNumPoints2FPGA_NUM_POINTS(int numPoints) //-V2506
{
    if (numPoints == 1024)
    {
        return ENUM_POINTS_FPGA::_1024;
    }
    else if (numPoints == 512) //-V2516
    {
        return ENUM_POINTS_FPGA::_512;
    }
    return ENUM_POINTS_FPGA::_281;
}
