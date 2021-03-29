// 2021/03/24 15:17:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "Display/BottomPart.h"
#include "Display/Display.h"
#include "Display/Grid.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "LAN/LAN.h"
#include "Settings/Settings.h"
#include "VCP/VCP.h"
#include <cstdio>
#include <cstring>


void BottomPart::Draw()
{
    int y0 = Display::HEIGHT - 19;
    int y1 = Display::HEIGHT - 10;
    int x = -1;

    VLine vline(17);

    int y = Grid::Bottom() + 2;

    vline.Draw(95, y, Color::FILL);
    vline.Draw(176, y);
    vline.Draw(221, y);
    vline.Draw(276, y);

    WriteTextVoltage(ChA, x + 1, y0);

    WriteTextVoltage(ChB, x + 1, y1);

    x += 98;

    TBase::E tBase = TBase::Get();
    int16 tShift = TShift::Get();

    if (!set.memory.mode_work.IsDirect())
    {
        //        DataSettings *ds = MODE_WORK_IS_LATEST ? Storage::dsLast : Storage::dsInt;
        //
        //        if (ds != 0)
        //        {
        //            tBase = ds->tBase;
        //            tShift = ds->tShift;
        //        }
    }

    Text("р\xa5%s", TBase::ToString(tBase)).Draw(x, y0);

    Text("\xa5%s", TShift::ToString(tShift).c_str()).Draw(x + 35, y0);

    if (set.memory.mode_work.IsDirect())
    {
        pchar source[3] = { "1", "2", "\x82" };

        Text("с\xa5\x10%s", source[TrigSource::Get()]).Draw(x, y1, Color::Trig());

        static pchar couple[] =
        {
            "\x92",
            "\x91",
            "\x92",
            "\x92"
        };

        static pchar polar[] =
        {
            "\xa7",
            "\xa6"
        };

        static pchar filtr[] =
        {
            "\xb5\xb6",
            "\xb5\xb6",
            "\xb3\xb4",
            "\xb1\xb2"
        };

        Text("\xa5\x10%s\x10\xa5\x10%s\x10\xa5\x10", couple[TrigInput::Get()],
            polar[set.trig.polarity]).Draw(x + 18, y1);

        Char(filtr[TrigInput::Get()][0]).Draw(x + 45, y1);
        Char(filtr[TrigInput::Get()][1]).Draw(x + 53, y1);
    }

    if (set.memory.mode_work.IsDirect())
    {
        const char mode[] =
        {
            '\xb7',
            '\xa0',
            '\xb0'
        };

        Text("\xa5\x10%c", mode[set.trig.start_mode]).Draw(x + 63, y1);
    }

    x += 82;
    y0 = y0 - 3;
    y1 = y1 - 6;

    Font::Set(TypeFont::S5);

    if (set.memory.mode_work.IsDirect())
    {
        int y2 = y1 + 6;
        WriteStringAndNumber("накопл", x, y0, set.display.enum_accum);
        WriteStringAndNumber("усредн", x, y1, set.display.enum_ave.value);
        WriteStringAndNumber("мн\x93мкс", x, y2, set.display.enum_min_max.ToAbs());
    }

    x += 42;

    Font::Set(TypeFont::S8);

    if (set.memory.mode_work.IsDirect())
    {
        Text mes_freq("\x7c=");
        float freq = FPGA::FreqMeter::GetFreq();

        mes_freq.Append(freq == 1.0f ? "******" : Frequency(freq).ToText().c_str());

        mes_freq.Draw(x + 3, Grid::Bottom() + 2);
    }

    DrawTime(x + 3, Grid::Bottom() + 11);

    Font::Set(TypeFont::UGO2);

    // ‘лешка
    if (FDrive::isConnected)
    {
        Char(Symbol::S8::FLASH_DRIVE).Draw4SymbolsInRect(x + 57, Grid::Bottom() + 2);
    }

    // Ethernet
    if ((LAN::clientIsConnected || LAN::cableIsConnected) && TIME_MS > 2000)
    {
        Char(Symbol::S8::ETHERNET).Draw4SymbolsInRect(x + 87, Grid::Bottom() + 2, LAN::clientIsConnected ?
            Color::FILL : Color::FLASH_01);
    }

    if (VCP::connectToHost || VCP::cableIsConnected)
    {
        Char(Symbol::UGO2::USB).Draw4SymbolsInRect(x + 72, Grid::Bottom() + 2, VCP::connectToHost ?
            Color::FILL : Color::FLASH_01);
    }

    Color::FILL.SetAsCurrent();
    // ѕиковый детектор
    if (PeackDetMode::IsEnabled())
    {
        Char('\x12').Draw(x + 38, Grid::Bottom() + 11);
        Char('\x13').Draw(x + 46, Grid::Bottom() + 11);
    }

    if (set.memory.mode_work.IsDirect())
    {
        Font::Set(TypeFont::S5);
        WriteStringAndNumber("—√Ћј∆.:", x + 57, Grid::Bottom() + 10, (int)Smoothing::NumPoints());
        Font::Set(TypeFont::S8);
    }
}


void BottomPart::WriteTextVoltage(const Channel &ch, int x, int y)
{
    static pchar couple[] =
    {
        "\x92",
        "\x91",
        "\x90"
    };

    bool inverse = ch.IsInversed();
    ModeCouple &mode_couple = set.chan[ch].mode_сouple;
    Divider::E multiplier = set.chan[ch].divider;
    Range::E range = set.chan[ch].range;
    uint rShift = (uint)RShift::Get(ch);
    bool enable = ch.IsEnabled();

    if (!set.memory.mode_work.IsDirect())
    {
        //        DataSettings *ds = MODE_WORK_IS_DIRECT ? Storage::set : Storage::dsInt;
        //
        //        if (ds != 0)
        //        {
        //            inverse = (ch == ChA) ? ds->inverseCh0 : ds->inverseCh1;
        //            modeCouple = (ch == ChA) ? ds->modeCouple0 : ds->modeCouple1;
        //            multiplier = (ch == ChA) ? ds->multiplier0 : ds->multiplier1;
        //            range = ds->range[ch];
        //            rShift = (ch == ChA) ? ds->rShiftCh0 : ds->rShiftCh1;
        //            enable = (ch == ChA) ? ds->enableCh0 : ds->enableCh1;
        //        }
    }

    if (enable)
    {
        Color color = ch.GetColor();
        Color colorDraw = inverse ? Color::WHITE : color;

        if (inverse)
        {
            const int widthField = 91;
            const int heightField = 8;
            Region(widthField, heightField).Fill(x, y, color);
        }

        Text("%s\xa5%s\xa5%s", (ch == ChA) ? (LANG_RU ? "1к" : "1c") : (LANG_RU ? "2к" : "2c"), couple[mode_couple],
            Range::ToString(range, multiplier)).Draw(x + 1, y, colorDraw);

        Text("\xa5%s", RShift::ToAbs((int16)rShift, range, multiplier).ToText().c_str()).Draw(x + 46, y);
    }
}


void BottomPart::WriteStringAndNumber(pchar text, int x, int y, int number)
{
    Text(text).Draw(x, y, Color::FILL);

    Text buffer;

    if (number == 0)
    {
        buffer.Append("-");
    }
    else
    {
        buffer.Append(Int(number).ToText(false));
    }

    buffer.DrawRelativelyRight(x + 41, y);
}


void BottomPart::DrawTime(int x, int y)
{
    int dField = 13;
    int dSeparator = -1;

    PackedTime time = HAL_RTC::GetPackedTime();

    Color::FILL.SetAsCurrent();

    //    if (MODE_WORK_IS_MEMINT || MODE_WORK_IS_LATEST)
    //    {
    //        DataSettings *ds = MODE_WORK_IS_MEMINT ? Storage::dsInt : Storage::dsLast;
    //
    //        if (ds != 0)
    //        {
    //            y -= 9;
    //            time.day = ds->time.day;
    //            time.hours = ds->time.hours;
    //            time.minutes = ds->time.minutes;
    //            time.seconds = ds->time.seconds;
    //            time.month = ds->time.month;
    //            time.year = ds->time.year;
    //            GF::Int2String((int)time.day, false, 2).Draw(x, y);
    //            Text(":").Draw(x + dField, y);
    //            GF::Int2String((int)time.month, false, 2).Draw(x + dField + dSeparator, y);
    //            Text(":").Draw(x + 2 * dField + dSeparator, y);
    //            GF::Int2String((int)time.year + 2000, false, 4).Draw(x + 2 * dField + 2 * dSeparator, y);
    //            y += 9;
    //        }
    //        else
    //        {
    //            return;
    //        }
    //    }


    x -= 3;
    Int((int)time.hours).ToText(false, 2).Draw(x, y);
    Text(":").Draw(x + dField, y);
    Int((int)time.minutes).ToText(false, 2).Draw(x + dField + dSeparator, y);
    Text(":").Draw(x + 2 * dField + dSeparator, y);
    Int((int)time.seconds).ToText(false, 2).Draw(x + 2 * dField + 2 * dSeparator, y);
}
