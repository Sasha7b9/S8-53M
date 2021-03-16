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

bool             FPGA::AutoFinder::auto_find_in_progress = false;

float            FPGA::FreqMeter::freq = 0.0f;

#define N_KR 100
const int        FPGA::Randomizer::Kr[] = { N_KR / 1, N_KR / 2, N_KR / 5, N_KR / 10, N_KR / 20 };
int              FPGA::Randomizer::number_measures_for_gates = 1000;
bool             FPGA::Randomizer::first_аfter_write = false;


// Функция вызывается, когда можно считывать очередной сигнал.
static void OnTimerCanReadData();


void FPGA::Init(void) 
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


void OnTimerCanReadData(void)
{
    FPGA::can_read_data = 1;
}


void FPGA::Start(void)
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


bool FPGA::ProcessingData(void)
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
                LOG_WRITE("флаг готовности %s", GF::Bin2String16(flag.flag, buffer));
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


void FPGA::Update(void)
{
    flag.Read();

    if (flag.IsPredReady())
    {
        TrigPolarity::Switch();

        if (flag.IsTrigReady())
        {
            LOG_WRITE("Триггер готов");
        }
        else
        {
            LOG_WRITE("Триггер не готов");
        }
    }
}


StateWorkFPGA::E FPGA::CurrentStateWork(void)
{
    return state_work;
}


void FPGA::OnPressStartStop(void)
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


bool FPGA::IsRunning(void)
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


int FPGA::CalculateShift(void)            // \todo Не забыть восстановить функцию
{
    uint16 rand = HAL_ADC3::GetValue();
    //LOG_WRITE("rand = %d", (int)rand);
    uint16 min = 0;
    uint16 max = 0;

    if (SET_TBASE == TBase::_200ns)
    {
        return rand < 3000 ? 0 : -1;    // set.debug.altShift; \todo Остановились на жёстком задании дополнительного смещения. На PageDebug выбор 
                                        // закомментирован, можно раскомментировать при необходимости
    }

    if (!Randomizer::CalculateGate(rand, &min, &max))
    {
        return TShift::NULL_VALUE;
    }
    
    //LOG_WRITE("ворота %d - %d", min, max);

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

    return -1;  // set.debug.altShift;      \todo Остановились на жёстком задании дополнительного смещения. На PageDebug выбор закомментирован, 
                                            //можно раскомментировать при необходимости
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


void FPGA::Reader::ReadPoint(void)
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


void FPGA::State::Save(void)
{
    FPGA::state.state_work_before_calibration = state_work;
    stored_settings = set;
}


void FPGA::State::Restore(void)
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


volatile static bool readPeriod = false;     // Установленный в true флаг означает, что частоту нужно считать по счётчику периода


static BitSet32 ReadRegFreq(void)
{
    BitSet32 fr;
    fr.half_word[0] = (uint8)HAL_FMC::Read(RD_FREQ_LOW);
    fr.half_word[1] = (uint8)HAL_FMC::Read(RD_FREQ_HI);
    return fr;
}


static BitSet32 ReadRegPeriod(void)
{
    BitSet32 period;
    period.half_word[0] = (uint8)HAL_FMC::Read(RD_PERIOD_LOW);
    period.half_word[1] = (uint8)HAL_FMC::Read(RD_PERIOD_HI);
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
    return 10e6F / period->word;
}


//static void ReadFreq(void)                         // Чтение счётчика частоты производится после того, как бит 4 флага RD_FL установится в едицину
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


//void ReadPeriod(void)
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

//    if(!readPeriod) 
//    {
//        if(_GET_BIT(flag, FL_FREQ_READY)) 
//        {
//            ReadFreq();
//        }
//    }
//    if(readPeriod && (_GET_BIT(flag, FL_PERIOD_READY) == 1)) 
//    {
//        ReadPeriod();
//    }
}


static float CalculateFreqFromCounterFreq(void)
{
    while (_GET_BIT(HAL_FMC::Read(RD_FL), FL_FREQ_READY) == 0) {};
    ReadRegFreq();
    while (_GET_BIT(HAL_FMC::Read(RD_FL), FL_FREQ_READY) == 0) {};
    BitSet32 fr = ReadRegFreq();
    if (fr.word >= 5)
    {
        return FreqCounterToValue(&fr);
    }
    return 0.0F;
}


static float CalculateFreqFromCounterPeriod(void)
{
    uint time = TIME_MS;
    while (TIME_MS - time < 1000 && _GET_BIT(HAL_FMC::Read(RD_FL), FL_PERIOD_READY) == 0) {};
    ReadRegPeriod();
    time = TIME_MS;
    while (TIME_MS - time < 1000 && _GET_BIT(HAL_FMC::Read(RD_FL), FL_PERIOD_READY) == 0) {};
    BitSet32 period = ReadRegPeriod();
    if (period.word > 0 && (TIME_MS - time < 1000))
    {
        return PeriodCounterToValue(&period);
    }
    return 0.0F;
}


float FPGA::FreqMeter::GetFreq(void)
{
    return freq;
}


bool FPGA::Randomizer::AllPointsRandomizer(void)
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


void FPGA::StartAutoFind(void)
{
    AutoFinder::auto_find_in_progress = true;
}


uint8 FPGA::CalculateMinWithout0(const uint8 buffer[100])
{
    // \todo На одном экземпляре был страшенныый глюк, когда без сигнала выбивались значения 0 и 255 в рандомных местах
    // Вот такой кастыиль на скорую ногу, чтобы нули выкинуть.
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
    // \todo На одном экземпляре был страшенныый глюк, когда без сигнала выбивались значения 0 и 255 в рандомных местах
    // Вот такой кастыиль на скорую ногу, чтобы нули выкинуть.
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


void FPGA::AutoFinder::Find(void)
{
    //LOG_WRITE(" ");
    //Timer::StartLogging();

    //LOG_WRITE("Канал 1");
    if (!FindWave(ChA))
    {
        //LOG_WRITE("Канал 2");
        if(!FindWave(ChB))
        {
            Display::ShowWarningBad(Warning::SignalNotFound);
        }
    }

    Init();
    Start();
    
    auto_find_in_progress = true;
}


bool FPGA::AutoFinder::FindWave(Channel::E chan)
{
    Settings settings = set;    // Сохраняем предыдущие настройки

    Stop(false);
    SET_ENABLED(chan) = true;
    TrigSource::Set(static_cast<TrigSource::E>(chan));
    TrigLev::Set(static_cast<TrigSource::E>(chan), TrigLevZero);
    RShift::Set(chan, RShiftZero);
    ModeCouple::Set(chan, ModeCouple::AC);
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


Range::E FPGA::AutoFinder::AccurateFindRange(Channel::E chan)
{
    /*
    Алгоритм поиска.
    Устанавливаем развёртку 50 мс.
    Устанавливаем закрытый вход.
    Включаем пиковый детектор.
    1. Устанавливаем растяжку 20В/дел.
    Не дожидаясь СИ, начинаем считывать точки в поточечном режиме и считываем их 50 штук - 2 клетки с небольшим. За это время
        на экране уложится чуть более одного периода частоты 10 Гц.
    Проверяем считанную информацию на выход за пределы экрана. Если вышла - берём предыдущую растяжку (например, 2В вместо 1В) и возвращаем
        её в качестве результата.
    Если не вышла - берём следующую растяжку и повторяем шаги с 1.
    */

    uint8 buffer[100];  // Сюда будем считывать точки

    TBase::Set(TBase::_50ms);
    ModeCouple::Set(chan, ModeCouple::AC);
    PeackDetMode::E peackDetMode = PEAKDET;
    PeackDetMode::Set(PeackDetMode::Enable);
    for (int range = Range::Count - 1; range >= 0; range--)
    {
        //Timer::LogPointMS("1");
        FPGA::Stop(false);
        Range::Set(chan, static_cast<Range::E>(range));
        HAL_TIM2::Delay(10);
        FPGA::Start();

        for (int i = 0; i < 50; i++)
        {
            while (_GET_BIT(HAL_FMC::Read(RD_FL), FL_POINT_READY) == 0) {};
            HAL_FMC::Read(RD_ADC_B);
            HAL_FMC::Read(RD_ADC_B);
            HAL_FMC::Read(RD_ADC_A);
            HAL_FMC::Read(RD_ADC_A);
        }

        if (chan == ChA)
        {
            for (int i = 0; i < 100; i += 2)
            {
                while (_GET_BIT(HAL_FMC::Read(RD_FL), FL_POINT_READY) == 0) {};
                HAL_FMC::Read(RD_ADC_B);
                HAL_FMC::Read(RD_ADC_B);
                buffer[i] = (uint8)HAL_FMC::Read(RD_ADC_A);
                buffer[i + 1] = (uint8)HAL_FMC::Read(RD_ADC_A);
            }
        }
        else
        {
            for (int i = 0; i < 100; i += 2)
            {
                while (_GET_BIT(HAL_FMC::Read(RD_FL), FL_POINT_READY) == 0) {};
                buffer[i] = (uint8)HAL_FMC::Read(RD_ADC_B);
                buffer[i + 1] = (uint8)HAL_FMC::Read(RD_ADC_B);
                HAL_FMC::Read(RD_ADC_A);
                HAL_FMC::Read(RD_ADC_A);
            }
        }

        /*
        if(chan == ChA)
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
            PeackDetMode::Set(peackDetMode);
            return static_cast<Range::E>(range);
        }

        uint8 min = AVE_VALUE - 30;
        uint8 max = AVE_VALUE + 30;

        if(range == Range::_2mV && CalculateMinWithout0(buffer) > min && CalculateMaxWithout255(buffer) < max)
        {
            PeackDetMode::Set(peackDetMode);
            return Range::Count;
        }
    }

    PeackDetMode::Set(peackDetMode);
    return Range::Count;
}


TBase::E FPGA::AutoFinder::AccurateFindTBase(Channel::E chan)
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


TBase::E FPGA::AutoFinder::FindTBase(Channel::E)
{
    TrigInput::Set(TrigInput::Full);
    HAL_TIM2::Delay(10);
    FPGA::Stop(false);
    float fr = CalculateFreqFromCounterFreq();

    TrigInput::Set(fr < 1e6F ? TrigInput::LPF : TrigInput::Full);

    fr = CalculateFreqFromCounterFreq();

    TBase::E tBase = TBase::Count;

    if (fr >= 50.0F)
    {
        tBase = CalculateTBase(fr);
        TBase::Set(tBase);
        Start();
        TrigInput::Set(fr < 500e3F ? TrigInput::LPF : TrigInput::HPF);
        return tBase;
    }
    else
    {
        TrigInput::Set(TrigInput::LPF);
        FreqMeter::freq = CalculateFreqFromCounterPeriod();
        if (fr > 0.0F)
        {
            tBase = CalculateTBase(fr);
            TBase::Set(tBase);
            HAL_TIM2::Delay(10);
            FPGA::Start();
            return tBase;
        }
    }

    return TBase::Count;
}


void StopTemporaryPause(void)
{
    FPGA::temporary_pause = false;
    Timer::Disable(TypeTimer::TemporaryPauseFPGA);
}


void FPGA::TemporaryPause(void)
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
    else if (type == TypeWriteAnalog::All && (IS_SHOW_REG_TRIGPARAM || IS_SHOW_REG_RANGE_A || IS_SHOW_REG_RANGE_B || IS_SHOW_REG_PARAM_A || IS_SHOW_REG_PARAM_B)) //-V560 //-V501
    {
        LOG_WRITE("полная запись в аналоговую часть = %s", str);
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
