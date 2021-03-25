#include "defines.h"
#include "common/Log_.h"
#include "FPGA/FPGA.h"
#include "LAN/LAN.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
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


void SCPI::TRIGGER::MODE(puchar buffer)
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
        if (0 == value)         { set.trig.start_mode = StartMode::Auto; PageTrig::OnPress_Mode(true); }
        else if (1 == value)    { set.trig.start_mode = StartMode::Wait; PageTrig::OnPress_Mode(true); }
        else if (2 == value)    { set.trig.start_mode = StartMode::Single; PageTrig::OnPress_Mode(true); }
        else if (3 == value)
        {
            SCPI_SEND(":TRIGGER:MODE %s", map[StartMode::Get()].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::TRIGGER::SOURCE(puchar buffer)
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
        if (0 == value)         { TrigSource::Set(TrigSource::A); }
        else if (1 == value)    { TrigSource::Set(TrigSource::B); }
        else if (2 == value)    { TrigSource::Set(TrigSource::Ext); }
        else if (3 == value)
        {
            SCPI_SEND(":TRIGGER:SOUCRE %s", map[StartMode::Get()].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::TRIGGER::POLARITY(puchar buffer)
{
    static const MapElement map[] =
    {
        {"FRONT", 0},
        {"BACK",  1},
        {"?",     2},
        {0}
    };
    ENTER_ANALYSIS
        if (0 == value)         { TrigPolarity::Set(TrigPolarity::Front); }
        else if (1 == value)    { TrigPolarity::Set(TrigPolarity::Back); }
        else if (2 == value)
        {
            SCPI_SEND(":TRIGGER:POLARITY %s", map[set.trig.polarity].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::TRIGGER::INPUT(puchar buffer)
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
        if (0 == value)         { TrigInput::Set(TrigInput::Full); }
        else if (1 == value)    { TrigInput::Set(TrigInput::AC); }
        else if (2 == value)    { TrigInput::Set(TrigInput::LPF); }
        else if (3 == value)    { TrigInput::Set(TrigInput::HPF); }
        else if (4 == value)
        {
            SCPI_SEND(":TRIGGER:INPUT %s", map[TrigInput::Get()].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::TRIGGER::FIND(puchar buffer)
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
        if (0 == value)         { set.trig.modeFind = TrigModeFind::Hand; }
        else if (1 == value)    { set.trig.modeFind = TrigModeFind::Auto; }
        else if (2 == value)    { TrigLev::FindAndSet(); }
        else if (3 == value)
        {
            SCPI_SEND(":TRIGGER:FIND %s", map[TrigModeFind::Get()].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::TRIGGER::OFFSET(puchar buffer)
{
    static const MapElement map[] =
    {
        {"?", 0},
        {0}
    };

    int intVal = 0;
    if (SCPI::FirstIsInt(buffer, &intVal, -240, 240))
    {
        int trigLev = RShift::ZERO + 2 * intVal;
        TrigLev::Set(TrigSource::Get(), (int16)trigLev);
        return;
    }

    ENTER_ANALYSIS
        if (value == 0)
        {
            int retValue = (int)(0.5F * (TrigLev::Get() - RShift::ZERO));
            SCPI_SEND(":TRIGGER:OFFSET %d", retValue);
        }
    LEAVE_ANALYSIS
}
