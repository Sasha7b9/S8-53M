#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "FPGA/DataSettings.h"
#include "FPGA/FPGA.h"
#include "FPGA/MathFPGA.h"
#include "FPGA/Storage.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"


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


// Добавочные смещения по времени для разверёток режима рандомизатора.
static int16 deltaTShift[TBase::Count] = {505, 489, 464, 412, 258};

/// Добавочные смещения по времени для разверёток режима рандомизатора.
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
    TrigLev::Polarity::Load();
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
                WriteToHardware(WR_ADD_RSHIFT_DAC1, 3, false);     // Почему-то при пиковом детекторе смещение появляется. Вот его и компенсируем.
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


void FPGA::SetAttribChannelsAndTrig(TypeWriteAnalog::E type) 
{
    uint data = 0;

    /*
    b0...b7 - Channel1
    b8...b15 - Channel2
    b16...b23 - Trig
    Передаваться биты должны начиная с b0
    */

    // Range0, Range1
    data |= masksRange[SET_RANGE_A];
    data |= (masksRange[SET_RANGE_B] << 8);

    // Параметры каналов
    static const uint maskCouple[2][3] = 
    {
        {0x0008, 0x0000, 0x0030},
        {0x0800, 0x0000, 0x3000}
    };

    data |= maskCouple[ChA][SET_COUPLE_A];
    data |= maskCouple[ChB][SET_COUPLE_B];

    static const uint maskFiltr[2][2] = 
    {
        {0x0000, 0x0080},
        {0x0000, 0x0100}
    };

    data |= maskFiltr[ChA][SET_FILTR_A];
    data |= maskFiltr[ChB][SET_FILTR_B];


    // Параметры синхронизации
    static const uint maskSource[3] = {0x000000, 0x800000, 0x400000};
    data |= maskSource[TRIG_SOURCE];

    static const uint maskInput[4] = {0x000000, 0x030000, 0x020000, 0x010000};
    data |= maskInput[TRIG_INPUT];

    WriteToAnalog(type, data);
}


void Range::Set(Channel::E chan, Range::E range)
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
        Load(chan);
    }
    else
    {
        Display::ShowWarningBad(chan == ChA ? Warning::LimitChan1_Volts : Warning::LimitChan2_Volts);
    }
};


void Range::Load(Channel::E chan)
{
    FPGA::SetAttribChannelsAndTrig(TypeWriteAnalog::Range0);
    RShift::Load(chan);
    if (chan == (Channel::E)TRIG_SOURCE)
    {
        TrigLev::Load();
    }
}


void TBase::Load()
{
    struct TBaseMaskStruct
    {
        uint8 maskNorm;      // Маска. Требуется для записи в аппаратную часть при выключенном режиме пикового детектора
        uint8 maskPeackDet;  // Маска. Требуется для записи в аппаратную часть при включенном режиме пикового детектора
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

    TBase::E tBase = SET_TBASE;
    uint8 mask = PEAKDET ? masksTBase[tBase].maskPeackDet : masksTBase[tBase].maskNorm;
    FPGA::BUS::Write(TypeRecord::FPGA, WR_RAZV, mask, true);
    ADD_SHIFT_T0 = deltaTShift[tBase];
}


void RShift::Load(Channel::E chan)
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
    FPGA::BUS::WriteToDAC(chan == ChA ? TypeWriteDAC::RShiftA : TypeWriteDAC::RShiftB, (uint16)(mask[chan] | (rShift << 2)));
}


void TrigLev::Set(TrigSource::E chan, int16 trigLev)
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
        TrigLev::Load();
        Display::RotateTrigLev();
    }
};


void TrigLev::Load()
{
    uint16 data = 0xa000;
    uint16 trigLev = (uint16)TRIG_LEVEL_SOURCE;
    trigLev = (uint16)(TrigLevMax + TrigLevMin - trigLev);
    data |= trigLev << 2;
    // FPGA_WriteToHardware(WR_DAC_LOW, data.byte[0], true);
    // FPGA_WriteToHardware(WR_DAC_HI, data.byte[1], true);
    FPGA::BUS::WriteToDAC(TypeWriteDAC::TrigLev, data);
}


void TShift::SetDelta(int16 shift)
{
    deltaTShift[SET_TBASE] = shift;
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

    if (IN_RANDOM_MODE)
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

    if (PEAKDET_IS_ENABLE)
    {
        data |= (1 << UPR_BIT_PEAKDET);
    }

    FPGA::BUS::Write(TypeRecord::FPGA, WR_UPR, data, false);
}


void FPGA::LoadKoeffCalibration(Channel::E /*chan*/)
{
//    FPGA::WriteToHardware(chan == ChA ? WR_CAL_A : WR_CAL_B, static_cast<uint8>(STRETCH_ADC(chan) * 0x80), false);
}


void TShift::Load()
{
    TBase::E tBase = SET_TBASE;
    int tShift = TSHIFT - sTime_TShiftMin() + timeCompensation[tBase];

    gPost = (uint16)tShift;

    if (IN_RANDOM_MODE)
    {
        extern const int Kr[];
        int k = 0;
        if (SET_TPOS_IS_LEFT)
        {
            k = SET_POINTS_IN_CHANNEL % Kr[tBase];
        }
        else if (SET_TPOS_IS_CENTER)
        {
            k = (SET_POINTS_IN_CHANNEL / 2) % Kr[tBase];
        }

        gPost = (uint16)((2 * gPost - k) / Kr[tBase]);

        FPGA::addShiftForFPGA = (TSHIFT * 2) % Kr[tBase];
        if (FPGA::addShiftForFPGA < 0)
        {
            FPGA::addShiftForFPGA = Kr[tBase] + FPGA::addShiftForFPGA;
        }
        gPred = ~(PRETRIGGERED);
    }
    else
    {
        gPred = (int16)NUM_BYTES_SET / 2 - (int16)gPost;

        if (gPred < 0)
        {
            gPred = 0;
        }
        gPred = ~(gPred + 3);
    }

    if (tShift < 0)
    {
        gPost = 0;
        gAddNStop = -tShift;
    }
    else
    {
        gAddNStop = 0;
    }

    gPost = (uint16)(~(gPost + 1));                   // Здесь просто для записи в железо дополняем

    if (!FPGA_IN_PROCESS_OF_READ)
    {
        if (SET_TBASE > 8)
        {
            ++gPost;
            --gPred;
        }
        FPGA::BUS::Write(TypeRecord::FPGA, WR_POST, gPost, true);
        FPGA::BUS::Write(TypeRecord::FPGA, WR_PRED, (uint)gPred, true);
    }
}


const char *FPGA::GetTShiftString(int16 tShiftRel, char buffer[20])
{
    float tShiftVal = TSHIFT_2_ABS(tShiftRel, SET_TBASE);
    return Time2String(tShiftVal, true, buffer);
}


bool Range::Increase(Channel::E chan)
{
    bool retValue = false;
    if (SET_RANGE(chan) < Range::Count - 1)
    {
        Set(chan, (Range::E)(SET_RANGE(chan) + 1));
        retValue = true;
    }
    else
    {
       Display::ShowWarningBad(chan == ChA ? Warning::LimitChan1_Volts : Warning::LimitChan2_Volts);
    }
    Display::Redraw();
    return retValue;
};


bool Range::Decrease(Channel::E chan)
{
    bool retValue = false;
    if (SET_RANGE(chan) > 0)
    {
        Range::Set(chan, (Range::E)(SET_RANGE(chan) - 1));
        retValue = true;
    }
    else
    {
        Display::ShowWarningBad(chan == ChA ? Warning::LimitChan1_Volts : Warning::LimitChan2_Volts);
    }
    Display::Redraw();
    return retValue;
};


void TrigSource::Set(E trigSource)
{
    TRIG_SOURCE = trigSource;
    FPGA::SetAttribChannelsAndTrig(TypeWriteAnalog::TrigParam);
    if (!TRIG_SOURCE_IS_EXT)
    {
        TrigLev::Set(TRIG_SOURCE, TRIG_LEVEL_SOURCE);
    }
}


void TrigPolarity::Set(E polarity)
{
    TRIG_POLARITY = polarity;
    TrigLev::Polarity::Load();
}


void TrigLev::Polarity::Load()
{
    FPGA::BUS::Write(TypeRecord::FPGA, WR_TRIG, TRIG_POLARITY_IS_FRONT ? 1U : 0U, true);
}


void TrigInput::Set(E trigInput)
{
    TRIG_INPUT = trigInput;
    FPGA::SetAttribChannelsAndTrig(TypeWriteAnalog::TrigParam);
}


void ChannelFiltr::Enable(Channel::E chan, bool enable)
{
    SET_FILTR(chan) = enable;
    FPGA::SetAttribChannelsAndTrig(chan == ChA ? TypeWriteAnalog::ChanParam0 : TypeWriteAnalog::ChanParam1);
}


void TrigLev::FindAndSet()
{
    TrigSource::E trigSource = TRIG_SOURCE;
    if (Storage::AllDatas() == 0 || TRIG_SOURCE_IS_EXT)
    {
        return;
    }

    Channel::E chanTrig = static_cast<Channel::E>(trigSource);
    uint8 *data0 = 0;
    uint8 *data1 = 0;
    DataSettings *ds_ = 0;

    Storage::GetDataFromEnd(0, &ds_, &data0, &data1);

    pUCHAR data = (chanTrig == ChA) ? data0 : data1;

    int lastPoint = static_cast<int>(ds_->length1channel) - 1;

    uint8 min = Math::GetMinFromArray(data, 0, lastPoint);
    uint8 max = Math::GetMaxFromArray(data, 0, lastPoint);

    uint8 aveValue = static_cast<uint8>((static_cast<int>(min) + static_cast<int>(max)) / 2);

    static const float scale = (float)(TrigLevMax - TrigLevZero) / (float)(MAX_VALUE - AVE_VALUE) / 2.4F;

    int16 trigLev = static_cast<int16>(TrigLevZero + scale * (static_cast<int>(aveValue) - AVE_VALUE) - (SET_RSHIFT(chanTrig) - RShiftZero));

    TrigLev::Set(trigSource, trigLev);
}
