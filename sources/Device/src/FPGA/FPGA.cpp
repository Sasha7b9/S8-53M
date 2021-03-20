#include "defines.h"
#include "common/Log_.h"
#include "common/Hardware/Timer_.h"
#include "common/Utils/Math_.h"
#include "Display/DataPainter.h"
#include "FPGA/DataSettings.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_AutoFinder.h"
#include "FPGA/FPGA_Reader.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/Storage.h"
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
bool             FPGA::Randomizer::first_аfter_write = false;


// Функция вызывается, когда можно считывать очередной сигнал.
static void OnTimerCanReadData();


void FPGA::Init() 
{
//    Storage::Clear();
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
    if(TBase::Get() >= MIN_TBASE_P2P)
    {
//        DataPainter::ResetP2Ppoints(false);
        Timer::Enable(TypeTimer::P2P, 1, ReaderFPGA::ReadPoint);
    }
    else
    {
        Timer::Disable(TypeTimer::P2P);
//        DataPainter::ResetP2Ppoints(true);
    }

    HAL_FMC::Write(WR_START, 1);

    time_start = TIME_MS;
    state_work = StateWorkFPGA::Wait;
    critical_situation = false;
}


void FPGA::Update()
{
    flag.Read();

    if (flag.IsPredLaunchReady())
    {
        if (flag.IsTrigReady())
        {
            if (flag.IsDataReady())
            {
                ReaderFPGA::ReadData();

                FPGA::Start();
            }
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
    if(first_аfter_write)
    {
        first_аfter_write = false;
        return false;
    }
    
    if(rand < 500 || rand > 3500)
    {
        LOG_WRITE("ОШИБКА!!! считано %d", rand);
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


void FPGA::BUS::Write(uint16 * const address, uint16 data)
{
    bool is_running = FPGA::IsRunning();

    Stop(true);

    HAL_FMC::Write(address, data);

    if(is_running)
    {
        Start();
    }

    Panel::EnableLEDTrig(false); // После каждой засылки выключаем лампочку синхронизации
}


void FPGA::BUS::WriteWithoutStart(uint16 * const address, uint16 data)
{
    *address = data;
}


void FPGA::State::Save()
{
    FPGA::state.state_work_before_calibration = state_work;
    stored_settings = set;
}


void FPGA::State::Restore()
{
    int16 rShiftAdd[2][Range::Count][2];
    for (int ch = 0; ch < 2; ch++)
    {
        for (int mode = 0; mode < 2; mode++)
        {
            for (int range = 0; range < Range::Count; range++)
            {
                rShiftAdd[ch][range][mode] = RSHIFT_ADD(ch, range, mode);
            }
        }
    }
    set = stored_settings;
    for (int ch = 0; ch < 2; ch++)
    {
        for (int mode = 0; mode < 2; mode++)
        {
            for (int range = 0; range < Range::Count; range++)
            {
                 RSHIFT_ADD(ch, range, mode) = rShiftAdd[ch][range][mode];
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


volatile static bool readPeriod = false;    // Установленный в true флаг означает, что частоту нужно считать по счётчику
                                            // периода


//static void ReadFreq() // Чтение счётчика частоты производится после того, как бит 4 флага RD_FL установится в едицину
//{                                           // После чтения автоматически запускается новый цикл счёта
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
    if (START_MODE_IS_AUTO)
    {
        if (IsPredLaunchReady() && !IsTrigReady() && TIME_MS - time_pred_launch_ready > 1000)
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
//    LOG_WRITE("%s : %d", __FUNCTION__, data);

    char *str = GF::Bin2String16(static_cast<uint16>(data)).c_str();

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
        LOG_WRITE("парам. синхр. = %s", str);
    }
    else if (type == TypeWriteAnalog::ChanParam0 && IS_SHOW_REG_PARAM_A)
    {
        LOG_WRITE("парам. кан. 1 = %s", str);
    }
    else if (type == TypeWriteAnalog::ChanParam1 && IS_SHOW_REG_PARAM_B)
    {
        LOG_WRITE("парам. кан. 2 = %s", str);
    }
    else if (type == TypeWriteAnalog::All && (IS_SHOW_REG_TRIGPARAM || IS_SHOW_REG_RANGE_A || IS_SHOW_REG_RANGE_B ||
        IS_SHOW_REG_PARAM_A || IS_SHOW_REG_PARAM_B))
    {
        LOG_WRITE("полная запись в аналоговую часть = %s", str);
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
//    LOG_WRITE("%s : %d", __FUNCTION__, data);

    if (type == TypeWriteDAC::RShiftA && IS_SHOW_REG_RSHIFT_A)
    {
        LOG_WRITE("rShift 0 = %s", GF::Bin2String16(data).c_str());
    }
    else if (type == TypeWriteDAC::RShiftB && IS_SHOW_REG_RSHIFT_B)
    {
        LOG_WRITE("rShfit 1 = %s", GF::Bin2String16(data).c_str());
    }
    else if (type == TypeWriteDAC::TrigLev && IS_SHOW_REG_TRIGLEV)
    {
        LOG_WRITE("trigLev = %s", GF::Bin2String16(data).c_str());
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


uint FPGA::SET::PointsInChannel()
{
    return ENUM_POINTS_FPGA::ToPoints(FPGA::SET::ENumPointsInChannel());
}


ENUM_POINTS_FPGA::E FPGA::SET::ENumPointsInChannel()
{
    return set.memory.fpgaNumPoints;
}


uint FPGA::SET::BytesInChannel()
{
    uint result = PointsInChannel();

    if (PeackDetMode::IsEnabled())
    {
        result *= 2;
    }

    return result;
}


uint FPGA::SET::BytesForData()
{
    uint result = 0;

    if (SET_ENABLED_A)    { result += BytesInChannel(); }

    if (SET_ENABLED_B)    { result += BytesInChannel(); }

    return result;
}
