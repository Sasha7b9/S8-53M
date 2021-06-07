// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "FPGA/FPGA_Types.h"
#include "Settings/Settings.h"


struct DataSettings;


struct StateWorkFPGA
{
    enum E
    {
        Stop,    // СТОП - не занимается считыванием информации.
        Wait,    // Ждёт поступления синхроимпульса.
        Work     // Идёт работа.
    } value;

    StateWorkFPGA(E v = Stop) : value(v) {}
    bool IsStop() const { return value == Stop; }
    bool IsWait() const { return value == Wait; }
    bool IsWork() const { return value == Work; }
    operator E() const  { return value; }
};


struct StateCalibration { enum E {
    None,
    ADCinProgress,
    RShift0start,
    RShift0inProgress,
    RShift1start,
    RShift1inProgress,
    ErrorCalibration0,
    ErrorCalibration1
};};


class FPGA
{
public:

    static bool temporary_pause;
    static bool can_read_data;

    static void Init();

    // Установить количество считываемых сигналов в секунду.
    static void SetNumSignalsInSec(int num_sig_in_sec);

    static void Update();

    // Запуск процесса сбора информации.
    static void Start();

    // Прерывает процесс сбора информации.
    static void Stop();

    // Возвращает true, если прибор находится не в процессе сбора информации.
    static bool IsRunning();

    // Запустить процесс поиска сигнала.
    static void StartAutoFind();

    // Установить временную паузу после изменения ручек - чтобы смещённый сигнал зафиксировать на некоторое время
    static void TemporaryPause();

    static StateWorkFPGA &CurrentStateWork();

    // Загрузить настройки в аппаратную часть из глобальной структуры SSettings.
    static void LoadSettings();

    static void OnPressStartStop();

private:

    // Дать запуск, если в этом есть необходимость (например, включён автоматический режим запуска)
    static void StartIfNeed();

public:

    struct BUS
    {
        static void Write(uint16 * const address, uint16 data, bool restart = true);

        // Эта функция используется тогда, когда запись в регистр не требует перезапуска процесса измерения. Например,
        // в регистр, управляющий режимом калибратора
        static void WriteWithoutStart(uint16 * const address, uint16 data);

        static void WriteToDAC(TypeWriteDAC::E type, uint16 data);

        static void WriteToAnalog(TypeWriteAnalog::E type, uint data);

        // Загрузить все параметры напряжения каналов и синхронизации в аппаратную часть.
        static void SetAttribChannelsAndTrig(TypeWriteAnalog::E type);
    };

    struct Flag
    {
        Flag() : flag(0), time_pred_launch_ready(0) {}

        void Read();

        inline bool IsTrigReady()       const { return _GET_BIT(flag, FL_TRIG_READY) == 1; }
        inline bool IsDataReady()       const { return _GET_BIT(flag, FL_DATA_READY) == 1; };
        inline bool IsPointReady()      const { return _GET_BIT(flag, FL_POINT_READY) == 1; };
        inline bool IsPredLaunchReady() const { return _GET_BIT(flag, FL_PRED_READY) == 1; }
        inline bool IsFirstByte0()      const { return _GET_BIT(flag, FL_FIRST_BYTE) == 0; }

        // Рассчитать время, когда был установлен флаг предзапуска
        void CalculateTimeReadyPredLaunch();

        // Запустить отсчёт послезапуска, если нужно (если у нас включена автоматическая синхронизация, после отсчёта
        // предзапуска прошло много времени, а синхронизация так и не прошла, нужно давать принудительную синхронизацию
        void RunPostLaunchIfNeed();

        uint16 flag;
        uint time_pred_launch_ready;     // Время, когда флаг PredReady установился. Если == 0, не установлен
    };

    static Flag flag;

    struct FreqMeter
    {
        // Получить значение частоты.
        static float GetFreq();

        static float freq;           // Частота, намеренная альтерой
    };

    struct State
    {
        State() : //-V730
            work_before_calibration(StateWorkFPGA::Stop),
            state_calibration(StateCalibration::None)
        { }

        // Сохраняет текущие настройки. Потом их можно восстановить функцией FPGA_RestoreState().
        void Save();

        StateWorkFPGA       work_before_calibration;
        StateWorkFPGA       work;
        StateCalibration::E state_calibration;              // Текущее состояние калибровки. Используется в процессе
                                                            // калибровки
        Settings            stored_settings;                // Здесь нужно уменьшить необходимый размер памяти -
                                                            // сохранять настройки только альтеры
    };

    static State state;

    struct Randomizer
    {
        // Возвращает true,если все точки получены в режиме рандомизатора.
        static bool AllPointsRandomizer();

        // Установить количество измерений, по которым будут рассчитываться ворота в режиме рандомизатора.
        static void SetNumberMeasuresForGates(int number);

        static bool CalculateGate(uint16 rand, uint16 *min, uint16 *max);

    private:

        static int number_measures_for_gates;
    };

    struct Calibrator
    {
        // Запуск функции полной калибровки - смещение и растяжка
        static void PerformCalibration();

        struct Balancer
        {
            // Запуск частичной калибровки - только смещение
            static void Perform(const Channel &ch);

        private:

            static void CalibrateAddRShift(const Channel &ch);
            static int16 CalculateAddRShift(float ave);
        };

        struct Stretcher
        {
            static void Perform(const Channel &ch);
        };

    private:

        static float ReadPoints1024(const Channel &ch);

        static void FuncDraw();
    };
};
