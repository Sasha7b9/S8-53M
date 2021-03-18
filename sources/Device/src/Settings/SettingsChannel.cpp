#include "defines.h"
#include "common/Utils/GlobalFunctions_.h"
#include "common/Utils/Math_.h"
#include "FPGA/FPGA.h"
#include "FPGA/MathFPGA.h"
#include "Settings/Settings.h"
#include <cstring>


void sChannel_SetRange(Channel::E ch, Range::E range)
{
    SET_RANGE(ch) = range;
}



