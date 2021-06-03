// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/Timer_.h"
#include "common/Utils/Math_.h"
#include "Display/Console.h"
#include "Display/Display.h"
#include "Display/PainterData.h"
#include "Display/Warnings.h"
#include "Display/Screen/BottomPart.h"
#include "Display/Screen/TopPart.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include <cstdio>
#include <cstring>


//static pFuncVV funcAdditionDraw = 0;
//static pFuncVV funcAfterDraw    = 0;

static bool needFinishDraw = true;      // Если 1, то дисплей нуждается в перерисовке
static uint numDrawingSignals = 0;      // Число нарисованных сигналов для режима накопления


pFuncVV Display::funcDrawTimer = nullptr;
pchar Display::Message::text = nullptr;
uint  Display::Message::timeStart = 0;


void Display::RotateTrigLev()
{
    if (set.display.time_show_levels)
    {
        TrigLev::show_level = true;
        Timer::Enable(TypeTimer::ShowLevelTrigLev, set.display.time_show_levels * 1000,
            FuncOnTimerDisableShowLevelTrigLev);
    }

    Redraw();
}


void Display::FuncOnTimerDisableShowLevelTrigLev()
{
    DisableShowLevelTrigLev();
}


void Display::Redraw()
{
    needFinishDraw = true;
}


bool Display::NeedForClearScreen()
{
    ENumAccumulation &enum_accum = set.display.enum_accum;;

    if (TBase::IsRandomize() ||
        enum_accum == 1 ||
        set.display.mode_acum.IsNoReset() ||
        PageTime::InSelfRecoredMode())
    {
        return true;
    }

    if (needFinishDraw)
    {
        needFinishDraw = false;
        return true;
    }

    if (set.display.mode_acum.IsReset() && (numDrawingSignals >= (uint)enum_accum))
    {
        numDrawingSignals = 0;
        return true;
    }
    return false;
}


bool Display::NeedUpdate()
{
    if (funcDrawTimer != nullptr)
    {
        return false;
    }

    static uint prevTime = 0;       // Время предыдущей отрисовки

    if (TIME_MS - prevTime >= set.display.enum_fps.DeltaTime())
    {
        prevTime = TIME_MS;

        return true;
    }

    return false;
}


void Display::Update()
{
    if (!NeedUpdate())
    {
        return;
    }

    bool need_clear = NeedForClearScreen();

    if (need_clear)
    {
        BeginFrame(Color::BLACK);
        Grid::Draw();
    }

    PainterData::DrawData();

    if (need_clear)
    {
//        DrawMath();
//        DrawSpectrum();

        Cursors::Draw();
        TopPart::Draw();
        BottomPart::Draw();
        RShift::DrawCursors();
        TrigLev::DrawCursor();
        RShift::Draw();
        Measure::DrawAll();
        Menu::DrawStringNavigation();
        TPos::Draw();
        TShift::Draw();
    }

    Menu::Draw();

    if (need_clear)
    {
        Warnings::Draw();
    }

    Console::Draw();

    if (need_clear)
    {
        TrigLev::WriteValue();
    }

    EndFrame();
}


int Display::DrawScaleLine(int x, bool forTrigLev)
{
    static const int DELTA = 5;

    if(set.display.alt_markers.IsHide())
    {
        return DELTA;
    }

    int width = 6;
    int topY = Grid::TOP + DELTA;
    int x2 = width + x + 2;
    int bottomY  = Grid::ChannelBottom() - DELTA;
    int centerY = (Grid::ChannelBottom() + Grid::TOP) / 2;
    int levels[] =
    {
        topY,
        bottomY,
        centerY,
        centerY - (bottomY - topY) / (forTrigLev ? 8 : 4),
        centerY + (bottomY - topY) / (forTrigLev ? 8 : 4)
    };

    for(int i = 0; i < 5; i++)
    {
        Line().Draw(x + 1, levels[i], x2 - 1, levels[i], Color::FILL);
    }

    return DELTA;
}


void Display::DrawTimeForFrame(uint timeTicks)
{
    if(!setNRST.show_stats)
    {
        return;
    }
    static char buffer[10];
    static bool first = true;
    static uint timeMSstartCalculation = 0;
    static int numFrames = 0;
    static float numMS = 0.0F;
    if(first)
    {
        timeMSstartCalculation = TIME_MS;
        first = false;
    }
    numMS += timeTicks / 120000.0F;
    numFrames++;
    
    if((TIME_MS - timeMSstartCalculation) >= 500)
    {
        std::sprintf(buffer, "%.1fms/%d", numMS / numFrames, numFrames * 2);
        timeMSstartCalculation = TIME_MS;
        numMS = 0.0F;
        numFrames = 0;
    }

    Rectangle(84, 10).Draw(Grid::Left(), Grid::FullBottom() - 10, Color::FILL);
    Region(82, 8).Fill(Grid::Left() + 1, Grid::FullBottom() - 9, Color::BACK);
    Text(buffer).Draw(Grid::Left() + 2, Grid::FullBottom() - 9, Color::FILL);

//    std::sprintf(message, "%d", Storage::NumElementsWithSameSettings());

    Text message("1");

//    char numAvail[10] = {0};
//    std::sprintf(numAvail, "%d", Storage::NumberAvailableEntries());
//    std::strcat(message, numAvail);
    message.Draw(Grid::Left() + 50, Grid::FullBottom() - 9);
}


void Display::DisableShowLevelTrigLev()
{
    TrigLev::show_level = false;
    Timer::Disable(TypeTimer::ShowLevelTrigLev);
}


void Display::SetDrawMode(DrawMode::E mode, pFuncVV func)
{
    funcDrawTimer = func;

    if (mode == DrawMode::Timer)
    {
        Timer::Enable(TypeTimer::DisplayUpdate, 100, funcDrawTimer);
    }
    else if (mode == DrawMode::Default)
    {
        funcDrawTimer = nullptr;

        Timer::Disable(TypeTimer::DisplayUpdate);
    }
}



void Display::SetAddDrawFunction(pFuncVV /*func*/)
{
//    funcAdditionDraw = func;
}


void Display::RemoveAddDrawFunction()
{
//    funcAdditionDraw = 0;
}


void Display::Clear()
{
    Region(WIDTH - 1, HEIGHT - 2).Fill(0, 0, Color::BACK);
}


void Display::ShiftScreen(int delta)
{
    set.display.shift_in_memory_in_points = Math::Limitation<int16>(
        (int16)(set.display.shift_in_memory_in_points + delta),
        0,
        (int16)(set.memory.enum_points_fpga.PointsInChannel() - 282)
        );
}


void Display::ChangedRShiftMarkers()
{
    RShift::draw_markers = set.display.alt_markers.IsHide();
    Timer::Enable(TypeTimer::RShiftMarkersAutoHide, 5000, FuncOnTimerRShiftMarkersAutoHide);
}

void Display::FuncOnTimerRShiftMarkersAutoHide()
{
    OnRShiftMarkersAutoHide();
}


void Display::OnRShiftMarkersAutoHide()
{
    RShift::draw_markers = false;
    Timer::Disable(TypeTimer::RShiftMarkersAutoHide);
}


void Display::RunAfterDraw(pFuncVV /*func*/)
{
//    funcAfterDraw = func;
}


void Display::Message::Show(pchar _text)
{
    text = _text;

    timeStart = TIME_MS;

    Display::SetDrawMode(DrawMode::Timer, FuncDraw);
}


void Display::Message::Hide()
{
    Display::SetDrawMode(DrawMode::Default);
}


void Display::Message::FuncDraw()
{
    uint time = ((TIME_MS - timeStart) / 50) % 100;

    if (time > 50)
    {
        time = (100 - time);
    }

    int width = 200;
    int height = 80;
    int x = (Display::WIDTH - width) / 2;
    int y = (Display::HEIGHT - height) / 2;

    Region(width, height).Fill(x, y, Color::BACK);
    Rectangle(width, height).Draw(x, y, Color::FILL);

    int length = Font::GetLengthText(text);

    if (length < width)
    {
        Text(text).DrawInCenterRect(x, y, width, height - 20);
    }
    else
    {
        Text(text).DrawInRectWithTransfers(x + 11, y + 20, width - 22, height - 20);
    }

    char buf[100] = { 0 };

    for (uint i = 0; i < time; i++)
    {
        std::strcat(buf, ".");
    }

    Text(buf).DrawInCenterRect(x, y + 20, width, height - 20);

    Display::EndFrame();
}
