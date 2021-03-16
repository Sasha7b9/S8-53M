#pragma once
#include "FPGA/DataSettings.h"
#include "FPGA/FPGA_Types.h"
#include "Settings/Settings.h"
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

    // ��������� ������� ������ �������.
    static void StartAutoFind();

    // ���������� ��������� ����� ����� ��������� ����� - ����� ��������� ������ ������������� �� ��������� �����
    static void TemporaryPause();

    static StateWorkFPGA::E CurrentStateWork();

private:

    static StateWorkFPGA::E state_work;

    static uint time_start;

    // ��������� ��������� � ���������� ����� �� ���������� ��������� SSettings.
    static void LoadSettings();

    static int CalculateShift();

    // ����������� ������.
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

        static uint16 data_rel_A[FPGA_MAX_POINTS];  // ����� ������������ ��� ������ ������ ������� ������
        static uint16 data_rel_B[FPGA_MAX_POINTS];  // ����� ������������ ��� ������ ������ ������� ������

        static int addition_shift;                  // �������������� ��������. ����� ��� ���������� ����������� �����
                                                    // � ������ �������������
    };

    // ����� �������
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

        // ��� ������� ������������ �����, ����� ������ � ������� �� ������� ����������� �������� ���������. ��������,
        // � �������, ����������� ������� �����������
        static void WriteWithoutStart(uint16 * const address, uint16 data);

        static void WriteToDAC(TypeWriteDAC::E type, uint16 data);

        static void WriteToAnalog(TypeWriteAnalog::E type, uint data);

        // ��������� ��� ��������� ���������� ������� � ������������� � ���������� �����.
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
        // �������� �������� �������.
        static float GetFreq();

        static float freq;           // �������, ���������� ��������
    };

    struct State
    {
        // ��������� ������� ���������. ����� �� ����� ������������ �������� FPGA_RestoreState().
        void Save();

        // ��������������� ���������, ����� ���������� �������� FPGA_SaveState().
        void Restore();

        bool                 needCalibration;           // ������������� � true �������� ��������, ��� ����������
                                                        // ���������� ����������
        StateWorkFPGA::E    stateWorkBeforeCalibration;
        StateCalibration::E stateCalibration;           // ������� ��������� ����������. ������������ � �������� ����������
        Settings            storingSettings;            // ����� ����� ��������� ����������� ������ ������ - ���������
                                                        // ��������� ������ �������
    };

    static State state;

    struct Randomizer
    {
        // ���������� true,���� ��� ����� �������� � ������ �������������.
        static bool AllPointsRandomizer();

        // ���������� ���������� ���������, �� ������� ����� �������������� ������ � ������ �������������.
        static void SetNumberMeasuresForGates(int number);

        static bool CalculateGate(uint16 rand, uint16 *min, uint16 *max);

        static const int Kr[];

    private:

        static int number_measures_for_gates;
    };

    struct Calibrator
    {
        // ��������� � ������� ������������ ���������� �������.
        static void LoadKoeff(Channel::E chan);

        // ������ ������� ����������.
        static void ProcedureCalibration();
    };
};
