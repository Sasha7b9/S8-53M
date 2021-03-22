#include "defines.h"
#include "common/Display/Primitives_.h"
#include "common/Utils/GlobalFunctions_.h"
#include "common/Utils/Math_.h"
#include "Display/Grid.h"
#include "FPGA/MathFPGA.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include <cmath>
#include <cstring>


using namespace Primitives;


float Cursors::GetPosU(const Channel &ch, int num)
{
    return CURS_POS_U(ch, num) / (Grid::ChannelBottom() == Grid::FullBottom() ? 1.0F : 2.0F);
}


bool Cursors::NecessaryDraw()
{
    return ((!CURS_CNTRL_U_IS_DISABLE(set.cursors.source)) || (!CURS_CNTRL_T_IS_DISABLE(set.cursors.source))) &&
        (CURS_SHOW || Menu::GetNameOpenedPage() == NamePage::SB_Curs);
}


String Cursors::GetVoltage(const Channel &source, int numCur)
{
    float voltage = MathFPGA::VoltageCursor(GetPosU(source, numCur), Range::Get(source), RShift::Get(source));

    return GF::Voltage2String(voltage, true);
}


String Cursors::GetTime(const Channel &source, int numCur)
{
    float time = MathFPGA::TimeCursor(CURS_POS_T(source, numCur), TBase::Get());

    return GF::Time2String(time, true);
}


String Cursors::GetPercentsU(const Channel &source)
{
    float dPerc = DELTA_U100(source);

    float dValue = std::fabsf(GetPosU(source, 0) - GetPosU(source, 1));

    String result = GF::Float2String(dValue / dPerc * 100.0F, false, 5);

    result.Append("%");

    return result;
}


String Cursors::GetPercentsT(const Channel &source)
{
    float dPerc = DELTA_T100(source);

    float dValue = std::fabsf(CURS_POS_T0(source) - CURS_POS_T1(source));

    String result = GF::Float2String(dValue / dPerc * 100.0F, false, 6);

    result.Append("%");

    return result;
}


void Cursors::Draw()
{
    Channel::E source = set.cursors.source;
    Color::Cursors(source).SetAsCurrent();

    if (Cursors::NecessaryDraw())
    {
        bool bothCursors = !CURS_CNTRL_T_IS_DISABLE(source) && !CURS_CNTRL_U_IS_DISABLE(source);  // Признак того, что
                    // включены и вертикальные и горизонтальные курсоры - надо нарисовать квадраты в местах пересечения
        int x0 = -1;
        int x1 = -1;
        int y0 = -1;
        int y1 = -1;

        if (bothCursors)
        {
            x0 = static_cast<int>(Grid::Left() + CURS_POS_T0(source));
            x1 = static_cast<int>(Grid::Left() + CURS_POS_T1(source));
            y0 = static_cast<int>(Grid::TOP + Cursors::GetPosU(source, 0));
            y1 = static_cast<int>(Grid::TOP + Cursors::GetPosU(source, 1));

            Rectangle(4, 4).Draw(x0 - 2, y0 - 2);
            Rectangle(4, 4).Draw(x1 - 2, y1 - 2);
        }

        CursCntrl::E cntrl = CURS_CNTRL_T(source);
        if (cntrl != CursCntrl::Disable)
        {
            DrawVertical(static_cast<int>(CURS_POS_T0(source)), y0);
            DrawVertical(static_cast<int>(CURS_POS_T1(source)), y1);
        }
        cntrl = CURsU_CNTRL;
        if (cntrl != CursCntrl::Disable)
        {
            DrawHorizontal(static_cast<int>(Cursors::GetPosU(source, 0)), x0);
            DrawHorizontal(static_cast<int>(Cursors::GetPosU(source, 1)), x1);
        }
    }
}


void Cursors::DrawVertical(int x, int yTearing)
{
    x += Grid::Left();

    if (yTearing == -1)
    {
        DashedVLine(1, 1).Draw(x, Grid::TOP + 2, Grid::ChannelBottom() - 1, 0);
    }
    else
    {
        DashedVLine(1, 1).Draw(x, Grid::TOP + 2, yTearing - 2, 0);
        DashedVLine(1, 1).Draw(x, yTearing + 2, Grid::ChannelBottom() - 1, 0);
    }

    Rectangle(2, 2).Draw(x - 1, Grid::TOP - 1);
    Rectangle(2, 2).Draw(x - 1, Grid::ChannelBottom() - 1);
}


void Cursors::DrawHorizontal(int y, int xTearing)
{
    y += Grid::TOP;

    if (xTearing == -1)
    {
        DashedHLine(1, 1).Draw(y, Grid::Left() + 2, Grid::Right() - 1, 0);
    }
    else
    {
        DashedHLine(1, 1).Draw(y, Grid::Left() + 2, xTearing - 2, 0);
        DashedHLine(1, 1).Draw(y, xTearing + 2, Grid::Right() - 1, 0);
    }

    Rectangle(2, 2).Draw(Grid::Left() - 1, y - 1);
    Rectangle(2, 2).Draw(Grid::Right() - 1, y - 1);
}
