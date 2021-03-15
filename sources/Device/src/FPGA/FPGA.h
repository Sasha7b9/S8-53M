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
}; };


class FPGA
{
public:

    struct Flag
    {
        static uint16 Read();
        static uint16 flag;
    };

    struct FreqMeter
    {
        // �������� �������� �������.
        static float GetFreq();
    };

    static bool inProcessingOfRead;

    static void Init();

    // ���������� ���������� ����������� �������� � �������.
    static void SetNumSignalsInSec(int numSigInSec);

    static void Update();

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

    // ��������������� ���������, ����� ���������� �������� FPGA_SaveState().
    static void RestoreState();

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
    // ��������/��������� ������ �� ����� ������.
    static void EnableChannelFiltr(Channel::E chan, bool enable);
    // ��������� � ������� ������������ ���������� �������.
    static void LoadKoeffCalibration(Channel::E chan);
    // ���������� ������������� �������� �� ������� � ��������� ����, ��������� ��� ������ �� �����.
    static const char* GetTShiftString(int16 tShiftRel, char buffer[20]);
    // ������ ������� ����������.
    static void ProcedureCalibration();

    static StateWorkFPGA::E CurrentStateWork();

    static StateFPGA state;

    static int addShiftForFPGA;

    // ��������� ��� ��������� ���������� ������� � ������������� � ���������� �����.
    static void SetAttribChannelsAndTrig(TypeWriteAnalog::E type);

private:
    // ��������� ��������� � ���������� ����� �� ���������� ��������� SSettings.
    static void LoadSettings();

    static void WriteToAnalog(TypeWriteAnalog::E type, uint data);
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

public:

    struct BUS
    {
        // ������ � ������� ���� ������ ��������:
        // address - ����� ��������,
        // value - ������������ ��������,
        // restart - true ��������, ��� ����� ������ ����� �������� ����� ���������, ���� �� ����� ������ �� ��������� �
        //           ������ �����.
        static void WriteToHardware(uint16 *const address, uint16 value, bool restart);

        static void Write(TypeRecord::E type, uint16 *address, uint data, bool restart);

        static void Write(TypeRecord::E type, uint16 *address, uint data);

        static void WriteToDAC(TypeWriteDAC::E type, uint16 data);
    };
};
