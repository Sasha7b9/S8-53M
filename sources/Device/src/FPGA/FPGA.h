#pragma once
#include "FPGA/DataSettings.h"
#include "FPGA/FPGA_Types.h"
#include "Settings/Settings.h"
#include "Settings/SettingsTrig.h"
#include "Settings/SettingsService.h"


struct DataSettings;


struct StateWorkFPGA { enum E {
    Stop,    // СТОП - не занимается считыванием информации.
    Wait,    // Ждёт поступления синхроимпульса.
    Work,    // Идёт работа.
    Pause    // Это состояние, когда временно приостановлен прибор, например, для чтения данных или для записи значений регистров.
};};

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

    static bool   in_processing_of_read;
    static int    add_shift;
    static int    add_N_stop;
    static int16  pred;
    static uint16 post;
    static bool   temporary_pause;

    static void Init();

    // Установить количество считываемых сигналов в секунду.
    static void SetNumSignalsInSec(int numSigInSec);

    static void Update();

    // Запускает цикл сбора информации.
    static void OnPressStartStop();

    // Запуск процесса сбора информации.
    static void Start();

    // Прерывает процесс сбора информации.
    static void Stop(bool pause);

    // Возвращает true, если прибор находится не в процессе сбора информации.
    static bool IsRunning();

    // Запустить процесс поиска сигнала.
    static void StartAutoFind();

    // Установить временную паузу после изменения ручек - чтобы смещённый сигнал зафиксировать на некоторое время
    static void TemporaryPause();

    static StateWorkFPGA::E CurrentStateWork();

private:

    static StateWorkFPGA::E state_work;

    static uint time_start;

    // Загрузить настройки в аппаратную часть из глобальной структуры SSettings.
    static void LoadSettings();

    static int CalculateShift();

    // Инвертирует данные.
    static void InverseDataIsNecessary(Channel::E chan, uint16 *data);

    static uint8 CalculateMinWithout0(const uint8 buffer[100]);

    static uint8 CalculateMaxWithout255(const uint8 buffer[100]);

    static bool ProcessingData();

public:

    struct Reader
    {
        static void ClearData();

        static void Read(bool necessaryShift, bool saveToStorage);

        static void ReadRandomizeMode();

        static void ReadRealMode(bool necessaryShift);

        static DataSettings ds;

        static uint16 data_rel_A[FPGA_MAX_POINTS];  // Буфер используется для чтения данных первого канала
        static uint16 data_rel_B[FPGA_MAX_POINTS];  // Буфер используется для чтения данных второго канала

        static int addition_shift;                  // Дополнительное смещение. Нужно для правильной расстановки точек
                                                    // в режиме рандомизатора
    };

    // Поиск сигнала
    struct AutoFinder
    {
        static void Find();

        static bool FindWave(Channel::E chan);

        static Range::E AccurateFindRange(Channel::E chan);

        static TBase::E FindTBase(Channel::E chan);

        static TBase::E AccurateFindTBase(Channel::E chan);

        static bool auto_find_in_progress;
    };

    struct BUS
    {
        static void Write(uint16 * const address, uint16 data);

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
        Flag() : flag(0) {}
        void Read();
        inline bool IsTrigReady() const  { return _GET_BIT(flag, FL_TRIG_READY) == 1; }
        inline bool IsDataReady() const  { return _GET_BIT(flag, FL_DATA_READY) == 1; };
        inline bool IsPointReady() const { return _GET_BIT(flag, FL_POINT_READY) == 1; };
        inline bool IsPredReady() const  { return _GET_BIT(flag, FL_PRED_READY) == 1; };
        uint16 flag;
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
        // Сохраняет текущие настройки. Потом их можно восстановить функцией FPGA_RestoreState().
        void Save();

        // Восстанавливает настройки, ранее сохранённые функцией FPGA_SaveState().
        void Restore();

        bool                 needCalibration;           // Установленное в true значение означает, что необходимо
                                                        // произвести калибровку
        StateWorkFPGA::E    stateWorkBeforeCalibration;
        StateCalibration::E stateCalibration;           // Текущее состояние калибровки. Используется в процессе калибровки
        Settings            storingSettings;            // Здесь нужно уменьшить необходимый размер памяти - сохранять
                                                        // настройки только альтеры
    };

    static State state;

    struct Randomizer
    {
        // Возвращает true,если все точки получены в режиме рандомизатора.
        static bool AllPointsRandomizer();

        // Установить количество измерений, по которым будут рассчитываться ворота в режиме рандомизатора.
        static void SetNumberMeasuresForGates(int number);

        static bool CalculateGate(uint16 rand, uint16 *min, uint16 *max);

        static const int Kr[];

    private:

        static int number_measures_for_gates;
    };

    struct Calibrator
    {
        // Загрузить в аппарат коэффициенты калибровки каналов.
        static void LoadKoeff(Channel::E chan);

        // Запуск функции калибровки.
        static void ProcedureCalibration();
    };
};
