#include "defines.h"
#include "common/Log_c.h"
#include "common/Display/Painter_c.h"
#include "LAN/LAN.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include "VCP/VCP.h"
#include "VCP/SCPI/SCPI.h"
#include "VCP/SCPI/Utils/MapSCPI.h"


ENTER_PARSE_FUNC(SCPI::ProcessDISPLAY)
    {"AUTOSEND",    SCPI::DISPLAY::AUTOSEND},
    {"MAPPING",     SCPI::DISPLAY::MAPPING},    // �����������
    {"MAP",         SCPI::DISPLAY::MAPPING},
    {"ACCUMULATION",SCPI::DISPLAY::ACCUM},      // ����������
    {"ACCUM",       SCPI::DISPLAY::ACCUM},     
    {"AVERAGE",     SCPI::DISPLAY::AVERAGE},    // ����������
    {"AVE",         SCPI::DISPLAY::AVERAGE},
    {"MINMAX",      SCPI::DISPLAY::MINMAX},     // ��� ����
    {"FILTR",       SCPI::DISPLAY::FILTR},      // �����������
    {"FPS",         SCPI::DISPLAY::FPS},        // ������� ������
    {"WINDOW",      SCPI::DISPLAY::WINDOW},     // ���� ������
    {"GRID",        SCPI::DISPLAY::GRID},       // �����
LEAVE_PARSE_FUNC



ENTER_PARSE_FUNC(SCPI::DISPLAY::GRID)
    {"TYPE",        SCPI::DISPLAY::GRID_TYPE},
    {"BRIGHTNESS",  SCPI::DISPLAY::GRID_BRIGHTNESS},
LEAVE_PARSE_FUNC



ENTER_PARSE_FUNC(SCPI::DISPLAY::ACCUM)
    {"NUMBER",  SCPI::DISPLAY::ACCUM_NUMBER},
    {"NUM",     SCPI::DISPLAY::ACCUM_NUMBER},
    {"MODE",    SCPI::DISPLAY::ACCUM_MODE},
    {"CLEAR",   SCPI::DISPLAY::ACCUM_CLEAR},
LEAVE_PARSE_FUNC



ENTER_PARSE_FUNC(SCPI::DISPLAY::AVERAGE)
    {"NUMBER",  SCPI::DISPLAY::AVERAGE_NUMBER},
    {"NUM",     SCPI::DISPLAY::AVERAGE_NUMBER},
    {"MODE",    SCPI::DISPLAY::AVERAGE_MODE},
LEAVE_PARSE_FUNC



void SCPI::DISPLAY::AUTOSEND(pUCHAR buffer)
{
    static int counter = 0;
    counter++;
    
    static const MapElement map[] =
    {
        {"1", 1},       // ���� ������ ��� ������� ������� ������ � �������� � ��������
        {"2", 2},       // ���� ������ ��� ������� ����������� �������
        {"3", 3},       // ���� ������ ��� ������� ������� ������ ��� ������� � �������
        {0}
    };
    ENTER_ANALYSIS
        if (1 == value)      { Painter::SendFrame(true);  }
        else if (2 == value) { Painter::SendFrame(false); }
        else if (3 == value) { Painter::SendFrame(true); }
    LEAVE_ANALYSIS
}



void SCPI::DISPLAY::MAPPING(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"POINTS",  1},
        {"LINES",   2},
        {"?",       3},
        {0}
    };
    ENTER_ANALYSIS
        if (1 == value)         { MODE_DRAW_SIGNAL = ModeDrawSignal::Points; }
        else if (2 == value)    { MODE_DRAW_SIGNAL = ModeDrawSignal::Lines; }
        else if (3 == value)    { SCPI_SEND(":DISPLAY:MAPPING %s", MODE_DRAW_IS_SIGNAL_LINES ? "LINES" : "POINTS"); }
    LEAVE_ANALYSIS
}



void SCPI::DISPLAY::ACCUM_NUMBER(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"DIS",         0},
        {"2",           1},
        {"4",           2},
        {"8",           3},
        {"16",          4},
        {"32",          5},
        {"64",          6},
        {"128",         7},
        {"INFINITY",    8},
        {"?",           9},
        {0}
    };
    ENTER_ANALYSIS
        if (value <= 7)         { ENUM_ACCUM = (ENumAccumulation::E)value; }
        else if (8 == value)    { ENUM_ACCUM = ENumAccumulation::Infinity; }
        else if (9 == value)
        {
            SCPI_SEND(":DISPLAY:ACCUMULATION:NUMBER %s", map[ENUM_ACCUM].key);
        }
    LEAVE_ANALYSIS
}


void SCPI::DISPLAY::ACCUM_MODE(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"NORESET", 0},
        {"RESET",   1},
        {"?",       2},
        {0}
    };
    ENTER_ANALYSIS
        if (value <= 1) { MODE_ACCUM = (ModeAccumulation::E)value; }
        else if (2 == value)
        {
            SCPI_SEND(":DISPLAY:ACCUMULATION:MODE %s", map[MODE_ACCUM].key);
        }
    LEAVE_ANALYSIS
}


void SCPI::DISPLAY::ACCUM_CLEAR(pUCHAR )
{
    PageDisplay::OnPress_Accumulation_Clear();
}


void SCPI::DISPLAY::AVERAGE_NUMBER(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"DIS", 0},
        {"2",   1},
        {"4", 2},
        {"8", 3},
        {"16", 4},
        {"32", 5},
        {"64", 6},
        {"128", 7},
        {"256", 8},
        {"512", 9},
        {"?", 10},
        {0}
    };

    ENTER_ANALYSIS
        if (value <= 9) { ENUM_AVE = (ENumAveraging::E)value; }
        else if (10 == value)
        {
            SCPI_SEND(":DISPLAY:AVERAGE:NUMBER %s", map[ENUM_AVE].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::DISPLAY::AVERAGE_MODE(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"ACCURACY",    0},
        {"APPROXIMATE", 1},
        {"?",           2},
        {0}
    };
    ENTER_ANALYSIS
        if (value <= 1) { MODE_AVE = (ModeAveraging::E)value; }
        else if (2 == value)
        {
            SCPI_SEND(":DISPLAY:AVARAGE:MODE %s", map[MODE_AVE].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::DISPLAY::MINMAX(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"1", 0},
        {"2", 1},
        {"4", 2},
        {"8", 3},
        {"16", 4},
        {"32", 5},
        {"64", 6},
        {"128", 7},
        {"DIS", 8},
        {"?", 9},
        {0}
    };
    ENTER_ANALYSIS
        if (value <= 7)         { ENUM_MIN_MAX = (ENumMinMax::E)value; }
        else if (8 == value)    { ENUM_MIN_MAX = ENumMinMax::_1; }
        else if (9 == value)
        {
            SCPI_SEND(":DISPLAY:MINMAX %s", map[ENUM_MIN_MAX].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::DISPLAY::FILTR(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"1", 0},
        {"2", 1},
        {"3", 2},
        {"4", 3},
        {"5", 4},
        {"6", 5},
        {"7", 6},
        {"8", 7},
        {"9", 8},
        {"10", 9},
        {"DIS", 10},
        {"?", 11},
        {0}
    };
    ENTER_ANALYSIS
        if (value <= 9)         { SMOOTHING = (Smoothing::E)value; }
        else if (10 == value)   { SMOOTHING = Smoothing::Disable; }
        else if (11 == value)
        {
            SCPI_SEND(":DISPLAY:FILTR %s", map[SMOOTHING].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::DISPLAY::FPS(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"25",  0},
        {"10",  1},
        {"5",   2},
        {"2",   3},
        {"1",   4},
        {"?",   5},
        {0}
    };
    ENTER_ANALYSIS
        if (value < 5) { ENUM_SIGNALS_IN_SEC = (ENumSignalsInSec::E)value; PageDisplay::OnChanged_RefreshFPS(true); }
        else if (5 == value)
        {
            SCPI_SEND(":DISPLAY:FPS %s", map[ENUM_SIGNALS_IN_SEC].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::DISPLAY::WINDOW(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"STANDARD",    0},
        {"SIMPLE",      1},
        {"?",           2},
        {0}
    };
    ENTER_ANALYSIS
        if (value == 0) { set.display.showFullMemoryWindow = true; }
        else if (1 == value) { set.display.showFullMemoryWindow = false; }
        else if (2 == value)
        {
            SCPI_SEND(":DISPLAY:WINDOW %s", set.display.showFullMemoryWindow ? "STANDARD" : "SIMPLE");
        }
    LEAVE_ANALYSIS
}



void SCPI::DISPLAY::GRID_BRIGHTNESS(pUCHAR buffer)
{
    int intVal = 0;
    if (SCPI::FirstIsInt(buffer, &intVal, 0, 100))
    {
        BRIGHTNESS_GRID = (int16)intVal;
        Display::RunAfterDraw(PageDisplay::OnChanged_Grid_Brightness);
    }
    else
    {
        static const MapElement map[] =
        {
            {"?", 0},
            {0}
        };

        ENTER_ANALYSIS
            if (0 == value)
            {
                Color_Init(PageDisplay::colorTypeGrid);
                SCPI_SEND(":DISPLAY:GRID:BRIGHTNESS %d", (int)(PageDisplay::colorTypeGrid->brightness * 100.0F));
            }
        LEAVE_ANALYSIS
    }
}



void SCPI::DISPLAY::GRID_TYPE(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"1", 0},
        {"2", 1},
        {"3", 2},
        {"4", 3},
        {"?", 4},
        {0}
    };

    ENTER_ANALYSIS
        if (value <= 3) { TYPE_GRID = (TypeGrid::E)value; }
        else if (4 == value)
        {
            SCPI_SEND(":DISPLAY:GRID:TYPE %s", map[TYPE_GRID].key);
        }
    LEAVE_ANALYSIS
}
