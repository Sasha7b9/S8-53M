#pragma once
#include "FPGA_Types.h"
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

    // Удаляет данные. Нужно для режима рандомизаотра, где информация каждого цикла не является самостоятельной.
    static void ClearData();

    // Принудительно запустить синхронизацию.
    static void SwitchingTrig();

    // Запустить процесс поиска сигнала.
    static void StartAutoFind();

    // Установить временную паузу после изменения ручек - чтобы смещённый сигнал зафиксировать на некоторое время
    static void TemporaryPause();

    static StateWorkFPGA::E CurrentStateWork();

private:
    // Загрузить настройки в аппаратную часть из глобальной структуры SSettings.
    static void LoadSettings();

    static int CalculateShift();

    // Инвертирует данные.
    static void InverseDataIsNecessary(Channel::E chan, uint16 *data);

    static uint8 CalculateMinWithout0(const uint8 buffer[100]);

    static uint8 CalculateMaxWithout255(const uint8 buffer[100]);

    static bool ProcessingData();

private:

    struct Reader
    {

        static void Read(bool necessaryShift, bool saveToStorage);

        static void ReadRandomizeMode();

        static void ReadRealMode(bool necessaryShift);

    };

public:

    // Поиск сигнала
    struct AutoFinder
    {
        static void Find();

        static bool FindWave(Channel::E chan);

        static Range::E AccurateFindRange(Channel::E chan);

        static TBase::E FindTBase(Channel::E chan);

        static TBase::E AccurateFindTBase(Channel::E chan);
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
        static uint16 Read();
        static uint16 flag;
    };

    struct FreqMeter
    {
        // Получить значение частоты.
        static float GetFreq();
    };

    struct State
    {
        // Сохраняет текущие настройки. Потом их можно восстановить функцией FPGA_RestoreState().
        static void Save();

        // Восстанавливает настройки, ранее сохранённые функцией FPGA_SaveState().
        static void Restore();

        bool needCalibration;           // Установленное в true значение означает, что необходимо произвести калибровку.
        StateWorkFPGA::E        stateWorkBeforeCalibration;
        StateCalibration::E     stateCalibration;   // Текущее состояние калибровки. Используется в процессе калибровки.
    };

    static State state;

    struct Randomizer
    {
        // Установить дополнительное смещение. Нужно для правильной расстановки точек в режиме рандомизатора.
        static void SetAdditionShift(int shift);

        // Возвращает true,если все точки получены в режиме рандомизатора.
        static bool AllPointsRandomizer();

        // Установить количество измерений, по которым будут рассчитываться ворота в режиме рандомизатора.
        static void SetNumberMeasuresForGates(int number);

        static bool CalculateGate(uint16 rand, uint16 *min, uint16 *max);
    };

    struct Calibrator
    {
        // Загрузить в аппарат коэффициенты калибровки каналов.
        static void LoadKoeff(Channel::E chan);

        // Запуск функции калибровки.
        static void ProcedureCalibration();
    };
};
