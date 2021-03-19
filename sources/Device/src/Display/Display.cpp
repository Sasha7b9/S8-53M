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
#include "FPGA/FPGA.h"
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
static const char               *warnings[NUM_WARNINGS] = {0};      // ����� ��������������� ���������.
static uint                     timeWarnings[NUM_WARNINGS] = {0};   // ����� �����, ����� ��������������� ���������
                                                                    // ��������� �� �����.

//static pFuncVV funcOnHand       = 0;
//static pFuncVV funcAdditionDraw = 0;
//static pFuncVV funcAfterDraw    = 0;

static bool showLevelRShiftA = false;    // ����� �� �������� �������������� ����� ������ �������� ������� ������
static bool showLevelRShiftB = false;
static bool showLevelTrigLev = false;    // ����� �� �������� �������������� ����� ������ �������� ������ �������������
static bool trigEnable = false;
static bool drawRShiftMarkers = false;
static bool needFinishDraw = true;      // ���� 1, �� ������� ��������� � �����������
static uint numDrawingSignals = 0;      // ����� ������������ �������� ��� ������ ����������



void Display::DrawStringNavigation() 
{
    String string = Menu::StringNavigation();

    uint length = string.Size();

    if(length != 0) 
    {
        int height = 10;
        Primitives::Rectangle((int)length + 2, height).Draw(Grid::Left(), Grid::TOP, Color::FILL);
        Region((int)length, height - 2).Fill(Grid::Left() + 1, Grid::TOP + 1, Color::BACK);
        string.Draw(Grid::Left() + 2, Grid::TOP + 1, Color::FILL);
    }
}


void Display::RotateRShift(const Channel &ch)
{
    LAST_AFFECTED_CHANNEL = ch.value;

    if(TIME_SHOW_LEVELS)
    {
        if (ch.IsA()) { showLevelRShiftA = true; }
        else                    { showLevelRShiftB = true; }
        Timer::Enable(ch.IsA() ? TypeTimer::ShowLevelRShift0 : TypeTimer::ShowLevelRShift1,
            TIME_SHOW_LEVELS  * 1000, ch.IsA() ? FuncOnTimerDisableShowLevelRShiftA :
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
    int startX = 43;

    if(MODE_WORK_IS_DIRECT)
    {
        startX += 29;
    }

    if(Cursors::NecessaryDraw())
    {
        int x = startX;
        int y1 = 0;
        int y2 = 9;

        HLine().Draw(x, 1, Grid::TOP - 2, Color::FILL);
        x += 3;
        Channel source = CURS_SOURCE;

        Color colorText = source.GetColor();

        if(!CURS_CNTRL_U_IS_DISABLE(source))
        {
            Text("1:").Draw(x, y1, colorText);
            Text("2:").Draw(x, y2);
            x += 7;
            Cursors::GetVoltage(source, 0).Draw(x, y1);
            Cursors::GetVoltage(source, 1).Draw(x, y2);
            x = startX + 49;
            float pos0 = MathFPGA::VoltageCursor(Cursors::GetPosU(source, 0), SET_RANGE(source), SET_RSHIFT(source));
            float pos1 = MathFPGA::VoltageCursor(Cursors::GetPosU(source, 1), SET_RANGE(source), SET_RSHIFT(source));
            float delta = std::fabsf(pos1 - pos0);
            Text(":dU=").Draw(x, y1);
            GF::Voltage2String(delta, false).Draw(x + 17, y1);
            Text(":").Draw(x, y2);
            Cursors::GetPercentsU(source).Draw(x + 10, y2);
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
            Cursors::GetTime(source, 0).Draw(x, y1);
            Cursors::GetTime(source, 1).Draw(x, y2);
            x = startX + 153;
            float pos0 = MathFPGA::TimeCursor(CURS_POS_T0(source), SET_TBASE);
            float pos1 = MathFPGA::TimeCursor(CURS_POS_T1(source), SET_TBASE);
            float delta = std::fabsf(pos1 - pos0);
            Text(":dT=").Draw(x, y1);
            GF::Time2String(delta, false).Draw(x + 17, y1);
            Text(":").Draw(x, y2);
            Cursors::GetPercentsT(source).Draw(x + 8, y2);

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
    // �������������
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
            Text(LANG_RU ? "��" : "Tr").Draw(x + 3, 3 + y, Color::BACK);
        }
    }

    // ����� ������
    static pchar strings_[][2] =
    {
        {"���",     "MEAS"},
        {"����",    "LAST"},
        {"����",    "INT"}
    };

    if(!MODE_WORK_IS_DIRECT)
    {
        x += 18;
        HLine().Draw(x, 1, Grid::TOP - 2, Color::FILL);
        x += 2;
        Text(LANG_RU ? "�����" : "mode").Draw(LANG_RU ? x : x + 3, -1);
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
        DrawCursorRShift(Channel::A);
        DrawCursorRShift(Channel::B);
    }
    else
    {
        DrawCursorRShift(Channel::B);
        DrawCursorRShift(Channel::A);
    }
}



bool Display::NeedForClearScreen()
{
    int numAccum = NUM_ACCUM;

    if (FPGA_IN_RANDOMIZE_MODE || numAccum == 1 || MODE_ACCUM_IS_NORESET || SET_SELFRECORDER)
    {
        return true;
    }

    if (needFinishDraw)
    {
        needFinishDraw = false;
        return true;
    }

    if (MODE_ACCUM_IS_RESET && (numDrawingSignals >= static_cast<uint>(numAccum)))
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
        Grid::Draw();
    }

    DataPainter::DrawData();

    if (needClear)
    {
//        DrawMath();
//        DrawSpectrum();
        Cursors::Draw();
//        DrawHiPart();
        DrawLowPart();
//        DrawCursorsWindow();
//        DrawCursorTrigLevel();
        DrawCursorsRShift();
        Measure::DrawAll();
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
        float trigLev = RSHIFT_2_ABS(TRIG_LEVEL_SOURCE, SET_RANGE(TRIG_SOURCE));     // WARN ����� ��� �������
                                                                    // ������������� ����������� �������������� �������.
        TrigSource::E trigSource = TRIG_SOURCE;
        if (TRIG_INPUT_IS_AC && trigSource <= TrigSource::B_)
        {
            int16 rShift = SET_RSHIFT(trigSource);
            float rShiftAbs = RSHIFT_2_ABS(rShift, SET_RANGE(trigSource));
            trigLev += rShiftAbs;
        }
        char buffer[20];
        std::strcpy(buffer, LANG_RU ? "�� ����� = " : "Trig lvl = ");
        std::strcat(buffer, GF::Voltage2String(trigLev, true).c_str());
        int width = 96;
        int x = (Grid::Width() - width) / 2 + Grid::Left();
        int y = Grid::BottomMessages() - 20;
        Primitives::Rectangle(width, 10).Draw(x, y, Color::FILL);
        Region(width - 2, 8).Fill(x + 1, y + 1, Color::BACK);
        Text(buffer).Draw(x + 2, y + 1, Color::FILL);
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
    TrigSource::E ch = TRIG_SOURCE;
    if (ch == TrigSource::Ext_)
    {
        return;
    }
    int trigLev = TRIG_LEVEL(ch) + (SET_RSHIFT(ch) - RShiftZero);
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

    const char simbols[3] = {'1', '2', '�'};

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
        int shiftFull = TRIG_LEVEL_SOURCE + (TRIG_SOURCE_IS_EXT ? 0 : SET_RSHIFT(ch));
        int yFull = static_cast<int>(Grid::TOP + DELTA + height - scale * (shiftFull - RShiftMin - TrigLevMin) - 4);
        Region(4, 6).Fill(left + 2, yFull + 1, Color::Trig());
        Font::Set(TypeFont::S5);
        Char(simbols[TRIG_SOURCE]).Draw(left + 3, yFull - 5, Color::BACK);
        Font::Set(TypeFont::S8);
    }
}


void Display::DrawCursorRShift(const Channel &ch)
{
    float x = static_cast<float>(Grid::Right() - Grid::Width() - Measure::GetDeltaGridLeft());

    if (ch.IsMath())
    {
        int rShift = SET_RSHIFT_MATH;
        float scale = (float)Grid::MathHeight() / 960;
        float y = (Grid::MathTop() + Grid::MathBottom()) / 2.0F - scale * (rShift - RShiftZero);
        Char(Symbol::S8::RSHIFT_NORMAL).Draw(static_cast<int>(x - 9), static_cast<int>(y - 4), Color::FILL);
        Char('m').Draw(static_cast<int>(x - 8), static_cast<int>(y - 5), Color::BACK);
        return;
    }
    if(!ch.IsEnabled())
    {
        return;
    }

    int rShift = SET_RSHIFT(ch);
 
    float scale = Grid::ChannelHeight() / (STEP_RSHIFT * 200.0F);
    float y = Grid::ChannelCenterHeight() - scale * (rShift - RShiftZero);

    if(y > Grid::ChannelBottom())
    {
        Char(Symbol::S8::RSHIFT_LOWER).Draw(static_cast<int>(x - 7), Grid::ChannelBottom() - 11, ch.GetColor());
        Point().Draw(static_cast<int>(x - 5), Grid::ChannelBottom() - 2);
        y = static_cast<float>(Grid::ChannelBottom() - 7);
        x++;
    }
    else if(y < Grid::TOP)
    {
        Char(Symbol::S8::RSHIFT_ABOVE).Draw(static_cast<int>(x - 7), Grid::TOP + 2, ch.GetColor());
        Point().Draw(static_cast<int>(x - 5), Grid::TOP + 2);
        y = Grid::TOP + 7;
        x++;
    }
    else
    {
        Char(Symbol::S8::RSHIFT_NORMAL).Draw(static_cast<int>(x - 8), static_cast<int>(y - 4), ch.GetColor());
        if(((ch == ChA) ? showLevelRShiftA : showLevelRShiftB) && MODE_WORK_IS_DIRECT) //-V2570
        {
            DashedHLine(7, 3).Draw(static_cast<int>(y), Grid::Left(), Grid::Right(), 0);
        }
    }

    Font::Set(TypeFont::S5);

    if((!Menu::IsMinimize() || !Menu::IsShown()) && drawRShiftMarkers)
    {
        float scaleFull = (float)Grid::ChannelHeight() / (RShiftMax - RShiftMin) *
            (sService_MathEnabled() ? 0.9F : 0.91F);

        float yFull = Grid::ChannelCenterHeight() - scaleFull * (rShift - RShiftZero);

        Region(4, 6).Fill(4, static_cast<int>(yFull - 3), ch.GetColor());
        Char(ch == ChA ? '1' : '2').Draw(5, static_cast<int>(yFull - 9), Color::BACK);
    }
    Char(ch == ChA ? '1' : '2').Draw(static_cast<int>(x - 7), static_cast<int>(y - 9), Color::BACK);
    Font::Set(TypeFont::S8);
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
//    // ������ TPos
//    int shiftTPos = TPos::InPoints(
//        (PeackDetMode::E)Storage::set->peakDet, (int)Storage::set->length1channel, SET_TPOS) - SHIFT_IN_MEMORY;
//
//    float scale = static_cast<float>((lastPoint - firstPoint) / Grid::Width());
//    int gridLeft = Grid::Left();
//    int x = static_cast<int>(gridLeft + shiftTPos * scale - 3);
//    if (GF::IntInRange(x + 3, gridLeft, Grid::Right() + 1))
//    {
//        Char(Symbol::S8::TPOS_2).Draw2SymbolsInPosition(x, Grid::TOP - 1, Symbol::S8::TPOS_3, Color::BACK, Color::FILL);
//    };
//
//    // ������ tShift
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


void Display::WriteTextVoltage(const Channel &ch, int x, int y)
{
    static pchar couple[] =
    {
        "\x92",
        "\x91",
        "\x90"
    };

    bool inverse = SET_INVERSE(ch);
    ModeCouple::E modeCouple = SET_COUPLE(ch);
    Divider::E multiplier = SET_DIVIDER(ch);
    Range::E range = SET_RANGE(ch);
    uint rShift = (uint)SET_RSHIFT(ch);
    bool enable = SET_ENABLED(ch);

    if (!MODE_WORK_IS_DIRECT)
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

    if(enable)
    {
        Color color = ch.GetColor();
        Color colorDraw = inverse ? Color::WHITE : color;

        if(inverse)
        {
            const int widthField = 91;
            const int heightField = 8;
            Region(widthField, heightField).Fill(x, y, color);
        }

        String("%s\xa5%s\xa5%s", (ch == ChA) ? (LANG_RU ? "1�" : "1c") : (LANG_RU ? "2�" : "2c"), couple[modeCouple],
            Range::ToString(range, multiplier)).Draw(x + 1, y, colorDraw);

        String("\xa5%s", RShift::ToString((int16)rShift, range, multiplier).c_str()).Draw(x + 46, y);
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

    TBase::E tBase = SET_TBASE;
    int16 tShift = TSHIFT;

    if (!MODE_WORK_IS_DIRECT)
    {
//        DataSettings *ds = MODE_WORK_IS_LATEST ? Storage::dsLast : Storage::dsInt;
//
//        if (ds != 0)
//        {
//            tBase = ds->tBase;
//            tShift = ds->tShift;
//        }
    }

    Text(String("�\xa5%s", TBase::ToString(tBase))).Draw(x, y0);

    Text(String("\xa5%s", TShift::ToString(tShift).c_str())).Draw(x + 35, y0);

    if (MODE_WORK_IS_DIRECT)
    {
        pchar source[3] = { "1", "2", "\x82" };

        Text(String("�\xa5\x10%s", source[TRIG_SOURCE])).Draw(x, y1, Color::Trig());
    }

    if (MODE_WORK_IS_DIRECT)
    {
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

        Text(String("\xa5\x10%s\x10\xa5\x10%s\x10\xa5\x10", couple[TRIG_INPUT], polar[TRIG_POLARITY])).Draw(x + 18, y1);
        Char(filtr[TRIG_INPUT][0]).Draw(x + 45, y1);
        Char(filtr[TRIG_INPUT][1]).Draw(x + 53, y1);
    }

    if (MODE_WORK_IS_DIRECT)
    {
        const char mode[] =
        {
            '\xb7',
            '\xa0',
            '\xb0'
        };

        Text(String("\xa5\x10%c", mode[START_MODE])).Draw(x + 63, y1);
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
        WriteStringAndNumber("������", x, y0, NUM_ACCUM);
        WriteStringAndNumber("������", x, y1, NUM_AVE);
        WriteStringAndNumber("��\x93���", x, y2, NUM_MIN_MAX);
    }

    x += 42;
    VLine().Draw(x, Grid::Bottom() + 2, HEIGHT - 2);

    Font::Set(TypeFont::S8);

    if (MODE_WORK_IS_DIRECT)
    {
        char mesFreq[20] = "\x7c=";
        float freq = FPGA::FreqMeter::GetFreq();
        if (freq == -1.0F)
        {
            std::strcat(mesFreq, "******");
        }
        else
        {
            std::strcat(mesFreq, GF::Freq2String(freq).c_str());
        }
        Text(mesFreq).Draw(x + 3, Grid::Bottom() + 2);
    }

    DrawTime(x + 3, Grid::Bottom() + 11);

    HLine().Draw(x + 55, Grid::Bottom() + 2, HEIGHT - 2);

    Font::Set(TypeFont::UGO2);

    // ������
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
    // ������� ��������
    if(!PEAKDET_IS_DISABLE)
    {
       Char('\x12').Draw(x + 38, Grid::Bottom() + 11);
       Char('\x13').Draw(x + 46, Grid::Bottom() + 11);
    }

    if (MODE_WORK_IS_DIRECT)
    {
        Font::Set(TypeFont::S5);
        WriteStringAndNumber("�����.:", x + 57, Grid::Bottom() + 10, (int)Smoothing::NumPoints());
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

//    std::sprintf(message, "%d", Storage::NumElementsWithSameSettings());

    String message("1");

//    char numAvail[10] = {0};
//    std::sprintf(numAvail, "%d", Storage::NumberAvailableEntries());
//    std::strcat(message, numAvail);
    message.Draw(Grid::Left() + 50, Grid::FullBottom() - 9);
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
    LIMITATION(SHIFT_IN_MEMORY, static_cast<int16>(SHIFT_IN_MEMORY + delta), 0,
        static_cast<int16>(sMemory_GetNumPoints(false) - 282));
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

    return static_cast<int>(SIZE_BUFFER_FOR_STRINGS - (strings[firstEmptyString - 1] - bufferForStrings) -
        std::strlen(strings[firstEmptyString - 1]) - 1);
}


void Display::DeleteFirstString()
{
    if(FirstEmptyString() < 2)
    {
        return;
    }
    int delta = static_cast<int>(std::strlen(strings[0])) + 1;
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
    if(CONSOLE_IN_PAUSE)
    {
        return;
    }
    static int num = 0;
    char buffer[100];
    std::sprintf(buffer, "%d\x11", num++);
    std::strcat(buffer, string);
    int size = static_cast<int>(std::strlen(buffer)) + 1;
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
