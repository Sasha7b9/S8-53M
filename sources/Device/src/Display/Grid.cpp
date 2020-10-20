#include "defines.h"
#include "Display/Grid.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"


int Grid::Bottom()
{
    return Grid::TOP + Height();
}


int Grid::Delta()
{
    return DELTA;
}


int Grid::Left(void)
{
    return (Menu::IsMinimize() && Menu::IsShown() ? 9 : 20) + Measure::GetDeltaGridLeft();
}


int Grid::Right(void)
{
    return ((Menu::IsMinimize() && Menu::IsShown()) ? 9 : 20) + 280;
}


int Grid::ChannelBottom(void)
{
    return (SettingsDisplay::IsSeparate()) ? (Grid::TOP + Height() / 2) : FullBottom();
}


int Grid::SignalWidth(void)
{
    return Width();
}


int Grid::ChannelHeight(void)
{
    return (SettingsDisplay::IsSeparate()) ? FullHeight() / 2 : FullHeight();
}


int Grid::ChannelCenterHeight(void)
{
    return (Grid::TOP + ChannelBottom()) / 2;
}


int Grid::FullBottom(void)
{
    return Grid::Bottom() - Measure::GetDeltaGridBottom();
}


int Grid::FullHeight(void)
{
    return FullBottom() - Grid::TOP;
}


int Grid::Width(void)
{
    return Right() - Left();
}


int Grid::Height()
{
    return CELLS_IN_HEIGHT * DELTA;
}


int Grid::FullCenterHeight(void)
{
    return (FullBottom() + Grid::TOP) / 2;
}


int Grid::WidthInCells(void)
{
    return Menu::IsShown() ? 10 : 14;
}


float Grid::DeltaY(void)
{
    float delta = (FullBottom() - Grid::TOP) / 10.0F;
    return SettingsDisplay::IsSeparate() ? (delta / 2.0F) : delta;
}


float Grid::DeltaX(void)
{
    float delta = (Right() - Left()) / 14.0F;
    return delta;
}


int Grid::MathTop(void)
{
    return MathBottom() - MathHeight();
}


int Grid::MathHeight(void)
{
    if (ENABLED_FFT || MODE_DRAW_MATH_IS_SEPARATE)
    {
        return FullHeight() / 2;
    }
    return FullHeight();
}


int Grid::MathBottom(void)
{
    return FullBottom();
}


int Grid::BottomMessages(void)
{
    int retValue = FullBottom();
    if (MODE_WORK_IS_MEMINT)
    {
        retValue -= 12;
    }
    else if (SHOW_MEASURES)
    {
        retValue = TOP_MEASURES;
        if (MEAS_NUM_IS_6_1 || MEAS_NUM_IS_6_2 || MEAS_NUM_IS_1)
        {
            retValue = FullBottom();
        }
    }
    return retValue - 12;;
}
