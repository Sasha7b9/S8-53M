#include "defines.h"
#include "common/Log_.h"
#include "common/Display/Painter_.h"
#include "LAN/LAN.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "VCP/VCP.h"
#include "VCP/SCPI/SCPI.h"
#include "VCP/SCPI/Utils/MapSCPI.h"


ENTER_PARSE_FUNC(SCPI::ProcessDISPLAY)
    {"AUTOSEND",    SCPI::DISPLAY::AUTOSEND},
    {"MAPPING",     SCPI::DISPLAY::MAPPING},    // Отображение
    {"MAP",         SCPI::DISPLAY::MAPPING},
    {"ACCUMULATION",SCPI::DISPLAY::ACCUM},      // Накопление
    {"ACCUM",       SCPI::DISPLAY::ACCUM},     
    {"AVERAGE",     SCPI::DISPLAY::AVERAGE},    // Усреднение
    {"AVE",         SCPI::DISPLAY::AVERAGE},
    {"MINMAX",      SCPI::DISPLAY::MINMAX},     // Мин Макс
    {"FILTR",       SCPI::DISPLAY::FILTR},      // Сглаживание
    {"FPS",         SCPI::DISPLAY::FPS},        // Частота обновл
    {"WINDOW",      SCPI::DISPLAY::WINDOW},     // Окно памяти
    {"GRID",        SCPI::DISPLAY::GRID},       // СЕТКА
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



void SCPI::DISPLAY::AUTOSEND(puchar buffer)
{
    static int counter = 0;
    counter++;
    
    static const MapElement map[] =
    {
        {"1", 1},       // Этот запрос для запроса первого фрейма с палитрой и шрифтами
        {"2", 2},       // Этот запрос для запроса последующих фреймов
        {"3", 3},       // Этот запрос для запроса первого фрейма без палитры и шрифтов
        {0}
    };
    ENTER_ANALYSIS
        if (1 == value)      { Painter::SendFrame(true);  }
        else if (2 == value) { Painter::SendFrame(false); }
        else if (3 == value) { Painter::SendFrame(true); }
    LEAVE_ANALYSIS
}



void SCPI::DISPLAY::MAPPING(puchar buffer)
{
    static const MapElement map[] =
    {
        {"POINTS",  1},
        {"LINES",   2},
        {"?",       3},
        {0}
    };

    ModeDrawSignal &mode = set.display.mode_draw_signal;

    ENTER_ANALYSIS
        if (1 == value)         { mode = ModeDrawSignal::Points; }
        else if (2 == value)    { mode = ModeDrawSignal::Lines; }
        else if (3 == value)    { SCPI_SEND(":DISPLAY:MAPPING %s", mode.IsLines() ? "LINES" : "POINTS"); }
    LEAVE_ANALYSIS
}



void SCPI::DISPLAY::ACCUM_NUMBER(puchar buffer)
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
        if (value <= 7)         { ENumAccumulation::Set((ENumAccumulation::E)value); }
        else if (8 == value)    { ENumAccumulation::Set(ENumAccumulation::Infinity); }
        else if (9 == value)
        {
            SCPI_SEND(":DISPLAY:ACCUMULATION:NUMBER %s", map[ENumAccumulation::Get()].key);
        }
    LEAVE_ANALYSIS
}


void SCPI::DISPLAY::ACCUM_MODE(puchar buffer)
{
    static const MapElement map[] =
    {
        {"NORESET", 0},
        {"RESET",   1},
        {"?",       2},
        {0}
    };
    ENTER_ANALYSIS
        if (value <= 1) { ModeAccumulation::Set((ModeAccumulation::E)value); }
        else if (2 == value)
        {
            SCPI_SEND(":DISPLAY:ACCUMULATION:MODE %s", map[ModeAccumulation::Get()].key);
        }
    LEAVE_ANALYSIS
}


void SCPI::DISPLAY::ACCUM_CLEAR(puchar )
{
    PageDisplay::OnPress_Accumulation_Clear();
}


void SCPI::DISPLAY::AVERAGE_NUMBER(puchar buffer)
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
        if (value <= 9) { set.display.enum_ave = (ENumAveraging::E)value; }
        else if (10 == value)
        {
            SCPI_SEND(":DISPLAY:AVERAGE:NUMBER %s", map[set.display.enum_ave].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::DISPLAY::AVERAGE_MODE(puchar buffer)
{
    static const MapElement map[] =
    {
        {"ACCURACY",    0},
        {"APPROXIMATE", 1},
        {"?",           2},
        {0}
    };
    ENTER_ANALYSIS
        if (value <= 1) { set.display.mode_ave = (ModeAveraging::E)value; }
        else if (2 == value)
        {
            SCPI_SEND(":DISPLAY:AVARAGE:MODE %s", map[set.display.mode_ave].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::DISPLAY::MINMAX(puchar buffer)
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
        if (value <= 7)         { set.display.enum_min_max = (ENumMinMax::E)value; }
        else if (8 == value)    { set.display.enum_min_max = ENumMinMax::_1; }
        else if (9 == value)
        {
            SCPI_SEND(":DISPLAY:MINMAX %s", map[set.display.enum_min_max].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::DISPLAY::FILTR(puchar buffer)
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
        if (value <= 9)         { Smoothing::Set((Smoothing::E)value); }
        else if (10 == value)   { Smoothing::Set(Smoothing::Disable); }
        else if (11 == value)
        {
            SCPI_SEND(":DISPLAY:FILTR %s", map[Smoothing::Get()].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::DISPLAY::FPS(puchar buffer)
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
        if (value < 5) { set.display.enum_fps = (ENumSignalsInSec::E)value; PageDisplay::OnChanged_RefreshFPS(true); }
        else if (5 == value)
        {
            SCPI_SEND(":DISPLAY:FPS %s", map[set.display.enum_fps].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::DISPLAY::WINDOW(puchar buffer)
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



void SCPI::DISPLAY::GRID_BRIGHTNESS(puchar buffer)
{
    int intVal = 0;
    if (SCPI::FirstIsInt(buffer, &intVal, 0, 100))
    {
        set.display.brightness_grid = (int16)intVal;
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
                PageDisplay::colorTypeGrid->Init();
                SCPI_SEND(":DISPLAY:GRID:BRIGHTNESS %d", (int)(PageDisplay::colorTypeGrid->brightness * 100.0F));
            }
        LEAVE_ANALYSIS
    }
}



void SCPI::DISPLAY::GRID_TYPE(puchar buffer)
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
        if (value <= 3) { set.display.type_grid.value = (TypeGrid::E)value; }
        else if (4 == value)
        {
            SCPI_SEND(":DISPLAY:GRID:TYPE %s", map[set.display.type_grid].key);
        }
    LEAVE_ANALYSIS
}
