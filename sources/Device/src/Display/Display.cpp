#include "defines.h"
#include "common/Log_.h"
#include "common/Display/Painter_.h"
#include "common/Display/Primitives_.h"
#include "common/Display/Text_.h"
#include "common/Display/Font/Font_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/Timer_.h"
#include "common/Utils/Math_.h"
#include "Display/DataPainter.h"
#include "Display/Display.h"
#include "FDrive/FDrive.h"
#include "FPGA/DataSettings.h"
#include "FPGA/FPGA_Reader.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/MathFPGA.h"
#include "FPGA/Storage.h"
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

using namespace Primitives;


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

static bool showLevelRShiftA = false;    // Нужно ли рисовать горизонтальную линию уровня смещения первого канала
static bool showLevelRShiftB = false;
static bool showLevelTrigLev = false;    // Нужно ли рисовать горизонтальную линию уровня смещения уровня синхронизации
static bool trigEnable = false;
static bool drawRShiftMarkers = false;
int Display::topMeasures = Grid::Bottom();
static bool needFinishDraw = true;      // Если 1, то дисплей нуждается в перерисовке
static uint numDrawingSignals = 0;      // Число нарисованных сигналов для режима накопления



void Display::DrawStringNavigation() 
{
    char buffer[100];
    char *string = Menu::StringNavigation(buffer);
    if(string) 
    {
        int length = Font::GetLengthText(string);
        int height = 10;
        Primitives::Rectangle(length + 2, height).Draw(Grid::Left(), Grid::TOP, Color::FILL);
        Region(length, height - 2).Fill(Grid::Left() + 1, Grid::TOP + 1, Color::BACK);
        Text(string).Draw(Grid::Left() + 2, Grid::TOP + 1, Color::FILL);
    }
}


void Display::RotateRShift(Channel::E chan)
{
    DataPainter::ResetP2Ppoints(true);
    LAST_AFFECTED_CHANNEL = chan;
    if(TIME_SHOW_LEVELS)
    {
        if (chan == ChA) { showLevelRShiftA = true; }
        else                    { showLevelRShiftB = true; }
        Timer::Enable((chan == ChA) ? TypeTimer::ShowLevelRShift0 : TypeTimer::ShowLevelRShift1,
            TIME_SHOW_LEVELS  * 1000, (chan == ChA) ? FuncOnTimerDisableShowLevelRShiftA :
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
    if (TIME_SHOW_LEVELS)
    {
        showLevelTrigLev = true;
        Timer::Enable(TypeTimer::ShowLevelTrigLev, TIME_SHOW_LEVELS * 1000, FuncOnTimerDisableShowLevelTrigLev);
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


void Display::DrawTime(int x, int y)
{
    int dField = 10;
    int dSeparator = 2;

    PackedTime time = HAL_RTC::GetPackedTime();
    
    Color::FILL.SetAsCurrent();
    
    if (MODE_WORK_IS_MEMINT || MODE_WORK_IS_LATEST)
    {
        DataSettings *ds = MODE_WORK_IS_MEMINT ? Storage::dsInt : Storage::dsLast;

        if (ds != 0)
        {
            y -= 9;
            time.day = ds->time.day;
            time.hours = ds->time.hours;
            time.minutes = ds->time.minutes;
            time.seconds = ds->time.seconds;
            time.month = ds->time.month;
            time.year = ds->time.year;
            GF::Int2String((int)time.day, false, 2).Draw(x, y);
            Text(":").Draw(x + dField, y);
            GF::Int2String((int)time.month, false, 2).Draw(x + dField + dSeparator, y);
            Text(":").Draw(x + 2 * dField + dSeparator, y);
            GF::Int2String((int)time.year + 2000, false, 4).Draw(x + 2 * dField + 2 * dSeparator, y);
            y += 9;
        }
        else
        {
            return;
        }
    }
    
    
    GF::Int2String((int)time.hours, false, 2).Draw(x, y);
    Text(":").Draw(x + dField, y);
    GF::Int2String((int)time.minutes, false, 2).Draw(x + dField + dSeparator, y);
    Text(":").Draw(x + 2 * dField + dSeparator, y);
    GF::Int2String((int)time.seconds, false, 2).Draw(x + 2 * dField + 2 * dSeparator, y);
}


void Display::DrawHiPart()
{
    WriteCursors();
    DrawHiRightPart();
}


void Display::WriteCursors()
{
    char buffer[20];
    int startX = 43;
    if(MODE_WORK_IS_DIRECT)
    {
        startX += 29;
    }

    if(sCursors_NecessaryDrawCursors())
    {
        int x = startX;
        int y1 = 0;
        int y2 = 9;

        HLine().Draw(x, 1, Grid::TOP - 2, Color::FILL);
        x += 3;
        Channel::E source = CURS_SOURCE;
        Color colorText = Color::Channel(source);
        if(!CURS_CNTRL_U_IS_DISABLE(source))
        {
            Text("1:").Draw(x, y1, colorText);
            Text("2:").Draw(x, y2);
            x += 7;
            Text(sCursors_GetCursVoltage(source, 0, buffer)).Draw(x, y1);
            Text(sCursors_GetCursVoltage(source, 1, buffer)).Draw(x, y2);
            x = startX + 49;
            float pos0 = MathFPGA::VoltageCursor(sCursors_GetCursPosU(source, 0), SET_RANGE(source), SET_RSHIFT(source));
            float pos1 = MathFPGA::VoltageCursor(sCursors_GetCursPosU(source, 1), SET_RANGE(source), SET_RSHIFT(source));
            float delta = std::fabsf(pos1 - pos0);
            Text(":dU=").Draw(x, y1);
            GF::Voltage2String(delta, false).Draw(x + 17, y1);
            Text(":").Draw(x, y2);
            sCursors_GetCursorPercentsU(source).Draw(x + 10, y2);
        }

        x = startX + 101;
        HLine().Draw(x, 1, Grid::TOP - 2, Color::FILL);
        x += 3;
        if(!CURS_CNTRL_T_IS_DISABLE(source))
        {
            colorText.SetAsCurrent();
            Text("1:").Draw(x, y1);
            Text("2:").Draw(x, y2);
            x+=7;
            Text(sCursors_GetCursorTime(source, 0, buffer)).Draw(x, y1);
            Text(sCursors_GetCursorTime(source, 1, buffer)).Draw(x, y2);
            x = startX + 153;
            float pos0 = MathFPGA::TimeCursor(CURS_POS_T0(source), SET_TBASE);
            float pos1 = MathFPGA::TimeCursor(CURS_POS_T1(source), SET_TBASE);
            float delta = std::fabsf(pos1 - pos0);
            Text(":dT=").Draw(x, y1);
            GF::Time2String(delta, false).Draw(x + 17, y1);
            Text(":").Draw(x, y2);
            sCursors_GetCursorPercentsT(source).Draw(x + 8, y2);

            if(CURSORS_SHOW_FREQ)
            {
                int width = 65;
                int x0 = Grid::Right() - width;
                Primitives::Rectangle(width, 12).Draw(x0, Grid::TOP, Color::FILL);
                Region(width - 2, 10).Fill(x0 + 1, Grid::TOP + 1, Color::BACK);
                Text("1/dT=").Draw(x0 + 1, Grid::TOP + 2, colorText);
                GF::Freq2String(1.0F / delta).Draw(x0 + 25, Grid::TOP + 2);
            }
        }
    }
}


void Display::DrawHiRightPart()
    {
    // Синхроимпульс
    int y = 2;

    static const int xses[3] = {280, 271, 251};
    int x = xses[MODE_WORK];

    if (!MODE_WORK_IS_LATEST)
    {
        HLine().Draw(x, 1, Grid::TOP - 2, Color::FILL);

        x += 2;

        if (trigEnable)
        {
            Region(Grid::TOP - 3, Grid::TOP - 7).Fill(x, 1 + y);
            Text(LANG_RU ? "СИ" : "Tr").Draw(x + 3, 3 + y, Color::BACK);
        }
    }

    // Режим работы
    static const char *strings_[][2] =
    {
        {"ИЗМ",     "MEAS"},
        {"ПОСЛ",    "LAST"},
        {"ВНТР",    "INT"}
    };

    if(!MODE_WORK_IS_DIRECT)
    {
        x += 18;
        HLine().Draw(x, 1, Grid::TOP - 2, Color::FILL);
        x += 2;
        Text(LANG_RU ? "режим" : "mode").Draw(LANG_RU ? x : x + 3, -1);
        Text(strings_[MODE_WORK][LANG]).DrawInCenterRect(x + 1, 9, 25, 8);
    }
    else
    {
        x -= 9;
    }

    if (!MODE_WORK_IS_LATEST)
    {

        x += 27;
        HLine().Draw(x, 1, Grid::TOP - 2, Color::FILL);

        x += 2;
        y = 1;
        if (FPGA::CurrentStateWork() == StateWorkFPGA::Work)
        {
            Char(Symbol::S8::PLAY).Draw4SymbolsInRect(x, 1);
        }
        else if (FPGA::CurrentStateWork() == StateWorkFPGA::Stop)
        {
            Region(10, 10).Fill(x + 3, y + 3);
        }
        else if (FPGA::CurrentStateWork() == StateWorkFPGA::Wait)
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



void Display::DrawCursorsRShift()
{
    if (!DISABLED_DRAW_MATH)
    {
        DrawCursorRShift(Channel::Math);
    }
    if(LAST_AFFECTED_CHANNEL_IS_B)
    {
        DrawCursorRShift(ChA);
        DrawCursorRShift(ChB);
    }
    else
    {
        DrawCursorRShift(ChB);
        DrawCursorRShift(ChA);
    }
}



bool Display::NeedForClearScreen()
{
    int numAccum = NUM_ACCUM;

    if (sTime_RandomizeModeEnabled() || numAccum == 1 || MODE_ACCUM_IS_NORESET || SET_SELFRECORDER)
    {
        return true;
    }

    if (needFinishDraw)
    {
        needFinishDraw = false;
        return true;
    }

    if (MODE_ACCUM_IS_RESET && (numDrawingSignals >= static_cast<uint>(numAccum))) //-V560
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


static void DrawIndicator()
{
    static int x = 0;
    static int dx = 1;

    Region(10, 10).Fill(x, 0, Color::FILL);

    x += dx;

    if (x > 310)
    {
        dx = -1;
    }
    else if (x < 0)
    {
        dx = 1;
    }
}


void Display::Update()
{
//    uint timeStart = gTimerTics;

    bool needClear = NeedForClearScreen();

    if (needClear)
    {
        BeginFrame(Color::BLACK);
//        DrawMemoryWindow();
        DrawFullGrid();
    }

    DataPainter::DrawData();

    if (needClear)
    {
//        DrawMath();
//        DrawSpectrum();
//        DrawCursors();
//        DrawHiPart();
        DrawLowPart();
//        DrawCursorsWindow();
//        DrawCursorTrigLevel();
//        DrawCursorsRShift();
//        DrawMeasures();
//        DrawStringNavigation();
//        DrawCursorTShift();
    }

    Menu::Draw();

    DrawIndicator();

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
    if (showLevelTrigLev && MODE_WORK_IS_DIRECT)
    {
        float trigLev = RSHIFT_2_ABS(TRIG_LEVEL_SOURCE, SET_RANGE(TRIG_SOURCE));     // WARN Здесь для внешней синхронизации неправильно рассчитывается уровень.
        TrigSource::E trigSource = TRIG_SOURCE;
        if (TRIG_INPUT_IS_AC && trigSource <= TrigSource::B_)
        {
            int16 rShift = SET_RSHIFT(trigSource);
            float rShiftAbs = RSHIFT_2_ABS(rShift, SET_RANGE(trigSource));
            trigLev += rShiftAbs;
        }
        char buffer[20];
        std::strcpy(buffer, LANG_RU ? "Ур синхр = " : "Trig lvl = "); //-V2513
        std::strcat(buffer, GF::Voltage2String(trigLev, true).c_str()); //-V2513
        int width = 96;
        int x = (Grid::Width() - width) / 2 + Grid::Left();
        int y = Grid::BottomMessages() - 20;
        Primitives::Rectangle(width, 10).Draw(x, y, Color::FILL);
        Region(width - 2, 8).Fill(x + 1, y + 1, Color::BACK);
        Text(buffer).Draw(x + 2, y + 1, Color::FILL);
    }
}


void Display::DrawGridSpectrum()
{
    if (SCALE_FFT_IS_LOG)
    {
        static const int nums[] = {4, 6, 8};
        static const char *strs[] = {"0", "-10", "-20", "-30", "-40", "-50", "-60", "-70"};
        int numParts = nums[FFT_MAX_DB];
        float scale = (float)Grid::MathHeight() / numParts;
        for (int i = 1; i < numParts; i++)
        {
            int y = static_cast<int>(Grid::MathTop() + i * scale);
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
            Text("дБ").Draw(5, Grid::MathTop() + 1);
        }
    }
    else if (SCALE_FFT_IS_LINEAR)
    {
        static const char *strs[] = {"1.0", "0.8", "0.6", "0.4", "0.2"};
        float scale = (float)Grid::MathHeight() / 5;
        for (int i = 1; i < 5; i++)
        {
            int y = static_cast<int>(Grid::MathTop() + i * scale);
            HLine().Draw(y, Grid::Left(), Grid::Left() + 256, Color::GRID);
            if (!Menu::IsMinimize())
            {
                Text(strs[i]).Draw(5, y - 4, Color::FILL);
            }
        }
    }
    HLine().Draw(Grid::Left() + 256, Grid::MathTop(), Grid::MathBottom(), Color::FILL);
}



void Display::DrawFullGrid()
{
    if (SettingsDisplay::IsSeparate())
    {
        DrawGrid(Grid::Left(), Grid::TOP, Grid::Width(), Grid::FullHeight() / 2);
        if (ENABLED_FFT)
        {
            DrawGridSpectrum();
        }
        if (!DISABLED_DRAW_MATH)
        {
            DrawGrid(Grid::Left(), Grid::TOP + Grid::FullHeight() / 2, Grid::Width(), Grid::FullHeight() / 2);
        }
        HLine().Draw(Grid::TOP + Grid::FullHeight() / 2, Grid::Left(), Grid::Left() + Grid::Width(), Color::FILL);
    }
    else
    {
        DrawGrid(Grid::Left(), Grid::TOP, Grid::Width(), Grid::FullHeight());
    }
}



int Display::CalculateCountV()
{
    if (MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if (MEAS_NUM_IS_1_5)
        {
            return MEAS_SOURCE_IS_A_B ? 42 : 44;
        }
        if (MEAS_NUM_IS_2_5)
        {
            return MEAS_SOURCE_IS_A_B ? 69 : 39;
        }
        if (MEAS_NUM_IS_3_5)
        {
            return MEAS_SOURCE_IS_A_B ? 54 : 68;
        }
    }

    return 49;
}



int Display::CalculateCountH()
{
    if (MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if (MEAS_NUM_IS_6_1)
        {
            return 73;
        }
        if (MEAS_NUM_IS_6_2)
        {
            return 83;
        }
    }
    return 69;
}



void Display::DrawGridType1(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX, float deltaY, float stepX, float stepY)
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

    MultiVPointLine(17, masX, static_cast<int>(stepY), CalculateCountV()).Draw(static_cast<int>(top + stepY), Color::GRID);

    uint8 mas[13];
    mas[0] = (uint8)(top + 1);
    for (int i = 1; i < 5; i++)
    {
        mas[i] = static_cast<uint8>(top + deltaY * i);
    }
    for (int i = 5; i < 8; i++)
    {
        mas[i] = static_cast<uint8>(centerY - 6 + i);
    }
    for (int i = 8; i < 12; i++)
    {
        mas[i] = static_cast<uint8>(centerY + deltaY * (i - 7));
    }
    mas[12] = (uint8)(bottom - 1);

    MultiHPointLine(13, mas, static_cast<int>(stepX), CalculateCountH()).Draw(static_cast<int>(left + stepX), Color::GRID);
}



void Display::DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY)
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

void Display::DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY, int stepX, int stepY)
{
    HPointLine(static_cast<float>(stepX)).Draw(centerY, left + stepX, right);
    uint8 masY[6] = {(uint8)(top + 1), (uint8)(top + 2), (uint8)(centerY - 1), (uint8)(centerY + 1), (uint8)(bottom - 2), (uint8)(bottom - 1)};
    MultiHPointLine(6, masY, deltaX, (right - top) / deltaX).Draw(left + deltaX, Color::GRID);
    VPointLine(static_cast<float>(stepY)).Draw(centerX, top + stepY, bottom, Color::GRID);
    uint16 masX[6] = {(uint16)(left + 1), (uint16)(left + 2), (uint16)(centerX - 1), (uint16)(centerX + 1), (uint16)(right - 2), (uint16)(right - 1)};
    MultiVPointLine(6, masX, deltaY, (bottom - top) / deltaY).Draw(top + deltaY, Color::GRID);
}

void Display::DrawGrid(int left, int top, int width, int height)
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

    float deltaX = Grid::DeltaX() *(float)width / width;
    float deltaY = Grid::DeltaY() * (float)height / height;
    float stepX = deltaX / 5;
    float stepY = deltaY / 5;
    
    int centerX = left + width / 2;
    int centerY = top + height / 2;

    Color::GRID.SetAsCurrent();
    if (TYPE_GRID_IS_1)
    {
        DrawGridType1(left, top, right, bottom, static_cast<float>(centerX), static_cast<float>(centerY), deltaX, deltaY, stepX, stepY);
    }
    else if (TYPE_GRID_IS_2)
    {
        DrawGridType2(left, top, right, bottom, static_cast<int>(deltaX), static_cast<int>(deltaY), static_cast<int>(stepX), static_cast<int>(stepY));
    }
    else if (TYPE_GRID_IS_3)
    {
        DrawGridType3(left, top, right, bottom, centerX, centerY, static_cast<int>(deltaX), static_cast<int>(deltaY), static_cast<int>(stepX), static_cast<int>(stepY));
    }
}



#define  DELTA 5


void Display::DrawScaleLine(int x, bool forTrigLev)
{
    if(ALT_MARKERS_HIDE)
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
    if((!Menu::IsMinimize() || !Menu::IsShown()) && drawRShiftMarkers)
    {
        DrawScaleLine(2, false);
    }
}


void Display::DrawCursorTrigLevel()
{
    TrigSource::E chan = TRIG_SOURCE;
    if (chan == TrigSource::Ext_)
    {
        return;
    }
    int trigLev = TRIG_LEVEL(chan) + (SET_RSHIFT(chan) - RShiftZero);
    float scale = 1.0F / ((TrigLevMax - TrigLevMin) / 2.0F / Grid::ChannelHeight());
    int y0 = static_cast<int>((Grid::TOP + Grid::ChannelBottom()) / 2 + scale * (TrigLevZero - TrigLevMin));
    int y = static_cast<int>(y0 - scale * (trigLev - TrigLevMin));

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

    Char(simbols[TRIG_SOURCE]).Draw(x + 5, y - 9, Color::BACK);
    Font::Set(TypeFont::S8);

    if (drawRShiftMarkers && !Menu::IsMinimize())
    {
        DrawScaleLine(WIDTH - 11, true);
        int left = Grid::Right() + 9;
        int height = Grid::ChannelHeight() - 2 * DELTA;
        int shiftFullMin = RShiftMin + TrigLevMin;
        int shiftFullMax = RShiftMax + TrigLevMax;
        scale = (float)height / (shiftFullMax - shiftFullMin);
        int shiftFull = TRIG_LEVEL_SOURCE + (TRIG_SOURCE_IS_EXT ? 0 : SET_RSHIFT(chan));
        int yFull = static_cast<int>(Grid::TOP + DELTA + height - scale * (shiftFull - RShiftMin - TrigLevMin) - 4);
        Region(4, 6).Fill(left + 2, yFull + 1, Color::Trig());
        Font::Set(TypeFont::S5);
        Char(simbols[TRIG_SOURCE]).Draw(left + 3, yFull - 5, Color::BACK);
        Font::Set(TypeFont::S8);
    }
}


void Display::DrawCursorRShift(Channel::E chan)
{
    float x = static_cast<float>(Grid::Right() - Grid::Width() - Measure::GetDeltaGridLeft());

    if (chan == Channel::Math)
    {
        int rShift = SET_RSHIFT_MATH;
        float scale = (float)Grid::MathHeight() / 960;
        float y = (Grid::MathTop() + Grid::MathBottom()) / 2.0F - scale * (rShift - RShiftZero);
        Char(Symbol::S8::RSHIFT_NORMAL).Draw(static_cast<int>(x - 9), static_cast<int>(y - 4), Color::FILL);
        Char('m').Draw(static_cast<int>(x - 8), static_cast<int>(y - 5), Color::BACK);
        return;
    }
    if(!sChannel_Enabled(chan))
    {
        return;
    }

    int rShift = SET_RSHIFT(chan);
 
    float scale = Grid::ChannelHeight() / (STEP_RSHIFT * 200.0F);
    float y = Grid::ChannelCenterHeight() - scale * (rShift - RShiftZero);

    if(y > Grid::ChannelBottom())
    {
        Char(Symbol::S8::RSHIFT_LOWER).Draw(static_cast<int>(x - 7), Grid::ChannelBottom() - 11, Color::Channel(chan));
        Point().Draw(static_cast<int>(x - 5), Grid::ChannelBottom() - 2);
        y = static_cast<float>(Grid::ChannelBottom() - 7);
        x++;
    }
    else if(y < Grid::TOP)
    {
        Char(Symbol::S8::RSHIFT_ABOVE).Draw(static_cast<int>(x - 7), Grid::TOP + 2, Color::Channel(chan));
        Point().Draw(static_cast<int>(x - 5), Grid::TOP + 2);
        y = Grid::TOP + 7;
        x++;
    }
    else
    {
        Char(Symbol::S8::RSHIFT_NORMAL).Draw(static_cast<int>(x - 8), static_cast<int>(y - 4), Color::Channel(chan));
        if(((chan == ChA) ? showLevelRShiftA : showLevelRShiftB) && MODE_WORK_IS_DIRECT) //-V2570
        {
            DashedHLine(7, 3).Draw(static_cast<int>(y), Grid::Left(), Grid::Right(), 0);
        }
    }

    Font::Set(TypeFont::S5);

    if((!Menu::IsMinimize() || !Menu::IsShown()) && drawRShiftMarkers)
    {
        float scaleFull = (float)Grid::ChannelHeight() / (RShiftMax - RShiftMin) * (sService_MathEnabled() ? 0.9F : 0.91F);
        float yFull = Grid::ChannelCenterHeight() - scaleFull * (rShift - RShiftZero);

        Region(4, 6).Fill(4, static_cast<int>(yFull - 3), Color::Channel(chan));
        Char(chan == ChA ? '1' : '2').Draw(5, static_cast<int>(yFull - 9), Color::BACK);
    }
    Char(chan == ChA ? '1' : '2').Draw(static_cast<int>(x - 7), static_cast<int>(y - 9), Color::BACK);
    Font::Set(TypeFont::S8);
}


void Display::DrawCursorTShift()
{
    int firstPoint = 0;
    int lastPoint = 0;
    SettingsDisplay::PointsOnDisplay(&firstPoint, &lastPoint);

    if (Storage::set == nullptr)
    {
        return;
    }

    // Рисуем TPos
    int shiftTPos = sTime_TPosInPoints((PeackDetMode::E)Storage::set->peakDet, (int)Storage::set->length1channel, SET_TPOS) - SHIFT_IN_MEMORY;
    float scale = static_cast<float>((lastPoint - firstPoint) / Grid::Width());
    int gridLeft = Grid::Left();
    int x = static_cast<int>(gridLeft + shiftTPos * scale - 3);
    if (GF::IntInRange(x + 3, gridLeft, Grid::Right() + 1))
    {
        Char(Symbol::S8::TPOS_2).Draw2SymbolsInPosition(x, Grid::TOP - 1, Symbol::S8::TPOS_3, Color::BACK, Color::FILL);
    };

    // Рисуем tShift
    int shiftTShift = sTime_TPosInPoints((PeackDetMode::E)Storage::set->peakDet, (int)Storage::set->length1channel, SET_TPOS) - sTime_TShiftInPoints((PeackDetMode::E)Storage::set->peakDet);
    if(GF::IntInRange(shiftTShift, firstPoint, lastPoint))
    {
        x = gridLeft + shiftTShift - firstPoint - 3;
        Char(Symbol::S8::TSHIFT_NORM_1).Draw2SymbolsInPosition(x, Grid::TOP - 1, Symbol::S8::TSHIFT_NORM_2, Color::BACK, Color::FILL);
    }
    else if(shiftTShift < firstPoint)
    {
        Char(Symbol::S8::TSHIFT_LEFT_1).Draw2SymbolsInPosition(gridLeft + 1, Grid::TOP, Symbol::S8::TSHIFT_LEFT_2, Color::BACK, Color::FILL);
        Line().Draw(Grid::Left() + 9, Grid::TOP + 1, Grid::Left() + 9, Grid::TOP + 7, Color::BACK);
    }
    else if(shiftTShift > lastPoint)
    {
        Char(Symbol::S8::TSHIFT_RIGHT_1).Draw2SymbolsInPosition(Grid::Right() - 8, Grid::TOP, Symbol::S8::TSHIFT_RIGHT_2, Color::BACK, Color::FILL);
        Line().Draw(Grid::Right() - 9, Grid::TOP + 1, Grid::Right() - 9, Grid::TOP + 7, Color::BACK);
    }
}


void Display::DrawHorizontalCursor(int y, int xTearing)
{
    y += Grid::TOP;
    if(xTearing == -1)
    {
        DashedHLine(1, 1).Draw(y, Grid::Left() + 2, Grid::Right() - 1, 0);
    }
    else
    {
        DashedHLine(1, 1).Draw(y, Grid::Left() + 2, xTearing - 2, 0);
        DashedHLine(1, 1).Draw(y, xTearing + 2, Grid::Right() - 1, 0);
    }
    Primitives::Rectangle(2, 2).Draw(Grid::Left() - 1, y - 1);
    Primitives::Rectangle(2, 2).Draw(Grid::Right() - 1, y - 1);
}


void Display::DrawVerticalCursor(int x, int yTearing)
{
    x += Grid::Left();
    if(yTearing == -1)
    {
        DashedVLine(1, 1).Draw(x, Grid::TOP + 2, Grid::ChannelBottom() - 1, 0);
    }
    else
    {
        DashedVLine(1, 1).Draw(x, Grid::TOP + 2, yTearing - 2, 0);
        DashedVLine(1, 1).Draw(x, yTearing + 2, Grid::ChannelBottom() - 1, 0);
    }
    Primitives::Rectangle(2, 2).Draw(x - 1, Grid::TOP - 1);
    Primitives::Rectangle(2, 2).Draw(x - 1, Grid::ChannelBottom() - 1);
}


void Display::DrawCursors()
{
    Channel::E source = CURS_SOURCE;
    Color::Cursors(source).SetAsCurrent();
    if (sCursors_NecessaryDrawCursors())
    {
        bool bothCursors = !CURS_CNTRL_T_IS_DISABLE(source) && !CURS_CNTRL_U_IS_DISABLE(source);  // Признак того, что включены и вертикальные и 
                                                                            // горизонтальные курсоры - надо нарисовать квадраты в местах пересечения
        int x0 = -1;
        int x1 = -1;
        int y0 = -1;
        int y1 = -1;

        if (bothCursors)
        {
            x0 = static_cast<int>(Grid::Left() + CURS_POS_T0(source));
            x1 = static_cast<int>(Grid::Left() + CURS_POS_T1(source));
            y0 = static_cast<int>(Grid::TOP + sCursors_GetCursPosU(source, 0));
            y1 = static_cast<int>(Grid::TOP + sCursors_GetCursPosU(source, 1));

            Primitives::Rectangle(4, 4).Draw(x0 - 2, y0 - 2);
            Primitives::Rectangle(4, 4).Draw(x1 - 2, y1 - 2);
        }

        CursCntrl::E cntrl = CURS_CNTRL_T(source);
        if (cntrl != CursCntrl::Disable)
        {
            DrawVerticalCursor(static_cast<int>(CURS_POS_T0(source)), y0);
            DrawVerticalCursor(static_cast<int>(CURS_POS_T1(source)), y1);
        }
        cntrl = CURsU_CNTRL;
        if (cntrl != CursCntrl::Disable)
        {
            DrawHorizontalCursor(static_cast<int>(sCursors_GetCursPosU(source, 0)), x0);
            DrawHorizontalCursor(static_cast<int>(sCursors_GetCursPosU(source, 1)), x1);
        }
    }
}


void Display::DrawMeasures()
{
    if(!SHOW_MEASURES)
    {
        topMeasures = Grid::Bottom();
        return;
    }

    Processing::CalculateMeasures();

    if(MEAS_FIELD_IS_HAND)
    {
        int x0 = MEAS_POS_CUR_T0 - SHIFT_IN_MEMORY + Grid::Left();
        int y0 = MEAS_POS_CUR_U0 + Grid::TOP;
        int x1 = MEAS_POS_CUR_T1 - SHIFT_IN_MEMORY + Grid::Left();
        int y1 = MEAS_POS_CUR_U1 + Grid::TOP;
        GF::SortInt(&x0, &x1);
        GF::SortInt(&y0, &y1);
        Primitives::Rectangle(x1 - x0, y1 - y0).Draw(x0, y0, Color::FILL);
    }

    int x0 = Grid::Left() - Measure::GetDeltaGridLeft();
    int dX = Measure::GetDX();
    int dY = Measure::GetDY();
    int y0 = Measure::GetTopTable();

    int numRows = Measure::NumRows();
    int numCols = Measure::NumCols();

    for(int str = 0; str < numRows; str++)
    {
        for(int elem = 0; elem < numCols; elem++)
        {
            int x = x0 + dX * elem;
            int y = y0 + str * dY;
            bool active = Measure::IsActive(str, elem) && Menu::GetNameOpenedPage() == NamePage::SB_MeasTuneMeas;
            Measure::E meas = Measure::Type(str, elem);
            if(meas != Measure::None)
            {
                Region(dX, dY).Fill(x, y, Color::BACK);
                Primitives::Rectangle(dX, dY).Draw(x, y, Color::FILL);
                topMeasures = Math::MinFrom2(topMeasures, y);
            }
            if(active)
            {
                Region(dX - 4, dY - 4).Fill(x + 2, y + 2, Color::FILL);
            }
            if(meas != Measure::None)
            {
                char buffer[20];
                Text(Measure::Name(str, elem)).Draw(x + 4, y + 2, active ? Color::BACK : Color::FILL);
                if(meas == MEAS_MARKED)
                {
                    Region(dX - 2, 9).Fill(x + 1, y + 1, active ? Color::BACK : Color::FILL);
                    Text(Measure::Name(str, elem)).Draw(x + 4, y + 2, active ? Color::FILL : Color::BACK);
                }
                if(MEAS_SOURCE_IS_A)
                {
                    Text(Processing::GetStringMeasure(meas, ChA, buffer)).Draw(x + 2, y + 11, Color::Channel(ChA));
                }
                else if(MEAS_SOURCE_IS_B)
                {
                    Text(Processing::GetStringMeasure(meas, ChB, buffer)).Draw(x + 2, y + 11, Color::Channel(ChB));
                }
                else
                {
                    Text(Processing::GetStringMeasure(meas, ChA, buffer)).Draw(x + 2, y + 11, Color::Channel(ChA));
                    Text(Processing::GetStringMeasure(meas, ChB, buffer)).Draw(x + 2, y + 20, Color::Channel(ChB));
                }
            }
        }
    }

    if(Menu::GetNameOpenedPage() == NamePage::SB_MeasTuneMeas)
    {
        Measure::DrawPageChoice();
    }
}



void Display::WriteTextVoltage(Channel::E chan, int x, int y)
{
    static const char *couple[] =
    {
        "\x92",
        "\x91",
        "\x90"
    };

    bool inverse = SET_INVERSE(chan);
    ModeCouple::E modeCouple = SET_COUPLE(chan);
    Divider::E multiplier = SET_DIVIDER(chan);
    Range::E range = SET_RANGE(chan);
    uint rShift = (uint)SET_RSHIFT(chan);
    bool enable = SET_ENABLED(chan);

    if (!MODE_WORK_IS_DIRECT)
    {
        DataSettings *ds = MODE_WORK_IS_DIRECT ? Storage::set : Storage::dsInt;
        if (ds != 0)
        {
            inverse = (chan == ChA) ? ds->inverseCh0 : ds->inverseCh1;
            modeCouple = (chan == ChA) ? ds->modeCouple0 : ds->modeCouple1;
            multiplier = (chan == ChA) ? ds->multiplier0 : ds->multiplier1;
            range = ds->range[chan];
            rShift = (chan == ChA) ? ds->rShiftCh0 : ds->rShiftCh1;
            enable = (chan == ChA) ? ds->enableCh0 : ds->enableCh1;
        }
    }

    if(enable)
    {
        Color color = Color::Channel(chan);
        Color colorDraw = inverse ? Color::WHITE : color;

        if(inverse)
        {
            const int widthField = 91;
            const int heightField = 8;
            Region(widthField, heightField).Fill(x, y, color);
        }

        char buffer[100] = {0};

        std::sprintf(buffer, "%s\xa5%s\xa5%s", (chan == ChA) ? (LANG_RU ? "1к" : "1c") : (LANG_RU ? "2к" : "2c"), couple[modeCouple],
            sChannel_Range2String(range, multiplier));

        Text(buffer).Draw(x + 1, y, colorDraw);

        char bufferTemp[20];
        std::sprintf(buffer, "\xa5%s", sChannel_RShift2String((int16)rShift, range, multiplier, bufferTemp));
        Text(buffer).Draw(x + 46, y);
    }
}



void Display::WriteStringAndNumber(pchar text, int x, int y, int number)
{
    char buffer[100];
    Text(text).Draw(x, y, Color::FILL);
    if(number == 0)
    {
        std::sprintf(buffer, "-");
    }
    else
    {
        std::sprintf(buffer, "%d", number);
    }
    Text(buffer).DrawRelativelyRight(x + 41, y);
}



void Display::DrawLowPart()
{
    int y0 = HEIGHT - 19;
    int y1 = HEIGHT - 10;
    int x = -1;

    HLine().Draw(Grid::ChannelBottom(), 1, Grid::Left() - Measure::GetDeltaGridLeft() - 2, Color::FILL);
    HLine().Draw(Grid::FullBottom(), 1, Grid::Left() - Measure::GetDeltaGridLeft() - 2);

    WriteTextVoltage(ChA, x + 2, y0);

    WriteTextVoltage(ChB, x + 2, y1);

    VLine().Draw(x + 95, Grid::Bottom() + 2, HEIGHT - 2, Color::FILL);

    x += 98;
    char buffer[100] = {0};

    TBase::E tBase = SET_TBASE;
    int16 tShift = TSHIFT;

    if (!MODE_WORK_IS_DIRECT)
    {
        DataSettings *ds = MODE_WORK_IS_LATEST ? Storage::dsLast : Storage::dsInt;
        if (ds != 0)
        {
            tBase = ds->tBase;
            tShift = ds->tShift;
        }
    }

    std::sprintf(buffer, "р\xa5%s", TBase::ToString(tBase));
    Text(buffer).Draw(x, y0);

    buffer[0] = 0;
    char bufForVal[20];
    std::sprintf(buffer, "\xa5%s", TShift::ToString(tShift, bufForVal));
    Text(buffer).Draw(x + 35, y0);

    buffer[0] = 0;
    if (MODE_WORK_IS_DIRECT)
    {
        const char *source[3] = { "1", "2", "\x82" };
        std::sprintf(buffer, "с\xa5\x10%s", source[TRIG_SOURCE]);
    }

    Text(buffer).Draw(x, y1, Color::Trig());

    buffer[0] = 0;
    static const char *couple[] =
    {
        "\x92",
        "\x91",
        "\x92",
        "\x92"
    };
    static const char *polar[] =
    {
        "\xa7",
        "\xa6"
    };
    static const char *filtr[] =
    {
        "\xb5\xb6",
        "\xb5\xb6",
        "\xb3\xb4",
        "\xb1\xb2"
    };
    if (MODE_WORK_IS_DIRECT)
    {
        std::sprintf(buffer, "\xa5\x10%s\x10\xa5\x10%s\x10\xa5\x10", couple[TRIG_INPUT], polar[TRIG_POLARITY]);
        Text(buffer).Draw(x + 18, y1);
        Char(filtr[TRIG_INPUT][0]).Draw(x + 45, y1);
        Char(filtr[TRIG_INPUT][1]).Draw(x + 53, y1);
    }

    buffer[0] = '\0';
    const char mode[] =
    {
        '\xb7',
        '\xa0',
        '\xb0'
    };
    if (MODE_WORK_IS_DIRECT)
    {
        std::sprintf(buffer, "\xa5\x10%c", mode[START_MODE]);
        Text(buffer).Draw(x + 63, y1);
    }
    
    VLine().Draw(x + 79, Grid::Bottom() + 2, HEIGHT - 2, Color::FILL);

    HLine().Draw(Grid::Bottom(), Grid::Right() + 2, WIDTH - 2);
    HLine().Draw(Grid::ChannelBottom(), Grid::Right() + 2, WIDTH - 2);

    x += 82;
    y0 = y0 - 3;
    y1 = y1 - 6;
    Font::Set(TypeFont::S5);
    
    if (MODE_WORK_IS_DIRECT)
    {
        int y2 = y1 + 6;
        WriteStringAndNumber("накопл", x, y0, NUM_ACCUM);
        WriteStringAndNumber("усредн", x, y1, NUM_AVE);
        WriteStringAndNumber("мн\x93мкс", x, y2, NUM_MIN_MAX);
    }

    x += 42;
    VLine().Draw(x, Grid::Bottom() + 2, HEIGHT - 2);

    Font::Set(TypeFont::S8);

    if (MODE_WORK_IS_DIRECT)
    {
        char mesFreq[20] = "\x7c=";
        float freq = FPGA::FreqMeter::GetFreq();
        if (freq == -1.0F) //-V550 //-V2550
        {
            std::strcat(mesFreq, "******"); //-V2513
        }
        else
        {
            std::strcat(mesFreq, GF::Freq2String(freq).c_str()); //-V2513
        }
        Text(mesFreq).Draw(x + 3, Grid::Bottom() + 2);
    }

    DrawTime(x + 3, Grid::Bottom() + 11);

    HLine().Draw(x + 55, Grid::Bottom() + 2, HEIGHT - 2);

    Font::Set(TypeFont::UGO2);

    // Флешка
    if (FDrive::isConnected)
    {
        Char(Symbol::S8::FLASH_DRIVE).Draw4SymbolsInRect(x + 57, Grid::Bottom() + 2);
    }

    // Ethernet
    if ((LAN::clientIsConnected || LAN::cableIsConnected) && TIME_MS > 2000)
    {
        Char(Symbol::S8::ETHERNET).Draw4SymbolsInRect(x + 87, Grid::Bottom() + 2, LAN::clientIsConnected ? Color::FILL : Color::FLASH_01);
    }

    if (VCP::connectToHost || VCP::cableIsConnected)
    {
        Char(Symbol::UGO2::USB).Draw4SymbolsInRect(x + 72, Grid::Bottom() + 2, VCP::connectToHost ? Color::FILL : Color::FLASH_01);
    }
    
    Color::FILL.SetAsCurrent();
    // Пиковый детектор
    if(!PEAKDET_IS_DISABLE)
    {
       Char('\x12').Draw(x + 38, Grid::Bottom() + 11);
       Char('\x13').Draw(x + 46, Grid::Bottom() + 11);
    }

    if (MODE_WORK_IS_DIRECT)
    {
        Font::Set(TypeFont::S5);
        WriteStringAndNumber("СГЛАЖ.:", x + 57, Grid::Bottom() + 10, Smoothing::NumPoints());
        Font::Set(TypeFont::S8);
    }
}


void Display::DrawTimeForFrame(uint timeTicks)
{
    if(!SHOW_STATS)
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

    Primitives::Rectangle(84, 10).Draw(Grid::Left(), Grid::FullBottom() - 10, Color::FILL);
    Region(82, 8).Fill(Grid::Left() + 1, Grid::FullBottom() - 9, Color::BACK);
    Text(buffer).Draw(Grid::Left() + 2, Grid::FullBottom() - 9, Color::FILL);

    char message[20] = {0};
    std::sprintf(message, "%d", Storage::NumElementsWithSameSettings());
    std::strcat(message, "/"); //-V2513
    char numAvail[10] = {0};
    std::sprintf(numAvail, "%d", Storage::NumberAvailableEntries());
    std::strcat(message, numAvail); //-V2513
    Text(message).Draw(Grid::Left() + 50, Grid::FullBottom() - 9);
}


void Display::DisableShowLevelRShiftA()
{
    showLevelRShiftA = false;
    Timer::Disable(TypeTimer::ShowLevelRShift0);
}


void Display::DisableShowLevelRShiftB()
{
    showLevelRShiftB = false;
    Timer::Disable(TypeTimer::ShowLevelRShift1);
}


void Display::DisableShowLevelTrigLev()
{
    showLevelTrigLev = false;
    Timer::Disable(TypeTimer::ShowLevelTrigLev);
}


void Display::EnableTrigLabel(bool enable)
{
    trigEnable = enable;
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
    LIMITATION(SHIFT_IN_MEMORY, static_cast<int16>(SHIFT_IN_MEMORY + delta), 0, static_cast<int16>(sMemory_GetNumPoints(false) - 282));
}


void Display::ChangedRShiftMarkers()
{
    drawRShiftMarkers = ALT_MARKERS_HIDE;
    Timer::Enable(TypeTimer::RShiftMarkersAutoHide, 5000, FuncOnTimerRShiftMarkersAutoHide);
}

void Display::FuncOnTimerRShiftMarkersAutoHide()
{
    OnRShiftMarkersAutoHide();
}


void Display::OnRShiftMarkersAutoHide()
{
    drawRShiftMarkers = false;
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
    return static_cast<int>(SIZE_BUFFER_FOR_STRINGS - (strings[firstEmptyString - 1] - bufferForStrings) - std::strlen(strings[firstEmptyString - 1]) - 1); //-V2513
}


void Display::DeleteFirstString()
{
    if(FirstEmptyString() < 2)
    {
        return;
    }
    int delta = static_cast<int>(std::strlen(strings[0])) + 1; //-V2513
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



void Display::AddString(const char *string)
{
    if(CONSOLE_IN_PAUSE)
    {
        return;
    }
    static int num = 0;
    char buffer[100];
    std::sprintf(buffer, "%d\x11", num++);
    std::strcat(buffer, string); //-V2513
    int size = static_cast<int>(std::strlen(buffer)) + 1; //-V2513
    while(CalculateFreeSize() < size)
    {
        DeleteFirstString();
    }
    if(!strings[0])
    {
        strings[0] = bufferForStrings;
        std::strcpy(strings[0], buffer); //-V2513
    }
    else
    {
        char *addressLastString = strings[FirstEmptyString() - 1];
        char *address = addressLastString + std::strlen(addressLastString) + 1; //-V2513
        strings[FirstEmptyString()] = address;
        std::strcpy(address, buffer); //-V2513
    }
}



void Display::AddStringToIndicating(const char *string)
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
    if(!CONSOLE_IN_PAUSE)
    {
    }
    else if(lastStringForPause > NUM_STRINGS - 1)
    {
        lastStringForPause--;
    }
}



void Display::OneStringDown()
{
    if(!CONSOLE_IN_PAUSE)
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
    Font::Set(sDebug_GetSizeFontForConsole() == 5 ? TypeFont::S5 : TypeFont::S8);
    int height = Font::GetSize();

    int last_string = FirstEmptyString() - 1;
    int numStr = NUM_STRINGS;
    if(height == 8 && numStr > 22)
    {
        numStr = 22;
    }
    int delta = 0;
    if(SHOW_STRING_NAVIGATION)
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


void Display::ShowWarn(const char *message)
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
        if (time - timeWarnings[i] > static_cast<uint>(TIME_MESSAGES * 1000))
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
    Primitives::Rectangle(width + 4, height + 4).Draw(Grid::Left(), y, Color::FILL);
    Primitives::Rectangle(width + 2, height + 2).Draw(Grid::Left() + 1, y + 1, Color::BACK);
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
