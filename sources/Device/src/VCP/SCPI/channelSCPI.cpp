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


static Channel::E chan = Channel::A;



ENTER_PARSE_FUNC(SCPI::ProcessCHANNEL)
        {"INPUT",       SCPI::CHANNEL::INPUT},
        {"COUPLING",    SCPI::CHANNEL::COUPLE},
        {"COUPL",       SCPI::CHANNEL::COUPLE},
        {"FILTR",       SCPI::CHANNEL::FILTR},
        {"SET_INVERSE", SCPI::CHANNEL::INVERSE},
        {"INV",         SCPI::CHANNEL::INVERSE},
        {"SET_RANGE",   SCPI::CHANNEL::RANGE},
        {"OFFSET",      SCPI::CHANNEL::OFFSET},
        {"FACTOR",      SCPI::CHANNEL::FACTOR},
        {"FACT",        SCPI::CHANNEL::FACTOR},
        {0}
    };

    chan = (char)(*(buffer - 2)) == '1' ? Channel::A : Channel::B; //-V2533

    SCPI::ProcessingCommand(commands, const_cast<uint8 *>(buffer));
}



void SCPI::CHANNEL::INPUT(pUCHAR buffer)
{
    static const MapElement map[] = 
    {
        {"ON",  0},
        {"OFF", 1},
        {"?",   2},
        {0}
    };
    ENTER_ANALYSIS
        if (0 == value)         { SET_ENABLED(chan) = true; }
        else if (1 == value)    { SET_ENABLED(chan) = false; }
        else if (2 == value) //-V2516
        {
            SCPI_SEND(":CHANNEL%d:INPUT %s", Tables::GetNumChannel(chan), sChannel_Enabled(chan) ? "ON" : "OFF");
        }
    LEAVE_ANALYSIS
}



void SCPI::CHANNEL::COUPLE(pUCHAR buffer)
{
    static const pFuncVB func[2] = {PageChannelA::OnChanged_Couple, PageChannelB::OnChanged_Couple};

    static const MapElement map[] = 
    {
        {"DC",  0},
        {"AC",  1},
        {"GND", 2},
        {"?",   3},
        {0}
    };
    ENTER_ANALYSIS
        if (0 == value)         { SET_COUPLE(chan) = ModeCouple::DC; func[chan](true); }
        else if (1 == value)    { SET_COUPLE(chan) = ModeCouple::AC; func[chan](true); }
        else if (2 == value)    { SET_COUPLE(chan) = ModeCouple::GND; func[chan](true); }
        else if (3 == value) //-V2516
        {
            SCPI_SEND(":CHANNEL%d:COUPLING %s", Tables::GetNumChannel(chan), map[SET_COUPLE(chan)].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::CHANNEL::FILTR(pUCHAR buffer)
{
    static const pFuncVB func[2] = {PageChannelA::OnChanged_Filtr, PageChannelB::OnChanged_Filtr};

    static const MapElement map[] =
    {
        {"ON",  0},
        {"OFF", 1},
        {"?",   2},
        {0}
    };
    ENTER_ANALYSIS
        if (0 == value)         { SET_FILTR(chan) = true; func[chan](true); }
        else if (1 == value)    { SET_FILTR(chan) = false; func[chan](true); }
        else if (2 == value) //-V2516
        {
            SCPI_SEND(":CHANNEL%d:FILTR %s", Tables::GetNumChannel(chan), SET_FILTR(chan) ? "ON" : "OFF");
        }
    LEAVE_ANALYSIS
}



void SCPI::CHANNEL::INVERSE(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"ON",  0},
        {"OFF", 1},
        {"?",   2},
        {0}
    };
    ENTER_ANALYSIS
        if (0 == value)         { SET_INVERSE(chan) = true; }
        else if (1 == value)    { SET_INVERSE(chan) = false; }
        else if (2 == value) //-V2516
        {
            SCPI_SEND(":CHANNEL%d:SET_INVERSE %s", Tables::GetNumChannel(chan), SET_INVERSE(chan) ? "ON" : "OFF");
        }
    LEAVE_ANALYSIS
}



void SCPI::CHANNEL::RANGE(pUCHAR buffer)
{
    static const MapElement map[] = 
    {
        {"2MV",   (uint8)Range::_2mV}, //-V2533
        {"5MV",   (uint8)Range::_5mV}, //-V2533
        {"10MV",  (uint8)Range::_10mV}, //-V2533
        {"20MV",  (uint8)Range::_20mV}, //-V2533
        {"50MV",  (uint8)Range::_50mV}, //-V2533
        {"100MV", (uint8)Range::_100mV}, //-V2533
        {"200MV", (uint8)Range::_200mV}, //-V2533
        {"500MV", (uint8)Range::_500mV}, //-V2533
        {"1V",    (uint8)Range::_1V}, //-V2533
        {"2V",    (uint8)Range::_2V}, //-V2533
        {"5V",    (uint8)Range::_5V}, //-V2533
        {"10V",   (uint8)Range::_10V}, //-V2533
        {"20V",   (uint8)Range::_20V}, //-V2533
        {"?",     (uint8)Range::Count}, //-V2533
        {0}
    };
    ENTER_ANALYSIS
        if (value <= (uint8)Range::_20V)      { FPGA::SetRange(chan, (Range::E)value); } //-V2533
        else if (value == (uint8)Range::Count) //-V2516 //-V2533
        {
            SCPI_SEND(":CHANNEL%d:SET_RANGE %s", Tables::GetNumChannel(chan), map[SET_RANGE(chan)].key);
        }
    LEAVE_ANALYSIS
}



void SCPI::CHANNEL::OFFSET(pUCHAR buffer) //-V2506
{
    static const MapElement map[] =
    {
        {"?", 0},
        {0}
    };
    int intVal = 0;
    if (SCPI::FirstIsInt(buffer, &intVal, -240, 240))
    {
        int rShift = RShiftZero + 2 * intVal;
        FPGA::SetRShift(chan, (int16)rShift); //-V2533
        return;
    }
    ENTER_ANALYSIS
        if (value == 0)
        {
            int retValue = static_cast<int>(0.5F * (SET_RSHIFT(chan) - RShiftZero));
            SCPI_SEND(":CHANNNEL%d:OFFSET %d", Tables::GetNumChannel(chan), retValue);
        }
    LEAVE_ANALYSIS
}



void SCPI::CHANNEL::FACTOR(pUCHAR buffer)
{
    static const MapElement map[] =
    {
        {"X1",  0},
        {"X10", 1},
        {"?",   2},
        {0}
    };
    ENTER_ANALYSIS
        if (value == 0)         { SET_DIVIDER(chan) = Divider::_1; }
        else if (value == 1)    { SET_DIVIDER(chan) = Divider::_10; }
        else if (value == 2) //-V2516
        {
            SCPI_SEND(":CHANNEL%d:PROBE %s", Tables::GetNumChannel(chan), map[SET_DIVIDER(chan)].key);
        }
    LEAVE_ANALYSIS
}
