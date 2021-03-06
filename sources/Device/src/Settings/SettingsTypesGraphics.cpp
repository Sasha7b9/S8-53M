// 2021/03/18 9:50:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Settings/SettingsTypes_.h"
#include "common/Utils/Math_.h"
#include "FPGA/FPGA_Types.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"


void RShift::Draw()
{
    if (!ModeDrawMath::IsDisabled())
    {
        Draw(Channel::Math);
    }

    Draw(Channel::FirstDrawing());
    Draw(Channel::SecondDrawing());
}


void RShift::DrawCursors()
{
    if ((!Menu::IsMinimize() || !Menu::IsShown()) && RShift::draw_markers)
    {
        Display::DrawScaleLine(2, false);
    }
}


void RShift::Draw(const Channel &ch)
{
    float x = (float)(Grid::Right() - Grid::Width() - Measure::GetDeltaGridLeft());

    if (ch.IsMath())
    {
        int rShift = RShift::GetMath();
        float scale = (float)Grid::MathHeight() / 960;
        float y = (Grid::MathTop() + Grid::MathBottom()) / 2.0F - scale * (rShift - RShift::ZERO);
        Char(Symbol::S8::RSHIFT_NORMAL).Draw((int)(x - 9), (int)(y - 4), Color::FILL);
        Char('m').Draw((int)(x - 8), (int)(y - 5), Color::BACK);
        return;
    }
    if (!ch.IsEnabled())
    {
        return;
    }

    int rShift = set.chan[ch].rshift;

    float scale = Grid::ChannelHeight() / (RShift::STEP() * 200.0F);

    float y = Grid::ChannelCenterHeight() - scale * (rShift - RShift::ZERO);

    if (rShift > RShift::ZERO)      // \todo ������� ��������
    {
        y += 1;
    }

    if (y > Grid::ChannelBottom())
    {
        Char(Symbol::S8::RSHIFT_LOWER).Draw((int)(x - 7), Grid::ChannelBottom() - 11, ch.GetColor());
        Point().Draw((int)(x - 5), Grid::ChannelBottom() - 2);
        y = (float)(Grid::ChannelBottom() - 7);
        x++;
    }
    else if (y < Grid::TOP)
    {
        Char(Symbol::S8::RSHIFT_ABOVE).Draw((int)(x - 7), Grid::TOP + 2, ch.GetColor());
        Point().Draw((int)(x - 5), Grid::TOP + 2);
        y = Grid::TOP + 7;
        x++;
    }
    else
    {
        Char(Symbol::S8::RSHIFT_NORMAL).Draw((int)(x - 8), (int)(y - 4), ch.GetColor());
        if ((show_level[ch]) && set.memory.mode_work.IsDirect())
        {
            DashedHLine(7, 3).Draw((int)(y), Grid::Left(), Grid::Right(), 0);
        }
    }

    Font::Set(TypeFont::S5);

    if ((!Menu::IsMinimize() || !Menu::IsShown()) && draw_markers)
    {
        float scaleFull = (float)Grid::ChannelHeight() / (RShift::MAX - RShift::MIN) *
            (SettingsMath::IsMathEnabled() ? 0.9F : 0.91F);

        float yFull = Grid::ChannelCenterHeight() - scaleFull * (rShift - RShift::ZERO);

        Region(4, 6).Fill(4, (int)(yFull - 3), ch.GetColor());
        Char(ch == ChA ? '1' : '2').Draw(5, (int)(yFull - 9), Color::BACK);
    }
    Char(ch == ChA ? '1' : '2').Draw((int)(x - 7), (int)(y - 9), Color::BACK);
    Font::Set(TypeFont::S8);
}


void TrigLev::DrawCursor()
{
    TrigSource::E ch = TrigSource::Get();
    if (ch == TrigSource::Ext)
    {
        return;
    }
    int trigLev = TrigLev::Get(ch) + (set.chan[ch].rshift - RShift::ZERO);
    float scale = 1.0F / ((TrigLev::MAX - TrigLev::MIN) / 2.0F / Grid::ChannelHeight());
    int y0 = (int)((Grid::TOP + Grid::ChannelBottom()) / 2 + scale * (TrigLev::ZERO - TrigLev::MIN));
    int y = (int)(y0 - scale * (trigLev - TrigLev::MIN));

    y = (y - Grid::ChannelCenterHeight()) + Grid::ChannelCenterHeight();

    int x = Grid::Right();
    Color::Trig().SetAsCurrent();
    if (y > Grid::ChannelBottom())
    {
        Char(Symbol::S8::TRIG_LEV_LOWER).Draw(x + 3, Grid::ChannelBottom() - 11);;
        Point().Draw(x + 5, Grid::ChannelBottom() - 2);
        y = Grid::ChannelBottom() - 7;
        x--;
    }
    else if (y < Grid::TOP)
    {
        Char(Symbol::S8::TRIG_LEV_ABOVE).Draw(x + 3, Grid::TOP + 2);
        Point().Draw(x + 5, Grid::TOP + 2);
        y = Grid::TOP + 7;
        x--;
    }
    else
    {
        Char(Symbol::S8::TRIG_LEV_NORMAL).Draw(x + 1, y - 4);
    }
    Font::Set(TypeFont::S5);

    const char simbols[3] = { '1', '2', '�' };

    Char(simbols[TrigSource::Get()]).Draw(x + 5, y - 9, Color::BACK);
    Font::Set(TypeFont::S8);

    if (RShift::draw_markers && !Menu::IsMinimize())
    {
        int delta = Display::DrawScaleLine(Display::WIDTH - 11, true);
        int left = Grid::Right() + 9;
        int height = Grid::ChannelHeight() - 2 * delta;
        int shiftFullMin = RShift::MIN + TrigLev::MIN;
        int shiftFullMax = RShift::MAX + TrigLev::MAX;
        scale = (float)height / (shiftFullMax - shiftFullMin);
        int shiftFull = TrigLev::Get() + (TrigSource::IsExt() ? 0 : set.chan[ch].rshift);
        int yFull = (int)(Grid::TOP + delta + height - scale * (shiftFull - RShift::MIN - TrigLev::MIN) - 4);
        Region(4, 6).Fill(left + 2, yFull + 1, Color::Trig());
        Font::Set(TypeFont::S5);
        Char(simbols[TrigSource::Get()]).Draw(left + 3, yFull - 5, Color::BACK);
        Font::Set(TypeFont::S8);
    }
}


void TrigLev::WriteValue()
{
    if (show_level && set.memory.mode_work.IsDirect())
    {
        float trigLev = RShift::ToAbs(TrigLev::Get(), set.chan[TrigSource::Get()].range);     // WARN ����� ��� �������
                                                                    // ������������� ����������� �������������� �������.
        int trigSource = TrigSource::Get();

        if (TrigInput::IsAC() && trigSource <= TrigSource::B)
        {
            int16 rShift = set.chan[trigSource].rshift;
            float rShiftAbs = RShift::ToAbs(rShift, set.chan[trigSource].range);
            trigLev += rShiftAbs;
        }

        Text text(LANG_RU ? "�� ����� = " : "Trig lvl = ");
        text.Append(Voltage(trigLev).ToText());

        int width = 97;
        int x = (Grid::Width() - width) / 2 + Grid::Left();
        int y = Grid::BottomMessages() - 20;
        Rectangle(width, 11).Draw(x, y, Color::FILL);
        Region(width - 2, 9).Fill(x + 1, y + 1, Color::BACK);
        text.Draw(x + 2, y + 1, Color::FILL);
    }
}


void TShift::Draw()
{
    BitSet64 bs = SettingsDisplay::PointsOnDisplay();

    // ������ tShift
    int tshift = TPos::InPoints(set.memory.enum_points_fpga, set.time.t_pos) - set.time.shift;

    if (Math::InRange(tshift, bs.first, bs.second))
    {
        int x = Grid::Left() + tshift - bs.first - 3;
        Char(Symbol::S8::TSHIFT_NORM_1).Draw2SymbolsInPosition(x, Grid::TOP - 1, Symbol::S8::TSHIFT_NORM_2, Color::BACK,
            Color::FILL);
    }
    else if (tshift < bs.first)
    {
        Char(Symbol::S8::TSHIFT_LEFT_1).Draw2SymbolsInPosition(Grid::Left() + 1, Grid::TOP, Symbol::S8::TSHIFT_LEFT_2,
            Color::BACK, Color::FILL);
        Line().Draw(Grid::Left() + 9, Grid::TOP + 1, Grid::Left() + 9, Grid::TOP + 7, Color::BACK);
    }
    else if (tshift > bs.second)
    {
        Char(Symbol::S8::TSHIFT_RIGHT_1).Draw2SymbolsInPosition(Grid::Right() - 8, Grid::TOP,
            Symbol::S8::TSHIFT_RIGHT_2, Color::BACK, Color::FILL);
        Line().Draw(Grid::Right() - 9, Grid::TOP + 1, Grid::Right() - 9, Grid::TOP + 7, Color::BACK);
    }
}


void TPos::Draw()
{
    int x = TPos::InPoints(set.memory.enum_points_fpga, set.time.t_pos) -
        set.display.shift_in_memory_in_points +
        Grid::Left();

    if (x >= Grid::Left() && x <= Grid::Right())
    {
        Draw(x, Grid::TOP - 1);
    }
}


void TPos::Draw(int x, int y)
{
    Region(6, 6).Fill(x - 3, y, Color::BACK);

    Char(Symbol::S8::TPOS_1).Draw(x - 3, y, Color::FILL);
}


Channel &Channel::FirstDrawing()
{
    return set.display.last_affected_channel.IsA() ? ChB : ChA;
}


Channel &Channel::SecondDrawing()
{
    return FirstDrawing().IsA() ? ChB : ChA;
}
