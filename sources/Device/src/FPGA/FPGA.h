// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "FPGA/FPGA_Types.h"
#include "Settings/Settings.h"


struct DataSettings;


struct StateWorkFPGA
{
    enum E
    {
        Stop,    // ���� - �� ���������� ����������� ����������.
        Wait,    // ��� ����������� ��������������.
        Work     // ��� ������.
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

    // ���������� ���������� ����������� �������� � �������.
    static void SetNumSignalsInSec(int num_sig_in_sec);

    static void Update();

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

    static StateWorkFPGA &CurrentStateWork();

    // ��������� ��������� � ���������� ����� �� ���������� ��������� SSettings.
    static void LoadSettings();

    static void OnPressStartStop();

private:

    // ���� ������, ���� � ���� ���� ������������� (��������, ������� �������������� ����� �������)
    static void StartIfNeed();

public:

    struct BUS
    {
        static void Write(uint16 * const address, uint16 data, bool restart = true);

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

        inline bool IsTrigReady()       const { return _GET_BIT(flag, FL_TRIG_READY) == 1; }
        inline bool IsDataReady()       const { return _GET_BIT(flag, FL_DATA_READY) == 1; };
        inline bool IsPointReady()      const { return _GET_BIT(flag, FL_POINT_READY) == 1; };
        inline bool IsPredLaunchReady() const { return _GET_BIT(flag, FL_PRED_READY) == 1; }
        inline bool IsFirstByte0()      const { return _GET_BIT(flag, FL_FIRST_BYTE) == 0; }

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
        State() : //-V730
            work_before_calibration(StateWorkFPGA::Stop),
            state_calibration(StateCalibration::None)
        { }

        // ��������� ������� ���������. ����� �� ����� ������������ �������� FPGA_RestoreState().
        void Save();

        StateWorkFPGA       work_before_calibration;
        StateWorkFPGA       work;
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

    private:

        static int number_measures_for_gates;
    };

    struct Calibrator
    {
        // ������ ������� ������ ���������� - �������� � ��������
        static void PerformCalibration();

        struct Balancer
        {
            // ������ ��������� ���������� - ������ ��������
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
