#include "FPGA.h"
#include "Storage.h"
#include "FPGA_Types.h"
#include "Menu/Pages/PageMemory.h"
#include "Display/Display.h"
#include "Hardware/Hardware.h"
#include "common/Hardware/HAL/HAL.h"
#include "Panel/Panel.h"
#include "common/Keyboard/Controls.h"
#include "Settings/Settings.h"
#include "Settings/SettingsChannel.h"
#include "Utils/Math.h"
#include "Utils/ProcessingSignal.h"
#include "Utils/Generator.h"
#include "Hardware/Timer.h"
#include "common/Log.h"
#include "Settings/Settings.h"
#include "Utils/Generator.h"
#include <cstring>



#define NULL_TSHIFT 1000000

static float freq = 0.0F;           // �������, ���������� ��������.
static float prevFreq = 0.0F;
static StateWorkFPGA::E stateWork = StateWorkFPGA::Stop;


volatile static int numberMeasuresForGates = 1000;

static int additionShift = 0;
#define n 200
static const int Kr[] = {n / 2, n / 5, n / 10, n / 20, n / 50};

static DataSettings ds;


static uint8 dataRel0[FPGA_MAX_POINTS] = {0};   // ����� ������������ ��� ������ ������ ������� ������.
static uint8 dataRel1[FPGA_MAX_POINTS] = {0};   // ����� ������������ ��� ������ ������ ������� ������.

static Settings storingSettings;                // ����� ����� ��������� ����������� ������ ������ - ��������� ��������� ������ �������
static uint timeStart = 0;

// ������� ����������, ����� ����� ��������� ��������� ������.
static void OnTimerCanReadData();

static void ReadPoint();

static uint8 ReadFlag();

static void ReadFreq();

static void ReadPeriod();


void FPGA::Init(void) 
{
    Storage::Clear();
    FPGA::LoadSettings();
    FPGA::SetNumSignalsInSec(ENumSignalsInSec::NumSignalsInS());
    FPGA::SetNumberMeasuresForGates(NUM_MEAS_FOR_GATES);
    FPGA::SetNumberMeasuresForGates(NUM_MEAS_FOR_GATES);
} 


void FPGA::SetNumSignalsInSec(int numSigInSec) 
{
    Timer::Enable(TypeTimer::NumSignalsInSec, static_cast<int>(1000.F / numSigInSec), OnTimerCanReadData);
}


void OnTimerCanReadData(void)
{
    FPGA_CAN_READ_DATA = 1;
}


void FPGA::Start(void)
{
    if(SET_TBASE >= MIN_TBASE_P2P)
    {
        Display::ResetP2Ppoints(false);
        Timer::Enable(TypeTimer::P2P, 1, ReadPoint);
    }
    else
    {
        Timer::Disable(TypeTimer::P2P);
        Display::ResetP2Ppoints(true);
    }
    HAL_FSMC::Write(WR_START, 1);
    FillDataPointer(&ds);
    timeStart = gTimerMS;
    stateWork = StateWorkFPGA::Wait;
    FPGA_CRITICAL_SITUATION = 0;
}


void FPGA::SwitchingTrig(void)
{
    HAL_FSMC::Write(WR_TRIG_F, TRIG_POLARITY_IS_FRONT ? 0x00U : 0x01U);
    HAL_FSMC::Write(WR_TRIG_F, TRIG_POLARITY_IS_FRONT ? 0x01U : 0x00U);
}


bool FPGA::ProcessingData(void)
{
    bool retValue = false;

    int num = (sTime_RandomizeModeEnabled() && (!START_MODE_IS_SINGLE) && SAMPLE_TYPE_IS_EQUAL) ? Kr[SET_TBASE] : 1;
    
   for (int i = 0; i < num; i++)
   {
        uint8 flag = ReadFlag();
        if (FPGA_CRITICAL_SITUATION)
        {
            if (gTimerMS - timeStart > 500)
            {
                SwitchingTrig();
                TRIG_AUTO_FIND = 1;
                FPGA_CRITICAL_SITUATION = 0;
            }
            else if (_GET_BIT(flag, BIT_TRIG))
            {
                FPGA_CRITICAL_SITUATION = 0;
            }
        }
        else if (_GET_BIT(flag, BIT_DATA_READY))
        {
            if (set.debug.showRegisters.flag)
            {
                char buffer[9];
                LOG_WRITE("���� ���������� %s", Bin2String(flag, buffer));
            }
            Panel::EnableLEDTrig(true);
            FPGA::Stop(true);
            DataRead(_GET_BIT(flag, BIT_SIGN_SHIFT_POINT), (num == 1) || (i == num - 1));
            retValue = true;
            if (!START_MODE_IS_SINGLE)
            {
                Start();
                stateWork = StateWorkFPGA::Work;
            }
            else
            {
                FPGA::Stop(false);
            }
        }
        else
        {
            if (flag & (1 << 2))
            {
                if (START_MODE_IS_AUTO)
                {
                    FPGA_CRITICAL_SITUATION = 1;
                }
                timeStart = gTimerMS;
            }
        }
        Panel::EnableLEDTrig(_GET_BIT(flag, BIT_TRIG) ? true : false);
    }

    return retValue;
}


void FPGA::Update(void)
{
    ReadFlag();

    if (gStateFPGA.needCalibration)              // ���� ����� � ����� ���������� -
    {
        FPGA::ProcedureCalibration();            // ��������� �.
        gStateFPGA.needCalibration = false;
    }
    if (FPGA_TEMPORARY_PAUSE)
    {
        return;
    }

	if(AUTO_FIND_IN_PROGRESS)
    {
		AutoFind();
		return;
	}

    //if(((FPGA_CAN_READ_DATA == 0) && !sTime_RandomizeModeEnabled()) || (stateWork == StateWorkFPGA::Stop))
    if((FPGA_CAN_READ_DATA == 0) || (stateWork == StateWorkFPGA::Stop))
    {
        return;
    }

    if(SET_SELFRECORDER)
    {
        ReadPoint();
    }

    ProcessingData();

    FPGA_CAN_READ_DATA = 0;
}


StateWorkFPGA::E FPGA::CurrentStateWork(void)
{
    return stateWork;
}


void FPGA::OnPressStartStop(void)
{
    if(stateWork == StateWorkFPGA::Stop) 
    {
        FPGA::Start();
    } 
    else
    {
        FPGA::Stop(false);
    } 
}


void FPGA::Stop(bool pause) 
{
    Timer::Disable(TypeTimer::P2P);
    HAL_FSMC::Write(WR_STOP, 1);
    stateWork = pause ? StateWorkFPGA::Pause : StateWorkFPGA::Stop;
}


bool FPGA::IsRunning(void)
{
    return stateWork != StateWorkFPGA::Stop;
}


#define WRITE_AND_OR_INVERSE(addr, data, ch)                                                      \
    if(SET_INVERSE(ch))                                                                   \
    {                                                                                               \
        data = (uint8)((int)(2 * AVE_VALUE) - LimitationUInt8(data, MIN_VALUE, MAX_VALUE));    \
    }                                                                                               \
    *addr = data;

/*
static uint8 InverseIfNecessary(uint8 data, Channel::E chan)
{
    if (set.chan[chan].inverse)  
    {
        return (uint8)((int)(2 * AVE_VALUE) - LimitationUInt8(data, MIN_VALUE, MAX_VALUE));
    }
    return data;
}
*/


void FPGA::ReadRandomizeMode(void)
{
    int Tsm = CalculateShift();
    if (Tsm == NULL_TSHIFT)
    {
        return;
    };

    int step = Kr[SET_TBASE];
    int index = Tsm - step - additionShift;

    if (index < 0)
    {
        index += step;        // WARN
    }

    uint8 *pData0 = &dataRel0[index];
    pUCHAR const pData0Last = &dataRel0[FPGA_MAX_POINTS - 1];
    uint8 *pData1 = &dataRel1[index];
    pUCHAR const pData1Last = &dataRel1[FPGA_MAX_POINTS - 1];

    pUCHAR const first0 = &dataRel0[0];
    pUCHAR const last0 = pData0Last;
    pUCHAR const first1 = &dataRel1[0];
    pUCHAR const last1 = pData1Last;

    int numAve = NUM_AVE_FOR_RAND;

    if (ENumAveraging::NumAverages() > numAve)
    {
        numAve = ENumAveraging::NumAverages();
    }

    int addShiftMem = step / 2;

    if (START_MODE_IS_SINGLE || SAMPLE_TYPE_IS_REAL)
    {
        FPGA::ClearData();
    }

    while (pData0 < &dataRel0[FPGA_MAX_POINTS])
    {
        volatile uint8 data10 = *RD_ADC_B2; //-V2551
        //uint8 data11 = *RD_ADC_B1;
        volatile uint8 data00 = *RD_ADC_A2; //-V2551
        //uint8 data01 = *RD_ADC_A1;

        /*
        if (*pData0 == 0 || numAve == 1 || startMode == StartMode::Single)
        {
        */
            if (pData0 >= first0 && pData0 <= last0)
            {
                WRITE_AND_OR_INVERSE(pData0, data00, Channel::A);
            }

            uint8 *addr = pData0 + addShiftMem;
            if (addr >= first0 && addr <= last0)
            {
//                WRITE_AND_OR_INVERSE(addr, data01, Channel::A);
            }

            if (pData1 >= first1 && pData1 <= last1)
            {
                WRITE_AND_OR_INVERSE(pData1, data10, Channel::B);
            }
            addr = pData1 + addShiftMem;
            if (addr >= first1 && addr <= last1)
            {
//                WRITE_AND_OR_INVERSE(addr, data11, Channel::B);
            }
        /*
        }
        else
        {
            if (pData0 >= first0 && pData0 <= last0)
            {
                *pData0 = (float)(numAve - 1) / (float)(numAve)* (*pData0) + InverseIfNecessary(data00, Channel::A) * 1.0f / (float)numAve;
            }

            uint8 *addr = pData0 + addShiftMem;
            if (addr >= first0 && addr <= last0)
            {
                *addr = (float)(numAve - 1) / (float)(numAve)* (*(pData0 + addShiftMem)) + InverseIfNecessary(data01, Channel::A) * 1.0f / (float)numAve;
            }

            if (pData1 >= first1 && pData1 <= last1)
            {
                *pData1 = (float)(numAve - 1) / (float)(numAve)* (*pData1) + InverseIfNecessary(data10, Channel::B) * 1.0f / (float)numAve;
            }

            addr = pData1 + addShiftMem;

            if (addr >= first1 && addr <= last1)
            {
                *addr = (float)(numAve - 1) / (float)(numAve)* (*(pData1 + addShiftMem)) + InverseIfNecessary(data11, Channel::B) * 1.0f / (float)numAve;
            }
        }
        */

        pData0 += step;
        pData1 += step;
    }

    if (START_MODE_IS_SINGLE || SAMPLE_TYPE_IS_REAL)
    {
        Processing::InterpolationSinX_X(dataRel0, SET_TBASE);
        Processing::InterpolationSinX_X(dataRel1, SET_TBASE);
    }
}


void FPGA::ReadRealMode(bool necessaryShift)
{
    uint8 *p0 = &dataRel0[0];
    uint8 *p1 = &dataRel1[0];
    uint8 *endP = &dataRel0[FPGA_MAX_POINTS];

    if (ds.peakDet != PeackDetMode::Disable)
    {
        uint8 *p0min = p0;
        uint8 *p0max = p0min + 512;
        uint8 *p1min = p1;
        uint8 *p1max = p1min + 512;
        while ((p0max < endP) && (FPGA_IN_PROCESS_READ == 1))
        {
            uint8 data = *RD_ADC_B2;
            *p1max++ = data;
            data = *RD_ADC_B1;
            *p1min++ = data;
            data = *RD_ADC_A2;
            *p0min++ = data;
            data = *RD_ADC_A1;
            *p0max++ = data;
        }
    }
    else
    {
        while ((p0 < endP) && (FPGA_IN_PROCESS_READ == 1))
        {
            *p1++ = *RD_ADC_B2;
            *p1++ = *RD_ADC_B1;
            uint8 data = *RD_ADC_A2;
            *p0++ = data;
            data = *RD_ADC_A1;
            *p0++ = data;
        }

        int shift = 0;
        if (SET_TBASE == TBase::_100ns || SET_TBASE == TBase::_200ns)
        {
            shift = CalculateShift();
        }
        else if (necessaryShift)
        {
            //shift = set.debug.altShift;       WARN ������������ �� ������ ������� ��������������� ��������. �� PageDebug ����� ���������������, ����� �������� ��� �������������
            shift = -1;
        }
        if (shift != 0)
        {
            if (shift < 0)
            {
                shift = -shift;
                for (int i = FPGA_MAX_POINTS - shift - 1; i >= 0; i--)
                {
                    dataRel0[i + shift] = dataRel0[i];
                    dataRel1[i + shift] = dataRel1[i];
                }
            }
            else
            {
                for (int i = shift; i < FPGA_MAX_POINTS; i++)
                {
                    dataRel0[i - shift] = dataRel0[i];
                    dataRel1[i - shift] = dataRel1[i];
                }
            }
        }
    }
}


void FPGA::DataRead(bool necessaryShift, bool saveToStorage) 
{
    Panel::EnableLEDTrig(false);
    FPGA_IN_PROCESS_READ = 1;
    if(static_cast<TBase::E>(ds.tBase) < TBase::_100ns)
    {
        ReadRandomizeMode();
    } 
    else 
    {
        ReadRealMode(necessaryShift);
    }

    static uint prevTime = 0;

    if (saveToStorage || (gTimerMS - prevTime > 500))
    {
        prevTime = gTimerMS;
        if (!sTime_RandomizeModeEnabled())
        {
            InverseDataIsNecessary(Channel::A, dataRel0);
            InverseDataIsNecessary(Channel::B, dataRel1);
        }

        Storage::AddData(dataRel0, dataRel1, ds);

        if (TRIG_MODE_FIND_IS_AUTO && (TRIG_AUTO_FIND == 1))
        {
            FPGA::FindAndSetTrigLevel();
            TRIG_AUTO_FIND = 0;
        }
    }
    FPGA_IN_PROCESS_READ = 0;
}


void FPGA::SetAdditionShift(int shift)
{
    additionShift = shift;
}


bool FPGA::CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax)
{   
    if(FPGA_FIRST_AFTER_WRITE)
    {
        FPGA_FIRST_AFTER_WRITE = 0;
        return false;
    }
    
    if(rand < 500 || rand > 3500)
    {
        LOG_WRITE("������!!! ������� %d", rand);
        return false;
    }
    
    static const int numValues = 1000;

    static float minGate = 0.0F;
    static float maxGate = 0.0F;
    static int numElements = 0;
    static uint16 min = 0xffff;
    static uint16 max = 0;

    numElements++;
    if(rand < min)
    {
        min = rand;
    }
    if(rand > max)
    {
        max = rand;
    }

    if(minGate == 0.0F) //-V550 //-V2550
    {
        *eMin = min;
        *eMax = max;
        if(numElements < numValues)
        {
            //LOG_WRITE("min %u, max %u, rand %d", *eMin, *eMax, rand);
            return true;
        }
        minGate = min;
        maxGate = max;
        numElements = 0;
        min = 0xffff;
        max = 0;
    }

    if(numElements == numValues)
    {
        minGate = 0.9f * minGate + 0.1f * min;
        maxGate = 0.9f * maxGate + 0.1f * max;
        //LOG_WRITE("%.0f ... %.0f, min = %u, max = %u", minGate, maxGate, min, max);
        numElements = 0;
        min = 0xffff;
        max = 0;
    }
    *eMin = static_cast<uint16>(minGate); //-V519
    *eMax = static_cast<uint16>(maxGate); //-V519

    //LOG_WRITE("min %u, max %u, rand %d", *eMin, *eMax, rand);
    return true;
}


int FPGA::CalculateShift(void)            // \todo �� ������ ������������ �������
{
    uint16 rand = HAL_ADC3::GetValue();
    //LOG_WRITE("rand = %d", (int)rand);
    uint16 min = 0;
    uint16 max = 0;

    if (SET_TBASE == TBase::_200ns)
    {
        return rand < 3000 ? 0 : -1;    // set.debug.altShift; \todo ������������ �� ������ ������� ��������������� ��������. �� PageDebug ����� 
                                        // ���������������, ����� ����������������� ��� �������������
    }

    if (!CalculateGate(rand, &min, &max))
    {
        return NULL_TSHIFT;
    }
    
    //LOG_WRITE("������ %d - %d", min, max);

    //min += 100;
    //max -= 100;

    if (sTime_RandomizeModeEnabled())
    {
        float tin = static_cast<float>(rand - min) / (max - min) * 10e-9F;
        int retValue = static_cast<int>(tin / 10e-9F * Kr[SET_TBASE]);
        return retValue;
    }

    if (SET_TBASE == TBase::_100ns && rand < (min + max) / 2)
    {
        return 0;
    }

    return -1;  // set.debug.altShift;      \todo ������������ �� ������ ������� ��������������� ��������. �� PageDebug ����� ���������������, 
                                            //����� ����������������� ��� �������������
}


void FPGA::WriteToHardware(uint8 * const address, uint8 value, bool restart)
{
    FPGA_FIRST_AFTER_WRITE = 1;
    if(restart)
    {
        if(FPGA_IN_PROCESS_READ)
        {
            FPGA::Stop(true);
            FPGA_IN_PROCESS_READ = 0;
            HAL_FSMC::Write(address, value);
            FPGA::Start();
        }
        else
        {
            if(stateWork != StateWorkFPGA::Stop)
            {
                FPGA::Stop(true);
                HAL_FSMC::Write(address, value);
                FPGA::Start();
            }
            else
            {
                HAL_FSMC::Write(address, value);
            }
        }
    }
    else
    {
        HAL_FSMC::Write(address, value);
    }
}


void ReadPoint(void)
{
    if(_GET_BIT(ReadFlag(), BIT_POINT_READY))
    {
        uint8 dataB1 = *RD_ADC_B1;
        uint8 dataB2 = *RD_ADC_B2;
        uint8 dataA1 = *RD_ADC_A1;
        uint8 dataA2 = *RD_ADC_A2;
        Display::AddPoints(dataA2, dataA1, dataB2, dataB1);
    }
}


void FPGA::SaveState(void)
{
    gStateFPGA.stateWorkBeforeCalibration = stateWork;
    storingSettings = set;
}


void FPGA::RestoreState(void)
{
    int16 rShiftAdd[2][Range::Count][2];
    for (int chan = 0; chan < 2; chan++)
    {
        for (int mode = 0; mode < 2; mode++)
        {
            for (int range = 0; range < Range::Count; range++)
            {
                rShiftAdd[chan][range][mode] = RSHIFT_ADD(chan, range, mode);
            }
        }
    }
    set = storingSettings;
    for (int chan = 0; chan < 2; chan++)
    {
        for (int mode = 0; mode < 2; mode++)
        {
            for (int range = 0; range < Range::Count; range++)
            {
                 RSHIFT_ADD(chan, range, mode) = rShiftAdd[chan][range][mode];
            }
        }
    }
    FPGA::LoadSettings();
    if(gStateFPGA.stateWorkBeforeCalibration != StateWorkFPGA::Stop)
    {
        gStateFPGA.stateWorkBeforeCalibration = StateWorkFPGA::Stop;
        FPGA::OnPressStartStop();
    }
}


static bool readPeriod = false;     // ������������� � true ���� ��������, ��� ������� ����� ������� �� �������� �������


static BitSet32 ReadRegFreq(void)
{
    BitSet32 fr;
    fr.byte[0] = HAL_FSMC::Read(RD_ADDR_FREQ_LOW);
    fr.byte[1] = HAL_FSMC::Read(RD_ADDR_FREQ_MID);
    fr.byte[2] = HAL_FSMC::Read(RD_ADDR_FREQ_HI);
    fr.byte[3] = 0;
    return fr;
}


static BitSet32 ReadRegPeriod(void)
{
    BitSet32 period;
    period.byte[0] = HAL_FSMC::Read(RD_ADDR_PERIOD_LOW_LOW);
    period.byte[1] = HAL_FSMC::Read(RD_ADDR_PERIOD_LOW);
    period.byte[2] = HAL_FSMC::Read(RD_ADDR_PERIOD_MID);
    period.byte[3] = HAL_FSMC::Read(RD_ADDR_PERIOD_HI);
    return period;
}


static float FreqCounterToValue(const BitSet32 *fr)
{
    return fr->word * 10.0F;
}


static float PeriodCounterToValue(const BitSet32 *period)
{
    if (period->word == 0)
    {
        return 0.0F;
    }
    return 10e6f / period->word;
}


static void ReadFreq(void)                         // ������ �������� ������� ������������ ����� ����, ��� ��� 4 ����� RD_FL ����������� � �������
{                                           // ����� ������ ������������� ����������� ����� ���� �����
    BitSet32 freqFPGA = ReadRegFreq();

    if(freqFPGA.word < 1000) 
    {
        readPeriod = true;
    } 
    else 
    {
        float fr = FreqCounterToValue(&freqFPGA);
        if (fr < prevFreq * 0.9f || fr > prevFreq * 1.1f)
        {
            freq = ERROR_VALUE_FLOAT;
        }
        else
        {
            freq = fr;
        }
        prevFreq = fr;
    }
}


void ReadPeriod(void)
{
    BitSet32 periodFPGA = ReadRegPeriod();
    float fr = PeriodCounterToValue(&periodFPGA);
    if (fr < prevFreq * 0.9f || fr > prevFreq * 1.1f)
    {
        freq = ERROR_VALUE_FLOAT;
    }
    else
    {
        freq = fr;
    }
    prevFreq = fr;
    readPeriod = false;
}


static uint8 ReadFlag(void)
{
    uint8 flag = HAL_FSMC::Read(RD_FL);
    if(!readPeriod) 
    {
        if(_GET_BIT(flag, BIT_FREQ_READY)) 
        {
            ReadFreq();
        }
    }
    if(readPeriod && (_GET_BIT(flag, BIT_PERIOD_READY) == 1)) 
    {
        ReadPeriod();
    }

    return flag;
}


static float CalculateFreqFromCounterFreq(void)
{
    while (_GET_BIT(HAL_FSMC::Read(RD_FL), BIT_FREQ_READY) == 0) {};
    ReadRegFreq();
    while (_GET_BIT(HAL_FSMC::Read(RD_FL), BIT_FREQ_READY) == 0) {};
    BitSet32 fr = ReadRegFreq();
    if (fr.word >= 5)
    {
        return FreqCounterToValue(&fr);
    }
    return 0.0F;
}


static float CalculateFreqFromCounterPeriod(void)
{
    uint time = gTimerMS;
    while (gTimerMS - time < 1000 && _GET_BIT(HAL_FSMC::Read(RD_FL), BIT_PERIOD_READY) == 0) {};
    ReadRegPeriod();
    time = gTimerMS;
    while (gTimerMS - time < 1000 && _GET_BIT(HAL_FSMC::Read(RD_FL), BIT_PERIOD_READY) == 0) {};
    BitSet32 period = ReadRegPeriod();
    if (period.word > 0 && (gTimerMS - time < 1000))
    {
        return PeriodCounterToValue(&period);
    }
    return 0.0F;
}


float FPGA::GetFreq(void) 
{
    return freq;
}


void FPGA::ClearData(void)
{
    std::memset(dataRel0, 0, FPGA_MAX_POINTS);
    std::memset(dataRel1, 0, FPGA_MAX_POINTS);
}


bool FPGA::AllPointsRandomizer(void)
{
    if(SET_TBASE < TBase::_100ns) 
    {
        for(int i = 0; i < 281; i++) 
        {
            if(dataRel0[i] == 0) 
            {
                return false;   
            }
        }
    }
    return true;
}


void FPGA::InverseDataIsNecessary(Channel::E chan, uint8 *data)
{
    if(SET_INVERSE(chan))
    {
        for (int i = 0; i < FPGA_MAX_POINTS; i++)
        {
            data[i] = (uint8)((int)(2 * AVE_VALUE) - LimitationUInt8(data[i], MIN_VALUE, MAX_VALUE));
        }
    }
}


void FPGA::SetNumberMeasuresForGates(int number)
{
    numberMeasuresForGates = number;
}


void FPGA::StartAutoFind(void)
{
	AUTO_FIND_IN_PROGRESS = 1;
}


uint8 FPGA::CalculateMinWithout0(const uint8 buffer[100])
{
    // \todo �� ����� ���������� ��� ����������� ����, ����� ��� ������� ���������� �������� 0 � 255 � ��������� ������
    // ��� ����� �������� �� ������ ����, ����� ���� ��������.
    uint8 min = 255;
    
    for (int i = 1; i < 100; i++)
    {
        if (buffer[i] > 0 && buffer[i] < min)
        {
            min = buffer[i];
        }
    }
    return min;
}


uint8 FPGA::CalculateMaxWithout255(const uint8 buffer[100])
{
    // \todo �� ����� ���������� ��� ����������� ����, ����� ��� ������� ���������� �������� 0 � 255 � ��������� ������
    // ��� ����� �������� �� ������ ����, ����� ���� ��������.
    uint8 max = 0;
    for(int i = 1; i < 100; i++)
    {
        if(buffer[i] < 255 && buffer[i] > max)
        {
            max = buffer[i];
        }
    }
    return max;
}


TBase::E CalculateTBase(float freq_)
{
    if     (freq_ >= 100e6F)  { return TBase::_2ns;   }
    else if(freq_ >= 40e6F)   { return TBase::_5ns;   }
    else if(freq_ >= 20e6F)   { return TBase::_10ns;  }
    else if(freq_ >= 10e6F)   { return TBase::_20ns;  }
    else if(freq_ >= 3e6F)    { return TBase::_50ns;  }
    else if(freq_ >= 2e6F)    { return TBase::_100ns; }
    else if(freq_ >= 900e3F)  { return TBase::_200ns; }
    else if(freq_ >= 400e3F)  { return TBase::_500ns; }
    else if(freq_ >= 200e3F)  { return TBase::_1us;   }
    else if(freq_ >= 90e3F)   { return TBase::_2us;   }
    else if(freq_ >= 30e3F)   { return TBase::_5us;   }
    else if(freq_ >= 20e3F)   { return TBase::_10us;  }
    else if(freq_ >= 10e3F)   { return TBase::_20us;  }
    else if(freq_ >= 4e3F)    { return TBase::_50us;  }
    else if(freq_ >= 2e3F)    { return TBase::_100us; }
    else if(freq_ >= 1e3F)    { return TBase::_200us; }
    else if(freq_ >= 350.0F) { return TBase::_500us; }
    else if(freq_ >= 200.0F) { return TBase::_1ms;   }
    else if(freq_ >= 100.0F) { return TBase::_2ms;   }
    else if(freq_ >= 40.0F)  { return TBase::_5ms;   }
    else if(freq_ >= 20.0F)  { return TBase::_10ms;  }
    else if(freq_ >= 10.0F)  { return TBase::_20ms;  }
    else if(freq_ >= 4.0F)   { return TBase::_50ms;  }
    else if(freq_ >= 2.0F)   { return TBase::_100ms; }
    return TBase::_200ms;
}


void FPGA::AutoFind(void)
{
    //LOG_WRITE(" ");
    //Timer::StartLogging();

    //LOG_WRITE("����� 1");
    if (!FindWave(Channel::A))
    {
        //LOG_WRITE("����� 2");
        if(!FindWave(Channel::B))
        {
            Display::ShowWarningBad(Warning::SignalNotFound);
        }
    }

    Init();
    Start();
    
    AUTO_FIND_IN_PROGRESS = 0;
}


bool FPGA::FindWave(Channel::E chan)
{
    Settings settings = set;    // ��������� ���������� ���������

    Stop(false);
    SET_ENABLED(chan) = true;
    FPGA::SetTrigSource(static_cast<TrigSource::E>(chan));
    FPGA::SetTrigLev(static_cast<TrigSource::E>(chan), TrigLevZero);
    FPGA::SetRShift(chan, RShiftZero);
    FPGA::SetModeCouple(chan, ModeCouple::AC);
    Range::E range = AccurateFindRange(chan);
    //LOG_WRITE("Range %s", RangeName(range));
    if(range != Range::Count)
    {
        SET_RANGE(chan) = range;
        TBase::E tBase = AccurateFindTBase(chan);
        if (tBase != TBase::Count)
        {
            SET_TBASE = tBase;
            TRIG_SOURCE = static_cast<TrigSource::E>(chan);
            return true;
        }
    }

    set = settings;
    FPGA::LoadSettings();
    return false;
}


Range::E FPGA::AccurateFindRange(Channel::E chan)
{
    /*
    �������� ������.
    ������������� �������� 50 ��.
    ������������� �������� ����.
    �������� ������� ��������.
    1. ������������� �������� 20�/���.
    �� ��������� ��, �������� ��������� ����� � ���������� ������ � ��������� �� 50 ���� - 2 ������ � ���������. �� ��� �����
        �� ������ �������� ���� ����� ������ ������� ������� 10 ��.
    ��������� ��������� ���������� �� ����� �� ������� ������. ���� ����� - ���� ���������� �������� (��������, 2� ������ 1�) � ����������
        � � �������� ����������.
    ���� �� ����� - ���� ��������� �������� � ��������� ���� � 1.
    */

    uint8 buffer[100];  // ���� ����� ��������� �����

    SetTBase(TBase::_50ms);
    FPGA::SetModeCouple(chan, ModeCouple::AC);
    PeackDetMode::E peackDetMode = PEAKDET;
    FPGA::SetPeackDetMode(PeackDetMode::Enable);
    for (int range = Range::Count - 1; range >= 0; range--)
    {
        //Timer::LogPointMS("1");
        FPGA::Stop(false);
        FPGA::SetRange(chan, static_cast<Range::E>(range));
        HAL_TIM2::Delay(10);
        FPGA::Start();

        for (int i = 0; i < 50; i++)
        {
            while (_GET_BIT(HAL_FSMC::Read(RD_FL), BIT_POINT_READY) == 0) {};
            HAL_FSMC::Read(RD_ADC_B2);
            HAL_FSMC::Read(RD_ADC_B1);
            HAL_FSMC::Read(RD_ADC_A2);
            HAL_FSMC::Read(RD_ADC_A1);
        }

        if (chan == Channel::A)
        {
            for (int i = 0; i < 100; i += 2)
            {
                while (_GET_BIT(HAL_FSMC::Read(RD_FL), BIT_POINT_READY) == 0) {};
                HAL_FSMC::Read(RD_ADC_B2);
                HAL_FSMC::Read(RD_ADC_B1);
                buffer[i] = HAL_FSMC::Read(RD_ADC_A2);
                buffer[i + 1] = HAL_FSMC::Read(RD_ADC_A1);
            }
        }
        else
        {
            for (int i = 0; i < 100; i += 2)
            {
                while (_GET_BIT(HAL_FSMC::Read(RD_FL), BIT_POINT_READY) == 0) {};
                buffer[i] = HAL_FSMC::Read(RD_ADC_B2);
                buffer[i + 1] = HAL_FSMC::Read(RD_ADC_B1);
                HAL_FSMC::Read(RD_ADC_A2);
                HAL_FSMC::Read(RD_ADC_A1);
            }
        }

        /*
        if(chan == Channel::A)
        {
            LOG_WRITE("min = %d, max = %d", CalculateMinWithout0(buffer), CalculateMaxWithout255(buffer));
        }
        */

        if (CalculateMinWithout0(buffer) < MIN_VALUE || CalculateMaxWithout255(buffer) > MAX_VALUE)
        {
            if (range < Range::_20V)
            {
                range++;
            }
            FPGA::SetPeackDetMode(peackDetMode);
            return static_cast<Range::E>(range);
        }

        uint8 min = AVE_VALUE - 30;
        uint8 max = AVE_VALUE + 30;

        if(range == Range::_2mV && CalculateMinWithout0(buffer) > min && CalculateMaxWithout255(buffer) < max)
        {
            FPGA::SetPeackDetMode(peackDetMode);
            return Range::Count;
        }
    }
    FPGA::SetPeackDetMode(peackDetMode);
    return Range::Count;
}


TBase::E FPGA::AccurateFindTBase(Channel::E chan)
{
    for (int i = 0; i < 5; i++)
    {
        TBase::E tBase = FindTBase(chan);
        TBase::E secondTBase = FindTBase(chan); //-V656

        if (tBase == secondTBase && tBase != TBase::Count)
        {
            return tBase;
        }
    }
    return TBase::Count;
}


TBase::E FPGA::FindTBase(Channel::E)
{
    SetTrigInput(TrigInput::Full);
    HAL_TIM2::Delay(10);
    FPGA::Stop(false);
    float fr = CalculateFreqFromCounterFreq();

    FPGA::SetTrigInput(fr < 1e6f ? TrigInput::LPF : TrigInput::Full);

    fr = CalculateFreqFromCounterFreq();

    TBase::E tBase = TBase::Count;

    if (fr >= 50.0F)
    {
        tBase = CalculateTBase(fr);
        FPGA::SetTBase(tBase);
        FPGA::Start();
        FPGA::SetTrigInput(fr < 500e3F ? TrigInput::LPF : TrigInput::HPF);
        return tBase;
    }
    else
    {
        FPGA::SetTrigInput(TrigInput::LPF);
        freq = CalculateFreqFromCounterPeriod();
        if (fr > 0.0F)
        {
            tBase = CalculateTBase(fr);
            FPGA::SetTBase(tBase);
            HAL_TIM2::Delay(10);
            FPGA::Start();
            return tBase;
        }
    }

    return TBase::Count;
}


void StopTemporaryPause(void)
{
    FPGA_TEMPORARY_PAUSE = 0;
    Timer::Disable(TypeTimer::TemporaryPauseFPGA);
}


void FPGA::TemporaryPause(void)
{
    FPGA_TEMPORARY_PAUSE = 1;
    Timer::Enable(TypeTimer::TemporaryPauseFPGA, 500, StopTemporaryPause);
}


void FPGA::FillDataPointer(DataSettings *dp)
{
    dp->enableCh0 = sChannel_Enabled(Channel::A) ? 1U : 0U;
    dp->enableCh1 = sChannel_Enabled(Channel::B) ? 1U : 0U;
    dp->inverseCh0 = SET_INVERSE_A ? 1U : 0U;
    dp->inverseCh1 = SET_INVERSE_B ? 1U : 0U;
    dp->range[0] = SET_RANGE_A;
    dp->range[1] = SET_RANGE_B;
    dp->rShiftCh0 = (uint)SET_RSHIFT_A;
    dp->rShiftCh1 = (uint)SET_RSHIFT_B;
    dp->tBase = SET_TBASE;
    dp->tShift = TSHIFT;
    dp->modeCouple0 = SET_COUPLE_A;
    dp->modeCouple1 = SET_COUPLE_B;
    dp->length1channel = static_cast<uint>(sMemory_GetNumPoints(false));
    dp->trigLevCh0 = (uint)TRIG_LEVEL_A;
    dp->trigLevCh1 = (uint)TRIG_LEVEL_B;
    dp->peakDet = (uint)PEAKDET;
    dp->multiplier0 = SET_DIVIDER_A;
    dp->multiplier1 = SET_DIVIDER_B;
}


void FPGA::FindAndSetTrigLevel(void)
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

    pUCHAR data = (chanTrig == Channel::A) ? data0 : data1;

    int lastPoint = static_cast<int>(ds_->length1channel) - 1;

    uint8 min = Math_GetMinFromArray(data, 0, lastPoint);
    uint8 max = Math_GetMaxFromArray(data, 0, lastPoint);

    uint8 aveValue = static_cast<uint8>((static_cast<int>(min) + static_cast<int>(max)) / 2);

    static const float scale = (float)(TrigLevMax - TrigLevZero) / (float)(MAX_VALUE - AVE_VALUE) / 2.4f;

    int16 trigLev = static_cast<int16>(TrigLevZero + scale * (static_cast<int>(aveValue) - AVE_VALUE) - (SET_RSHIFT(chanTrig) - RShiftZero));

    FPGA::SetTrigLev(trigSource, trigLev);
}


#define CLC_HI              pinG2.Set();
#define CLC_LOW             pinG2.Reset();
#define DATA_SET(x)         pinG3.Write(x);


void FPGA::WriteToAnalog(TypeWriteAnalog::E type, uint data)
{
    char buffer[19];

    char *str = Bin2String16(static_cast<uint16>(data), buffer);
    if (type == TypeWriteAnalog::Range0 && IS_SHOW_REG_RANGE_A)
    {
        LOG_WRITE("range 0 = %s", str);
    }
    else if (type == TypeWriteAnalog::Range1 && IS_SHOW_REG_RANGE_B)
    {
        LOG_WRITE("range 1 = %s", str);
    }
    else if (type == TypeWriteAnalog::TrigParam && IS_SHOW_REG_TRIGPARAM)
    {
        LOG_WRITE("�����. �����. = %s", str);
    }
    else if (type == TypeWriteAnalog::ChanParam0 && IS_SHOW_REG_PARAM_A)
    {
        LOG_WRITE("�����. ���. 1 = %s", str);
    }
    else if (type == TypeWriteAnalog::ChanParam1 && IS_SHOW_REG_PARAM_B)
    {
        LOG_WRITE("�����. ���. 2 = %s", str);
    }
    else if (type == TypeWriteAnalog::All && (IS_SHOW_REG_TRIGPARAM || IS_SHOW_REG_RANGE_A || IS_SHOW_REG_RANGE_B || IS_SHOW_REG_PARAM_A || IS_SHOW_REG_PARAM_B)) //-V560 //-V501
    {
        LOG_WRITE("������ ������ � ���������� ����� = %s", str);
    }

    pinG5.Reset();
    for (int i = 23; i >= 0; i--)
    {
        DATA_SET((data & (1 << i)) ? 1 : 0);
        CLC_HI
        CLC_LOW
    }
    pinG5.Set();
}


void FPGA::WriteToDAC(TypeWriteDAC::E type, uint16 data)
{
    char buffer[19];

    if (type == TypeWriteDAC::RShiftA && IS_SHOW_REG_RSHIFT_A)
    {
        LOG_WRITE("rShift 0 = %s", Bin2String16(data, buffer));
    }
    else if (type == TypeWriteDAC::RShiftB && IS_SHOW_REG_RSHIFT_B)
    {
        LOG_WRITE("rShfit 1 = %s", Bin2String16(data, buffer));
    }
    else if (type == TypeWriteDAC::TrigLev && IS_SHOW_REG_TRIGLEV)
    {
        LOG_WRITE("trigLev = %s", Bin2String16(data, buffer));
    }

    pinG7.Reset();
    for (int i = 15; i >= 0; i--)
    {
        DATA_SET((data & (1 << i)) ? 1 : 0);
        CLC_HI
        CLC_LOW
    }
    pinG7.Set();
}
