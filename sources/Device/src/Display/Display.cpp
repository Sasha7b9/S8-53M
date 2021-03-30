#include "defines.h"
#include "common/Log_.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/Timer_.h"
#include "common/Utils/Math_.h"
#include "Display/BottomPart.h"
#include "Display/Display.h"
#include "Display/PainterData.h"
#include "Display/TopPart.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Math.h"
#include "FPGA/FPGA_Reader.h"
#include "FPGA/FPGA_Types.h"
#include "LAN/LAN.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "Utils/ProcessingSignal.h"
#include "VCP/VCP.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>


#define MAX_NUM_STRINGS         35
#define SIZE_BUFFER_FOR_STRINGS 2000
static char                     *strings[MAX_NUM_STRINGS] = {0};
static char                     bufferForStrings[SIZE_BUFFER_FOR_STRINGS] = {0};
static int                      lastStringForPause = -1;

#define NUM_WARNINGS            10
static const char               *warnings[NUM_WARNINGS] = {0};      // Здесь предупреждающие сообщения.
static uint                     timeWarnings[NUM_WARNINGS] = {0};   // Здесь время, когда предупреждающее сообщение
                                                                    // поступило на экран.

//static pFuncVV funcOnHand       = 0;
//static pFuncVV funcAdditionDraw = 0;
//static pFuncVV funcAfterDraw    = 0;

static bool showLevelTrigLev = false;    // Нужно ли рисовать горизонтальную линию уровня смещения уровня синхронизации
static bool needFinishDraw = true;      // Если 1, то дисплей нуждается в перерисовке
static uint numDrawingSignals = 0;      // Число нарисованных сигналов для режима накопления



void Display::DrawStringNavigation() 
{
    Text string = Menu::StringNavigation();

    uint length = string.Size();

    if(length != 0) 
    {
        int height = 10;
        Rectangle((int)length + 2, height).Draw(Grid::Left(), Grid::TOP, Color::FILL);
        Region((int)length, height - 2).Fill(Grid::Left() + 1, Grid::TOP + 1, Color::BACK);
        string.Draw(Grid::Left() + 2, Grid::TOP + 1, Color::FILL);
    }
}


void Display::RotateRShift(const Channel &ch)
{
    set.display.last_affected_channel = ch.value;

    if(set.display.time_show_levels)
    {
        if (ch.IsA()) { RShift::show_level_A = true; }
        else                    { RShift::show_level_B = true; }
        Timer::Enable(ch.IsA() ? TypeTimer::ShowLevelRShift0 : TypeTimer::ShowLevelRShift1,
            set.display.time_show_levels * 1000, ch.IsA() ? FuncOnTimerDisableShowLevelRShiftA :
            FuncOnTimerDisableShowLevelRShiftB);
    };

    Redraw();
};


void Display::FuncOnTimerDisableShowLevelRShiftA()
{
    DisableShowLevelRShiftA();
}


void Display::FuncOnTimerDisableShowLevelRShiftB()
{
    DisableShowLevelRShiftB();
}


void Display::RotateTrigLev()
{
    if (set.display.time_show_levels)
    {
        showLevelTrigLev = true;
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


//static void CalculateCoord(int &x, int &y)
//{
//    int width = 40;
//
//    static int dirX = 1;
//    x += dirX;
//
//    if (x > 320 - width)
//    {
//        dirX = -dirX;
//    }
//
//    if (x < 0)
//    {
//        dirX = -dirX;
//    }
//
//    static int dirY = 1;
//    y += dirY;
//
//    if (y > 240 - width)
//    {
//        dirY = -dirY;
//    }
//
//    if (y < 0)
//    {
//        dirY = -dirY;
//    }
//}


//static void DrawIndicator()
//{
//    static int x = 0;
//    static int dx = 1;
//
//    Region(10, 10).Fill(x, 0, Color::FILL);
//
//    x += dx;
//
//    if (x > 310)
//    {
//        dx = -1;
//    }
//    else if (x < 0)
//    {
//        dx = 1;
//    }
//}


void Display::Update()
{

//    uint timeStart = gTimerTics;

    bool needClear = NeedForClearScreen();
    if (needClear)
    {
        BeginFrame(Color::BLACK);
//        DrawMemoryWindow();
        Grid::Draw();
    }

    PainterData::DrawData();

    if (needClear)
    {
//        DrawMath();
//        DrawSpectrum();
        Cursors::Draw();
        TopPart::Draw();
        BottomPart::Draw();
//        DrawCursorsWindow();
//        DrawCursorTrigLevel();
        RShift::Draw();
        Measure::DrawAll();
//        DrawStringNavigation();
//        DrawCursorTShift();
    }

    Menu::Draw();

//    DrawIndicator();

//    if (needClear)
//    {
//        DrawWarnings();
//    }
//
    DrawConsole();
//
//    if (needClear)
//    {
//        WriteValueTrigLevel();
//    }
//
//    DrawTimeForFrame(gTimerTics - timeStart);

//    Color::FILL.SetAsCurrent();

    EndFrame();
}


void Display::WriteValueTrigLevel()
{
    if (showLevelTrigLev && set.memory.mode_work.IsDirect())
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


#define  DELTA 5


void Display::DrawScaleLine(int x, bool forTrigLev)
{
    if(set.display.alt_markers.IsHide())
    {
        return;
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
}


void Display::DrawCursorsWindow()
{
    if((!Menu::IsMinimize() || !Menu::IsShown()) && RShift::draw_markers)
    {
        DrawScaleLine(2, false);
    }
}


void Display::DrawCursorTrigLevel()
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
    if(y > Grid::ChannelBottom())
    {
        Char(Symbol::S8::TRIG_LEV_LOWER).Draw(x + 3, Grid::ChannelBottom() - 11);;
        Point().Draw(x + 5, Grid::ChannelBottom() - 2);
        y = Grid::ChannelBottom() - 7;
        x--;
    }
    else if(y < Grid::TOP)
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

    const char simbols[3] = {'1', '2', 'В'};

    Char(simbols[TrigSource::Get()]).Draw(x + 5, y - 9, Color::BACK);
    Font::Set(TypeFont::S8);

    if (RShift::draw_markers && !Menu::IsMinimize())
    {
        DrawScaleLine(WIDTH - 11, true);
        int left = Grid::Right() + 9;
        int height = Grid::ChannelHeight() - 2 * DELTA;
        int shiftFullMin = RShift::MIN + TrigLev::MIN;
        int shiftFullMax = RShift::MAX + TrigLev::MAX;
        scale = (float)height / (shiftFullMax - shiftFullMin);
        int shiftFull = TrigLev::Get() + (TrigSource::IsExt() ? 0 : RShift::Get((Channel::E)ch));
        int yFull = (int)(Grid::TOP + DELTA + height - scale * (shiftFull - RShift::MIN - TrigLev::MIN) - 4);
        Region(4, 6).Fill(left + 2, yFull + 1, Color::Trig());
        Font::Set(TypeFont::S5);
        Char(simbols[TrigSource::Get()]).Draw(left + 3, yFull - 5, Color::BACK);
        Font::Set(TypeFont::S8);
    }
}


void Display::DrawCursorTShift()
{
//    int firstPoint = 0;
//    int lastPoint = 0;
//    SettingsDisplay::PointsOnDisplay(&firstPoint, &lastPoint);
//
//    if (Storage::set == nullptr)
//    {
//        return;
//    }
//
//    // Рисуем TPos
//    int shiftTPos = TPos::InPoints(
//        (PeackDetMode::E)Storage::set->peakDet, (int)Storage::set->length1channel, SET_TPOS) - SHIFT_IN_MEMORY;
//
//    float scale = (float)((lastPoint - firstPoint) / Grid::Width());
//    int gridLeft = Grid::Left();
//    int x = (int)(gridLeft + shiftTPos * scale - 3);
//    if (GF::IntInRange(x + 3, gridLeft, Grid::Right() + 1))
//    {
//        Char(Symbol::S8::TPOS_2).Draw2SymbolsInPosition(x, Grid::TOP - 1, Symbol::S8::TPOS_3, Color::BACK, Color::FILL);
//    };
//
//    // Рисуем tShift
//    int shiftTShift = TPos::InPoints((PeackDetMode::E)Storage::set->peakDet, (int)Storage::set->length1channel,
//        SET_TPOS) - TShift::InPoints((PeackDetMode::E)Storage::set->peakDet);
//
//    if(GF::IntInRange(shiftTShift, firstPoint, lastPoint))
//    {
//        x = gridLeft + shiftTShift - firstPoint - 3;
//        Char(Symbol::S8::TSHIFT_NORM_1).Draw2SymbolsInPosition(x, Grid::TOP - 1, Symbol::S8::TSHIFT_NORM_2, Color::BACK,
//            Color::FILL);
//    }
//    else if(shiftTShift < firstPoint)
//    {
//        Char(Symbol::S8::TSHIFT_LEFT_1).Draw2SymbolsInPosition(gridLeft + 1, Grid::TOP, Symbol::S8::TSHIFT_LEFT_2,
//            Color::BACK, Color::FILL);
//        Line().Draw(Grid::Left() + 9, Grid::TOP + 1, Grid::Left() + 9, Grid::TOP + 7, Color::BACK);
//    }
//    else if(shiftTShift > lastPoint)
//    {
//        Char(Symbol::S8::TSHIFT_RIGHT_1).Draw2SymbolsInPosition(Grid::Right() - 8, Grid::TOP,
//            Symbol::S8::TSHIFT_RIGHT_2, Color::BACK, Color::FILL);
//        Line().Draw(Grid::Right() - 9, Grid::TOP + 1, Grid::Right() - 9, Grid::TOP + 7, Color::BACK);
//    }
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


void Display::DisableShowLevelRShiftA()
{
    RShift::show_level_A = false;
    Timer::Disable(TypeTimer::ShowLevelRShift0);
}


void Display::DisableShowLevelRShiftB()
{
    RShift::show_level_B = false;
    Timer::Disable(TypeTimer::ShowLevelRShift1);
}


void Display::DisableShowLevelTrigLev()
{
    showLevelTrigLev = false;
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
    LIMITATION(set.display.shift_in_memory, (int16)(TShift::GetInMemory() + delta), 0,
        (int16)(SettingsMemory::GetNumPoints(false) - 282));
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


int Display::FirstEmptyString()
{
    for(int i = 0; i < MAX_NUM_STRINGS; i++)
    {
        if(strings[i] == 0)
        {
            return i;
        }
    }
    return MAX_NUM_STRINGS - 1;
}


int Display::CalculateFreeSize()
{
    int firstEmptyString = FirstEmptyString();

    if(firstEmptyString == 0)
    {
        return SIZE_BUFFER_FOR_STRINGS;
    }

    return (int)(SIZE_BUFFER_FOR_STRINGS - (strings[firstEmptyString - 1] - bufferForStrings) -
        std::strlen(strings[firstEmptyString - 1]) - 1);
}


void Display::DeleteFirstString()
{
    if(FirstEmptyString() < 2)
    {
        return;
    }
    int delta = (int)(std::strlen(strings[0])) + 1;
    int numStrings = FirstEmptyString();
    for(int i = 1; i < numStrings; i++)
    {
        strings[i - 1] = strings[i] - delta;
    }
    for(int i = numStrings - 1; i < MAX_NUM_STRINGS; i++)
    {
        strings[i] = 0; //-V557
    }
    for(int i = 0; i < SIZE_BUFFER_FOR_STRINGS - delta; i++)
    {
        bufferForStrings[i] = bufferForStrings[i + delta];
    }
}



void Display::AddString(pchar string)
{
    if(set.debug.console_in_pause)
    {
        return;
    }
    static int num = 0;
    char buffer[100];
    std::sprintf(buffer, "%d\x11", num++);
    std::strcat(buffer, string);
    int size = (int)(std::strlen(buffer)) + 1;
    while(CalculateFreeSize() < size)
    {
        DeleteFirstString();
    }
    if(!strings[0])
    {
        strings[0] = bufferForStrings;
        std::strcpy(strings[0], buffer);
    }
    else
    {
        char *addressLastString = strings[FirstEmptyString() - 1];
        char *address = addressLastString + std::strlen(addressLastString) + 1;
        strings[FirstEmptyString()] = address;
        std::strcpy(address, buffer);
    }
}



void Display::AddStringToIndicating(pchar string)
{
    if(FirstEmptyString() == (MAX_NUM_STRINGS - 1))
    {
        DeleteFirstString();
    }

    AddString(string);
}



void Display::SetPauseForConsole(bool pause)
{
    if(pause)
    {
        lastStringForPause = FirstEmptyString() - 1;
    }
    else
    {
        lastStringForPause = -1;
    }
}



void Display::OneStringUp()
{
    if(!set.debug.console_in_pause)
    {
    }
    else if(lastStringForPause > set.debug.num_srings - 1)
    {
        lastStringForPause--;
    }
}



void Display::OneStringDown()
{
    if(!set.debug.console_in_pause)
    {
    }
    else if(lastStringForPause < FirstEmptyString() - 1)
    {
        lastStringForPause++;
    }
}



void Display::DrawConsole()
{
    int count = 0;
    Font::Set(SettingsDebug::GetSizeFontForConsole() == 5 ? TypeFont::S5 : TypeFont::S8);
    int height = Font::GetSize();

    int last_string = FirstEmptyString() - 1;
    int numStr = set.debug.num_srings;

    if(height == 8 && numStr > 22)
    {
        numStr = 22;
    }

    int delta = 0;

    if(set.display.show_string_navigation)
    {
        numStr -= ((height == 8) ? 1 : 2);
        delta = 10;
    }

    int first_string = last_string - numStr + 1;

    if(first_string < 0)
    {
        first_string = 0;
    }

    for(int num_string = first_string; num_string <= last_string; num_string++)
    {
        int width = Font::GetLengthText(strings[num_string]);
        Region(width, height + 1).Fill(Grid::Left() + 1, Grid::TOP + 1 + count * (height + 1) + delta, Color::BACK);
        int y = Grid::TOP + 5 + count * (height + 1) - 4;

        if(Font::GetSize() == 5)
        {
            y -= 3;
        }

        Text(strings[num_string]).Draw(Grid::Left() + 2, y + delta, Color::FILL);

        count++;
    }

    Font::Set(TypeFont::S8);
}


void Display::ShowWarn(pchar message)
{
    if (warnings[0] == 0)
    {
        Timer::Enable(TypeTimer::ShowMessages, 100, OnTimerShowWarning);
    }
    bool alreadyStored = false;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (warnings[i] == 0 && !alreadyStored)
        {
            warnings[i] = message;
            timeWarnings[i] = TIME_MS;
            alreadyStored = true;
        }
        else if (warnings[i] == message)
        {
            timeWarnings[i] = TIME_MS;
            return;
        }
    }
}


void Display::OnTimerShowWarning()
{
    uint time = TIME_MS;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (time - timeWarnings[i] > (uint)(set.display.time_messages * 1000))
        {
            timeWarnings[i] = 0;
            warnings[i] = 0;
        }
    }

    int pointer = 0;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (warnings[i] != 0)
        {
            warnings[pointer] = warnings[i];
            timeWarnings[pointer] = timeWarnings[i];
            if (pointer != i)
            {
                timeWarnings[i] = 0;
                warnings[i] = 0;
            }
            pointer++;
        }
    }

    if (pointer == 0)
    {
        Timer::Disable(TypeTimer::ShowMessages);
    }
}


void Display::ClearFromWarnings()
{
    Timer::Disable(TypeTimer::ShowMessages);
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        warnings[i] = 0;
        timeWarnings[i] = 0;
    }
}



void Display::ShowWarningBad(Warning::E warning)
{
    Color::ResetFlash();
    ShowWarn(Tables::GetWarning(warning));
    Sound::WarnBeepBad();
}



void Display::ShowWarningGood(Warning::E warning)
{
    Color::ResetFlash();
    ShowWarn(Tables::GetWarning(warning));
    Sound::WarnBeepGood();
}


void Display::DrawStringInRectangle(int, int y, char const *text)
{
    int width = Font::GetLengthText(text);
    int height = 8;
    Rectangle(width + 4, height + 4).Draw(Grid::Left(), y, Color::FILL);
    Rectangle(width + 2, height + 2).Draw(Grid::Left() + 1, y + 1, Color::BACK);
    Region(width, height).Fill(Grid::Left() + 2, y + 2, Color::FLASH_10);
    Text(text).Draw(Grid::Left() + 3, y + 2, Color::FLASH_01);
}


void Display::DrawWarnings()
{
    int delta = 12;
    int y = Grid::BottomMessages();
    for(int i = 0; i < 10; i++)
    {
        if(warnings[i] != 0)
        {
            DrawStringInRectangle(Grid::Left(), y, warnings[i]);
            y -= delta;
        }
    }
}


void Display::RunAfterDraw(pFuncVV /*func*/)
{
//    funcAfterDraw = func;
}
