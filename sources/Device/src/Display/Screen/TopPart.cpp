// 2021/03/30 9:44:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "Display/Screen/Grid.h"
#include "Display/Screen/TopPart.h"
#include "FPGA/FPGA.h"
#include <cmath>


void TopPart::Draw()
{
    WriteCursors();
    DrawHiRightPart();
}


void TopPart::WriteCursors()
{
    int startX = 43;

    if (set.memory.mode_work.IsDirect())
    {
        startX += 29;
    }

    if (Cursors::NecessaryDraw())
    {
        int x = startX;
        int y1 = 0;
        int y2 = 9;

        VLine().Draw(x, 1, Grid::TOP - 2, Color::FILL);
        x += 3;
        Channel source = set.cursors.source;

        Color colorText = source.GetColor();

        if (!set.cursors.cntrlU[source].IsDisabled())
        {
            Text("1:").Draw(x, y1, colorText);
            Text("2:").Draw(x, y2);
            x += 7;
            Cursors::GetVoltage(source, 0).ToText().Draw(x, y1);
            Cursors::GetVoltage(source, 1).ToText().Draw(x, y2);
            x = startX + 49;

            float pos0 = Cursors::GetVoltage
                (Cursors::GetPosU(source, 0), set.chan[source].range, set.chan[source].rshift);

            float pos1 = Cursors::GetVoltage
                (Cursors::GetPosU(source, 1), set.chan[source].range, set.chan[source].rshift);

            float delta = std::fabsf(pos1 - pos0);
            Text(":dU=").Draw(x, y1);
            Voltage(delta).ToText(false).Draw(x + 17, y1);
            Text(":").Draw(x, y2);
            Cursors::GetPercentsU(source).Draw(x + 10, y2);
        }

        x = startX + 101;
        VLine().Draw(x, 1, Grid::TOP - 2, Color::FILL);
        x += 3;

        if (!set.cursors.cntrlT[source].IsDisabled())
        {
            colorText.SetAsCurrent();
            Text("1:").Draw(x, y1);
            Text("2:").Draw(x, y2);
            x += 7;
            Cursors::GetTime(source, 0).ToText().Draw(x, y1);
            Cursors::GetTime(source, 1).ToText().Draw(x, y2);
            x = startX + 153;
            float pos0 = Cursors::GetTime(set.cursors.posT[source][0], set.time.base);
            float pos1 = Cursors::GetTime(set.cursors.posT[source][1], set.time.base);
            float delta = std::fabsf(pos1 - pos0);
            Text(":dT=").Draw(x, y1);
            Time(delta).ToText(false).Draw(x + 17, y1);
            Text(":").Draw(x, y2);
            Cursors::GetPercentsT(source).Draw(x + 8, y2);

            if (set.cursors.show_freq)
            {
                int width = 65;
                int x0 = Grid::Right() - width;
                Rectangle(width, 12).Draw(x0, Grid::TOP, Color::FILL);
                Region(width - 2, 10).Fill(x0 + 1, Grid::TOP + 1, Color::BACK);
                Text("1/dT=").Draw(x0 + 1, Grid::TOP + 2, colorText);
                Frequency(1.0F / delta).ToText().Draw(x0 + 25, Grid::TOP + 2);
            }
        }
    }
}


void TopPart::DrawHiRightPart()
{
    // Синхроимпульс
    int y = 2;

    static const int xses[3] = { 280, 271, 251 };
    int x = xses[set.memory.mode_work];

    if (!set.memory.mode_work.IsLatest())
    {
        HLine().Draw(x, 1, Grid::TOP - 2, Color::FILL);

        x += 2;

        if (TrigLev::exist_impulse)
        {
            Region(Grid::TOP - 3, Grid::TOP - 7).Fill(x, 1 + y);
            Text(LANG_RU ? "СИ" : "Tr").Draw(x + 3, 3 + y, Color::BACK);
        }
    }

    // Режим работы
    static pchar strings_[][2] =
    {
        {"ИЗМ",     "MEAS"},
        {"ПОСЛ",    "LAST"},
        {"ВНТР",    "INT"}
    };

    if (!set.memory.mode_work.IsDirect())
    {
        x += 18;
        HLine().Draw(x, 1, Grid::TOP - 2, Color::FILL);
        x += 2;
        Text(LANG_RU ? "режим" : "mode").Draw(LANG_RU ? x : x + 3, -1);
        Text(strings_[set.memory.mode_work][LANG]).DrawInCenterRect(x + 1, 9, 25, 8);
    }
    else
    {
        x -= 9;
    }

    if (!set.memory.mode_work.IsLatest())
    {

        x += 27;
        HLine().Draw(x, 1, Grid::TOP - 2, Color::FILL);

        x += 2;
        y = 1;

        if (FPGA::CurrentStateWork().IsWork())
        {
            Char(Symbol::S8::PLAY).Draw4SymbolsInRect(x, 1);
        }
        else if (FPGA::CurrentStateWork().IsStop())
        {
            Region(10, 10).Fill(x + 3, y + 3);
        }
        else if (FPGA::CurrentStateWork().IsWait())
        {
            int w = 4;
            int h = 14;
            int delta = 4;
            x = x + 2;
            Region(w, h).Fill(x, y + 1);
            Region(w, h).Fill(x + w + delta, y + 1);
        }
    }
}
