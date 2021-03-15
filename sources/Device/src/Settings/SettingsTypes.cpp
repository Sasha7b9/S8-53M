#include "defines.h"
#include "common/Settings/SettingsTypes_.h"
#include "common/Utils/Math_.h"
#include "Display/Display.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/MathFPGA.h"
#include "Settings/Settings.h"
#include "Settings/SettingsChannel.h"
#include "Settings/SettingsTypes.h"


int Divider::ToAbs(Divider::E multiplier)
{
    static const int results[2] = { 1, 10 };

    return results[multiplier];
}


pchar Range::Name(Range::E range)
{
    static const char *names[Range::Count + 1] =
    {
        "2mV",
        "5mV",
        "10mV",
        "20mV",
        "50mV",
        "100mV",
        "200mV",
        "500mV",
        "1V",
        "2V",
        "5V",
        "10V",
        "20V",
        "Range::Count"
    };
    return names[range];
}


const char *TBaseName(TBase::E tBase)
{
    static const char *names[TBase::Count + 1] =
    {
        "2ns",
        "5ns",
        "10ns",
        "20ns",
        "50ns",
        "100ns",
        "200ns",
        "500ns",
        "1us",
        "2us",
        "5us",
        "10us",
        "20us",
        "50us",
        "100us",
        "200us",
        "500us",
        "1ms",
        "2ms",
        "5ms",
        "10ms",
        "20ms",
        "50ms",
        "100ms",
        "200ms",
        "500ms",
        "1s",
        "2s",
        "5s",
        "10s",
        "TBase::Count"
    };

    return names[tBase];
}


void RShift::Set(Channel::E chan, int16 rShift)
{
    if (!sChannel_Enabled(chan))
    {
        return;
    }
    Display::ChangedRShiftMarkers();

    if (rShift > RShiftMax || rShift < RShiftMin)
    {
        Display::ShowWarningBad(chan == Channel::A ? Warning::LimitChan1_RShift : Warning::LimitChan2_RShift);
    }

    LIMITATION(rShift, rShift, RShiftMin, RShiftMax);
    if (rShift > RShiftZero)
    {
        rShift &= 0xfffe;                                            // ƒелаем кратным двум, т.к. у нас 800 значений на 400 точек
    }
    else if (rShift < RShiftZero)
    {
        rShift = (rShift + 1) & 0xfffe;
    }
    SET_RSHIFT(chan) = rShift;
    RShift::Load(chan);
    Display::RotateRShift(chan);
};



void TBase::Decrease()
{
    if ((PEAKDET != PeackDetMode::Disable) && SET_TBASE <= MIN_TBASE_PEC_DEAT)
    {
        Display::ShowWarningBad(Warning::LimitSweep_Time);
        Display::ShowWarningBad(Warning::EnabledPeakDet);
        return;
    }

    if ((int)SET_TBASE > 0)
    {
        if (SET_SELFRECORDER && SET_TBASE == MIN_TBASE_P2P)
        {
            Display::ShowWarningBad(Warning::TooFastScanForSelfRecorder);
        }
        else
        {
            E base = (E)((int)SET_TBASE - 1);
            Set(base);
        }
    }
    else
    {
        Display::ShowWarningBad(Warning::LimitSweep_Time);
    }
}


void TBase::Increase()
{
    if (SET_TBASE < (TBase::Count - 1))
    {
        E base = (E)(SET_TBASE + 1);
        Set(base);
    }
    else
    {
        Display::ShowWarningBad(Warning::LimitSweep_Time);
    }
}


void TBase::Set(TBase::E tBase)
{
    if (!sChannel_Enabled(Channel::A) && !sChannel_Enabled(Channel::B))
    {
        return;
    }
    if (tBase < TBase::Count)
    {
        float tShiftAbsOld = TSHIFT_2_ABS(TSHIFT, SET_TBASE);
        sTime_SetTBase(tBase);
        Load();
        TShift::Set(static_cast<int>(TSHIFT_2_REL(tShiftAbsOld, SET_TBASE)));
        Display::Redraw();
    }
    else
    {
        Display::ShowWarningBad(Warning::LimitSweep_Time);
    }
};


void TShift::Set(int tShift)
{
    if (!sChannel_Enabled(Channel::A) && !sChannel_Enabled(Channel::B))
    {
        return;
    }

    if (tShift < sTime_TShiftMin() || tShift > TShiftMax)
    {
        LIMITATION(tShift, tShift, sTime_TShiftMin(), TShiftMax);
        Display::ShowWarningBad(Warning::LimitSweep_TShift);
    }

    sTime_SetTShift((int16)tShift);
    TShift::Load();
    Display::Redraw();
};


void PeackDetMode::Set(PeackDetMode::E peackDetMode)
{
    PEAKDET = peackDetMode;
    RegUPR::Load();
}


void ModeCouple::Set(Channel::E chan, ModeCouple::E modeCoupe)
{
    SET_COUPLE(chan) = modeCoupe;
    FPGA::SetAttribChannelsAndTrig(chan == Channel::A ? TypeWriteAnalog::ChanParam0 : TypeWriteAnalog::ChanParam1);
    RShift::Set(chan, SET_RSHIFT(chan));
}
