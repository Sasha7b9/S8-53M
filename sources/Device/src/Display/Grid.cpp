#include "defines.h"
#include "common/Display/Primitives_.h"
#include "common/Display/Text_.h"
#include "Display/Grid.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"


using namespace Primitives;


int Grid::Bottom()
{
    return Grid::TOP + Height();
}


int Grid::Delta()
{
    return DELTA;
}


int Grid::Left()
{
    return (Menu::IsMinimize() && Menu::IsShown() ? 9 : 20) + Measure::GetDeltaGridLeft();
}


int Grid::Right()
{
    return ((Menu::IsMinimize() && Menu::IsShown()) ? 9 : 20) + 280;
}


int Grid::ChannelBottom()
{
    return (SettingsDisplay::IsSeparate()) ? (Grid::TOP + Height() / 2) : FullBottom();
}


int Grid::SignalWidth()
{
    return Width();
}


int Grid::ChannelHeight()
{
    return (SettingsDisplay::IsSeparate()) ? FullHeight() / 2 : FullHeight();
}


int Grid::ChannelCenterHeight()
{
    return (Grid::TOP + ChannelBottom()) / 2;
}


int Grid::FullBottom()
{
    return Grid::Bottom() - Measure::GetDeltaGridBottom();
}


int Grid::FullHeight()
{
    return FullBottom() - Grid::TOP;
}


int Grid::Width()
{
    return Right() - Left();
}


int Grid::Height()
{
    return CELLS_IN_HEIGHT * DELTA;
}


int Grid::FullCenterHeight()
{
    return (FullBottom() + Grid::TOP) / 2;
}


int Grid::WidthInCells()
{
    return Menu::IsShown() ? 10 : 14;
}


float Grid::DeltaY()
{
    float delta = (FullBottom() - Grid::TOP) / 10.0F;
    return SettingsDisplay::IsSeparate() ? (delta / 2.0F) : delta;
}


float Grid::DeltaX()
{
    float delta = (Right() - Left()) / 14.0F;
    return delta;
}


int Grid::MathTop()
{
    return MathBottom() - MathHeight();
}


int Grid::MathHeight()
{
    if (SettingsMath::IsEnbaledFFT() || ModeDrawMath::IsSeparate())
    {
        return FullHeight() / 2;
    }
    return FullHeight();
}


int Grid::MathBottom()
{
    return FullBottom();
}


int Grid::BottomMessages()
{
    int result = FullBottom();

    if (ModeWork::IsMemInt())
    {
        result -= 12;
    }
    else if (SettingsMeasures::ShowMeasures())
    {
        result = Measure::top_measures;

        if (set.measures.number.Is6_1() ||
            set.measures.number.Is6_2() ||
            set.measures.number.Is1())
        {
            result = FullBottom();
        }
    }

    return result - 12;;
}


void Grid::Draw()
{
    if (SettingsDisplay::IsSeparate())
    {
        Draw(Grid::Left(), Grid::TOP, Grid::Width(), Grid::FullHeight() / 2);

        if (SettingsMath::IsEnbaledFFT())
        {
            DrawSpectrum();
        }
        if (!ModeDrawMath::IsDisabled())
        {
            Draw(Grid::Left(), Grid::TOP + Grid::FullHeight() / 2, Grid::Width(), Grid::FullHeight() / 2);
        }
        HLine().Draw(Grid::TOP + Grid::FullHeight() / 2, Grid::Left(), Grid::Left() + Grid::Width(), Color::FILL);
    }
    else
    {
        Draw(Grid::Left(), Grid::TOP, Grid::Width(), Grid::FullHeight());
    }
}


void Grid::Draw(int left, int top, int width, int height)
{
    int right = left + width;
    int bottom = top + height;

    Color::FILL.SetAsCurrent();

    if (top == Grid::TOP)
    {
        HLine().Draw(top, 1, left - 2);
        HLine().Draw(top, right + 2, WIDTH - 2);

        if (!Menu::IsMinimize() || !Menu::IsShown())
        {
            VLine().Draw(1, top + 2, bottom - 2);
            VLine().Draw(318, top + 2, bottom - 2);
        }
    }

    float deltaX = Grid::DeltaX() * (float)width / width;
    float deltaY = Grid::DeltaY() * (float)height / height;
    float stepX = deltaX / 5;
    float stepY = deltaY / 5;

    int centerX = left + width / 2;
    int centerY = top + height / 2;

    Color::GRID.SetAsCurrent();

    if (TypeGrid::Is1())
    {
        DrawType1(left, top, right, bottom, static_cast<float>(centerX), static_cast<float>(centerY), deltaX,
            deltaY, stepX, stepY);
    }
    else if (TypeGrid::Is2())
    {
        DrawType2(left, top, right, bottom, (int)(deltaX), (int)(deltaY),
            (int)(stepX), (int)(stepY));
    }
    else if (TypeGrid::Is3())
    {
        DrawType3(left, top, right, bottom, centerX, centerY, (int)(deltaX), (int)(deltaY),
            (int)(stepX), (int)(stepY));
    }
}


void Grid::DrawSpectrum()
{
    if (ScaleFFT::IsLog())
    {
        static const int nums[] = { 4, 6, 8 };
        static pchar strs[] = { "0", "-10", "-20", "-30", "-40", "-50", "-60", "-70" };
        int numParts = nums[FFTmaxDB::Get()];
        float scale = (float)Grid::MathHeight() / numParts;
        for (int i = 1; i < numParts; i++)
        {
            int y = (int)(Grid::MathTop() + i * scale);
            HLine().Draw(y, Grid::Left(), Grid::Left() + 256, Color::GRID);
            if (!Menu::IsMinimize())
            {
                Color::FILL.SetAsCurrent();
                Text(strs[i]).Draw(3, y - 4);
            }
        }
        if (!Menu::IsMinimize())
        {
            Color::FILL.SetAsCurrent();
            Text("Да").Draw(5, Grid::MathTop() + 1);
        }
    }
    else if (ScaleFFT::IsLinear())
    {
        static pchar strs[] = { "1.0", "0.8", "0.6", "0.4", "0.2" };
        float scale = (float)Grid::MathHeight() / 5;
        for (int i = 1; i < 5; i++)
        {
            int y = (int)(Grid::MathTop() + i * scale);
            HLine().Draw(y, Grid::Left(), Grid::Left() + 256, Color::GRID);
            if (!Menu::IsMinimize())
            {
                Text(strs[i]).Draw(5, y - 4, Color::FILL);
            }
        }
    }
    HLine().Draw(Grid::Left() + 256, Grid::MathTop(), Grid::MathBottom(), Color::FILL);
}


void Grid::DrawType1(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX,
    float deltaY, float stepX, float stepY)
{
    uint16 masX[17];
    masX[0] = (uint16)(left + 1);
    for (int i = 1; i < 7; i++)
    {
        masX[i] = static_cast<uint16>(left + deltaX * i);
    }
    for (int i = 7; i < 10; i++)
    {
        masX[i] = static_cast<uint16>(centerX - 8 + i);
    }
    for (int i = 10; i < 16; i++)
    {
        masX[i] = static_cast<uint16>(centerX + deltaX * (i - 9));
    }
    masX[16] = (uint16)(right - 1);

    MultiVPointLine(17, masX, (int)(stepY), CalculateCountV()).Draw((int)(top + stepY),
        Color::GRID);

    uint8 mas[13];
    mas[0] = (uint8)(top + 1);
    for (int i = 1; i < 5; i++)
    {
        mas[i] = (uint8)(top + deltaY * i);
    }
    for (int i = 5; i < 8; i++)
    {
        mas[i] = (uint8)(centerY - 6 + i);
    }
    for (int i = 8; i < 12; i++)
    {
        mas[i] = (uint8)(centerY + deltaY * (i - 7));
    }
    mas[12] = (uint8)(bottom - 1);

    MultiHPointLine(13, mas, (int)(stepX), CalculateCountH()).Draw((int)(left + stepX),
        Color::GRID);
}


void Grid::DrawType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY)
{
    uint16 masX[15];
    masX[0] = (uint16)(left + 1);
    for (int i = 1; i < 14; i++)
    {
        masX[i] = (uint16)(left + deltaX * i);
    }
    masX[14] = (uint16)(right - 1);
    MultiVPointLine(15, masX, stepY, CalculateCountV()).Draw(top + stepY, Color::GRID);

    uint8 mas[11];
    mas[0] = (uint8)(top + 1);
    for (int i = 1; i < 10; i++)
    {
        mas[i] = (uint8)(top + deltaY * i);
    }
    mas[10] = (uint8)(bottom - 1);
    MultiHPointLine(11, mas, stepX, CalculateCountH()).Draw(left + stepX, Color::GRID);
}


void Grid::DrawType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY,
    int stepX, int stepY)
{
    HPointLine(static_cast<float>(stepX)).Draw(centerY, left + stepX, right);

    uint8 masY[6] = { (uint8)(top + 1), (uint8)(top + 2), (uint8)(centerY - 1), (uint8)(centerY + 1),
        (uint8)(bottom - 2), (uint8)(bottom - 1) };

    MultiHPointLine(6, masY, deltaX, (right - top) / deltaX).Draw(left + deltaX, Color::GRID);

    VPointLine(static_cast<float>(stepY)).Draw(centerX, top + stepY, bottom, Color::GRID);

    uint16 masX[6] = { (uint16)(left + 1), (uint16)(left + 2), (uint16)(centerX - 1), (uint16)(centerX + 1),
        (uint16)(right - 2), (uint16)(right - 1) };

    MultiVPointLine(6, masX, deltaY, (bottom - top) / deltaY).Draw(top + deltaY, Color::GRID);
}


int Grid::CalculateCountV()
{
    if (set.measures.mode_view_signals.IsCompress())
    {
        bool source_is_AB = set.measures.source.IsAB();
        MeasuresNumber &number = set.measures.number;

        if (number.Is1_5())
        {
            return source_is_AB ? 42 : 44;
        }
        if (number.Is2_5())
        {
            return source_is_AB ? 69 : 39;
        }
        if (number.Is3_5())
        {
            return source_is_AB ? 54 : 68;
        }
    }

    return 49;
}


int Grid::CalculateCountH()
{
    if (set.measures.mode_view_signals.IsCompress())
    {
        if (set.measures.number.Is6_1())
        {
            return 73;
        }
        if (set.measures.number.Is6_2())
        {
            return 83;
        }
    }
    return 69;
}
