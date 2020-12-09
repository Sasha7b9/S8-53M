#include "defines.h"
#include "common/Log_c.h"
#include "FPGA/FPGA.h"
#include "LAN/LAN.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "VCP/VCP.h"
#include "VCP/SCPI/SCPI.h"
#include "VCP/SCPI/Utils/MapSCPI.h"


ENTER_PARSE_FUNC(SCPI::ProcessTBASE)
    {"SET_RANGE",   SCPI::TBASE::RANGE},
    {"OFFSET",      SCPI::TBASE::OFFSET},
    {"SAMPLING",    SCPI::TBASE::SAMPLING},
    {"SAMPL",       SCPI::TBASE::SAMPLING},
    {"PEACKDET",    SCPI::TBASE::PEACKDET},
    {"PEACK",       SCPI::TBASE::PEACKDET},
    {"TPOS",        SCPI::TBASE::TPOS},
    {"SELFRECORDER",SCPI::TBASE::SELFRECORDER},
    {"FUNCTIMEDIV", SCPI::TBASE::FUNCTIMEDIV},
LEAVE_PARSE_FUNC



void SCPI::TBASE::RANGE(pUCHAR buffer)
{
    static const MapElement map[] = 
    {
        {"2ns",     (uint8)TBase::_2ns}, //-V2533
        {"5ns",     (uint8)TBase::_5ns}, //-V2533
        {"10ns",    (uint8)TBase::_10ns}, //-V2533
        {"20ns",    (uint8)TBase::_20ns}, //-V2533
        {"50ns",    (uint8)TBase::_50ns}, //-V2533
        {"100ns",   (uint8)TBase::_100ns}, //-V2533
        {"200ns",   (uint8)TBase::_200ns}, //-V2533
        {"500ns",   (uint8)TBase::_500ns}, //-V2533
        {"1us",     (uint8)TBase::_1us}, //-V2533
        {"2us",     (uint8)TBase::_2us}, //-V2533
        {"5us",     (uint8)TBase::_5us}, //-V2533
        {"10us",    (uint8)TBase::_10us}, //-V2533
        {"20us",    (uint8)TBase::_20us}, //-V2533
        {"50us",    (uint8)TBase::_50us}, //-V2533
        {"100us",   (uint8)TBase::_100us}, //-V2533
        {"200us",   (uint8)TBase::_200us}, //-V2533
        {"500us",   (uint8)TBase::_500us}, //-V2533
        {"1ms",     (uint8)TBase::_1ms}, //-V2533
        {"2ms",     (uint8)TBase::_2ms}, //-V2533
        {"5ms",     (uint8)TBase::_5ms}, //-V2533
        {"10ms",    (uint8)TBase::_10ms}, //-V2533
        {"20ms",    (uint8)TBase::_20ms}, //-V2533
        {"50ms",    (uint8)TBase::_50ms}, //-V2533
        {"100ms",   (uint8)TBase::_100ms}, //-V2533
        {"200ms",   (uint8)TBase::_200ms}, //-V2533
        {"500ms",   (uint8)TBase::_500ms}, //-V2533
        {"1s",      (uint8)TBase::_1s}, //-V2533
        {"2s",      (uint8)TBase::_2s}, //-V2533
        {"5s",      (uint8)TBase::_5s}, //-V2533
        {"10s",     (uint8)TBase::_10s}, //-V2533
        {"?",       255},
        {0}
    };
    ENTER_ANALYSIS
        if (TBase::Count > value) { FPGA::SetTBase((TBase::E)value); } //-V2533
        else if (255 == value) //-V547 //-V2516
        {
            SCPI_SEND(":TBASE:SET_RANGE %s", Tables::GetTBaseStringEN(SET_TBASE));
        }
    LEAVE_ANALYSIS
}



void SCPI::TBASE::OFFSET(pUCHAR buffer) //-V2506
{
    static const MapElement map[] =
    {
        {"?", 0},
        {0}
    };

    int intVal = 0;
    if (SCPI::FirstIsInt(buffer, &intVal, -1024, 15000))
    {
        int tShift = intVal + 1024;
        FPGA::SetTShift(tShift);
        return;
    }

    ENTER_ANALYSIS
        if (0 == value)
        {
            int retValue = TSHIFT - 1024;
            SCPI_SEND(":TBASE:OFFSET %d", retValue);
        }
    LEAVE_ANALYSIS
}



void SCPI::TBASE::SAMPLING(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"EQUAL", 0},
        {"REAL",  1},
        {"?",     2},
        {0}
    };
    ENTER_ANALYSIS
        if (value < 2) { SAMPLE_TYPE = (SampleType::E)value; } //-V2533
        else if (2 == value) //-V2516
        {
            SCPI_SEND(":TBASE:SAMPLING %s", map[SAMPLE_TYPE].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::TBASE::PEACKDET(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"ON",  0},
        {"OFF", 1},
        {"?",   2},
        {0}
    };
    ENTER_ANALYSIS
        if (value < 2) { PEAKDET = (value == 0) ? PeackDetMode::Disable : PeackDetMode::Enable; PageTime::OnChanged_PeakDet(true); } // WARN SCPI для пикового детектора переделать
        else if (2 == value) //-V2516
        {
            SCPI_SEND(":TBASE:PEACKDET %s", PEAKDET ? "ON" : "OFF");
        }
    LEAVE_ANALYSIS
}



void SCPI::TBASE::TPOS(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"LEFT",   0},
        {"CENTER", 1},
        {"RIGHT",  2},
        {"?",      3},
        {0}
    };
    ENTER_ANALYSIS
        if (value < 3)      { SET_TPOS = (TPos::E)value; PageTime::OnChanged_TPos(true); } //-V2533
        else if (4 == value) //-V2516
        {
            SCPI_SEND(":TBASE:TPOS %s", map[SET_TPOS].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::TBASE::SELFRECORDER(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"ON", 0},
        {"OFF", 1},
        {"?", 2},
        {0}
    };
    ENTER_ANALYSIS
        if (value < 2) { SET_SELFRECORDER = (value == 0); }
        else if (2 == value) //-V2516
        {
            SCPI_SEND(":TBASE:SELFRECORDER %s", SET_SELFRECORDER ? "ON" : "OFF");
        }
    LEAVE_ANALYSIS
}



void SCPI::TBASE::FUNCTIMEDIV(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"TIME",   0},
        {"MEMORY", 1},
        {"?",      2},
        {0}
    };
    ENTER_ANALYSIS
        if (value < 2) { TIME_DIV_XPOS = (FunctionTime::E)value; } //-V2533
        else if (2 == value) //-V2516
        {
            SCPI_SEND(":TBASE:FUNCTIMEDIV %s", map[TIME_DIV_XPOS].key);
        }
    LEAVE_ANALYSIS
}
