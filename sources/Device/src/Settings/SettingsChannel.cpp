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


bool sChannel_Enabled(Channel::E ch)
{
    if (ch == Channel::Math)
    {
        return !DISABLED_DRAW_MATH;
    }
    if (ch == Channel::A_B)
    {
        return false;
    }
    return SET_ENABLED(ch);
}
