#pragma once
#include "FPGA_Types.h"
#include "Settings/SettingsTrig.h"
#include "Settings/SettingsService.h"


struct DataSettings;

extern uint16 gPost;
extern int16 gPred;
extern int gAddNStop;


#define FPGA_IN_PROCESS_OF_READ (FPGA::inProcessingOfRead)


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

    static bool inProcessingOfRead;

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

    static int addShiftForFPGA;

    // Загрузить все параметры напряжения каналов и синхронизации в аппаратную часть.
    static void SetAttribChannelsAndTrig(TypeWriteAnalog::E type);

private:
    // Загрузить настройки в аппаратную часть из глобальной структуры SSettings.
    static void LoadSettings();

    // Прочитать данные.
    static void DataRead(
                        bool necessaryShift,    // Признак того, что сигнал нужно смещать.
                        bool saveToStorage      // Нужно в режиме рандомизатора для указания, что пора сохранять измерение
                        );

    static bool CalculateGate(uint16 rand, uint16 *min, uint16 *max);

    static int CalculateShift();
    // Инвертирует данные.
    static void InverseDataIsNecessary(Channel::E chan, uint16 *data);

    static void AutoFind();

    static uint8 CalculateMinWithout0(const uint8 buffer[100]);

    static uint8 CalculateMaxWithout255(const uint8 buffer[100]);

    static bool ProcessingData();

    static void ReadRandomizeMode();

    static void ReadRealMode(bool necessaryShift);

    static Range::E AccurateFindRange(Channel::E chan);

    static TBase::E FindTBase(Channel::E chan);

    static TBase::E AccurateFindTBase(Channel::E chan);

    static bool FindWave(Channel::E chan);

public:

    struct BUS
    {
        struct TypeRecord
        {
            enum E
            {
                FPGA,
                Analog,
                DAC
            };
        };

        // Запись в регистр ПЛИС нового значения:
        // address - адрес регистра,
        // value - записываемое значение,
        // restart - true означает, что после записи нужно запусить режим измерений, если до этого прибор не находился в
        //           режиме паузы.
        static void WriteToHardware(uint16 *const address, uint16 value, bool restart);

        static void Write(TypeRecord::E type, uint16 *address, uint data, bool restart);

        static void Write(TypeRecord::E type, uint16 *address, uint data);

        static void WriteToDAC(TypeWriteDAC::E type, uint16 data);

        static void WriteToAnalog(TypeWriteAnalog::E type, uint data);
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
    };

    struct Calibrator
    {
        // Загрузить в аппарат коэффициенты калибровки каналов.
        static void LoadKoeff(Channel::E chan);

        // Запуск функции калибровки.
        static void ProcedureCalibration();
    };
};
