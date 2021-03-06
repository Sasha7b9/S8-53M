// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Containers/Buffer_.h"
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
            work_before_calibration(StateWorkFPGA::Stop)
        { }

        // ��������� ������� ���������. ����� �� ����� ������������ �������� FPGA_RestoreState().
        void Save();

        StateWorkFPGA       work_before_calibration;
        StateWorkFPGA       work;
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

        static bool CalibrationChannel(const Channel &ch);

        struct Balancer
        {
            // ������ ������������ �� ���� �����
            static void PerformOnGround(const Channel &ch);

            // ������ ������������ ��� AC � DC (�� ��������� ����������)
            static bool PerformNormal(const Channel &ch);

        private:

            static void CalibrateAddRShiftGND(const Channel &ch);
            static int16 CalculateAddRShift(float ave);
        };

        struct Stretcher
        {
            static bool Perform(const Channel &ch);

        private:

            // ���������� ����������� �������� ������ �� ���������� min � max
            static float Calculate(float min, float max);
        };

    private:

        static void ReadPoints1024(uint8 buffer[1024], const Channel &ch);

        // ������ 1024 ����� � ���������� �� ������� ��������
        static float ReadPoints1024ave(const Channel &ch);

        // ������ 1024 ����� � ���������� ����������� � ������������ ��������
        static void ReadPoints1024min_max(const Channel &ch, float *min, float *max);

        static void FuncDraw();

        static void DrawParametersChannel(Channel &ch, int x, int y);

        static uint timeStart;

        // ��������� ����� ���������� ��������
        static void FillBuffer(Buffer<int16> &buffer, int16 *first);
    };
};
