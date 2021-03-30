// 2021/03/18 9:50:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Settings/SettingsTypes_.h"
#include "FPGA/FPGA_Types.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"


void RShift::Draw()
{
    if (!ModeDrawMath::IsDisabled())
    {
        Draw(Channel::Math);
    }

    if (set.display.last_affected_channel == ChB)
    {
        Draw(Channel::A);
        Draw(Channel::B);
    }
    else
    {
        Draw(Channel::B);
        Draw(Channel::A);
    }
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

    int rShift = RShift::Get(ch);

    float scale = Grid::ChannelHeight() / (RShift::STEP() * 200.0F);
    float y = Grid::ChannelCenterHeight() - scale * (rShift - RShift::ZERO);

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
        if (((ch == ChA) ? show_level_A : show_level_B) && set.memory.mode_work.IsDirect())
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
    int trigLev = TrigLev::Get(ch) + (RShift::Get((Channel::E)ch) - RShift::ZERO);
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

    const char simbols[3] = { '1', '2', 'В' };

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
        int shiftFull = TrigLev::Get() + (TrigSource::IsExt() ? 0 : RShift::Get((Channel::E)ch));
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
        float trigLev = RShift::ToAbs(TrigLev::Get(), set.chan[TrigSource::Get()].range);     // WARN Здесь для внешней
                                                                    // синхронизации неправильно рассчитывается уровень.
        TrigSource::E trigSource = TrigSource::Get();
        if (TrigInput::IsAC() && trigSource <= TrigSource::B)
        {
            int16 rShift = RShift::Get((Channel::E)trigSource);
            float rShiftAbs = RShift::ToAbs(rShift, set.chan[trigSource].range);
            trigLev += rShiftAbs;
        }

        Text text(LANG_RU ? "Ур синхр = " : "Trig lvl = ");
        text.Append(Voltage(trigLev).ToText());

        int width = 96;
        int x = (Grid::Width() - width) / 2 + Grid::Left();
        int y = Grid::BottomMessages() - 20;
        Rectangle(width, 10).Draw(x, y, Color::FILL);
        Region(width - 2, 8).Fill(x + 1, y + 1, Color::BACK);
        text.Draw(x + 2, y + 1, Color::FILL);
    }
}
