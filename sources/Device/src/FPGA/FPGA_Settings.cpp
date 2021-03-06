// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "Display/Display.h"
#include "Display/Warnings.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Math.h"
#include "FPGA/ReaderFPGA.h"
#include "FPGA/Data/DataSettings.h"
#include "Menu/Pages/Definition.h"
#include "Panel/Panel.h"
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


void FPGA::LoadSettings()
{
    TBase::Load();
    LaunchFPGA::Load();
    Range::Load(ChA);
    RShift::Load(ChA);
    Range::Load(ChB);
    RShift::Load(ChB);
    TrigLev::Load();
    TrigPolarity::Load();
    RegUPR::Load();
    TrigLev::Load();
    PageTime::OnChanged_PeakDet(true);
    TrigSource::Set(set.trig.source);

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
    data |= masksRange[set.chan[Channel::A].range];
    data |= (masksRange[set.chan[Channel::B].range] << 8);

    // ��������� �������
    static const uint maskCouple[2][3] = 
    {
        {0x0008, 0x0000, 0x0030},
        {0x0800, 0x0000, 0x3000}
    };

    data |= maskCouple[ChA][set.chan[Channel::A].mode_couple];
    data |= maskCouple[ChB][set.chan[Channel::B].mode_couple];

    static const uint maskFiltr[2][2] = 
    {
        {0x0000, 0x0080},
        {0x0000, 0x0100}
    };

    data |= maskFiltr[ChA][set.chan[Channel::A].filtr];
    data |= maskFiltr[ChB][set.chan[Channel::B].filtr];


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

        if (set.display.linking_rshift.IsVoltage())
        {
            float rShiftAbs = RShift::ToAbs(set.chan[ch].rshift, set.chan[ch].range);
            float trigLevAbs = RShift::ToAbs(TrigLev::Get((TrigSource::E)ch.value), set.chan[ch].range);
            set.chan[ch].rshift = (int16)RShift::ToRel(rShiftAbs, range);
            set.trig.levelRel[(TrigSource::E)ch.value] = (int16)RShift::ToRel(trigLevAbs, range);
        }

        Load(ch);
    }
    else
    {
        Warnings::ShowWarningBad(ch == ChA ? Warning::LimitChan1_Volts : Warning::LimitChan2_Volts);
    }
};


void Range::Load(const Channel &ch)
{
    FPGA::BUS::SetAttribChannelsAndTrig(TypeWriteAnalog::RangeA);
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

    TBase::E base = set.time.base;
    uint8 mask = !PeackDetMode::IsEnabled() ? masksTBase[base].maskNorm : masksTBase[base].maskPeackDet;

    FPGA::BUS::Write(WR_RAZV, mask);
    setNRST.rand.shift_T0 = deltaTShift[base];
}


void RShift::Load(const Channel &ch)
{
    static const uint16 mask[2] = {0x2000, 0x6000};

    Range::E range = set.chan[ch].range;
    ModeCouple::E mode = set.chan[ch].mode_couple;
    static const int index[3] = {0, 1, 1};
    int16 rShiftAdd = setNRST.chan[ch].AddRShift(range, index[mode]);

    uint16 rShift = (uint16)(set.chan[ch].rshift + (ch.IsInversed() ? -1 : 1) * rShiftAdd);

    int16 delta = -(rShift - RShift::ZERO);
    if (ch.IsInversed())
    {
        delta = -delta;
    }
    rShift = (uint16)(delta + RShift::ZERO);

    rShift = (uint16)(RShift::MAX + RShift::MIN - rShift);

    FPGA::BUS::WriteToDAC
                        (ch == ChA ? TypeWriteDAC::RShiftA : TypeWriteDAC::RShiftB, (uint16)(mask[ch] | (rShift << 2)));
}


void TrigLev::Set(TrigSource::E ch, int16 trigLev)
{
    Display::ChangedRShiftMarkers();
    if (trigLev < TrigLev::MIN || trigLev > TrigLev::MAX)
    {
        Warnings::ShowWarningBad("������ ��������� - �������", "LIMIT SWEEP - LEVEL");
    }

    Math::Limitation(&trigLev, TrigLev::MIN, TrigLev::MAX);

    if (trigLev > TrigLev::ZERO)
    {
        trigLev &= 0xfffe;
    }
    else if (trigLev < TrigLev::ZERO)
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
    trigLev = (uint16)(TrigLev::MAX + TrigLev::MIN - trigLev);
    data |= trigLev << 2;
    // FPGA_WriteToHardware(WR_DAC_LOW, data.byte[0], true);
    // FPGA_WriteToHardware(WR_DAC_HI, data.byte[1], true);
    FPGA::BUS::WriteToDAC(TypeWriteDAC::TrigLev, data);
}


void TShift::SetDelta(int16 shift)
{
    deltaTShift[set.time.base] = shift;

    LaunchFPGA::Load();
}


void CalibratorMode::Set(CalibratorMode::E calibratorMode)
{
    set.service.calibrator = calibratorMode;

    RegUPR::Load();
}


CalibratorMode::E CalibratorMode::Get()
{
    return set.service.calibrator;
}


void RegUPR::Load()
{
    uint16 data = 0;

    if (TBase::IsRandomize())
    {
        _SET_BIT(data, 0);
    }

    const uint16 mask[3] =
    {
        (1 << UPR_BIT_CALIBRATOR_AC_DC),
        (1 << UPR_BIT_CALIBRATOR_VOLTAGE),
        (0)
    };

    data |= mask[CalibratorMode::Get()];

    if (PeackDetMode::IsEnabled())
    {
        data |= (1 << UPR_BIT_PEAKDET);
    }

    FPGA::BUS::Write(WR_UPR, data);
}


void LaunchFPGA::Load()
{
    if (!ReaderFPGA::mutex_read.IsLocked())
    {
        Calculate();

        if (setNRST.show_registers.launch)
        {
            LOG_WRITE("������ %d:%d", pred, post);
        }

        FPGA::BUS::Write(WR_POST, PostForWrite());
        FPGA::BUS::Write(WR_PRED, PredForWrite());
    }
}


String TShift::ToString(int shift)
{
    float abs = TShift::ToAbs(shift, set.time.base);

    return Time(abs).ToText(true);
}


bool Range::Increase(const Channel &ch)
{
    bool result = false;

    if (set.chan[ch].range < Range::Count - 1)
    {
        Set(ch, (Range::E)(set.chan[ch].range + 1));
        result = true;
    }
    else
    {
       Warnings::ShowWarningBad(ch == ChA ? Warning::LimitChan1_Volts : Warning::LimitChan2_Volts);
    }

    Display::Redraw();
 
    return result;
};


bool Range::Decrease(const Channel &ch)
{
    bool result = false;

    if (set.chan[ch].range > 0)
    {
        Range::Set(ch, (Range::E)(set.chan[ch].range - 1));
        result = true;
    }
    else
    {
        Warnings::ShowWarningBad(ch == ChA ? Warning::LimitChan1_Volts : Warning::LimitChan2_Volts);
    }

    Display::Redraw();

    return result;
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


void TrigPolarity::Switch()
{
    static const uint16 value[2][2] =
    {
        {1, 0},
        {0, 1}
    };

    HAL_FMC::Write(WR_TRIG, value[set.trig.polarity][0]);
    HAL_FMC::Write(WR_TRIG, value[set.trig.polarity][1]);

    timeSwitch = TIME_MS;

    led_Trig.Disable();
}


void TrigPolarity::Load()
{
    FPGA::BUS::Write(WR_TRIG, set.trig.polarity.IsFront() ? 0U : 1U);
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
    set.chan[ch].filtr = enable;

    FPGA::BUS::SetAttribChannelsAndTrig(ch == ChA ? TypeWriteAnalog::ChanParamA : TypeWriteAnalog::ChanParamB);
}


void TrigLev::FindAndSet()
{
//    if (/*Storage::AllDatas() == 0 ||*/ TrigSource::IsExt())
//    {
//        return;
//    }
//
//    TrigSource::E trigSource = TrigSource::Get();
//
//    Channel::E chanTrig = (Channel::E)(trigSource);
//    uint8 *data0 = 0;
//    uint8 *data1 = 0;
//    DataSettings *ds_ = 0;
//
////    Storage::GetDataFromEnd(0, &ds_, &data0, &data1);
//
//    puchar data = (chanTrig == ChA) ? data0 : data1;
//
//    uint lastPoint = ds_->BytesInChannel() - 1;
//
//    uint8 min = Math::GetMinFromArray(data, 0, lastPoint);
//    uint8 max = Math::GetMaxFromArray(data, 0, lastPoint);
//
//    uint8 aveValue = (uint8)(((int)(min) + (int)(max)) / 2);
//
//    static const float scale = (float)(TrigLev::MAX - TrigLev::ZERO) / (float)(Value::MAX - Value::AVE) / 2.4F;
//
//    int16 trigLev = (int16)(TrigLev::ZERO + scale * ((int)(aveValue) - Value::AVE) -
//        (set.chan[chanTrig].rshift - RShift::ZERO));
//
//    TrigLev::Set(trigSource, trigLev);
//
//    need_auto_find = false;
}
