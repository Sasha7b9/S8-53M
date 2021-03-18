#include "defines.h"
#include "common/Settings/SettingsTypes_.h"
#include "common/Utils/Math_.h"
#include "Display/Display.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Reader.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/MathFPGA.h"
#include "Settings/Settings.h"
#include "Settings/SettingsChannel.h"


bool TrigLev::need_auto_find = false;


Channel ChA(Channel::A);
Channel ChB(Channel::B);


int Divider::ToAbs(Divider::E multiplier)
{
    static const int results[2] = { 1, 10 };

    return results[multiplier];
}


pchar Range::Name(Range::E range)
{
    static pchar names[Range::Count + 1] =
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


pchar Range::ToString(Range::E range, Divider::E multiplier)
{
    // Структура для описания диапазона масштаба по напряжению.
    struct RangeStruct
    {
        const char *name[2][2];     // Название диапазона в текстовом виде, пригодном для вывода на экран.
    };

    // Массив структур описаний масштабов по напряжению.
    static const RangeStruct ranges[Range::Count] =
    {
        {"2\x10мВ",     "20\x10мВ", "2\x10mV",  "20\x10mV"},
        {"5\x10мВ",     "50\x10мВ", "5\x10mV",  "50\x10mV"},
        {"10\x10мВ",    "0.1\x10В", "10\x10mV", "0.1\x10V"},
        {"20\x10мВ",    "0.2\x10В", "20\x10mV", "0.2\x10V"},
        {"50\x10мВ",    "0.5\x10В", "50\x10mV", "0.5\x10V"},
        {"0.1\x10В",    "1\x10В",   "0.1\x10V", "1\x10V"},
        {"0.2\x10В",    "2\x10В",   "0.2\x10V", "2\x10V"},
        {"0.5\x10В",    "5\x10В",   "0.5\x10V", "5\x10V"},
        {"1\x10В",      "10\x10В",  "1\x10V",   "10\x10V"},
        {"2\x10В",      "20\x10В",  "2\x10V",   "20\x10V"},
        {"5\x10В",      "50\x10В",  "5\x10V",   "50\x10V"},
        {"10\x10В",     "100\x10В", "10\x10V",  "100\x10V"},
        {"20\x10В",     "200\x10В", "20\x10V",  "200\x10V"}
    };

    return ranges[range].name[LANG][multiplier];
}


pchar TBase::Name(TBase::E tBase)
{
    static pchar names[TBase::Count + 1] =
    {
        "1ns"
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


pchar TBase::ToString(E tbase)
{
    struct TBaseStruct
    {
        pchar name[2];
    };

    static const TBaseStruct tbases[TBase::Count] =
    {
        {"1\x10нс",     "1\x10ns"},
        {"2\x10нс",     "2\x10ns"},
        {"5\x10нс",     "5\x10ns"},
        {"10\x10нс",    "10\x10ns"},
        {"20\x10нс",    "20\x10ns"},
        {"50\x10нс",    "50\x10ns"},
        {"0.1\x10мкс",  "0.1\x10us"},
        {"0.2\x10мкс",  "0.2\x10us"},
        {"0.5\x10мкс",  "0.5\x10us"},
        {"1\x10мкс",    "1\x10us"},
        {"2\x10мкс",    "2\x10us"},
        {"5\x10мкс",    "5\x10us"},
        {"10\x10мкс",   "10\x10us"},
        {"20\x10мкс",   "20\x10us"},
        {"50\x10мкс",   "50\x10us"},
        {"0.1\x10мс",   "0.1\x10ms"},
        {"0.2\x10мс",   "0.2\x10ms"},
        {"0.5\x10мс",   "0.5\x10ms"},
        {"1\x10мс",     "1\x10ms"},
        {"2\x10мс",     "2\x10ms"},
        {"5\x10мс",     "5\x10ms"},
        {"10\x10мс",    "10\x10ms"},
        {"20\x10мс",    "20\x10ms"},
        {"50\x10мс",    "50\x10ms"},
        {"0.1\x10с",    "0.1\x10s"},
        {"0.2\x10с",    "0.2\x10s"},
        {"0.5\x10с",    "0.5\x10s"},
        {"1\x10с",      "1\x10s"},
        {"2\x10с",      "2\x10s"},
        {"5\x10с",      "5\x10s"},
        {"10\x10с",     "10\x10s"}
    };

    return tbases[tbase].name[LANG];
}


pchar TBase::ToStringEN(TBase::E tbase)
{
    Language::E lang = LANG;

    LANG = Language::English;

    pchar result = ToString(tbase);

    LANG = lang;

    return result;
}


void RShift::Set(const Channel &ch, int16 rShift)
{
    if (!ch.IsEnabled())
    {
        return;
    }
    Display::ChangedRShiftMarkers();

    if (rShift > RShiftMax || rShift < RShiftMin)
    {
        Display::ShowWarningBad(ch == ChA ? Warning::LimitChan1_RShift : Warning::LimitChan2_RShift);
    }

    LIMITATION(rShift, rShift, RShiftMin, RShiftMax);
    if (rShift > RShiftZero)
    {
        rShift &= 0xfffe;                                            // Делаем кратным двум, т.к. у нас 800 значений на 400 точек
    }
    else if (rShift < RShiftZero)
    {
        rShift = (rShift + 1) & 0xfffe;
    }
    SET_RSHIFT(ch) = rShift;
    RShift::Load(ch);
    Display::RotateRShift(ch);
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
    if (!ChA.IsEnabled() && !ChB.IsEnabled())
    {
        return;
    }
    if (tBase < TBase::Count)
    {
        float tShiftAbsOld = TSHIFT_2_ABS(TSHIFT, SET_TBASE);
        SET_TBASE = tBase;
        ReaderFPGA::ClearData();
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
    if (!ChA.IsEnabled() && !ChB.IsEnabled())
    {
        return;
    }

    if (tShift < sTime_TShiftMin() || tShift > TShiftMax)
    {
        LIMITATION(tShift, tShift, sTime_TShiftMin(), TShiftMax);
        Display::ShowWarningBad(Warning::LimitSweep_TShift);
    }

    TSHIFT = (int16)tShift;
    ReaderFPGA::ClearData();

    TShift::Load();
    Display::Redraw();
};


void PeackDetMode::Set(PeackDetMode::E peackDetMode)
{
    PEAKDET = peackDetMode;
    RegUPR::Load();
}


void ModeCouple::Set(const Channel &ch, ModeCouple::E modeCoupe)
{
    SET_COUPLE(ch) = modeCoupe;
    FPGA::BUS::SetAttribChannelsAndTrig(ch == ChA ? TypeWriteAnalog::ChanParam0 : TypeWriteAnalog::ChanParam1);
    RShift::Set(ch, SET_RSHIFT(ch));
}


bool Channel::IsEnabled() const
{
    if (value == Channel::Math)
    {
        return !DISABLED_DRAW_MATH;
    }
    if (value == Channel::A_B)
    {
        return false;
    }
    return SET_ENABLED(value);
}


int TPos::InPoints(PeackDetMode::E peakDet, int numPoints, TPos::E tPos)
{
    if (peakDet == PeackDetMode::Disable)
    {
        static const int m[3][3] =
        {
            {0, 140, 280},
            {0, 255, 511},
            {0, 512, 1022}
        };
        return m[sMemory_IntNumPoints2FPGA_NUM_POINTS(numPoints)][tPos];
    }
    else
    {
        static const int m[3][3] =
        {
            {0, 140, 280},
            {0, 256, 510},
            {0, 256, 510}
        };
        return m[sMemory_IntNumPoints2FPGA_NUM_POINTS(numPoints)][tPos];
    }
}
