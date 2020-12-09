#include "defines.h"
#include "common/Log_c.h"
#include "FPGA/FPGA.h"
#include "LAN/LAN.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include "VCP/VCP.h"
#include "VCP/SCPI/SCPI.h"
#include "VCP/SCPI/Utils/MapSCPI.h"


/*
    TRIG:
        MODE {AUTO | WAIT | SINGLE | ?}
        SOURCE {CHAN1 | CHAN2 | EXT | ?}
        POLARITY {FRONT | BACK | ?}
        INPUT {FULL | AC | LPF | HPF | ?}
        OFFSET {-200...200 | ?}
*/



ENTER_PARSE_FUNC(SCPI::ProcessTRIG)
    {"MODE",        SCPI::TRIGGER::MODE},
    {"SOURCE",      SCPI::TRIGGER::SOURCE},
    {"POLARITY",    SCPI::TRIGGER::POLARITY},
    {"POLAR",       SCPI::TRIGGER::POLARITY},
    {"INPUT",       SCPI::TRIGGER::INPUT},
    {"FIND",        SCPI::TRIGGER::FIND},
    {"OFFSET",      SCPI::TRIGGER::OFFSET},
LEAVE_PARSE_FUNC


void SCPI::TRIGGER::MODE(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"AUTO",   0},
        {"WAIT",   1},
        {"SINGLE", 2},
        {"?",      3},
        {0}
    };
    ENTER_ANALYSIS
        if (0 == value)         { START_MODE = StartMode::Auto; PageTrig::OnPress_Mode(true); }
        else if (1 == value)    { START_MODE = StartMode::Wait; PageTrig::OnPress_Mode(true); }
        else if (2 == value)    { START_MODE = StartMode::Single; PageTrig::OnPress_Mode(true); }
        else if (3 == value) //-V2516
        {
            SCPI_SEND(":TRIGGER:MODE %s", map[START_MODE].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::TRIGGER::SOURCE(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"CHAN1", 0},
        {"CHAN2", 1},
        {"EXT",   2},
        {"?",     3},
        {0}
    };
    ENTER_ANALYSIS
        if (0 == value)         { FPGA::SetTrigSource(TrigSource::A); }
        else if (1 == value)    { FPGA::SetTrigSource(TrigSource::B); }
        else if (2 == value)    { FPGA::SetTrigSource(TrigSource::Ext); }
        else if (3 == value) //-V2516
        {
            SCPI_SEND(":TRIGGER:SOUCRE %s", map[START_MODE].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::TRIGGER::POLARITY(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"FRONT", 0},
        {"BACK",  1},
        {"?",     2},
        {0}
    };
    ENTER_ANALYSIS
        if (0 == value)         { FPGA::SetTrigPolarity(TrigPolarity::Front); }
        else if (1 == value)    { FPGA::SetTrigPolarity(TrigPolarity::Back); }
        else if (2 == value) //-V2516
        {
            SCPI_SEND(":TRIGGER:POLARITY %s", map[TRIG_POLARITY].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::TRIGGER::INPUT(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"FULL", 0},
        {"AC",   1},
        {"LPF",  2},
        {"HPF",  3},
        {"?",    4},
        {0}
    };
    ENTER_ANALYSIS
        if (0 == value)         { FPGA::SetTrigInput(TrigInput::Full); }
        else if (1 == value)    { FPGA::SetTrigInput(TrigInput::AC); }
        else if (2 == value)    { FPGA::SetTrigInput(TrigInput::LPF); }
        else if (3 == value)    { FPGA::SetTrigInput(TrigInput::HPF); }
        else if (4 == value) //-V2516
        {
            SCPI_SEND(":TRIGGER:INPUT %s", map[TRIG_INPUT].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::TRIGGER::FIND(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"HAND", 0},
        {"AUTO", 1},
        {"FIND", 2},
        {"?",    3},
        {0}
    };
    ENTER_ANALYSIS
        if (0 == value)         { TRIG_MODE_FIND = TrigModeFind::Hand; }
        else if (1 == value)    { TRIG_MODE_FIND = TrigModeFind::Auto; }
        else if (2 == value)    { FPGA::FindAndSetTrigLevel(); }
        else if (3 == value) //-V2516
        {
            SCPI_SEND(":TRIGGER:FIND %s", map[TRIG_MODE_FIND].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::TRIGGER::OFFSET(pUCHAR buffer) //-V2506
{
    static const MapElement map[] =
    {
        {"?", 0},
        {0}
    };

    int intVal = 0;
    if (SCPI::FirstIsInt(buffer, &intVal, -240, 240))
    {
        int trigLev = RShiftZero + 2 * intVal;
        FPGA::SetTrigLev(TRIG_SOURCE, (int16)trigLev); //-V2533
        return;
    }

    ENTER_ANALYSIS
        if (value == 0)
        {
            int retValue = static_cast<int>(0.5F * (TRIG_LEVEL_SOURCE - RShiftZero));
            SCPI_SEND(":TRIGGER:OFFSET %d", retValue);
        }
    LEAVE_ANALYSIS
}
