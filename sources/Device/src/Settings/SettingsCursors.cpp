#include "defines.h"
#include "common/Utils/GlobalFunctions_.h"
#include "common/Utils/Math_.h"
#include "Display/Grid.h"
#include "FPGA/MathFPGA.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include <cmath>
#include <cstring>


float sCursors_GetCursPosU(Channel::E chan, int numCur)
{
    return CURS_POS_U(chan, numCur) / (Grid::ChannelBottom() == Grid::FullBottom() ? 1.0F : 2.0F);
}


bool sCursors_NecessaryDrawCursors()
{
    return ((!CURS_CNTRL_U_IS_DISABLE(CURS_SOURCE)) || (!CURS_CNTRL_T_IS_DISABLE(CURS_SOURCE))) && 
        (CURS_SHOW || Menu::GetNameOpenedPage() == NamePage::SB_Curs);
}


const char* sCursors_GetCursVoltage(Channel::E source, int numCur, char buffer[20])
{
    float voltage = MathFPGA::VoltageCursor(sCursors_GetCursPosU(source, numCur), SET_RANGE(source), SET_RSHIFT(source));

    std::strcpy(buffer, GF::Voltage2String(voltage, true).c_str());

    return buffer;
}


const char* sCursors_GetCursorTime(Channel::E source, int numCur, char buffer[20])
{
    float time = MathFPGA::TimeCursor(CURS_POS_T(source, numCur), SET_TBASE);
        
    return GF::Time2String(time, true, buffer);
}


String sCursors_GetCursorPercentsU(Channel::E source)
{
    float dPerc = DELTA_U100(source);

    float dValue = std::fabsf(sCursors_GetCursPosU(source, 0) - sCursors_GetCursPosU(source, 1));

    String result = GF::Float2String(dValue / dPerc * 100.0F, false, 5);

    result.Append("%");

    return result;
}


String sCursors_GetCursorPercentsT(Channel::E source)
{
    float dPerc = DELTA_T100(source);

    float dValue = std::fabsf(CURS_POS_T0(source) - CURS_POS_T1(source));

    String result = GF::Float2String(dValue / dPerc * 100.0F, false, 6);

    result.Append("%");

    return result;
}
