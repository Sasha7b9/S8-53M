#include "defines.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Utils/Math_.h"
#include "common/Utils/Containers/Values_.h"
#include "FPGA/FPGA_Math.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include <cmath>
#include <cstring>


float Cursors::GetPosU(const Channel &ch, int num)
{
    return set.cursors.posU[ch][num] / (Grid::ChannelBottom() == Grid::FullBottom() ? 1.0F : 2.0F);
}


bool Cursors::NecessaryDraw()
{
    return (!set.cursors.cntrlU[set.cursors.source].IsDisable() || !set.cursors.cntrlT[set.cursors.source].IsDisable()) &&
        (set.cursors.show || Menu::GetNameOpenedPage() == NamePage::SB_Curs);
}


Voltage Cursors::GetVoltage(const Channel &source, int numCur)
{
    return GetVoltage(GetPosU(source, numCur), set.chan[source].range, RShift::Get(source));
}


Voltage Cursors::GetVoltage(float shift_cur, Range::E range, int16 rshift)
{
    return MAX_VOLTAGE_ON_SCREEN(range) - shift_cur * MathFPGA::voltsInPixel[range] - RShift::ToAbs(rshift, range);
}


Time Cursors::GetTime(const Channel &source, int num_cur)
{
    return GetTime(set.cursors.posT[source][num_cur], set.time.base);
}


Time Cursors::GetTime(float shift_cur, TBase::E tBase)
{
    return shift_cur * MathFPGA::absStepTShift[tBase];
}


Text Cursors::GetPercentsU(const Channel &source)
{
    float dPerc = set.cursors.dU_100percents[source]; 

    float dValue = std::fabsf(GetPosU(source, 0) - GetPosU(source, 1));

    Text result = Float(dValue / dPerc * 100.0F).ToText(false, 5);

    result.Append("%");

    return result;
}


Text Cursors::GetPercentsT(const Channel &source)
{
    float dPerc = set.cursors.dT_100percents[source];

    float dValue = std::fabsf(set.cursors.posT[source][0] - set.cursors.posT[source][1]);

    Text result = Float(dValue / dPerc * 100.0F).ToText(false, 6);

    result.Append("%");

    return result;
}


void Cursors::Draw()
{
    Channel &source = set.cursors.source;
    Color::Cursors(source).SetAsCurrent();

    if (Cursors::NecessaryDraw())
    {
        // Признак того, что включены и вертикальные и горизонтальные курсоры - надо нарисовать квадраты в местах
        // пересечения
        bool bothCursors = !set.cursors.cntrlT[source].IsDisable() && !set.cursors.cntrlU[source].IsDisable();  

        int x0 = -1;
        int x1 = -1;
        int y0 = -1;
        int y1 = -1;

        if (bothCursors)
        {
            x0 = (int)(Grid::Left() + set.cursors.posT[source][0]);
            x1 = (int)(Grid::Left() + set.cursors.posT[source][1]);
            y0 = (int)(Grid::TOP + Cursors::GetPosU(source, 0));
            y1 = (int)(Grid::TOP + Cursors::GetPosU(source, 1));

            Rectangle(4, 4).Draw(x0 - 2, y0 - 2);
            Rectangle(4, 4).Draw(x1 - 2, y1 - 2);
        }

        CursCntrl &cntrl = set.cursors.cntrlT[source];

        if (!cntrl.IsDisable())
        {
            DrawVertical((int)(set.cursors.posT[source][0]), y0);
            DrawVertical((int)(set.cursors.posT[source][1]), y1);
        }

        cntrl = set.cursors.cntrlT[source];

        if (!cntrl.IsDisable())
        {
            DrawHorizontal((int)(Cursors::GetPosU(source, 0)), x0);
            DrawHorizontal((int)(Cursors::GetPosU(source, 1)), x1);
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
