#include "defines.h"
#include "common/Log_.h"
#include "common/Utils/Math_.h"
#include "FPGA/DataSettings.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/Storage.h"
#include "common/Hardware/Timer_.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"
#include "Utils/ProcessingSignal.h"
#include <cstring>


bool             FPGA::in_processing_of_read = false;
int              FPGA::add_shift = 0;
int              FPGA::add_N_stop = 0;
uint16           FPGA::post = 1024;
int16            FPGA::pred = 1024;
FPGA::Flag       FPGA::flag;
StateWorkFPGA::E FPGA::state_work = StateWorkFPGA::Stop;
uint             FPGA::time_start = 0;
bool             FPGA::temporary_pause = false;
bool             FPGA::can_read_data = true;
bool             FPGA::critical_situation = false;
FPGA::State      FPGA::state;

float            FPGA::FreqMeter::freq = 0.0f;

#define N_KR 100
const int        FPGA::Randomizer::Kr[] = { N_KR / 1, N_KR / 2, N_KR / 5, N_KR / 10, N_KR / 20 };
int              FPGA::Randomizer::number_measures_for_gates = 1000;
bool             FPGA::Randomizer::first_�fter_write = false;


// ������� ����������, ����� ����� ��������� ��������� ������.
static void OnTimerCanReadData();


void FPGA::Init() 
{
    Storage::Clear();
    FPGA::LoadSettings();
    FPGA::SetNumSignalsInSec(ENumSignalsInSec::NumSignalsInS());
    FPGA::Randomizer::SetNumberMeasuresForGates(NUM_MEAS_FOR_GATES);
    FPGA::Randomizer::SetNumberMeasuresForGates(NUM_MEAS_FOR_GATES);
} 


void FPGA::SetNumSignalsInSec(int num_sig_in_sec) 
{
    Timer::Enable(TypeTimer::NumSignalsInSec, static_cast<int>(1000.F / num_sig_in_sec), OnTimerCanReadData);
}


void OnTimerCanReadData()
{
    FPGA::can_read_data = 1;
}


void FPGA::Start()
{
    if(SET_TBASE >= MIN_TBASE_P2P)
    {
        Display::ResetP2Ppoints(false);
        Timer::Enable(TypeTimer::P2P, 1, Reader::ReadPoint);
    }
    else
    {
        Timer::Disable(TypeTimer::P2P);
        Display::ResetP2Ppoints(true);
    }

    HAL_FMC::Write(WR_START, 1);

    time_start = TIME_MS;
    state_work = StateWorkFPGA::Wait;
    critical_situation = false;
}


bool FPGA::ProcessingData()
{
    bool retValue = false;

    int num = (sTime_RandomizeModeEnabled() && (!START_MODE_IS_SINGLE) && SAMPLE_TYPE_IS_EQUAL) ?
        Randomizer::Kr[SET_TBASE] :
        1;
    
   for (int i = 0; i < num; i++)
   {
        flag.Read();

        if (critical_situation)
        {
            if (TIME_MS - time_start > 500)
            {
                TrigPolarity::Switch();

                TrigLev::need_auto_find = true;

                critical_situation = false;
            }
            else if (flag.IsTrigReady())
            {
                critical_situation = false;
            }
        }
        else if (flag.IsDataReady())
        {
            if (set.debug.showRegisters.flag)
            {
                char buffer[9];
                LOG_WRITE("���� ���������� %s", GF::Bin2String16(flag.flag, buffer));
            }
            Panel::EnableLEDTrig(true);
            FPGA::Stop(true);
            Reader::Read(true, (num == 1) || (i == num - 1));
            retValue = true;
            if (!START_MODE_IS_SINGLE)
            {
                Start();
                state_work = StateWorkFPGA::Work;
            }
            else
            {
                FPGA::Stop(false);
            }
        }
        else
        {
            if (flag.flag & (1 << 2))
            {
                if (START_MODE_IS_AUTO)
                {
                    critical_situation = true;
                }
                time_start = TIME_MS;
            }
        }
        Panel::EnableLEDTrig(flag.IsTrigReady() ? true : false);
    }

    return retValue;
}


void FPGA::Update()
{
    flag.Read();

    if (flag.IsPredReady())
    {
        LOG_WRITE("���������� �����");

        if (flag.IsTrigReady())
        {
            LOG_WRITE("������� �����");
        }
        else
        {
            LOG_WRITE("������� �� �����");
        }
    }
}


StateWorkFPGA::E FPGA::CurrentStateWork()
{
    return state_work;
}


void FPGA::OnPressStartStop()
{
    if(state_work == StateWorkFPGA::Stop) 
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
    HAL_FMC::Write(WR_STOP, 1);
    state_work = pause ? StateWorkFPGA::Pause : StateWorkFPGA::Stop;
}


bool FPGA::IsRunning()
{
    return state_work != StateWorkFPGA::Stop;
}


bool FPGA::Randomizer::CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax)
{   
    if(first_�fter_write)
    {
        first_�fter_write = false;
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
        minGate = 0.9F * minGate + 0.1F * min;
        maxGate = 0.9F * maxGate + 0.1F * max;
        //LOG_WRITE("%.0F ... %.0F, min = %u, max = %u", minGate, maxGate, min, max);
        numElements = 0;
        min = 0xffff;
        max = 0;
    }
    *eMin = static_cast<uint16>(minGate); //-V519
    *eMax = static_cast<uint16>(maxGate); //-V519

    //LOG_WRITE("min %u, max %u, rand %d", *eMin, *eMax, rand);
    return true;
}


int FPGA::CalculateShift()            // \todo �� ������ ������������ �������
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

    if (!Randomizer::CalculateGate(rand, &min, &max))
    {
        return TShift::NULL_VALUE;
    }
    
    //LOG_WRITE("������ %d - %d", min, max);

    //min += 100;
    //max -= 100;

    if (sTime_RandomizeModeEnabled())
    {
        float tin = static_cast<float>(rand - min) / (max - min) * 10e-9F;
        int retValue = static_cast<int>(tin / 10e-9F * Randomizer::Kr[SET_TBASE]);
        return retValue;
    }

    if (SET_TBASE == TBase::_100ns && rand < (min + max) / 2)
    {
        return 0;
    }

    return -1;  // set.debug.altShift;      \todo ������������ �� ������ ������� ��������������� ��������. �� PageDebug ����� ���������������, 
                                            //����� ����������������� ��� �������������
}


void FPGA::BUS::Write(uint16 * const address, uint16 data)
{
    bool is_running = FPGA::IsRunning();

    Stop(true);

    HAL_FMC::Write(address, data);

    if(is_running)
    {
        Start();
    }

    Panel::EnableLEDTrig(false); // ����� ������ ������� ��������� �������� �������������
}


void FPGA::BUS::WriteWithoutStart(uint16 * const address, uint16 data)
{
    *address = data;
}


void FPGA::Reader::ReadPoint()
{
    FPGA::flag.Read();

    if(FPGA::flag.IsPointReady())
    {
        uint16 dataB1 = *RD_ADC_B;
        uint16 dataB2 = *RD_ADC_B;
        uint16 dataA1 = *RD_ADC_A;
        uint16 dataA2 = *RD_ADC_A;
        Display::AddPoints(dataA2, dataA1, dataB2, dataB1);
    }
}


void FPGA::State::Save()
{
    FPGA::state.state_work_before_calibration = state_work;
    stored_settings = set;
}


void FPGA::State::Restore()
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
    set = stored_settings;
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
    if(state.state_work_before_calibration != StateWorkFPGA::Stop)
    {
        state.state_work_before_calibration = StateWorkFPGA::Stop;
        FPGA::OnPressStartStop();
    }
}


volatile static bool readPeriod = false;     // ������������� � true ���� ��������, ��� ������� ����� ������� �� �������� �������


//static void ReadFreq()                         // ������ �������� ������� ������������ ����� ����, ��� ��� 4 ����� RD_FL ����������� � �������
//{                                           // ����� ������ ������������� ����������� ����� ���� �����
//    BitSet32 freqFPGA = ReadRegFreq();
//
//    if(freqFPGA.word < 1000) 
//    {
//        readPeriod = true;
//    } 
//    else 
//    {
//        float fr = FreqCounterToValue(&freqFPGA);
//        if (fr < prevFreq * 0.9F || fr > prevFreq * 1.1F)
//        {
//            freq = ERROR_VALUE_FLOAT;
//        }
//        else
//        {
//            freq = fr;
//        }
//        prevFreq = fr;
//    }
//}


//void ReadPeriod()
//{
//    BitSet32 periodFPGA = ReadRegPeriod();
//    float fr = PeriodCounterToValue(&periodFPGA);
//    if (fr < prevFreq * 0.9F || fr > prevFreq * 1.1F)
//    {
//        freq = ERROR_VALUE_FLOAT;
//    }
//    else
//    {
//        freq = fr;
//    }
//    prevFreq = fr;
//    readPeriod = false;
//}


void FPGA::Flag::Read()
{
    flag = HAL_FMC::Read(RD_FL);

    CalculateTimePredReady();

    RunPostIfNeed();
}


void FPGA::Flag::CalculateTimePredReady()
{
    if (IsPredReady())
    {
        if (time_pred_ready == 0)
        {
            time_pred_ready = TIME_MS;
        }
    }
    else
    {
        time_pred_ready = 0;
    }
}


void FPGA::Flag::RunPostIfNeed()
{
    if (START_MODE_IS_AUTO)
    {
        if (IsPredReady() && !IsTrigReady() && TIME_MS - time_pred_ready > 1000)
        {
            TrigPolarity::Switch();
        }
    }
}


float FPGA::FreqMeter::GetFreq()
{
    return freq;
}


bool FPGA::Randomizer::AllPointsRandomizer()
{
    if(SET_TBASE < TBase::_100ns) 
    {
        for(int i = 0; i < 281; i++) 
        {
            if(Reader::data_rel_A[i] == 0) 
            {
                return false;   
            }
        }
    }
    return true;
}


void FPGA::InverseDataIsNecessary(Channel::E chan, uint16 *data)
{
    if(SET_INVERSE(chan))
    {
        for (int i = 0; i < FPGA_MAX_POINTS; i++)
        {
            data[i] = (uint8)((int)(2 * AVE_VALUE) - Math::Limitation<uint8>((uint8)data[i], MIN_VALUE, MAX_VALUE));
        }
    }
}


void FPGA::Randomizer::SetNumberMeasuresForGates(int number)
{
    number_measures_for_gates = number;
}


void FPGA::StartAutoFind()
{
    AutoFinder::auto_find_in_progress = true;
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


void StopTemporaryPause()
{
    FPGA::temporary_pause = false;
    Timer::Disable(TypeTimer::TemporaryPauseFPGA);
}


void FPGA::TemporaryPause()
{
    temporary_pause = true;
    Timer::Enable(TypeTimer::TemporaryPauseFPGA, 500, StopTemporaryPause);
}


#define CLC_HI      Pin::G2.Set();
#define CLC_LOW     Pin::G2.Reset();
#define DATA_SET(x) Pin::G3.Write(x);


void FPGA::BUS::WriteToAnalog(TypeWriteAnalog::E type, uint data)
{
    char buffer[19];

    char *str = GF::Bin2String16(static_cast<uint16>(data), buffer);
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

    Pin::G5.Reset();
    for (int i = 23; i >= 0; i--)
    {
        DATA_SET((data & (1 << i)) ? 1 : 0);
        CLC_HI
        CLC_LOW
    }
    Pin::G5.Set();
}


void FPGA::BUS::WriteToDAC(TypeWriteDAC::E type, uint16 data)
{
    char buffer[19];

    if (type == TypeWriteDAC::RShiftA && IS_SHOW_REG_RSHIFT_A)
    {
        LOG_WRITE("rShift 0 = %s", GF::Bin2String16(data, buffer));
    }
    else if (type == TypeWriteDAC::RShiftB && IS_SHOW_REG_RSHIFT_B)
    {
        LOG_WRITE("rShfit 1 = %s", GF::Bin2String16(data, buffer));
    }
    else if (type == TypeWriteDAC::TrigLev && IS_SHOW_REG_TRIGLEV)
    {
        LOG_WRITE("trigLev = %s", GF::Bin2String16(data, buffer));
    }

//    Pin::G7.Reset();

    for (int i = 15; i >= 0; i--)
    {
        DATA_SET((data & (1 << i)) ? 1 : 0);
        CLC_HI
        CLC_LOW
    }

//    Pin::G7.Set();
}
