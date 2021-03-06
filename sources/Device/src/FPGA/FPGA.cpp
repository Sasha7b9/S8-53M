// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Timer_.h"
#include "common/Utils/Math_.h"
#include "Display/PainterData.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_AutoFinder.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/ReaderFPGA.h"
#include "Panel/Panel.h"
#include "Settings/SettingsNRST.h"
#include "Utils/ProcessingSignal.h"
#include <cstring>


FPGA::Flag  FPGA::flag;
bool        FPGA::temporary_pause = false;
bool        FPGA::can_read_data = true;
FPGA::State FPGA::state;

float       FPGA::FreqMeter::freq = 0.0f;

int         FPGA::Randomizer::number_measures_for_gates = 1000;


// ������� ����������, ����� ����� ��������� ��������� ������.
static void OnTimerCanReadData();


void FPGA::Init() 
{
//    Storage::Clear();
    FPGA::LoadSettings();
    FPGA::SetNumSignalsInSec(set.display.enum_fps.NumSignalsInS());
    FPGA::Randomizer::SetNumberMeasuresForGates(setNRST.rand.num_measures_for_gates);
    FPGA::Randomizer::SetNumberMeasuresForGates(setNRST.rand.num_measures_for_gates);
} 


void FPGA::SetNumSignalsInSec(int num_sig_in_sec) 
{
    Timer::Enable(TypeTimer::NumSignalsInSec, (int)(1000.F / num_sig_in_sec), OnTimerCanReadData);
}


void OnTimerCanReadData()
{
    FPGA::can_read_data = 1;
}


void FPGA::Start()
{
    if(TBase::IsP2P())
    {
//        PainterData::ResetP2Ppoints(false);
        Timer::Enable(TypeTimer::P2P, 1, ReaderFPGA::ReadPoint);
    }
    else
    {
        Timer::Disable(TypeTimer::P2P);
//        PainterData::ResetP2Ppoints(true);
    }

    FPGA::BUS::Write(WR_PRED, LaunchFPGA::PredForWrite(), false);

    FPGA::BUS::Write(WR_START, 1, false);

    state.work = StateWorkFPGA::Wait;
}


void FPGA::Stop()
{
    Timer::Disable(TypeTimer::P2P);
    FPGA::BUS::Write(WR_STOP, 1, false);
    state.work = StateWorkFPGA::Stop;
}


static int CalculateNumberCycles()
{
    int result = 1;

    if (TBase::IsRandomize())
    {
        static const int num[] = { 25, 25, 20, 10, 5, 2 };

        result = num[set.time.base];
    }

    return result;
}


void FPGA::Update()
{
    if (state.work.IsStop())
    {
        return;
    }

    flag.Read();

    for (int i = 0; i < CalculateNumberCycles(); i++)
    {
        if (flag.IsPredLaunchReady())
        {
            if (flag.IsTrigReady())
            {
                if (flag.IsDataReady())
                {
                    ReaderFPGA::ReadData();
                    StartIfNeed();
                }
            }
        }
    }
}


void FPGA::StartIfNeed()
{
    if (!set.trig.start_mode.IsSingle())
    {
        Start();
    }
}


StateWorkFPGA &FPGA::CurrentStateWork()
{
    return state.work;
}


bool FPGA::IsRunning()
{
    return !state.work.IsStop();
}


bool FPGA::Randomizer::CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax)
{   
    if(rand < 500 || rand > 3500)
    {
        return false;
    }

    if (Panel::TimePassedAfterLastEvent() < 20)
    {
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

    if(minGate == 0.0F)
    {
        *eMin = min;
        *eMax = max;
        if(numElements < numValues)
        {
//          LOG_WRITE("min %u, max %u, rand %d", *eMin, *eMax, rand);
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
//        LOG_WRITE("��� %.0F ... %.0F, min = %u, max = %u", minGate, maxGate, min, max);
        numElements = 0;
        min = 0xffff;
        max = 0;
    }

    *eMin = (uint16)(minGate); //-V519
    *eMax = (uint16)(maxGate); //-V519

//  LOG_WRITE("min %u, max %u, rand %d", *eMin, *eMax, rand);
    return true;
}


void FPGA::BUS::Write(uint16 * const address, uint16 data, bool restart)
{
    bool is_running = FPGA::IsRunning();

    if (restart)
    {
        Stop();
    }

    HAL_FMC::Write(address, data);

    if(is_running && restart)
    {
        Start();
    }

    led_Trig.Disable();                 // ����� ������ ������� ��������� �������� �������������
}


void FPGA::BUS::WriteWithoutStart(uint16 * const address, uint16 data)
{
    HAL_FMC::Write(address, data);
}


void FPGA::State::Save()
{
    FPGA::state.work_before_calibration = work;
    stored_settings = set;
}


volatile static bool readPeriod = false;    // ������������� � true ���� ��������, ��� ������� ����� ������� �� ��������
                                            // �������


//static void ReadFreq() // ������ �������� ������� ������������ ����� ����, ��� ��� 4 ����� RD_FL ����������� � �������
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

    CalculateTimeReadyPredLaunch();

    RunPostLaunchIfNeed();
}


void FPGA::Flag::CalculateTimeReadyPredLaunch()
{
    if (IsPredLaunchReady())
    {
        if (time_pred_launch_ready == 0)
        {
            time_pred_launch_ready = TIME_MS;
        }
    }
    else
    {
        time_pred_launch_ready = 0;
    }
}


void FPGA::Flag::RunPostLaunchIfNeed()
{
    if (set.trig.start_mode.IsAuto())
    {
        if (IsPredLaunchReady() && !IsTrigReady() && TIME_MS - time_pred_launch_ready > 1000)
        {
            static uint timePrev = 0;

            if (TIME_MS - timePrev >= 100)
            {
                timePrev = TIME_MS;

                TrigPolarity::Switch();
            }
        }
    }
}


float FPGA::FreqMeter::GetFreq()
{
    return freq;
}


bool FPGA::Randomizer::AllPointsRandomizer()
{
//    if(SET_TBASE < TBase::_100ns) 
//    {
//        for(int i = 0; i < 281; i++) 
//        {
//            if(ReaderFPGA::data_a[i] == 0) 
//            {
//                return false;   
//            }
//        }
//    }

    return false;
}


void FPGA::Randomizer::SetNumberMeasuresForGates(int number)
{
    number_measures_for_gates = number;
}


void FPGA::StartAutoFind()
{
    AutoFinderFPGA::auto_find_in_progress = true;
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


#define CLC_HI      Pin::SPI4_CLK.Set();
#define CLC_LOW     Pin::SPI4_CLK.Reset();
#define DATA_SET(x) Pin::SPI4_DAT.Write(x);


void FPGA::BUS::WriteToAnalog(TypeWriteAnalog::E type, uint data)
{
#define STR_VALUE Uint16((uint16)(data)).ToStringBin().c_str()

    if (type == TypeWriteAnalog::RangeA && setNRST.show_registers.rangeA)
    {
        LOG_WRITE("range 0 = %s", STR_VALUE);
    }
    else if (type == TypeWriteAnalog::RangeB && setNRST.show_registers.rangeB)
    {
        LOG_WRITE("range 1 = %s", STR_VALUE);
    }
    else if (type == TypeWriteAnalog::TrigParam && setNRST.show_registers.trigParam)
    {
        LOG_WRITE("�����. �����. = %s", STR_VALUE);
    }
    else if (type == TypeWriteAnalog::ChanParamA && setNRST.show_registers.chanParamA)
    {
        LOG_WRITE("�����. ���. 1 = %s", STR_VALUE);
    }
    else if (type == TypeWriteAnalog::ChanParamB && setNRST.show_registers.chanParamB)
    {
        LOG_WRITE("�����. ���. 2 = %s", STR_VALUE);
    }
    else if (type == TypeWriteAnalog::All && (
            setNRST.show_registers.trigParam ||
            setNRST.show_registers.rangeA ||
            setNRST.show_registers.rangeB ||
            setNRST.show_registers.chanParamA ||
            setNRST.show_registers.chanParamB))
    {
        LOG_WRITE("������ ������ � ���������� ����� = %s", STR_VALUE);
    }

    Pin::SPI4_CS2.Reset();

    for (int i = 23; i >= 0; i--)
    {
        DATA_SET((data & (1 << i)) ? 1 : 0);
        CLC_HI
        CLC_LOW
    } 

    Pin::SPI4_CS2.Set();
}


void FPGA::BUS::WriteToDAC(TypeWriteDAC::E type, uint16 data)
{
#define STR_VALUE Uint16((uint16)(data)).ToStringBin().c_str()

    if (type == TypeWriteDAC::RShiftA && setNRST.show_registers.rShiftA)
    {
        LOG_WRITE("rShift 0 = %s", STR_VALUE);
    }
    else if (type == TypeWriteDAC::RShiftB && setNRST.show_registers.rShiftB)
    {
        LOG_WRITE("rShfit 1 = %s", STR_VALUE);
    }
    else if (type == TypeWriteDAC::TrigLev && setNRST.show_registers.trigLev)
    {
        LOG_WRITE("trigLev = %s", STR_VALUE);
    }

    Pin::SPI4_CS1.Reset();

    for (int i = 15; i >= 0; i--)
    {
        DATA_SET((data & (1 << i)) ? 1 : 0);
        CLC_HI
        CLC_LOW
    }

    Pin::SPI4_CS1.Set();
}


void FPGA::OnPressStartStop()
{

}
