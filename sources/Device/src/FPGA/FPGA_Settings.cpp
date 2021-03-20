#include "defines.h"
#include "common/Log_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/GlobalFunctions_.h"
#include "common/Utils/Math_.h"
#include "FPGA/DataSettings.h"
#include "FPGA/FPGA.h"
#include "FPGA/MathFPGA.h"
#include "FPGA/Storage.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"
#include <cstring>


uint TrigPolarity::timeSwitch = 0;


static const uint8 masksRange[Range::Count] =
{
    BIN_U8(0000000),
    BIN_U8(1000000),
    BIN_U8(1100000),
    BIN_U8(1010000),
    BIN_U8(1000100),
    BIN_U8(1100100),
    BIN_U8(1010100),
    BIN_U8(1000010),
    BIN_U8(1100010),
    BIN_U8(1010010),
    BIN_U8(1000110),
    BIN_U8(1100110),
    BIN_U8(1010110)
};


// ���������� �������� �� ������� ��� ��������� ������ �������������.
static int16 deltaTShift[TBase::Count] = {505, 489, 464, 412, 258};

/// ���������� �������� �� ������� ��� ��������� ������ �������������.
static int16 timeCompensation[TBase::Count] = { 550, 275, 120, 55, 25, 9, 4, 1 };


void FPGA::LoadSettings()
{
    TBase::Load();
    TShift::Load();
    Range::Load(ChA);
    RShift::Load(ChA);
    Range::Load(ChB);
    RShift::Load(ChB);
    TrigLev::Load();
    TrigPolarity::Load();
    RegUPR::Load();
    TrigLev::Load();

    /*
    LoadKoeffCalibration(ChA);
    LoadKoeffCalibration(ChB);
    SetAttribChannelsAndTrig(TypeWriteAnalog::All);
    LoadTBase();
    LoadTShift();
    LoadRange(ChA);
    LoadRShift(ChA);
    LoadRange(ChB);
    LoadRShift(ChB);
    LoadTrigLev();
    LoadTrigPolarity();
    LoadRegUPR();

    switch(BALANCE_ADC_TYPE) 
    {
        case BalanceADCtype::Settings:
            WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)SET_BALANCE_ADC_A, false);
            WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)SET_BALANCE_ADC_B, false);
            break;
        case BalanceADCtype::Hand:
            SetPeackDetMode(PEAKDET);
            SetTBase(SET_TBASE);
            if (PEAKDET)
            {
                WriteToHardware(WR_ADD_RSHIFT_DAC1, 3, false);     // ������-�� ��� ������� ��������� �������� ����������. ��� ��� � ������������.
                WriteToHardware(WR_ADD_RSHIFT_DAC2, 3, false);
            }
            else
            {
                WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)BALANCE_ADC_A, false);
                WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)BALANCE_ADC_B, false);
            }
            break;
        case BalanceADCtype::Disable:
            break;
    }
    */
}


void FPGA::BUS::SetAttribChannelsAndTrig(TypeWriteAnalog::E type) 
{
    uint data = 0;

    /*
    b0...b7 - Channel1
    b8...b15 - Channel2
    b16...b23 - Trig
    ������������ ���� ������ ������� � b0
    */

    // Range0, Range1
    data |= masksRange[Range::GetA()];
    data |= (masksRange[Range::GetB()] << 8);

    // ��������� �������
    static const uint maskCouple[2][3] = 
    {
        {0x0008, 0x0000, 0x0030},
        {0x0800, 0x0000, 0x3000}
    };

    data |= maskCouple[ChA][ModeCouple::GetA()];
    data |= maskCouple[ChB][ModeCouple::GetB()];

    static const uint maskFiltr[2][2] = 
    {
        {0x0000, 0x0080},
        {0x0000, 0x0100}
    };

    data |= maskFiltr[ChA][SET_FILTR_A];
    data |= maskFiltr[ChB][SET_FILTR_B];


    // ��������� �������������
    static const uint maskSource[3] = {0x000000, 0x800000, 0x400000};
    data |= maskSource[TrigSource::Get()];

    static const uint maskInput[4] = {0x000000, 0x030000, 0x020000, 0x010000};
    data |= maskInput[TrigInput::Get()];

    BUS::WriteToAnalog(type, data);
}


void Range::Set(const Channel &ch, Range::E range)
{
    if (!ch.IsEnabled())
    {
        return;
    }
    if (range < Range::Count)
    {
        set.chan[ch].range = range;

        if (LINKING_RSHIFT_IS_VOLTAGE)
        {
            float rShiftAbs = RSHIFT_2_ABS(RShift::Get(ch), Range::Get(ch));
            float trigLevAbs = RSHIFT_2_ABS(TrigLev::Get((TrigSource::E)ch.value), Range::Get(ch));
            set.chan[ch].rShiftRel = (int16)RShift::ToRel(rShiftAbs, range);
            set.trig.levelRel[(TrigSource::E)ch.value] = (int16)RShift::ToRel(trigLevAbs, range);
        }

        Load(ch);
    }
    else
    {
        Display::ShowWarningBad(ch == ChA ? Warning::LimitChan1_Volts : Warning::LimitChan2_Volts);
    }
};


Range::E Range::Get(const Channel &ch)
{
    return set.chan[ch].range;
}


Range::E Range::GetA()
{
    return set.chan[ChA].range;
}


Range::E Range::GetB()
{
    return set.chan[ChB].range;
}


void Range::Load(const Channel &ch)
{
    FPGA::BUS::SetAttribChannelsAndTrig(TypeWriteAnalog::Range0);
    RShift::Load(ch);
    if (ch == (Channel::E)TrigSource::Get())
    {
        TrigLev::Load();
    }
}


void TBase::Load()
{
    struct TBaseMaskStruct
    {
        uint8 maskNorm;      // �����. ��������� ��� ������ � ���������� ����� ��� ����������� ������ �������� ���������
        uint8 maskPeackDet;  // �����. ��������� ��� ������ � ���������� ����� ��� ���������� ������ �������� ���������
    };

    static const TBaseMaskStruct masksTBase[TBase::Count] =
    {
        {BIN_U8(00000000), BIN_U8(00000000)}, // 1ns
        {BIN_U8(00000000), BIN_U8(00000000)}, // 2ns
        {BIN_U8(00000000), BIN_U8(00000000)}, // 5ns
        {BIN_U8(00000000), BIN_U8(00000000)}, // 10ns
        {BIN_U8(00000000), BIN_U8(00000000)}, // 20ns
        {BIN_U8(00000000), BIN_U8(00000000)}, // 50ns
        {BIN_U8(00100001), BIN_U8(00100001)}, // 100ns
        {BIN_U8(00100010), BIN_U8(00100010)}, // 200ns
        {BIN_U8(00100011), BIN_U8(00100010)}, // 500ns
        {BIN_U8(01000101), BIN_U8(00100011)}, // 1us
        {BIN_U8(01000110), BIN_U8(01000101)}, // 2us
        {BIN_U8(01000111), BIN_U8(01000110)}, // 5us
        {BIN_U8(01001001), BIN_U8(01000111)}, // 10us
        {BIN_U8(01001010), BIN_U8(01001001)}, // 20us
        {BIN_U8(01001011), BIN_U8(01001010)}, // 50us
        {BIN_U8(01001101), BIN_U8(01001011)}, // 100us
        {BIN_U8(01001110), BIN_U8(01001101)}, // 200us
        {BIN_U8(01001111), BIN_U8(01001110)}, // 500us
        {BIN_U8(01010001), BIN_U8(01001111)}, // 1ms
        {BIN_U8(01010010), BIN_U8(01010001)}, // 2ms
        {BIN_U8(01010011), BIN_U8(01010010)}, // 5ms
        {BIN_U8(01010101), BIN_U8(01010011)}, // 10ms
        {BIN_U8(01010110), BIN_U8(01010101)}, // 20ms
        {BIN_U8(01010111), BIN_U8(01010110)}, // 50ms
        {BIN_U8(01011001), BIN_U8(01010111)}, // 100ms
        {BIN_U8(01011010), BIN_U8(01011001)}, // 200ms
        {BIN_U8(01011011), BIN_U8(01011010)}, // 500ms
        {BIN_U8(01011101), BIN_U8(01011011)}, // 1s
        {BIN_U8(01011110), BIN_U8(01011101)}, // 2s
        {BIN_U8(01011111), BIN_U8(01011110)}, // 5s
        {BIN_U8(01111111), BIN_U8(01011111)}  // 10s
    };

    TBase::E tBase = TBase::Get();
    uint8 mask = !PeackDetMode::IsEnabled() ? masksTBase[tBase].maskNorm : masksTBase[tBase].maskPeackDet;

    FPGA::BUS::Write(WR_RAZV, mask);
    ADD_SHIFT_T0 = deltaTShift[tBase];
}


void RShift::Load(const Channel &ch)
{
    static const uint16 mask[2] = {0x2000, 0x6000};

    Range::E range = Range::Get(ch);
    ModeCouple::E mode = ModeCouple::Get(ch);
    static const int index[3] = {0, 1, 1};
    int16 rShiftAdd = RSHIFT_ADD(ch, range, index[mode]);

    uint16 rShift = (uint16)(RShift::Get(ch) + (SET_INVERSE(ch) ? -1 : 1) * rShiftAdd);

    int16 delta = -(rShift - RShiftZero);
    if (SET_INVERSE(ch))
    {
        delta = -delta;
    }
    rShift = (uint16)(delta + RShiftZero);

    rShift = (uint16)(RShiftMax + RShiftMin - rShift);

    FPGA::BUS::WriteToDAC(ch == ChA ? TypeWriteDAC::RShiftA : TypeWriteDAC::RShiftB, (uint16)(mask[ch] | (rShift << 2)));
}


void TrigLev::Set(TrigSource::E ch, int16 trigLev)
{
    Display::ChangedRShiftMarkers();
    if (trigLev < TrigLevMin || trigLev > TrigLevMax)
    {
        Display::ShowWarningBad(Warning::LimitSweep_Level);
    }
    LIMITATION(trigLev, trigLev, TrigLevMin, TrigLevMax);

    if (trigLev > TrigLevZero)
    {
        trigLev &= 0xfffe;
    }
    else if (trigLev < TrigLevZero)
    {
        trigLev = (trigLev + 1) & 0xfffe;
    }

    if (TrigLev::Get(ch) != trigLev)
    {
        set.trig.levelRel[ch] = trigLev;
        TrigLev::Load();
        Display::RotateTrigLev();
    }
};


int16 TrigLev::Get(TrigSource::E ch)
{
    return set.trig.levelRel[ch];
}


int16 TrigLev::GetA()
{
    return set.trig.levelRel[TrigSource::A];
}


int16 TrigLev::GetB()
{
    return set.trig.levelRel[TrigSource::B];
}


int16 TrigLev::Get()
{
    return set.trig.levelRel[set.trig.source];
}


void TrigLev::Load()
{
    uint16 data = 0xa000;
    uint16 trigLev = (uint16)TrigLev::Get();
    trigLev = (uint16)(TrigLevMax + TrigLevMin - trigLev);
    data |= trigLev << 2;
    // FPGA_WriteToHardware(WR_DAC_LOW, data.byte[0], true);
    // FPGA_WriteToHardware(WR_DAC_HI, data.byte[1], true);
    FPGA::BUS::WriteToDAC(TypeWriteDAC::TrigLev, data);
}


void TShift::SetDelta(int16 shift)
{
    deltaTShift[TBase::Get()] = shift;
    Load();
}


void CalibratorMode::Set(CalibratorMode::E calibratorMode)
{
    CALIBRATOR = calibratorMode;
    RegUPR::Load();
}


void RegUPR::Load()
{
    uint16 data = 0;

    if (FPGA::SET::InRandomizeMode())
    {
        _SET_BIT(data, 0);
    }

    const uint16 mask[3] =
    {
        (1 << UPR_BIT_CALIBRATOR_AC_DC),
        (1 << UPR_BIT_CALIBRATOR_VOLTAGE),
        (0)
    };

    data |= mask[CALIBRATOR];

    if (PeackDetMode::IsEnabled())
    {
        data |= (1 << UPR_BIT_PEAKDET);
    }

    FPGA::BUS::Write(WR_UPR, data);
}


void FPGA::Calibrator::LoadKoeff(const Channel & /*chan*/)
{
//    FPGA::WriteToHardware(ch == ChA ? WR_CAL_A : WR_CAL_B, static_cast<uint8>(STRETCH_ADC(ch) * 0x80), false);
}


void TShift::Load()
{
    TBase::E tBase = TBase::Get();
    int tShift = TShift::Get() - TShift::Min() + timeCompensation[tBase];

    FPGA::post = (uint16)tShift;

    if (FPGA::SET::InRandomizeMode())
    {
        uint k = 0;
        if (TPos::IsLeft())
        {
            k = FPGA::SET::PointsInChannel() % FPGA::Randomizer::Kr[tBase];
        }
        else if (TPos::IsCenter())
        {
            k = (FPGA::SET::PointsInChannel() / 2) % FPGA::Randomizer::Kr[tBase];
        }

        FPGA::post = (uint16)((2 * FPGA::post - k) / FPGA::Randomizer::Kr[tBase]);

        FPGA::add_shift = (TShift::Get() * 2) % FPGA::Randomizer::Kr[tBase];

        if (FPGA::add_shift < 0)
        {
            FPGA::add_shift = FPGA::Randomizer::Kr[tBase] + FPGA::add_shift;
        }

        FPGA::pred = ~(PRETRIGGERED);
    }
    else
    {
        FPGA::pred = (int16)FPGA::SET::BytesInChannel() / 2 - (int16)FPGA::post;

        if (FPGA::pred < 0)
        {
            FPGA::pred = 0;
        }

        FPGA::pred = ~(FPGA::pred + 3);
    }

    if (tShift < 0)
    {
        FPGA::post = 0;
        FPGA::add_N_stop = -tShift;
    }
    else
    {
        FPGA::add_N_stop = 0;
    }

    FPGA::post = (uint16)(~(FPGA::post + 1));                   // ����� ������ ��� ������ � ������ ���������

    if (!FPGA::in_processing_of_read)
    {
        if (TBase::Get() > 8)
        {
            ++FPGA::post;
            --FPGA::pred;
        }
        FPGA::BUS::Write(WR_POST, FPGA::post);
        FPGA::BUS::Write(WR_PRED, (uint)FPGA::pred);
    }
}


String TShift::ToString(int16 tshift_rel)
{
    float tShiftVal = TSHIFT_2_ABS(tshift_rel, TBase::Get());

    return GF::Time2String(tShiftVal, true);
}


bool Range::Increase(const Channel &ch)
{
    bool retValue = false;
    if (Range::Get(ch) < Range::Count - 1)
    {
        Set(ch, (Range::E)(Range::Get(ch) + 1));
        retValue = true;
    }
    else
    {
       Display::ShowWarningBad(ch == ChA ? Warning::LimitChan1_Volts : Warning::LimitChan2_Volts);
    }
    Display::Redraw();
    return retValue;
};


bool Range::Decrease(const Channel &ch)
{
    bool retValue = false;
    if (Range::Get(ch) > 0)
    {
        Range::Set(ch, (Range::E)(Range::Get(ch) - 1));
        retValue = true;
    }
    else
    {
        Display::ShowWarningBad(ch == ChA ? Warning::LimitChan1_Volts : Warning::LimitChan2_Volts);
    }
    Display::Redraw();
    return retValue;
};


void TrigSource::Set(E trigSource)
{
    set.trig.source = trigSource;

    FPGA::BUS::SetAttribChannelsAndTrig(TypeWriteAnalog::TrigParam);

    if (!TrigSource::IsExt())
    {
        TrigLev::Set(TrigSource::Get(), TrigLev::Get());
    }
}


TrigSource::E TrigSource::Get()
{
    return set.trig.source;
}


bool TrigSource::IsA()
{
    return (set.trig.source == TrigSource::A);
}


bool TrigSource::IsB()
{
    return (set.trig.source == TrigSource::B);
}


bool TrigSource::IsExt()
{
    return (set.trig.source == TrigSource::Ext);
}


void TrigPolarity::Set(E polarity)
{
    set.trig.polarity = polarity;
    TrigPolarity::Load();
}


TrigPolarity::E TrigPolarity::Get()
{
    return set.trig.polarity;
}


bool TrigPolarity::IsFront()
{
    return (set.trig.polarity == TrigPolarity::Front);
}


void TrigPolarity::Switch()
{
    if (TrigPolarity::IsFront())
    {
        HAL_FMC::Write(WR_TRIG, 0);
        HAL_FMC::Write(WR_TRIG, 1);
    }
    else
    {
        HAL_FMC::Write(WR_TRIG, 1);
        HAL_FMC::Write(WR_TRIG, 0);
    }

    timeSwitch = TIME_MS;

    Panel::EnableLEDTrig(false);
}


void TrigPolarity::Load()
{
    FPGA::BUS::Write(WR_TRIG, TrigPolarity::IsFront() ? 1U : 0U);
}


void TrigInput::Set(E trigInput)
{
    set.trig.input = trigInput;
    FPGA::BUS::SetAttribChannelsAndTrig(TypeWriteAnalog::TrigParam);
}


TrigInput::E TrigInput::Get()
{
    return set.trig.input;
}


bool TrigInput::IsAC()
{
    return (set.trig.input == TrigInput::AC);
}


void ChannelFiltr::Enable(const Channel &ch, bool enable)
{
    SET_FILTR(ch) = enable;
    FPGA::BUS::SetAttribChannelsAndTrig(ch == ChA ? TypeWriteAnalog::ChanParam0 : TypeWriteAnalog::ChanParam1);
}


void TrigLev::FindAndSet()
{
    if (/*Storage::AllDatas() == 0 ||*/ TrigSource::IsExt())
    {
        return;
    }

    TrigSource::E trigSource = TrigSource::Get();

    Channel::E chanTrig = static_cast<Channel::E>(trigSource);
    uint8 *data0 = 0;
    uint8 *data1 = 0;
    DataSettings *ds_ = 0;

//    Storage::GetDataFromEnd(0, &ds_, &data0, &data1);

    puchar data = (chanTrig == ChA) ? data0 : data1;

    uint lastPoint = ds_->BytesInChannel() - 1;

    uint8 min = Math::GetMinFromArray(data, 0, lastPoint);
    uint8 max = Math::GetMaxFromArray(data, 0, lastPoint);

    uint8 aveValue = static_cast<uint8>((static_cast<int>(min) + static_cast<int>(max)) / 2);

    static const float scale = (float)(TrigLevMax - TrigLevZero) / (float)(MAX_VALUE - AVE_VALUE) / 2.4F;

    int16 trigLev = static_cast<int16>(TrigLevZero + scale * (static_cast<int>(aveValue) - AVE_VALUE) -
        (RShift::Get(chanTrig) - RShiftZero));

    TrigLev::Set(trigSource, trigLev);

    need_auto_find = false;
}
