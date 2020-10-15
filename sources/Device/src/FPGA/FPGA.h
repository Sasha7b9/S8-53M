#pragma once
#include "FPGA_Types.h"
#include "common/Keyboard/Controls_c.h"
#include "Settings/SettingsTrig.h"
#include "Settings/SettingsService.h"

class FPGA
{
public:

    static void Init();
    // Установить количество считываемых сигналов в секунду.
    static void SetNumSignalsInSec(int numSigInSec);

    static void Update();
    // Запись в регистр ПЛИС нового значения.
    static void WriteToHardware				        
                            (uint8 * const address,    // адрес регистра.
                                uint8 value,    // записываемое значение.
                                bool restart    // true означает, что после записи нужно запусить режим измерений, если до этого прибор не находился в режиме паузы.
                            );

    static StateWorkFPGA FPGA_CurrentStateWork();
    // Запускает цикл сбора информации.
    static void OnPressStartStop();
    // Запуск процесса сбора информации.
    static void Start();
    // Прерывает процесс сбора информации.
    static void Stop(bool pause);
    // Возвращает true, если прибор находится не в процессе сбора информации.
    static bool IsRunning();
    // Сохраняет текущие настройки. Потом их можно восстановить функцией FPGA_RestoreState().
    static void SaveState();
    // Восстанавливает настройки, ранее сохранённые функцией FPGA_SaveState().
    static void RestoreState();
    // Получить значение частоты.
    static float GetFreq();
    // Удаляет данные. Нужно для режима рандомизаотра, где информация каждого цикла не является самостоятельной.
    static void ClearData();
    // Установить дополнительное смещение. Нужно для правильной расстановки точек в режиме рандомизатора.
    static void SetAdditionShift(int shift);
    // Возвращает true,если все точки получены в режиме рандомизатора.
    static bool AllPointsRandomizer();
    // Установить количество измерений, по которым будут рассчитываться ворота в режиме рандомизатора.
    static void SetNumberMeasuresForGates(int number);
    // Принудительно запустить синхронизацию.
    static void SwitchingTrig();
    // Запустить процесс поиска сигнала.
    static void StartAutoFind();
    // Установить временную паузу после изменения ручек - чтобы смещённый сигнал зафиксировать на некоторое время
    static void TemporaryPause();

    static void FillDataPointer(DataSettings *dp);
    // Найти и установить уровень синхронизации по последнему считанному сигналу
    static void FindAndSetTrigLevel();
    // Установить режим канала по входу.
    static void SetModeCouple(Channel::E chan, ModeCouple::E modeCoupe);
    // Включить/выключить фильтр на входе канала.
    static void EnableChannelFiltr(Channel::E chan, bool enable);
    // Установить масштаб по напряжению.
    static void SetRange(Channel::E chan, Range::E range);
    // Увеличить масштаб по напряжению.
    static bool RangeIncrease(Channel::E chan);
    // Уменьшить масштаб по напряжению.
    static bool RangeDecrease(Channel::E chan);
    // Установить масштаб по времени.
    static void SetTBase(TBase::E tBase);
    // Уменьшить масштаб по времени.
    static void TBaseDecrease();
    // Увеличить масштаб по времени.
    static void TBaseIncrease();
    // Установить относительное смещение по напряжению.
    static void SetRShift(Channel::E chan, int16 rShift);
    // Установить относительное смещение по времени.
    static void SetTShift(int tShift);
    // Установить добавочное смещение по времени для режима рандомизатора. В каждой развёртке это смещение должно быть разное.
    static void SetDeltaTShift(int16 shift);
    // Включить/выключить режим пикового детектора.
    static void SetPeackDetMode(PeackDetMode::E peackDetMode);
    // Включить/выключить калибратор.
    static void SetCalibratorMode(CalibratorMode::E calibratorMode);
    // Загрузить в аппарат коэффициенты калибровки каналов.
    static void LoadKoeffCalibration(Channel::E chan);
    // Установить относительный уровень синхронизации.
    static void SetTrigLev(TrigSource::E chan, int16 trigLev);
    // Установить источник синхронизации.
    static void SetTrigSource(TrigSource::E trigSource);
    // Установить полярность синхронизации.
    static void SetTrigPolarity(TrigPolarity::E polarity);
    // Установить режим входа синхронизации.
    static void SetTrigInput(TrigInput::E trigInput);
    // Возвращает установленное смещение по времени в текстовом виде, пригодном для вывода на экран.
    static const char* GetTShiftString(int16 tShiftRel, char buffer[20]);
    // Запуск функции калибровки.
    static void ProcedureCalibration();

    static StateWorkFPGA::E CurrentStateWork();

private:
    // Загрузить настройки в аппаратную часть из глобальной структуры SSettings.
    static void LoadSettings();
    // Загрузка коэффицента развёртки в аппаратную часть.
    static void LoadTBase();
    // Загрузка смещения по времени в аппаратную часть.
    static void LoadTShift();
    // Загрузка масштаба по напряжению в аппаратную часть.
    static void LoadRange(Channel::E chan);
    // Загрузка смещения по напряжению в аппаратную часть.
    static void LoadRShift(Channel::E chan);
    // Загрузка уровня синхронизации в аппаратную часть.
    static void LoadTrigLev();                                  
    // Загузка полярности синхронизации в аппаратную часть.
    static void LoadTrigPolarity();
    // Загрузить все параметры напряжения каналов и синхронизации в аппаратную часть.
    static void SetAttribChannelsAndTrig(TypeWriteAnalog::E type);
    // Загрузить регистр WR_UPR (пиковый детектор и калибратор).
    static void LoadRegUPR();

    static void WriteToAnalog(TypeWriteAnalog::E type, uint data);

    static void WriteToDAC(TypeWriteDAC::E type, uint16 data);
    // Прочитать данные.
    static void DataRead(
                        bool necessaryShift,    // Признак того, что сигнал нужно смещать.
                        bool saveToStorage      // Нужно в режиме рандомизатора для указания, что пора сохранять измерение
                        );

    static bool CalculateGate(uint16 rand, uint16 *min, uint16 *max);

    static int CalculateShift();
    // Инвертирует данные.
    static void InverseDataIsNecessary(Channel::E chan, uint8 *data);

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
};
