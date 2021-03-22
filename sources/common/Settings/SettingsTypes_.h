#pragma once
#include "common/Utils/String_.h"


#define NumChannels 2


// ��� ������� ��� ������ �������������.
struct SampleType {
    enum E
    {
        Real,   // �������� ����� - � ���������� ��������� ������ ������� ��������� �����, ������ �� ��������������.
        Equal   // ������������� - ������ �������� �� ��������� ������, ���������� �� �������������.
    };

    static SampleType::E Get();
};


// ����� ����� �������, � ������� ��� ������.
struct ENUM_POINTS_FPGA
{
    enum E
    {
        _281,
        _512,
        _1024,
        _2048,
        _4096,
        _8192,
        _16384,
        Count
    };

    static uint ToPoints(E);
};


// �������� ������� ��� ����������� �������.
struct SourceFFT { enum E
{
    A_,          // ������ � ����������� ������� ������� ������.
    B_,          // ������ � ����������� ������� ������� ������.
    Both_        // ������ � ����������� �������� ����� �������.
};};


// ���� ����
struct Language { enum E
{
    Russian,    // �������
    English     // ����������
};};


// ��� �������� ���
struct StretchADCtype
{
    enum E
    {
        Disable,
        Settings,
        Hand
    };

    static bool IsDisabled();
    static bool IsHand();
    static bool IsSettings();
};


// ����� ��������� �������.
struct ModeDrawSignal
{
    enum E
    {
        Lines,      // ������ �������� �������.
        Points      // ������ �������� �������.
    };

    static E Get();
    static void Set(E v);
    static bool IsLines();
};


// ��� ����� �� ������.
struct TypeGrid
{
    enum E
    {
        _1,
        _2,
        _3,
        _4,
        Count
    };

    static E Get();

    static bool Is1();
    static bool Is2();
    static bool Is3();
};


// �������� �����
struct ColorScheme {
    enum E
    {
        WhiteLetters,   // � ���� ������ ��������� ��������� ���� ������� ����� - �� ����� ������ ����� �����.
        BlackLetters    // � ���� ������ ��������� ��������� ���� ������� ������ - �� ����� ������� ��������.
    };

    static E Get();

    static bool IsWhiteLetters();
};


// ������������ ����������.
struct ENumAccumulation
{
    enum E
    {
        _1,
        _2,
        _4,
        _8,
        _16,
        _32,
        _64,
        _128,
        Infinity
    };

    static E Get();
    static void Set(E v);
    static bool IsNone();
    static bool IsInfinity();
};


// ���������� ���������� �� ����������.
struct ENumAveraging
{ 
    enum E
    {
        _1,
        _2,
        _4,
        _8,
        _16,
        _32,
        _64,
        _128,
        _256,
        _512
    };

    static int NumAverages();
    static int ToAbs();
};


// ��� ���������� �� ����������.
struct ModeAveraging { enum E
    {
        Accurately,   // ��������� �����.
        Around        // ��������� ��������������.
    };

    // ���������� ����� ����������
    static ModeAveraging::E Current();
};


// ���������� ��������� ��� ������� ������������ � ������������� ��������.
struct ENumMinMax
{
    enum E
    {
        _1,
        _2,
        _4,
        _8,
        _16,
        _32,
        _64,
        _128
    };

    static int ToAbs();
};


// ���������� ����� ��� ������� ����������� �������.
struct Smoothing {
    
    enum E
    {
        Disable,
        _2points,
        _3points,
        _4points,
        _5points,
        _6points,
        _7points,
        _8points,
        _9points,
        _10points
    };

    // ���������� ����� ����� ������������� ������� (����� ������� - �����������).
    static uint NumPoints();
    static bool IsDisable();
    static E Get();
    static void Set(E v);
};


// ����������� FPS.
struct ENumSignalsInSec
{
    enum E
    {
        _25,
        _10,
        _5,
        _2,
        _1,
        Count
    };

    // ���������� ����������� ������� ������.
    static int NumSignalsInS();
};


// ����� ����������.
struct ModeAccumulation
{
    enum E
    {
        NoReset,   // � ���� ������ ������������ ������ N ��������� ���������.
        Reset      // � ���� ������ ���������� N ��������� ��������� � ����� ������������.
    };

    static bool IsNoReset();
    static bool IsReset();
    static void Set(E v);
    static E Get();
};


// ����� ����������� �������������� ������� �������� �������� �� ����������.
struct AltMarkers
{
    enum E
    {
        Hide,        // ������ �� ��������.
        Show,        // ������ ��������.
        AutoHide     // �������� � ������� ����� timeShowLevels.
    };

    static bool IsHide();
};


// ����� ����� ����� ����� ���������� ������� ������ �������� ����.
struct MenuAutoHide { enum E
    {
        None,   // �������.
        _5,     // ����� 5 ������.
        _10,    // ����� 10 ������.
        _15,    // ����� 15 ������.
        _30,    // ����� 30 ������.
        _60     // ����� 60 ������.
    };

    // ���������� �����, ����� ������� ���� ������������� ����������, ���� �� ���� ������ �������.
    static int Time();
};


// ��� �������� � �������� �� ���������
struct LinkingRShift
{
    enum E
    {
        Voltage,      // �������� ��������� � ����������
        Position      // �������� ���������� � ������� �� ������
    };

    static bool IsVoltage();
};


// ���������� � ������������ �� ������ ����������� ���������.
struct MeasuresNumber
{
    enum E
    {
        _1,             // 1 ��������� ����� �����.
        _2,             // 2 ��������� ����� �����.
        _1_5,           // 1 ������ � 5 �����������.
        _2_5,           // 2 ������ �� 5 ���������.
        _3_5,           // 3 ������ �� 5 ���������.
        _6_1,           // 6 ����� �� 1 ���������.
        _6_2            // 6 ����� �� 2 ���������.
    } value;

    bool Is1()   const { return value == _1; };
    bool Is6_1() const { return value == _6_1; };
    bool Is6_2() const { return value == _6_2; };
    bool Is1_5() const { return value == _1_5; };
    bool Is2_5() const { return value == _2_5; };
    bool Is3_5() const { return value == _3_5; };

    operator int() const { return (int)value; }
};


// ����, �� ������� ��������� ��������
struct MeasuresField
{
    enum E
    {
        Screen,         // ��������� ����� ������������� �� ��� ����� �������, ������� ����� �� ������.
        AllMemory,      // ��������� ����� ������������� �� ����� �������.
        Hand            // ��������� ����� ������������� �� ����, ����������� �������������.
    } value;

    bool IsHand() const { return value == Hand; };
};


// ����� ������.
struct ModeWork
{
    enum E
    {
        Direct,        // �������� �����.
        Latest,        // � ���� ������ ����� ����������� ��������� ����������� ���������.
        MemInt,        // � ���� ������ ����� ��������� �� flash-������ ��������� ������������� ����� �����������.
    };

    static E Get();
    static bool IsDirect();
    static bool IsLatest();
    static bool IsMemInt();
};


// ����� ������������ ������.
struct FileNamingMode
{
    enum E
    {
        Mask,        // ��������� �� �����.
        Manually     // ��������� �������.
    };

    static bool IsMask();
    static bool IsHand();
};


// ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������.
struct ModeShowIntMem
{
    enum E
    {
        Direct,  // ���������� ������ ��������� �������.
        Saved,   // ���������� ����������� ������.
        Both     // ���������� ������ ��������� ������� � ����������� ������.
    };

    static E Get();
};


// ��� ������ ��� ������� ������ ������.
struct ModeBtnMemory
{
    enum E
    {
        Menu,     // ����� ������������ ��������������� �������� ����.
        Save      // ���������� ����������� ������ �� ������.
    };

    static E Get();

    static bool IsSave();
};


// ��� ��������� ������ �� ������.
struct ModeSaveSignal
{
    enum E
    {
        BMP,     // ��������� ������ �� ������ � ������� .bmp.
        TXT      // ��������� ������ �� ������ � ���������� ����.
    };

    static bool IsBMP();
};


// ������� ����� �����/���.
struct FunctionTime {
    enum E
    {
        Time,              // ����� ��������� ��������� �� �������.
        ShiftInMemory      // ����� ��������� ������������ �� ������ �������� ������.
    };

    static E Get();
    static bool IsShiftInMemory();
};


// ����� ������ �������� ���������.
struct PeackDetMode
{
    enum E
    {
        Disable,
        Enable,
        Average
    };

    static void Set(PeackDetMode::E mode);
    static PeackDetMode::E Get();
    static bool IsEnabled();
};


// �������� ������������� � ������.
struct TPos
{
    enum E
    {
        Left,      // ������������� ��������� � ������ ������.
        Center,    // ������������� ��������� � ������ ������.
        Right      // ������������� ��������� � ����� ������.
    };

    static E Get();
    static bool IsLeft();
    static bool IsCenter();
    static bool IsRight();

    // ������ �������� ���������� ������ ������������� � ������
    static int InPoints(PeackDetMode::E peakDet, int numPoints, E tPos);
};


struct Channel
{
    enum E
    {
        A,
        B,
        A_B,
        Math
    } value;

    Channel(E v = A) : value(v) {}
    void Enable() const;
    void Disable() const;
    bool IsEnabled() const;
    bool IsA() const     { return (value == A); }
    bool IsB() const     { return (value == B); }
    bool IsAB() const    { return (value == A_B); }
    bool IsMath() const  { return (value == Math); }
    operator int() const { return (int)value; }
    Color GetColor() const;
    bool IsInversed() const;
    int ToNumber() const;
};

extern Channel ChA;
extern Channel ChB;


// ��������.
struct Divider {
    enum E
    {
        _1,
        _10
    };

    static int ToAbs(Divider::E divider);
    static int ToAbs(const Channel &ch);

    static E Get(const Channel &ch);
    static E GetA();
    static E GetB();
};

// ����� ������ �� �����.
struct ModeCouple
{
    enum E
    {
        DC,      // �������� ����.
        AC,      // �������� ����.
        GND      // ���� �������.
    };

    static void Set(const Channel &ch, ModeCouple::E mode);

    static ModeCouple::E Get(const Channel &ch);
    static ModeCouple::E GetA();
    static ModeCouple::E GetB();
};


// ������� �� ����������.
struct Range
{
    enum E
    {
        _2mV,
        _5mV,
        _10mV,
        _20mV,
        _50mV,
        _100mV,
        _200mV,
        _500mV,
        _1V,
        _2V,
        _5V,
        _10V,
        _20V,
        Count
    };

    static void Load(const Channel &ch);

    static void Set(const Channel &ch, Range::E range);

    static Range::E Get(const Channel &ch);
    static Range::E GetA();
    static Range::E GetB();
    static Range::E GetMath();

    static bool Increase(const Channel &ch);

    static bool Decrease(const Channel &ch);

    static pchar Name(Range::E range);

    static pchar ToString(Range::E range, Divider::E multiplier);
};

Range::E &operator++(Range::E &range);
Range::E &operator--(Range::E &range);


struct RShift
{
    static void Set(const Channel &ch, int16 rShift);

    static int16 Get(const Channel &ch);

    static int16 GetMath();

    static void Load(const Channel &ch);

    static int ToRel(float rshift_abs, Range::E range);

    static String ToString(int16 rShiftRel, Range::E range, Divider::E multiplier);

    static void Draw();

    static bool show_level_A;    // ����� �� �������� �������������� ����� ������ �������� ������� ������
    static bool show_level_B;
    static bool draw_markers;

private:

    static void Draw(const Channel &ch);
};


// ����� �������.
struct StartMode {
    enum E
    {
        Auto,             // ��������������.
        Wait,             // ������.
        Single            // �����������.
    };

    static E Get();

    static bool IsAuto();
    static bool IsSingle();
};

// �������� �������������.
struct TrigSource
{
    enum E
    {
        A,      // ����� 1.
        B,      // ����� 2.
        Ext     // �������.
    };

    static void Set(E source);

    static E Get();
    static bool IsA();
    static bool IsB();
    static bool IsExt();
};


// ���������� ������������
struct TrigPolarity
{
    enum E
    {
        Front,  // ������������� �� ������.
        Back    // ������������� �� �����.
    };

    static void Set(E polarity);

    static E Get();

    static bool IsFront();

    // ������������ ���������� - ��������������� ������ ����� ����������
    static void Switch();

    static void Load();

    static uint timeSwitch;     // ����� ��������������� ������� ����� ����������
};


// ���� �������������.
struct TrigInput
{
    enum E
    {
        Full,   // ������ ������.
        AC,     // ����������.
        LPF,    // ���.
        HPF     // ���.
    };

    static void Set(E input);

    static E Get();

    static bool IsAC();
};


// ����� ����������� ������� ������ �����.
struct ModeLongPressTrig {
    enum E
    {
        Level0,   // ����� ������ ������������� � 0.
        Auto      // �������������� ����� ������������� - ������� ��������������� ���������� ����� ���������� � ���������.
    };

    static E Get();

    static bool IsLevel0();
};


// ����� ��������� �������������.
struct TrigModeFind {
    enum E
    {
        Hand,   // ������� ������������� ��������������� �������.
        Auto    // ���������� ������ ������������� ������������ ������������� ����� ������� ������ ���������� �������
    };

    static E Get();

    static bool IsHand();
};


struct TShift
{
    static void Set(int tshift);

    static int16 Get();

    static void Load();

    // ���������� ���������� �������� �� ������� ��� ������ �������������. � ������ �������� ��� �������� ������ ����
    // ������.
    static void SetDelta(int16 shift);

    // ���������� ������������� �������� �� ������� � ��������� ����, ��������� ��� ������ �� �����
    static String ToString(int16 tshift_rel);

    // �������� �� ������� � ������ ������. �.�. �� ����� � ��� �������� ��� ��� ���������, ��� �������� ���������� ��
    // ����������� � ���������� ����� � ��� ����
    static int InPoints(PeackDetMode::E peakDet);

    // ����������� �������� �� �������, ������� ����� ���� �������� � ���������� �����.
    static int16 Min();

    // �������� �� �������, ��������������� ������� TPos.
    static int16 Zero();

    // �������� ������������� ������� ������� ������������ ������ ������.
    static int16 GetInMemory();
    static void SetInMemory(int16 shift);

    static const int NULL_VALUE = 1000000;
};


// ������� �� �������
struct TBase
{
    enum E
    {
        _1ns,
        _2ns,
        _5ns,
        _10ns,
        _20ns,
        _50ns,
        _100ns,
        _200ns,
        _500ns,
        _1us,
        _2us,
        _5us,
        _10us,
        _20us,
        _50us,
        _100us,
        _200us,
        _500us,
        _1ms,
        _2ms,
        _5ms,
        _10ms,
        _20ms,
        _50ms,
        _100ms,
        _200ms,
        _500ms,
        _1s,
        _2s,
        _5s,
        _10s,
        Count
    };

    static void Set(TBase::E tbase);

    static TBase::E Get();

    static void Load();

    static void Decrease();

    static void Increase();

    static pchar Name(TBase::E tbase);

    static pchar ToString(TBase::E tbase);
    static pchar ToStringEN(TBase::E tbase);
};


// ����� �������� ���������.
struct CursCntrl
{
    enum E
    {
        _1,            // ������.
        _2,            // ������.
        _1_2,          // ������.
        Disable        // �������.
    } value;

    bool IsDisable() const { return value == Disable; }
    bool Is1()       const { return value == _1; }
    bool Is2()       const { return value == _2; }
    bool Is1_2()     const { return value == _1_2; }
};


// ������������ ����������� ��������.
struct CursMovement
{
    enum E
    {
        Points,    // �� ������.
        Percents   // �� ���������.
    } value;

    bool IsPercents() const { return value == Percents; };
};


// ����� ������� ������ �������. ����� �������, �� � ����� ������������ �� �������� ����� ���������.
struct CursActive
{
    enum E
    {
        U,
        T,
        None
    } value;

    CursActive(E v = U) : value(v) {}

    bool IsT() const { return value == T; }
    bool IsU() const { return value == U; }
};


// ����� �������� ��������.
struct CursLookMode
{
    enum E
    {
        None,      // ������� �� ������.
        Voltage,   // ������� ������ �� ����������� �������������.
        Time,      // ������� ������ �� �������� �������������.
        Both       // ������� ������ �� �������� � �����������, � ����������� �� ����, ����� ������� ������� ���������.
    } value;

    bool IsVoltage() const { return value == Voltage; }
    bool IsTime() const    { return value == Time; }
    bool IsBoth() const    { return value == Both; }
};


// ������� �� ������� ��� ������ ���������.
struct ModeViewSignals
{
    enum E
    {
        AsIs,       // ���������� ������� ��� ����
        Compress    // ������� ����� � ���������
    };

    static E Get();

    static bool IsCompress();
};


struct ScaleMath { enum E
{
    Channel0,     // ������� ������ �� ������ 1
    Channel1,     // ������� ������ �� ������ 2
    Hand          // ������� ������� �������.
};};


// ��� ������������ ��� �������.
// ���� � ���, ��� ������ ��� �� ��������� ��-�� ������� ������������� ( ? ), ������� �� ������ �������������� �������� ��� ������ �� ��� ������.
struct BalanceADCtype
{
    enum E
    {
        Disable,     // ������������ ���������.
        Settings,    // ������������ �������� ������������, ������� �������� �������������.
        Hand         // ������������ �������� ������������, �������� �������.
    };

    static E Get();
};


// ��� ��������������� ��������
struct RShiftADCtype { enum E
{
    Disable,
    Settings,
    Hand
};};


// ������� ����� ��� ����������� �������.
struct ScaleFFT {
    enum E
    {
        Log,        // ��� �������� �������� ��������������� ����� ������ �������.
        Linear      // ��� �������� �������� �������� ����� ������ �������.
    };

    static E Get();

    static bool IsLog();

    static bool IsLinear();
};


// ��� ���� ��� ��������� ������� ����� �������� �������.
struct WindowFFT {
    enum E
    {
        Rectangle,  // ����� �������� ������� ������ �� ���������� �� ����.
        Hamming,    // ����� �������� ������� ������ ���������� �� ���� ��������.
        Blackman,   // ����� �������� ������� ������ ���������� �� ���� ��������.
        Hann        // ����� �������� ������� ������ ���������� �� ���� �����.
    };

    static bool IsHamming();
    static bool IsBlackman();
    static bool IsHann();
};


// ����������� �������� �� ������������ ���������� ������� �������.
struct FFTmaxDB {
    enum E
    {
        _40,        // ������������ ���������� �� -40��
        _60,        // ������������ ���������� �� -60��
        _80         // ������������ ���������� �� -80��
    };

    static E Get();
    static float GetReal();
};


// ����� ������� ������������.
struct Function
{
    enum E
    {
        Sum,        // ����� ���� �������.
        Mul         // ������������ ���� �������.
    };

    static E Get();

    static bool IsSum();
    static bool IsMul();
};


// ��� ������������ �������������� ������.
struct ModeDrawMath
{
    enum E
    {
        Disable,    // ����������� ��������������� ������� ���������.
        Separate,   // �������������� ������ ������������ �� ��������� ������.
        Together    // �������������� ������ ������������ ������ �������� �������.
    };

    static E Get();

    static bool IsDisabled();
    static bool IsSeparate();
};


// ��� ��������� ����� ���������, ����� �������� �������������� �������.
struct ModeRegSet
{
    enum E
    {
        Range,      // ����� ��������� ��������� ��������� �� ����������.
        RShift      // ����� ��������� ��������� ��������� �� ����������.
    };

    static E Get();

    static bool IsRShift();
    static bool IsRange();
};


// ����� ������ �����������.
struct CalibratorMode
{
    enum E
    {
        Freq,           // �� ������ ����������� 4�, 1���.
        DC,             // �� ������ ����������� 4� ����������� ����������.
        GND             // �� ������ ����������� 0�.
    };

    static void Set(CalibratorMode::E calibratorMode);

    static E Get();
};


struct TrigLev
{
    static void Load();

    static void Set(TrigSource::E ch, int16 trigLev);

    static int16 Get(TrigSource::E ch);
    static int16 GetA();
    static int16 GetB();
    // ���������� ������� ������������� �� �������� ��������� �������������
    static int16 Get();

    // ����� � ���������� ������� ������������� �� ���������� ���������� �������
    static void FindAndSet();

    static bool need_auto_find;   // ���� true, �� ����� ���������� ����� �������������
};


struct RegUPR
{
    static void Load();
};


// ������ �� ����� ������
struct ChannelFiltr
{
    static void Enable(const Channel &ch, bool enable);
};


