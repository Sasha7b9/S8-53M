#include "defines.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_types.h"
#include "Settings/Settings.h"




ENUM_POINTS_FPGA::E sMemory_IntNumPoints2FPGA_NUM_POINTS(int numPoints)
{
    if (numPoints == 1024)
    {
        return ENUM_POINTS_FPGA::_1024;
    }
    else if (numPoints == 512)
    {
        return ENUM_POINTS_FPGA::_512;
    }
    return ENUM_POINTS_FPGA::_281;
}
