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


//static pFuncVV funcOnHand       = 0;
//static pFuncVV funcAdditionDraw = 0;
//static pFuncVV funcAfterDraw    = 0;

static bool needFinishDraw = true;      // ≈сли 1, то дисплей нуждаетс€ в перерисовке
static uint numDrawingSignals = 0;      // „исло нарисованных сигналов дл€ режима накоплени€


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


void Display::Update()
{
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
    if(!set.debug.show_stats)
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


void Display::SetDrawMode(DrawMode::E /*mode*/, pFuncVV /*func*/)
{
//    funcOnHand = (mode == DrawMode::Auto) ? 0 : func;
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
    set.display.shift_in_memory_in_points = Math::Limitation<int16>((int16)(TShift::GetInMemory() + delta),
        0,
        (int16)(set.memory.enum_points_fpga.BytesInChannel() - 282));
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
