#pragma once
#include "FPGA_Types.h"
//#include "common/Keyboard/Controls_.h"
#include "Settings/SettingsTrig.h"
#include "Settings/SettingsService.h"


struct DataSettings;


struct StateWorkFPGA { enum E {
    Stop,    // ���� - �� ���������� ����������� ����������.
    Wait,    // ��� ����������� ��������������.
    Work,    // ��� ������.
    Pause    // ��� ���������, ����� �������� ������������� ������, ��������, ��� ������ ������ ��� ��� ������ �������� ���������.
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


struct StateFPGA
{
    bool needCalibration;				        // ������������� � true �������� ��������, ��� ���������� ���������� ����������.
    StateWorkFPGA::E stateWorkBeforeCalibration;
    StateCalibration::E stateCalibration;          // ������� ��������� ����������. ������������ � �������� ����������.
};


struct TypeRecord { enum E {
    FPGA,
    Analog,
    DAC
};};


class FPGA
{
public:

    static void Init();
    // ���������� ���������� ����������� �������� � �������.
    static void SetNumSignalsInSec(int numSigInSec);

    static void Update();

    static StateWorkFPGA FPGA_CurrentStateWork();
    // ��������� ���� ����� ����������.
    static void OnPressStartStop();
    // ������ �������� ����� ����������.
    static void Start();
    // ��������� ������� ����� ����������.
    static void Stop(bool pause);
    // ���������� true, ���� ������ ��������� �� � �������� ����� ����������.
    static bool IsRunning();
    // ��������� ������� ���������. ����� �� ����� ������������ �������� FPGA_RestoreState().
    static void SaveState();
    // ��������������� ���������, ����� ����������� �������� FPGA_SaveState().
    static void RestoreState();
    // �������� �������� �������.
    static float GetFreq();
    // ������� ������. ����� ��� ������ �������������, ��� ���������� ������� ����� �� �������� ���������������.
    static void ClearData();
    // ���������� �������������� ��������. ����� ��� ���������� ����������� ����� � ������ �������������.
    static void SetAdditionShift(int shift);
    // ���������� true,���� ��� ����� �������� � ������ �������������.
    static bool AllPointsRandomizer();
    // ���������� ���������� ���������, �� ������� ����� �������������� ������ � ������ �������������.
    static void SetNumberMeasuresForGates(int number);
    // ������������� ��������� �������������.
    static void SwitchingTrig();
    // ��������� ������� ������ �������.
    static void StartAutoFind();
    // ���������� ��������� ����� ����� ��������� ����� - ����� ��������� ������ ������������� �� ��������� �����
    static void TemporaryPause();

    static void FillDataPointer(DataSettings *dp);
    // ����� � ���������� ������� ������������� �� ���������� ���������� �������
    static void FindAndSetTrigLevel();
    // ���������� ����� ������ �� �����.
    static void SetModeCouple(Channel::E chan, ModeCouple::E modeCoupe);
    // ��������/��������� ������ �� ����� ������.
    static void EnableChannelFiltr(Channel::E chan, bool enable);
    // ���������� ������� �� ����������.
    static void SetRange(Channel::E chan, Range::E range);
    // ��������� ������� �� ����������.
    static bool RangeIncrease(Channel::E chan);
    // ��������� ������� �� ����������.
    static bool RangeDecrease(Channel::E chan);
    // ���������� ������� �� �������.
    static void SetTBase(TBase::E tBase);
    // ��������� ������� �� �������.
    static void TBaseDecrease();
    // ��������� ������� �� �������.
    static void TBaseIncrease();
    // ���������� ������������� �������� �� ����������.
    static void SetRShift(Channel::E chan, int16 rShift);
    // ���������� ������������� �������� �� �������.
    static void SetTShift(int tShift);
    // ���������� ���������� �������� �� ������� ��� ������ �������������. � ������ �������� ��� �������� ������ ���� ������.
    static void SetDeltaTShift(int16 shift);
    // ��������/��������� ����� �������� ���������.
    static void SetPeackDetMode(PeackDetMode::E peackDetMode);
    // ��������/��������� ����������.
    static void SetCalibratorMode(CalibratorMode::E calibratorMode);
    // ��������� � ������� ������������ ���������� �������.
    static void LoadKoeffCalibration(Channel::E chan);
    // ���������� ������������� ������� �������������.
    static void SetTrigLev(TrigSource::E chan, int16 trigLev);
    // ���������� �������� �������������.
    static void SetTrigSource(TrigSource::E trigSource);
    // ���������� ���������� �������������.
    static void SetTrigPolarity(TrigPolarity::E polarity);
    // ���������� ����� ����� �������������.
    static void SetTrigInput(TrigInput::E trigInput);
    // ���������� ������������� �������� �� ������� � ��������� ����, ��������� ��� ������ �� �����.
    static const char* GetTShiftString(int16 tShiftRel, char buffer[20]);
    // ������ ������� ����������.
    static void ProcedureCalibration();

    static StateWorkFPGA::E CurrentStateWork();

    static StateFPGA state;

private:
    // ��������� ��������� � ���������� ����� �� ���������� ��������� SSettings.
    static void LoadSettings();
    // �������� ����������� �������� � ���������� �����.
    static void LoadTBase();
    // �������� �������� �� ������� � ���������� �����.
    static void LoadTShift();
    // �������� �������� �� ���������� � ���������� �����.
    static void LoadRange(Channel::E chan);
    // �������� �������� �� ���������� � ���������� �����.
    static void LoadRShift(Channel::E chan);
    // �������� ������ ������������� � ���������� �����.
    static void LoadTrigLev();                                  
    // ������� ���������� ������������� � ���������� �����.
    static void LoadTrigPolarity();
    // ��������� ��� ��������� ���������� ������� � ������������� � ���������� �����.
    static void SetAttribChannelsAndTrig(TypeWriteAnalog::E type);
    // ��������� ������� WR_UPR (������� �������� � ����������).
    static void LoadRegUPR();

public:
    // ������ � ������� ���� ������ ��������.
    // address - ����� ��������,
    // value - ������������ ��������,
    // restart - true ��������, ��� ����� ������ ����� �������� ����� ���������, ���� �� ����� ������ �� ��������� �
    // ������ �����
    static void WriteToHardware(uint16 *const address, uint16 value, bool restart);

    static void Write(TypeRecord::E type, uint16 *address, uint data, bool restart);

private:
    static void WriteToAnalog(TypeWriteAnalog::E type, uint data);

    static void WriteToDAC(TypeWriteDAC::E type, uint16 data);
    // ��������� ������.
    static void DataRead(
                        bool necessaryShift,    // ������� ����, ��� ������ ����� �������.
                        bool saveToStorage      // ����� � ������ ������������� ��� ��������, ��� ���� ��������� ���������
                        );

    static bool CalculateGate(uint16 rand, uint16 *min, uint16 *max);

    static int CalculateShift();
    // ����������� ������.
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
};
