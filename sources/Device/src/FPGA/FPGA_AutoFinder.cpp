// 2021/03/17 8:01:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_AutoFinder.h"


bool AutoFinderFPGA::auto_find_in_progress = false;


static float CalculateFreqFromCounterFreq();
static TBase::E CalculateTBase(float freq_);
static float CalculateFreqFromCounterPeriod();
static BitSet32 ReadRegFreq();
static float FreqCounterToValue(const BitSet32 *fr);
static BitSet32 ReadRegPeriod();
static float PeriodCounterToValue(const BitSet32 *period);


void AutoFinderFPGA::Find()
{
    //LOG_WRITE(" ");
    //Timer::StartLogging();

    //LOG_WRITE("Канал 1");
    if (!FindWave(Channel::A))
    {
        //LOG_WRITE("Канал 2");
        if (!FindWave(Channel::B))
        {
            Display::ShowWarningBad(Warning::SignalNotFound);
        }
    }

    FPGA::Init();
    FPGA::Start();

    auto_find_in_progress = true;
}


bool AutoFinderFPGA::FindWave(const Channel &ch)
{
    Settings settings = set;    // Сохраняем предыдущие настройки

    FPGA::Stop();
    ch.Enable();
    TrigSource::Set(static_cast<TrigSource::E>(ch.value));
    TrigLev::Set(static_cast<TrigSource::E>(ch.value), TrigLev::ZERO);
    RShift::Set(ch, RShift::ZERO);
    ModeCouple::Set(ch, ModeCouple::AC);
    Range::E range = AccurateFindRange(ch);
    //LOG_WRITE("Range %s", RangeName(range));
    if (range != Range::Count)
    {
        set.chan[ch].range = range;
        TBase::E tBase = AccurateFindTBase(ch);
        if (tBase != TBase::Count)
        {
            TBase::Set(tBase);
            set.trig.source = static_cast<TrigSource::E>(ch.value);
            return true;
        }
    }

    set = settings;
    FPGA::LoadSettings();
    return false;
}


Range::E AutoFinderFPGA::AccurateFindRange(const Channel &ch)
{
    /*
    Алгоритм поиска.
    Устанавливаем развёртку 50 мс.
    Устанавливаем закрытый вход.
    Включаем пиковый детектор.
    1. Устанавливаем растяжку 20В/дел.
    Не дожидаясь СИ, начинаем считывать точки в поточечном режиме и считываем их 50 штук - 2 клетки с небольшим. За это
        время на экране уложится чуть более одного периода частоты 10 Гц.
    Проверяем считанную информацию на выход за пределы экрана. Если вышла - берём предыдущую растяжку (например, 2В
        вместо 1В) и возвращаем её в качестве результата.
    Если не вышла - берём следующую растяжку и повторяем шаги с 1.
    */

    uint8 buffer[100];  // Сюда будем считывать точки

    TBase::Set(TBase::_50ms);
    ModeCouple::Set(ch, ModeCouple::AC);
    PeackDetMode::E peackDetMode = PeackDetMode::Get();
    PeackDetMode::Set(PeackDetMode::Enable);
    for (int range = Range::Count - 1; range >= 0; range--)
    {
        //Timer::LogPointMS("1");
        FPGA::Stop();
        Range::Set(ch, static_cast<Range::E>(range));
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

        if (ch.IsA())
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
        if(ch == ChA)
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

        if (range == Range::_2mV && CalculateMinWithout0(buffer) > min && CalculateMaxWithout255(buffer) < max)
        {
            PeackDetMode::Set(peackDetMode);
            return Range::Count;
        }
    }

    PeackDetMode::Set(peackDetMode);
    return Range::Count;
}


TBase::E AutoFinderFPGA::FindTBase(const Channel &)
{
    TrigInput::Set(TrigInput::Full);
    HAL_TIM2::Delay(10);
    FPGA::Stop();
    float fr = CalculateFreqFromCounterFreq();

    TrigInput::Set(fr < 1e6F ? TrigInput::LPF : TrigInput::Full);

    fr = CalculateFreqFromCounterFreq();

    TBase::E tBase = TBase::Count;

    if (fr >= 50.0F)
    {
        tBase = CalculateTBase(fr);
        TBase::Set(tBase);
        FPGA::Start();
        TrigInput::Set(fr < 500e3F ? TrigInput::LPF : TrigInput::HPF);
        return tBase;
    }
    else
    {
        TrigInput::Set(TrigInput::LPF);
        FPGA::FreqMeter::freq = CalculateFreqFromCounterPeriod();
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


TBase::E AutoFinderFPGA::AccurateFindTBase(const Channel &ch)
{
    for (int i = 0; i < 5; i++)
    {
        TBase::E tBase = FindTBase(ch);
        TBase::E secondTBase = FindTBase(ch); //-V656

        if (tBase == secondTBase && tBase != TBase::Count)
        {
            return tBase;
        }
    }
    return TBase::Count;
}


static float CalculateFreqFromCounterFreq()
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


static TBase::E CalculateTBase(float freq_)
{
    if (freq_ >= 100e6F) { return TBase::_2ns; }
    else if (freq_ >= 40e6F) { return TBase::_5ns; }
    else if (freq_ >= 20e6F) { return TBase::_10ns; }
    else if (freq_ >= 10e6F) { return TBase::_20ns; }
    else if (freq_ >= 3e6F) { return TBase::_50ns; }
    else if (freq_ >= 2e6F) { return TBase::_100ns; }
    else if (freq_ >= 900e3F) { return TBase::_200ns; }
    else if (freq_ >= 400e3F) { return TBase::_500ns; }
    else if (freq_ >= 200e3F) { return TBase::_1us; }
    else if (freq_ >= 90e3F) { return TBase::_2us; }
    else if (freq_ >= 30e3F) { return TBase::_5us; }
    else if (freq_ >= 20e3F) { return TBase::_10us; }
    else if (freq_ >= 10e3F) { return TBase::_20us; }
    else if (freq_ >= 4e3F) { return TBase::_50us; }
    else if (freq_ >= 2e3F) { return TBase::_100us; }
    else if (freq_ >= 1e3F) { return TBase::_200us; }
    else if (freq_ >= 350.0F) { return TBase::_500us; }
    else if (freq_ >= 200.0F) { return TBase::_1ms; }
    else if (freq_ >= 100.0F) { return TBase::_2ms; }
    else if (freq_ >= 40.0F) { return TBase::_5ms; }
    else if (freq_ >= 20.0F) { return TBase::_10ms; }
    else if (freq_ >= 10.0F) { return TBase::_20ms; }
    else if (freq_ >= 4.0F) { return TBase::_50ms; }
    else if (freq_ >= 2.0F) { return TBase::_100ms; }
    return TBase::_200ms;
}


static float CalculateFreqFromCounterPeriod()
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


static BitSet32 ReadRegFreq()
{
    BitSet32 fr;
    fr.half_word[0] = (uint8)HAL_FMC::Read(RD_FREQ_LOW);
    fr.half_word[1] = (uint8)HAL_FMC::Read(RD_FREQ_HI);
    return fr;
}


static float FreqCounterToValue(const BitSet32 *fr)
{
    return fr->word * 10.0F;
}


static BitSet32 ReadRegPeriod()
{
    BitSet32 period;
    period.half_word[0] = (uint8)HAL_FMC::Read(RD_PERIOD_LOW);
    period.half_word[1] = (uint8)HAL_FMC::Read(RD_PERIOD_HI);
    return period;
}


static float PeriodCounterToValue(const BitSet32 *period)
{
    if (period->word == 0)
    {
        return 0.0F;
    }
    return 10e6F / period->word;
}


uint8 AutoFinderFPGA::CalculateMinWithout0(const uint8 buffer[100])
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


uint8 AutoFinderFPGA::CalculateMaxWithout255(const uint8 buffer[100])
{
    // \todo На одном экземпляре был страшенныый глюк, когда без сигнала выбивались значения 0 и 255 в рандомных местах
    // Вот такой кастыиль на скорую ногу, чтобы нули выкинуть.
    uint8 max = 0;
    for (int i = 1; i < 100; i++)
    {
        if (buffer[i] < 255 && buffer[i] > max)
        {
            max = buffer[i];
        }
    }
    return max;
}
