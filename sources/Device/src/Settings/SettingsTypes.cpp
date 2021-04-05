#include "defines.h"
#include "common/Log_.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Hardware/Timer_.h"
#include "common/Settings/SettingsTypes_.h"
#include "common/Utils/Math_.h"
#include "Display/Display.h"
#include "Display/Warnings.h"
#include "Display/Screen/Grid.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Math.h"
#include "FPGA/FPGA_Reader.h"
#include "FPGA/FPGA_Types.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


bool RShift::show_level[NumChannels] = { false, false };
bool RShift::draw_markers = false;

bool TrigLev::need_auto_find = false;
bool TrigLev::exist_impulse = false;
bool TrigLev::show_level = false;

TBase::E TBase::MIN_P2P = TBase::_20ms;
TBase::E TBase::MIN_PEAK_DET = TBase::_500ns;
TBase::E TBase::MAX_RANOMIZE = TBase::_50ns;

int LaunchFPGA::pred = 0;
int LaunchFPGA::post = 0;

Channel ChA(Channel::A);
Channel ChB(Channel::B);


int Divider::ToAbs(Divider::E multiplier)
{
    static const int results[2] = { 1, 10 };

    return results[multiplier];
}


int Divider::ToAbs(const Channel &ch)
{
    return ToAbs(set.chan[ch].divider);
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

    if (rShift > RShift::MAX || rShift < RShift::MIN)
    {
        Warnings::ShowWarningBad(ch == ChA ? Warning::LimitChan1_RShift : Warning::LimitChan2_RShift);
    }

    Math::Limitation(&rShift, RShift::MIN, RShift::MAX);

    if (rShift > RShift::ZERO)
    {
        rShift &= 0xfffe;                                            // Делаем кратным двум, т.к. у нас 800 значений на 400 точек
    }
    else if (rShift < RShift::ZERO)
    {
        rShift = (rShift + 1) & 0xfffe;
    }

    set.chan[ch].rshift = rShift;

    RShift::Load(ch);

    OnChanged(ch);
};


int16 RShift::Get(const Channel &ch)
{
    return set.chan[ch].rshift;
}


int16 RShift::GetMath()
{
    return set.math.rShift;
}


void TBase::Decrease()
{
    if (PeackDetMode::IsEnabled() && TBase::Get() <= TBase::MIN_PEAK_DET)
    {
        Warnings::ShowWarningBad(Warning::LimitSweep_Time);
        Warnings::ShowWarningBad(Warning::EnabledPeakDet);
        return;
    }

    if ((int)TBase::Get() > 0)
    {
        if (PageTime::InSelfRecoredMode() && TBase::Get() == TBase::MIN_P2P)
        {
            Warnings::ShowWarningBad(Warning::TooFastScanForSelfRecorder);
        }
        else
        {
            E base = (E)((int)TBase::Get() - 1);
            Set(base);
        }
    }
    else
    {
        Warnings::ShowWarningBad(Warning::LimitSweep_Time);
    }
}


void TBase::Increase()
{
    if (TBase::Get() < (TBase::Count - 1))
    {
        E base = (E)(TBase::Get() + 1);
        Set(base);
    }
    else
    {
        Warnings::ShowWarningBad(Warning::LimitSweep_Time);
    }
}


bool TBase::IsRandomize()
{
    return (set.time.base <= MAX_RANOMIZE);
}


bool TBase::IsP2P()
{
    return (set.time.base >= MIN_P2P);
}


void TBase::Set(TBase::E tBase)
{
    if (!ChA.IsEnabled() && !ChB.IsEnabled())
    {
        return;
    }
    if (tBase < TBase::Count)
    {
        float tShiftAbsOld = TShift::ToAbs(TShift::Get(), TBase::Get());
        set.time.base = tBase;
        Load();
        TShift::Set((int)(TShift::ToRel(tShiftAbsOld, TBase::Get())));
        Display::Redraw();
    }
    else
    {
        Warnings::ShowWarningBad(Warning::LimitSweep_Time);
    }
};


TBase::E TBase::Get()
{
    return set.time.base;
}


void TShift::Set(int tShift)
{
    if (!ChA.IsEnabled() && !ChB.IsEnabled())
    {
        return;
    }

    if (tShift < TShift::Min() || tShift > TShift::MAX)
    {
        Math::Limitation<int>(&tShift, TShift::Min(), TShift::MAX);

        Warnings::ShowWarningBad(Warning::LimitSweep_TShift);
    }

    set.time.shift = (int16)tShift;

    LaunchFPGA::Load();

    Display::Redraw();
};


int16 TShift::Get()
{
    return set.time.shift;
}


void PeackDetMode::Set(PeackDetMode::E peackDetMode)
{
    set.time.peak_det = peackDetMode;
    RegUPR::Load();
}


PeackDetMode::E PeackDetMode::Get()
{
    return set.time.peak_det;
}


bool PeackDetMode::IsEnabled()
{
    return (set.time.peak_det != PeackDetMode::Disable);
}


void ModeCouple::Set(const Channel &ch, ModeCouple::E mode_сouple)
{
    set.chan[ch].mode_сouple = mode_сouple;

    FPGA::BUS::SetAttribChannelsAndTrig(ch == ChA ? TypeWriteAnalog::ChanParam0 : TypeWriteAnalog::ChanParam1);

    RShift::Set(ch, RShift::Get(ch));
}


bool Channel::IsEnabled() const
{
    if (value == Channel::Math)
    {
        return !ModeDrawMath::IsDisabled();
    }
    if (value == Channel::A_B)
    {
        return false;
    }
    return set.chan[value].enable;
}


bool Channel::IsInversed() const
{
    return set.chan[value].inverse;
}


int Channel::ToNumber() const
{
    return (value == A) ? 1 : 2;
}


bool TPos::IsLeft()
{
    return (set.time.t_pos == TPos::Left);
}


bool TPos::IsCenter()
{
    return (set.time.t_pos == TPos::Center);
}


bool TPos::IsRight()
{
    return (set.time.t_pos == TPos::Right);
}


int TPos::InPoints(PeackDetMode::E peak_det, uint num_points, TPos::E tPos)
{
    if (peak_det == PeackDetMode::Disable)
    {
        static const int m[3][3] =
        {
            {0, 140, 280},
            {0, 255, 511},
            {0, 512, 1022}
        };
        return m[EnumPointsFPGA::FromPoints(num_points)][tPos];
    }
    else
    {
        static const int m[3][3] =
        {
            {0, 140, 280},
            {0, 256, 510},
            {0, 256, 510}
        };
        return m[EnumPointsFPGA::FromPoints(num_points)][tPos];
    }
}


int TShift::InPoints(PeackDetMode::E peakDet)
{
    return TShift::Get() * (peakDet == PeackDetMode::Disable ? 2 : 1);
}


int16 TShift::Min()
{
    int16 result = -(int16)set.memory.enum_points_fpga.ToPoints();

    switch (set.time.t_pos)
    {
    case TPos::Left:                    break;
    case TPos::Center:  result /= 2;    break;
    case TPos::Right:   result = 0;     break;
    }

    return result;
}


int16 TShift::Zero()
{
    return -TShift::Min();
}


int16 TShift::GetInMemory()
{
    return set.display.shift_in_memory;
}


void TShift::SetInMemory(int16 shift)
{
    set.display.shift_in_memory = shift;
}


uint EnumPointsFPGA::ToPoints(E v)
{
    static const uint points[Count] =
    {
        281,
        512,
        1024,
        2048,
        4096,
        8192,
        16384
    };

    return points[v];
}


uint EnumPointsFPGA::ToPoints()
{
    return EnumPointsFPGA::ToPoints(value);
}


uint EnumPointsFPGA::BytesInChannel()
{
    uint result = ToPoints();

    if (PeackDetMode::IsEnabled())
    {
        result *= 2;
    }

    return result;
}


uint EnumPointsFPGA::BytesForData()
{
    uint result = 0;

    uint bytes_in_channel = BytesInChannel();

    if (ChA.IsEnabled()) { result += bytes_in_channel; }

    if (ChB.IsEnabled()) { result += bytes_in_channel; }

    return result;
}


EnumPointsFPGA::E EnumPointsFPGA::FromPoints(uint num_points)
{
    struct Struct
    {
        uint num_points;
        E enum_points;
    };

    static const Struct structs[Count] =
    {
        { 8192, _16k },
        { 4096, _8k },
        { 2048, _4k },
        { 1024, _2k },
        { 512,  _1k},
        { 281,  _512 },
        { 1,    _281}
    };

    for (int i = 0; i < Count; i++)
    {
        if (num_points > structs[i].num_points)
        {
            return structs[i].enum_points;
        }
    }

    return EnumPointsFPGA::_16k;
}


SampleType::E SampleType::Get()
{
    return set.time.sample_type;
}


FunctionTime::E FunctionTime::Get()
{
    return set.time.function_time;
}


bool FunctionTime::IsShiftInMemory()
{
    return (set.time.function_time == FunctionTime::ShiftInMemory);
}


ModeLongPressTrig::E ModeLongPressTrig::Get()
{
    return set.trig.modeLongPressTrig;
}


bool ModeLongPressTrig::IsLevel0()
{
    return (set.trig.modeLongPressTrig == ModeLongPressTrig::Level0);
}


TrigModeFind::E TrigModeFind::Get()
{
    return set.trig.modeFind;
}


bool TrigModeFind::IsHand()
{
    return (set.trig.modeFind == TrigModeFind::Hand);
}


ColorScheme::E ColorScheme::Get()
{
    return set.service.colorScheme;
}


bool ColorScheme::IsWhiteLetters()
{
    return (set.service.colorScheme == ColorScheme::WhiteLetters);
}


float MaxDbFFT::GetReal()
{
    static const float db[] = { -40.0F, -60.0F, -80.0F };
    return db[value];
}


ModeDrawMath::E ModeDrawMath::Get()
{
    return set.math.modeDraw;
}


bool ModeDrawMath::IsDisabled()
{
    return (set.math.modeDraw == ModeDrawMath::Disable);
}


bool ModeDrawMath::IsSeparate()
{
    return (set.math.modeDraw == ModeDrawMath::Separate);
}


ModeRegSet::E ModeRegSet::Get()
{
    return set.math.modeRegSet;
}


bool ModeRegSet::IsRange()
{
    return (set.math.modeRegSet == ModeRegSet::Range);
}


bool ModeRegSet::IsRShift()
{
    return (set.math.modeRegSet == ModeRegSet::RShift);
}


BalanceADCtype::E BalanceADCtype::Get()
{
    return set.debug.balance_ADC_type;
}


bool StretchADCtype::IsDisabled()
{
    return (set.debug.stretch_ADC_type == StretchADCtype::Disable);
}


bool StretchADCtype::IsHand()
{
    return (set.debug.stretch_ADC_type == StretchADCtype::Hand);
}


bool StretchADCtype::IsSettings()
{
    return (set.debug.stretch_ADC_type == StretchADCtype::Settings);
}


int ENumMinMax::ToAbs()
{
    return (1 << value);
}


int16 RShift::STEP()
{
    return (int16)(((RShift::MAX - RShift::MIN) / 24) / Grid::Delta());
}


Time TShift::ToAbs(float tshift, TBase::E tbase)
{
    return (MathFPGA::absStepTShift[(tbase)] * (tshift) * 2.0F);
}


float TShift::ToRel(float tshift_abs, TBase::E tbase)
{
    return ((tshift_abs) / MathFPGA::absStepTShift[tbase] / 2.0F);
}


int TBase::StepRand()
{
#define N_KR 100
    static const int steps[] = { N_KR / 1, N_KR / 2, N_KR / 5, N_KR / 10, N_KR / 20, N_KR / 50 };

    return IsRandomize() ? steps[set.time.base] : 1;
}


void RShift::OnChanged(const Channel &ch)
{
    set.display.last_affected_channel = ch.value;

    if (set.display.time_show_levels)
    {
        show_level[ch] = true;

        Timer::Enable(ch.IsA() ? TypeTimer::ShowLevelRShiftA : TypeTimer::ShowLevelRShiftB,
            set.display.time_show_levels * 1000, ch.IsA() ? FuncOnTimerDisableShowA :
            FuncOnTimerDisableShowB);
    };

    Display::Redraw();
};


void RShift::FuncOnTimerDisableShowA()
{
    DisableShowLevel(ChA);
}


void RShift::FuncOnTimerDisableShowB()
{
    DisableShowLevel(ChB);
}


void RShift::DisableShowLevel(const Channel &ch)
{
    static const TypeTimer::E types[NumChannels] = { TypeTimer::ShowLevelRShiftA, TypeTimer::ShowLevelRShiftB };

    show_level[ch] = false;

    Timer::Disable(types[ch]);
}


void LaunchFPGA::Calculate()
{
    // Добавочные смещения по времени для разверёток режима рандомизатора.
    static const int16 timeCompensation[TBase::Count] = { 550, 275, 120, 55, 25, 9, 4, 1 };

    int shift = TShift::Get() - TShift::Min() + timeCompensation[set.time.base];

    post = shift;

    if (TBase::IsRandomize())
    {
        uint k = 0;

        int step = TBase::StepRand();

        if (TPos::IsLeft())
        {
            k = set.memory.enum_points_fpga.ToPoints() % step;
        }
        else if (TPos::IsCenter())
        {
            k = (set.memory.enum_points_fpga.ToPoints() / 2) % step;
        }

        post = (uint16)((2 * post - k) / step);

        FPGA::add_shift = (TShift::Get() * 2) % step;

        if (FPGA::add_shift < 0)
        {
            FPGA::add_shift = step + FPGA::add_shift;
        }
    }

    pred = (int)set.memory.enum_points_fpga.BytesInChannel() / 2 - post;

    Math::LimitBelow(&pred, 0);

    if (shift < 0)
    {
        post = 0;
        FPGA:: add_N_stop = -shift;
    }
    else
    {
        FPGA::add_N_stop = 0;
    }
}
