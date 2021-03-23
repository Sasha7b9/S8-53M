#pragma once
#include "FPGA/DataSettings.h"
#include "FPGA/FPGA_Types.h"
#include "Settings/Settings.h"


struct DataSettings;


struct StateWorkFPGA { enum E {
    Stop,    // ���� - �� ���������� ����������� ����������.
    Wait,    // ��� ����������� ��������������.
    Work     // ��� ������.
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
    static bool   can_read_data;

    static void Init();

    // ���������� ���������� ����������� �������� � �������.
    static void SetNumSignalsInSec(int num_sig_in_sec);

    static void Update();

    // ��������� ���� ����� ����������.
    static void OnPressStartStop();

    // ������ �������� ����� ����������.
    static void Start();

    // ��������� ������� ����� ����������.
    static void Stop();

    // ���������� true, ���� ������ ��������� �� � �������� ����� ����������.
    static bool IsRunning();

    // ��������� ������� ������ �������.
    static void StartAutoFind();

    // ���������� ��������� ����� ����� ��������� ����� - ����� ��������� ������ ������������� �� ��������� �����
    static void TemporaryPause();

    static StateWorkFPGA::E CurrentStateWork();

    // ��������� ��������� � ���������� ����� �� ���������� ��������� SSettings.
    static void LoadSettings();

private:

    static StateWorkFPGA::E state_work;

public:

    struct SET
    {
        // ������� ����� � ��������� ������ ������
        static uint PointsInChannel();
        static ENUM_POINTS_FPGA::E ENumPointsInChannel();

        // ������� ������ � ��������� ������ ������
        static uint BytesInChannel();

        // ������� ���� ����� ��� ���������� ������ ����� �������
        static uint BytesForData();

        static bool InRandomizeMode();

        static bool InSelfRecorderMode();
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
        Flag() : flag(0), time_pred_launch_ready(0) {}

        void Read();

        inline bool IsTrigReady() const  { return _GET_BIT(flag, FL_TRIG_READY) == 1; }

        inline bool IsDataReady() const  { return _GET_BIT(flag, FL_DATA_READY) == 1; };

        inline bool IsPointReady() const { return _GET_BIT(flag, FL_POINT_READY) == 1; };

        inline bool IsPredLaunchReady() const  { return _GET_BIT(flag, FL_PRED_READY) == 1; }

        // ���������� �����, ����� ��� ���������� ���� �����������
        void CalculateTimeReadyPredLaunch();

        // ��������� ������ ������������, ���� ����� (���� � ��� �������� �������������� �������������, ����� �������
        // ����������� ������ ����� �������, � ������������� ��� � �� ������, ����� ������ �������������� �������������
        void RunPostLaunchIfNeed();

        uint16 flag;
        uint time_pred_launch_ready;     // �����, ����� ���� PredReady �����������. ���� == 0, �� ����������
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
        State() :
            need_calibration(false),
            state_work_before_calibration(StateWorkFPGA::Stop),
            state_calibration(StateCalibration::None)
        { }

        // ��������� ������� ���������. ����� �� ����� ������������ �������� FPGA_RestoreState().
        void Save();

        // ��������������� ���������, ����� ���������� �������� FPGA_SaveState().
        void Restore();

        bool                need_calibration;               // ������������� � true �������� ��������, ��� ����������
                                                            // ���������� ����������
        StateWorkFPGA::E    state_work_before_calibration;
        StateCalibration::E state_calibration;              // ������� ��������� ����������. ������������ � ��������
                                                            // ����������
        Settings            stored_settings;                // ����� ����� ��������� ����������� ������ ������ -
                                                            // ��������� ��������� ������ �������
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

        static bool first_�fter_write;      // ������������ � ������ �������������. ����� ������ ������ ��������� �
                                            // ������� ����� �� ������������ ������ ��������� ������ � ���, ������ ���
                                            // ��� �������� � ������ ������
    };

    struct Calibrator
    {
        // ��������� � ������� ������������ ���������� �������.
        static void LoadKoeff(const Channel &ch);

        // ������ ������� ����������.
        static void ProcedureCalibration();
    };
};
