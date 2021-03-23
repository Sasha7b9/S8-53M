// 2021/03/18 9:50:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Primitives_.h"
#include "common/Display/Text_.h"
#include "common/Display/Font/Font_.h"
#include "common/Settings/SettingsTypes_.h"
#include "FPGA/FPGA_Types.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"


using namespace Primitives;


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


void RShift::Draw(const Channel &ch)
{
    float x = static_cast<float>(Grid::Right() - Grid::Width() - Measure::GetDeltaGridLeft());

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

    float scale = Grid::ChannelHeight() / (STEP_RSHIFT * 200.0F);
    float y = Grid::ChannelCenterHeight() - scale * (rShift - RShift::ZERO);

    if (y > Grid::ChannelBottom())
    {
        Char(Symbol::S8::RSHIFT_LOWER).Draw((int)(x - 7), Grid::ChannelBottom() - 11, ch.GetColor());
        Point().Draw((int)(x - 5), Grid::ChannelBottom() - 2);
        y = static_cast<float>(Grid::ChannelBottom() - 7);
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
        if (((ch == ChA) ? show_level_A : show_level_B) && ModeWork::IsDirect())
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
