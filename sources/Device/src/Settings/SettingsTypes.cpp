#include "defines.h"
#include "common/Log_.h"
#include "common/Settings/SettingsTypes_.h"
#include "common/Utils/Math_.h"
#include "Display/Display.h"
#include "Display/Grid.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Math.h"
#include "FPGA/FPGA_Reader.h"
#include "FPGA/FPGA_Types.h"
#include "Settings/Settings.h"


bool RShift::show_level_A = false;
bool RShift::show_level_B = false;
bool RShift::draw_markers = false;

bool TrigLev::need_auto_find = false;

TBase::E TBase::MIN_P2P = TBase::_20ms;
TBase::E TBase::MIN_PEAK_DET = TBase::_500ns;


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
        Display::ShowWarningBad(ch == ChA ? Warning::LimitChan1_RShift : Warning::LimitChan2_RShift);
    }

    LIMITATION(rShift, rShift, RShift::MIN, RShift::MAX);

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
    Display::RotateRShift(ch);
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
        Display::ShowWarningBad(Warning::LimitSweep_Time);
        Display::ShowWarningBad(Warning::EnabledPeakDet);
        return;
    }

    if ((int)TBase::Get() > 0)
    {
        if (FPGA::SET::InSelfRecorderMode() && TBase::Get() == TBase::MIN_P2P)
        {
            Display::ShowWarningBad(Warning::TooFastScanForSelfRecorder);
        }
        else
        {
            E base = (E)((int)TBase::Get() - 1);
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
    if (TBase::Get() < (TBase::Count - 1))
    {
        E base = (E)(TBase::Get() + 1);
        Set(base);
    }
    else
    {
        Display::ShowWarningBad(Warning::LimitSweep_Time);
    }
}


bool TBase::IsRandomize()
{
    return (set.time.base < TBase::_100ns);
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
        Display::ShowWarningBad(Warning::LimitSweep_Time);
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
        LIMITATION(tShift, tShift, TShift::Min(), TShift::MAX);
        Display::ShowWarningBad(Warning::LimitSweep_TShift);
    }

    set.time.shift = (int16)tShift;

    TShift::Load();
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


TPos::E TPos::Get()
{
    return set.time.t_pos;
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
        return m[SettingsMemory::IntNumPoints2FPGA_NUM_POINTS(numPoints)][tPos];
    }
    else
    {
        static const int m[3][3] =
        {
            {0, 140, 280},
            {0, 256, 510},
            {0, 256, 510}
        };
        return m[SettingsMemory::IntNumPoints2FPGA_NUM_POINTS(numPoints)][tPos];
    }
}


int TShift::InPoints(PeackDetMode::E peakDet)
{
    return TShift::Get() * (peakDet == PeackDetMode::Disable ? 2 : 1);
}


int16 TShift::Min()
{
    static const int16 m[3][3] = { {-511, -441, -371},
    {-511, -383, -255},
    {-511, -255, 0} };

    ENUM_POINTS_FPGA::E numPoints = FPGA::SET::ENumPointsInChannel();
    if ((int)numPoints < 3)
    {
        return m[numPoints][TPos::Get()];
    }

    LOG_ERROR("");

    return 0;
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


uint ENUM_POINTS_FPGA::ToPoints(E v)
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


StartMode::E StartMode::Get()
{
    return set.trig.start_mode;
}


bool StartMode::IsAuto()
{
    return (set.trig.start_mode == StartMode::Auto);
}


bool StartMode::IsSingle()
{
    return (set.trig.start_mode == StartMode::Single);
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


ModeWork::E ModeWork::Get()
{
    return set.memory.modeWork;
}


bool ModeWork::IsDirect()
{
    return (set.memory.modeWork == ModeWork::Direct);
}


bool ModeWork::IsLatest()
{
    return (set.memory.modeWork == ModeWork::Latest);
}


bool ModeWork::IsMemInt()
{
    return (set.memory.modeWork == ModeWork::MemInt);
}


bool FileNamingMode::IsMask()
{
    return (set.memory.fileNamingMode == FileNamingMode::Mask);
}


bool FileNamingMode::IsHand()
{
    return (set.memory.fileNamingMode == FileNamingMode::Manually);
}


ModeShowIntMem::E ModeShowIntMem::Get()
{
    return set.memory.modeShowIntMem;
}


ModeBtnMemory::E ModeBtnMemory::Get()
{
    return set.memory.modeBtnMemory;
}


bool ModeBtnMemory::IsSave()
{
    return (set.memory.modeBtnMemory == ModeBtnMemory::Save);
}


bool ModeSaveSignal::IsBMP()
{
    return (set.memory.modeSaveSignal == ModeSaveSignal::BMP);
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
