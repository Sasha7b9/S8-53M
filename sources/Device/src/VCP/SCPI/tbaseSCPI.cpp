#include "defines.h"
#include "common/Log_.h"
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



void SCPI::TBASE::RANGE(puchar buffer)
{
    static const MapElement map[] = 
    {
        {"2ns",     (uint8)TBase::_2ns},
        {"5ns",     (uint8)TBase::_5ns},
        {"10ns",    (uint8)TBase::_10ns},
        {"20ns",    (uint8)TBase::_20ns},
        {"50ns",    (uint8)TBase::_50ns},
        {"100ns",   (uint8)TBase::_100ns},
        {"200ns",   (uint8)TBase::_200ns},
        {"500ns",   (uint8)TBase::_500ns},
        {"1us",     (uint8)TBase::_1us},
        {"2us",     (uint8)TBase::_2us},
        {"5us",     (uint8)TBase::_5us},
        {"10us",    (uint8)TBase::_10us},
        {"20us",    (uint8)TBase::_20us},
        {"50us",    (uint8)TBase::_50us},
        {"100us",   (uint8)TBase::_100us},
        {"200us",   (uint8)TBase::_200us},
        {"500us",   (uint8)TBase::_500us},
        {"1ms",     (uint8)TBase::_1ms},
        {"2ms",     (uint8)TBase::_2ms},
        {"5ms",     (uint8)TBase::_5ms},
        {"10ms",    (uint8)TBase::_10ms},
        {"20ms",    (uint8)TBase::_20ms},
        {"50ms",    (uint8)TBase::_50ms},
        {"100ms",   (uint8)TBase::_100ms},
        {"200ms",   (uint8)TBase::_200ms},
        {"500ms",   (uint8)TBase::_500ms},
        {"1s",      (uint8)TBase::_1s},
        {"2s",      (uint8)TBase::_2s},
        {"5s",      (uint8)TBase::_5s},
        {"10s",     (uint8)TBase::_10s},
        {"?",       255},
        {0}
    };
    ENTER_ANALYSIS
        if (TBase::Count > value) { TBase::Set((TBase::E)value); }
        else if (255 == value) //-V547
        {
            SCPI_SEND(":TBASE:SET_RANGE %s", TBase::ToStringEN(TBase::Get()));
        }
    LEAVE_ANALYSIS
}



void SCPI::TBASE::OFFSET(puchar buffer)
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
        TShift::Set(tShift);
        return;
    }

    ENTER_ANALYSIS
        if (0 == value)
        {
            int retValue = TShift::Get() - 1024;
            SCPI_SEND(":TBASE:OFFSET %d", retValue);
        }
    LEAVE_ANALYSIS
}



void SCPI::TBASE::SAMPLING(puchar buffer)
{
    static const MapElement map[] =
    {
        {"EQUAL", 0},
        {"REAL",  1},
        {"?",     2},
        {0}
    };

    ENTER_ANALYSIS
        if (value < 2) { set.time.sampleType = (SampleType::E)value; }
        else if (2 == value)
        {
            SCPI_SEND(":TBASE:SAMPLING %s", map[SampleType::Get()].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::TBASE::PEACKDET(puchar buffer)
{
    static const MapElement map[] =
    {
        {"ON",  0},
        {"OFF", 1},
        {"?",   2},
        {0}
    };
    ENTER_ANALYSIS
        if (value < 2) { set.time.peakDet = (value == 0) ? PeackDetMode::Disable : PeackDetMode::Enable;
                                     PageTime::OnChanged_PeakDet(true); } // WARN SCPI для пикового детектора переделать
        else if (2 == value)
        {
            SCPI_SEND(":TBASE:PEACKDET %s", PeackDetMode::IsEnabled() ? "ON" : "OFF");
        }
    LEAVE_ANALYSIS
}



void SCPI::TBASE::TPOS(puchar buffer)
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
        if (value < 3)      { set.time.tPos = (TPos::E)value; PageTime::OnChanged_TPos(true); }
        else if (4 == value)
        {
            SCPI_SEND(":TBASE:TPOS %s", map[TPos::Get()].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::TBASE::SELFRECORDER(puchar buffer)
{
    static const MapElement map[] =
    {
        {"ON", 0},
        {"OFF", 1},
        {"?", 2},
        {0}
    };
    ENTER_ANALYSIS
        if (value < 2) { set.time.selfRecorder = (value == 0); }
        else if (2 == value)
        {
            SCPI_SEND(":TBASE:SELFRECORDER %s", FPGA::SET::InSelfRecorderMode() ? "ON" : "OFF");
        }
    LEAVE_ANALYSIS
}



void SCPI::TBASE::FUNCTIMEDIV(puchar buffer)
{
    static const MapElement map[] =
    {
        {"TIME",   0},
        {"MEMORY", 1},
        {"?",      2},
        {0}
    };
    ENTER_ANALYSIS
        if (value < 2) { TIME_DIV_XPOS = (FunctionTime::E)value; }
        else if (2 == value)
        {
            SCPI_SEND(":TBASE:FUNCTIMEDIV %s", map[TIME_DIV_XPOS].key);
        }
    LEAVE_ANALYSIS
}
