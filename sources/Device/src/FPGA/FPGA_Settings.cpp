#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "FPGA/FPGA.h"
#include "FPGA/MathFPGA.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"


static const uint8 masksRange[Range::Count] =
{
    BINARY_U8(0000000), //-V2501
    BINARY_U8(1000000), //-V2501
    BINARY_U8(1100000), //-V2501
    BINARY_U8(1010000), //-V2501
    BINARY_U8(1000100), //-V2501
    BINARY_U8(1100100), //-V2501
    BINARY_U8(1010100), //-V2501
    BINARY_U8(1000010), //-V2501
    BINARY_U8(1100010), //-V2501
    BINARY_U8(1010010), //-V2501
    BINARY_U8(1000110), //-V2501
    BINARY_U8(1100110), //-V2501
    BINARY_U8(1010110) //-V2501
};


// ���������� �������� �� ������� ��� ��������� ������ �������������.
static int16 deltaTShift[TBase::Count] = {505, 489, 464, 412, 258};

struct TBaseMaskStruct
{
    uint8 maskNorm;         // �����. ��������� ��� ������ � ���������� ����� ��� ����������� ������ �������� ���������.
    uint8 maskPeackDet;     // �����. ��������� ��� ������ � ���������� ����� ��� ���������� ������ �������� ���������.
};

static const TBaseMaskStruct masksTBase[TBase::Count] =
{
    {BINARY_U8(00000000), BINARY_U8(00000000)}, //-V2501
    {BINARY_U8(00000000), BINARY_U8(00000000)}, //-V2501
    {BINARY_U8(00000000), BINARY_U8(00000000)}, //-V2501
    {BINARY_U8(00000000), BINARY_U8(00000000)}, //-V2501
    {BINARY_U8(00000000), BINARY_U8(00000000)}, //-V2501
    {BINARY_U8(00000000), BINARY_U8(00000000)}, //-V2501
    {BINARY_U8(00100010), BINARY_U8(00000000)}, //-V2501
    {BINARY_U8(00100001), BINARY_U8(00100011)}, //-V2501
    {BINARY_U8(01000000), BINARY_U8(00100001)}, //-V2501
    {BINARY_U8(01000010), BINARY_U8(01000000)}, //-V2501
    {BINARY_U8(01000001), BINARY_U8(01000011)}, //-V2501
    {BINARY_U8(01000100), BINARY_U8(01000001)}, //-V2501
    {BINARY_U8(01000110), BINARY_U8(01000100)}, //-V2501
    {BINARY_U8(01000101), BINARY_U8(01000111)}, //-V2501
    {BINARY_U8(01001000), BINARY_U8(01000101)}, //-V2501
    {BINARY_U8(01001010), BINARY_U8(01001000)}, //-V2501
    {BINARY_U8(01001001), BINARY_U8(01001011)}, //-V2501
    {BINARY_U8(01001100), BINARY_U8(01001001)}, //-V2501
    {BINARY_U8(01001110), BINARY_U8(01001100)}, //-V2501
    {BINARY_U8(01001101), BINARY_U8(01001111)}, //-V2501
    {BINARY_U8(01010000), BINARY_U8(01001101)}, //-V2501
    {BINARY_U8(01010010), BINARY_U8(01010000)}, //-V2501
    {BINARY_U8(01010001), BINARY_U8(01010011)}, //-V2501
    {BINARY_U8(01010100), BINARY_U8(01010001)}, //-V2501
    {BINARY_U8(01010110), BINARY_U8(01010100)}, //-V2501
    {BINARY_U8(01010101), BINARY_U8(01010111)}, //-V2501
    {BINARY_U8(01011000), BINARY_U8(01010101)}, //-V2501
    {BINARY_U8(01011010), BINARY_U8(01011000)}, //-V2501
    {BINARY_U8(01011001), BINARY_U8(01011011)}, //-V2501
    {BINARY_U8(01011100), BINARY_U8(01011001)} //-V2501
};



void FPGA::LoadSettings()
{
    LoadKoeffCalibration(Channel::A);
    LoadKoeffCalibration(Channel::B);
    SetAttribChannelsAndTrig(TypeWriteAnalog::All);
    LoadTBase();
    LoadTShift();
    LoadRange(Channel::A);
    LoadRShift(Channel::A);
    LoadRange(Channel::B);
    LoadRShift(Channel::B);
    LoadTrigLev();
    LoadTrigPolarity();
    LoadRegUPR();

    switch(BALANCE_ADC_TYPE) 
    {
        case BalanceADCtype::Settings:
            WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)SET_BALANCE_ADC_A, false); //-V2563
            WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)SET_BALANCE_ADC_B, false); //-V2563
            break;
        case BalanceADCtype::Hand:
            SetPeackDetMode(PEAKDET);
            SetTBase(SET_TBASE);
            if (PEAKDET)
            {
                WriteToHardware(WR_ADD_RSHIFT_DAC1, 3, false);     // ������-�� ��� ������� ��������� �������� ����������. ��� ��� � ������������. //-V2563
                WriteToHardware(WR_ADD_RSHIFT_DAC2, 3, false); //-V2563
            }
            else
            {
                WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)BALANCE_ADC_A, false); //-V2563
                WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)BALANCE_ADC_B, false); //-V2563
            }
            break;
        case BalanceADCtype::Disable:
            break;
    }
}


void FPGA::SetAttribChannelsAndTrig(TypeWriteAnalog::E type) 
{
    uint data = 0;

    /*
    b0...b7 - Channel1
    b8...b15 - Channel2
    b16...b23 - Trig
    ������������ ���� ������ ������� � b0
    */

    // Range0, Range1
    data |= masksRange[SET_RANGE_A];
    data |= (masksRange[SET_RANGE_B] << 8);

    // ��������� �������
    static const uint maskCouple[2][3] = 
    {
        {0x0008, 0x0000, 0x0030},
        {0x0800, 0x0000, 0x3000}
    };

    data |= maskCouple[Channel::A][SET_COUPLE_A];
    data |= maskCouple[Channel::B][SET_COUPLE_B];

    static const uint maskFiltr[2][2] = 
    {
        {0x0000, 0x0080},
        {0x0000, 0x0100}
    };

    data |= maskFiltr[Channel::A][SET_FILTR_A];
    data |= maskFiltr[Channel::B][SET_FILTR_B];


    // ��������� �������������
    static const uint maskSource[3] = {0x000000, 0x800000, 0x400000};
    data |= maskSource[TRIG_SOURCE];

    static const uint maskInput[4] = {0x000000, 0x030000, 0x020000, 0x010000};
    data |= maskInput[TRIG_INPUT];

    WriteToAnalog(type, data);
}


void FPGA::SetRange(Channel::E chan, Range::E range)
{
    if (!sChannel_Enabled(chan))
    {
        return;
    }
    if (range < Range::Count)
    {
        sChannel_SetRange(chan, range);
        if (LINKING_RSHIFT_IS_VOLTAGE)
        {
            float rShiftAbs = RSHIFT_2_ABS(SET_RSHIFT(chan), SET_RANGE(chan));
            float trigLevAbs = RSHIFT_2_ABS(TRIG_LEVEL(chan), SET_RANGE(chan));
            SET_RSHIFT(chan) = (int16)MathFPGA::RShift2Rel(rShiftAbs, range);
            TRIG_LEVEL(chan) = (int16)MathFPGA::RShift2Rel(trigLevAbs, range);
        }
        LoadRange(chan);
    }
    else
    {
        Display::ShowWarningBad(chan == Channel::A ? Warning::LimitChan1_Volts : Warning::LimitChan2_Volts);
    }
};


void FPGA::LoadRange(Channel::E chan)
{
    SetAttribChannelsAndTrig(TypeWriteAnalog::Range0);
    LoadRShift(chan);
    if (chan == (Channel::E)TRIG_SOURCE)
    {
        LoadTrigLev();
    }
}


void FPGA::SetTBase(TBase::E tBase)
{
    if (!sChannel_Enabled(Channel::A) && !sChannel_Enabled(Channel::B))
    {
        return;
    }
    if (tBase < TBase::Count)
    {
        float tShiftAbsOld = TSHIFT_2_ABS(TSHIFT, SET_TBASE); //-V2564
        sTime_SetTBase(tBase);
        LoadTBase();
        FPGA::SetTShift(static_cast<int>(TSHIFT_2_REL(tShiftAbsOld, SET_TBASE)));
        Display::Redraw();
    }
    else
    {
        Display::ShowWarningBad(Warning::LimitSweep_Time);
    }
};


void FPGA::LoadTBase()
{
    TBase::E tBase = SET_TBASE;
    uint8 mask = PEAKDET ? masksTBase[tBase].maskPeackDet : masksTBase[tBase].maskNorm;
    FPGA::WriteToHardware(WR_RAZVERTKA, mask, true); //-V2563
    ADD_SHIFT_T0 = deltaTShift[tBase];
}


void FPGA::TBaseDecrease()
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
            TBase::E base = (TBase::E)((int)SET_TBASE - 1);
            FPGA::SetTBase(base);
        }
    }
    else
    {
        Display::ShowWarningBad(Warning::LimitSweep_Time);
    }
}


void FPGA::TBaseIncrease()
{
    if (SET_TBASE < (TBase::Count - 1))
    {
        TBase::E base = (TBase::E)(SET_TBASE + 1);
        FPGA::SetTBase(base);
    }
    else
    {
        Display::ShowWarningBad(Warning::LimitSweep_Time);
    }
}


void FPGA::SetRShift(Channel::E chan, int16 rShift)
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
        rShift &= 0xfffe;                                            // ������ ������� ����, �.�. � ��� 800 �������� �� 400 �����
    }
    else if (rShift < RShiftZero)
    {
        rShift = (rShift + 1) & 0xfffe;
    }
    SET_RSHIFT(chan) = rShift;
    LoadRShift(chan);
    Display::RotateRShift(chan);
};


void FPGA::LoadRShift(Channel::E chan)
{
    static const uint16 mask[2] = {0x2000, 0x6000};

    Range::E range = SET_RANGE(chan);
    ModeCouple::E mode = SET_COUPLE(chan);
    static const int index[3] = {0, 1, 1};
    int16 rShiftAdd = RSHIFT_ADD(chan, range, index[mode]);

    uint16 rShift = (uint16)(SET_RSHIFT(chan) + (SET_INVERSE(chan) ? -1 : 1) * rShiftAdd);

    int16 delta = -(rShift - RShiftZero);
    if (SET_INVERSE(chan))
    {
        delta = -delta;
    }
    rShift = (uint16)(delta + RShiftZero);

    rShift = (uint16)(RShiftMax + RShiftMin - rShift);
    WriteToDAC(chan == Channel::A ? TypeWriteDAC::RShiftA : TypeWriteDAC::RShiftB, (uint16)(mask[chan] | (rShift << 2)));
}


void FPGA::SetTrigLev(TrigSource::E chan, int16 trigLev)
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

    if (TRIG_LEVEL(chan) != trigLev)
    {
        TRIG_LEVEL(chan) = trigLev;
        LoadTrigLev();
        Display::RotateTrigLev();
    }
};


void FPGA::LoadTrigLev()
{
    uint16 data = 0xa000;
    uint16 trigLev = (uint16)TRIG_LEVEL_SOURCE;
    trigLev = (uint16)(TrigLevMax + TrigLevMin - trigLev);
    data |= trigLev << 2;
    // FPGA_WriteToHardware(WR_DAC_LOW, data.byte[0], true);
    // FPGA_WriteToHardware(WR_DAC_HI, data.byte[1], true);
    WriteToDAC(TypeWriteDAC::TrigLev, data);
}


void FPGA::SetTShift(int tShift)
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
    LoadTShift();
    Display::Redraw();
};


void FPGA::SetDeltaTShift(int16 shift)
{
    deltaTShift[SET_TBASE] = shift;
    LoadTShift();
}


void FPGA::SetPeackDetMode(PeackDetMode::E peackDetMode)
{
    PEAKDET = peackDetMode;
    LoadRegUPR();
}


void FPGA::SetCalibratorMode(CalibratorMode::E calibratorMode)
{
    CALIBRATOR = calibratorMode;
    LoadRegUPR();
}


void FPGA::LoadRegUPR()
{
    uint8 data = 0;
    if (sTime_RandomizeModeEnabled())
    {
        _SET_BIT(data, 0);
    }
    if (!PEAKDET_IS_DISABLE)
    {
        _SET_BIT(data, 1);
    }
    if (CALIBRATOR_IS_FREQ)
    {
        _SET_BIT(data, 2);
    }
    else if (CALIBRATOR_IS_DC)
    {
        _SET_BIT(data, 3);
    }

    FPGA::WriteToHardware(WR_UPR, data, true); //-V2563
}


void FPGA::LoadKoeffCalibration(Channel::E chan)
{
    FPGA::WriteToHardware(chan == Channel::A ? WR_CAL_A : WR_CAL_B, static_cast<uint8>(STRETCH_ADC(chan) * 0x80), false); //-V2563 //-V2564
}


void FPGA::LoadTShift()
{
    static const int16 k[TBase::Count] = {50, 20, 10, 5, 2};
    int16 tShift = TSHIFT - sTime_TShiftMin() + 1;
    int16 tShiftOld = tShift;
    TBase::E tBase = SET_TBASE;
    if (tBase < TBase::_100ns)
    {
        tShift = tShift / k[tBase] + deltaTShift[tBase];
    }
    
    int additionShift = (k[tBase] == 0) ? 0 : ((tShiftOld % k[tBase]) * 2);

    FPGA::SetAdditionShift(additionShift);
    uint16 post = (uint16)tShift;
    post = (uint16)(~post);
    WriteToHardware(WR_POST_LOW, (uint8)post, true); //-V2563
    WriteToHardware(WR_POST_HI, (uint8)(post >> 8), true); //-V2563
    uint16 pred = (uint16)((tShift > 511) ? 1023 : (511 - post));
    pred = (uint16)((~(pred - 1)) & 0x1ff);
    WriteToHardware(WR_PRED_LOW, (uint8)pred, true); //-V2563
    WriteToHardware(WR_PRED_HI, (uint8)(pred >> 8), true); //-V2563
}


const char *FPGA::GetTShiftString(int16 tShiftRel, char buffer[20])
{
    float tShiftVal = TSHIFT_2_ABS(tShiftRel, SET_TBASE); //-V2564
    return Time2String(tShiftVal, true, buffer);
}


bool FPGA::RangeIncrease(Channel::E chan)
{
    bool retValue = false;
    if (SET_RANGE(chan) < Range::Count - 1)
    {
        SetRange(chan, (Range::E)(SET_RANGE(chan) + 1));
        retValue = true;
    }
    else
    {
       Display::ShowWarningBad(chan == Channel::A ? Warning::LimitChan1_Volts : Warning::LimitChan2_Volts);
    }
    Display::Redraw();
    return retValue;
};


bool FPGA::RangeDecrease(Channel::E chan)
{
    bool retValue = false;
    if (SET_RANGE(chan) > 0)
    {
        SetRange(chan, (Range::E)(SET_RANGE(chan) - 1));
        retValue = true;
    }
    else
    {
        Display::ShowWarningBad(chan == Channel::A ? Warning::LimitChan1_Volts : Warning::LimitChan2_Volts);
    }
    Display::Redraw();
    return retValue;
};


void FPGA::SetTrigSource(TrigSource::E trigSource)
{
    TRIG_SOURCE = trigSource;
    SetAttribChannelsAndTrig(TypeWriteAnalog::TrigParam);
    if (!TRIG_SOURCE_IS_EXT)
    {
        SetTrigLev(TRIG_SOURCE, TRIG_LEVEL_SOURCE);
    }
}


void FPGA::SetTrigPolarity(TrigPolarity::E polarity)
{
    TRIG_POLARITY = polarity;
    LoadTrigPolarity();
}


void FPGA::LoadTrigPolarity()
{
    WriteToHardware(WR_TRIG_F, TRIG_POLARITY_IS_FRONT ? 0x01U : 0x00U, true); //-V2563
}


void FPGA::SetTrigInput(TrigInput::E trigInput)
{
    TRIG_INPUT = trigInput;
    SetAttribChannelsAndTrig(TypeWriteAnalog::TrigParam);
}


void FPGA::SetModeCouple(Channel::E chan, ModeCouple::E modeCoupe)
{
    SET_COUPLE(chan) = modeCoupe;
    SetAttribChannelsAndTrig(chan == Channel::A ? TypeWriteAnalog::ChanParam0 : TypeWriteAnalog::ChanParam1);
    SetRShift(chan, SET_RSHIFT(chan));
}


void FPGA::EnableChannelFiltr(Channel::E chan, bool enable)
{
    SET_FILTR(chan) = enable;
    SetAttribChannelsAndTrig(chan == Channel::A ? TypeWriteAnalog::ChanParam0 : TypeWriteAnalog::ChanParam1);
}
