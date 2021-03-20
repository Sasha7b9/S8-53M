#pragma once
#include "common/Utils/String_.h"


#define NumChannels 2


// ��������.
struct Divider { enum E
    {
        _1,
        _10
    };
    static int ToAbs(Divider::E divider);
};


// ��� ������� ��� ������ �������������.
struct SampleType { enum E
{
    Real,   // �������� ����� - � ���������� ��������� ������ ������� ��������� �����, ������ �� ��������������.
    Equal   // ������������� - ������ �������� �� ��������� ������, ���������� �� �������������.
};};


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
struct StretchADCtype { enum E
{
    Disable,
    Settings,
    Hand
};};


// ����� ��������� �������.
struct ModeDrawSignal {enum E
{
    Lines,      // ������ �������� �������.
    Points      // ������ �������� �������.
};};


// ��� ����� �� ������.
struct TypeGrid { enum E
{
    _1,
    _2,
    _3,
    _4,
    Count
};};


// �������� �����
struct ColorScheme { enum E
{
    WhiteLetters,   // � ���� ������ ��������� ��������� ���� ������� ����� - �� ����� ������ ����� �����.
    BlackLetters    // � ���� ������ ��������� ��������� ���� ������� ������ - �� ����� ������� ��������.
};};


// ������������ ����������.
struct ENumAccumulation { enum E
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
};};


// ���������� ���������� �� ����������.
struct ENumAveraging { enum E
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
struct ENumMinMax { enum E
{
    _1,
    _2,
    _4,
    _8,
    _16,
    _32,
    _64,
    _128
};};


// ���������� ����� ��� ������� ����������� �������.
struct Smoothing { enum E
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
};


// ����������� FPS.
struct ENumSignalsInSec { enum E
    {
        _25,
        _10,
        _5,
        _2,
        _1
    };

    // ���������� ����������� ������� ������.
    static int NumSignalsInS();
};


// ����� ����������.
struct ModeAccumulation { enum E
{
    NoReset,   // � ���� ������ ������������ ������ N ��������� ���������.
    Reset      // � ���� ������ ���������� N ��������� ��������� � ����� ������������.
};};


// ����� ����������� �������������� ������� �������� �������� �� ����������.
struct AltMarkers { enum E
{
    Hide,        // ������ �� ��������.
    Show,        // ������ ��������.
    AutoHide     // �������� � ������� ����� timeShowLevels.
};};


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
struct LinkingRShift { enum E
{
    Voltage,      // �������� ��������� � ����������
    Position      // �������� ���������� � ������� �� ������
};};


// ���������� � ������������ �� ������ ����������� ���������.
struct MeasuresNumber { enum E
{
    _1,             // 1 ��������� ����� �����.
    _2,             // 2 ��������� ����� �����.
    _1_5,           // 1 ������ � 5 �����������.
    _2_5,           // 2 ������ �� 5 ���������.
    _3_5,           // 3 ������ �� 5 ���������.
    _6_1,           // 6 ����� �� 1 ���������.
    _6_2            // 6 ����� �� 2 ���������.
};};


// ����, �� ������� ��������� ��������
struct MeasuresField { enum E
{
    Screen,         // ��������� ����� ������������� �� ��� ����� �������, ������� ����� �� ������.
    AllMemory,      // ��������� ����� ������������� �� ����� �������.
    Hand            // ��������� ����� ������������� �� ����, ����������� �������������.
};};


// ����� ������.
struct ModeWork { enum E
{
    Direct,        // �������� �����.
    Latest,        // � ���� ������ ����� ����������� ��������� ���������� ���������.
    MemInt,        // � ���� ������ ����� ��������� �� flash-������ ��������� ������������� ����� ����������.
};};


// ����� ������������ ������.
struct FileNamingMode { enum E
{
    Mask,        // ��������� �� �����.
    Manually     // ��������� �������.
};};


// ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������.
struct ModeShowIntMem { enum E
{
    Direct,  // ���������� ������ ��������� �������.
    Saved,   // ���������� ���������� ������.
    Both     // ���������� ������ ��������� ������� � ���������� ������.
};};


// ��� ������ ��� ������� ������ ������.
struct ModeBtnMemory { enum E
{
    Menu,     // ����� ������������ ��������������� �������� ����.
    Save      // ���������� ����������� ������ �� ������.
};};


// ��� ��������� ������ �� ������.
struct ModeSaveSignal { enum E
{
    BMP,     // ��������� ������ �� ������ � ������� .bmp.
    TXT      // ��������� ������ �� ������ � ���������� ����.
};};


// ������� ����� �����/���.
struct FunctionTime { enum E
{
    Time,              // ����� ��������� ��������� �� �������.
    ShiftInMemory      // ����� ��������� ������������ �� ������ �������� ������.
};};


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

    Channel(E v) : value(v) {}
    bool IsEnabled() const;
    bool IsA() const     { return (value == Channel::A); }
    bool IsB() const     { return (value == Channel::B); }
    bool IsMath() const  { return (value == Channel::Math); }
    operator int() const { return (int)value; }
    Color GetColor() const;
};

extern Channel ChA;
extern Channel ChB;

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
struct StartMode { enum E
{
    Auto,             // ��������������.
    Wait,             // ������.
    Single            // �����������.
};};

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
};


// ����� ����������� ������� ������ �����.
struct ModeLongPressTrig { enum E
{
    Level0,   // ����� ������ ������������� � 0.
    Auto      // �������������� ����� ������������� - ������� ��������������� ���������� ����� ���������� � ���������.
};};


// ����� ��������� �������������.
struct TrigModeFind { enum E
{
    Hand,   // ������� ������������� ��������������� �������.
    Auto    // ���������� ������ ������������� ������������ ������������� ����� ������� ������ ���������� �������
};};


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
struct CursCntrl { enum E
{
    _1,            // ������.
    _2,            // ������.
    _1_2,          // ������.
    Disable        // �������.
};};


// ������������ ����������� ��������.
struct CursMovement { enum E
{
    Points,    // �� ������.
    Percents   // �� ���������.
};};


// ����� ������� ������ �������. ����� �������, �� � ����� ������������ �� �������� ����� ���������.
struct CursActive { enum E
{
    U,
    T,
    None
};};


// ����� �������� ��������.
struct CursLookMode { enum E
{
    None,      // ������� �� ������.
    Voltage,   // ������� ������ �� ����������� �������������.
    Time,      // ������� ������ �� �������� �������������.
    Both       // ������� ������ �� �������� � �����������, � ����������� �� ����, ����� ������� ������� ���������.
};};


// ������� �� ������� ��� ������ ���������.
struct ModeViewSignals { enum E
{
    AsIs,       // ���������� ������� ��� ����
    Compress    // ������� ����� � ���������
};};


struct ScaleMath { enum E
{
    Channel0,     // ������� ������ �� ������ 1
    Channel1,     // ������� ������ �� ������ 2
    Hand          // ������� ������� �������.
};};


// ��� ������������ ��� �������.
// ���� � ���, ��� ������ ��� �� ��������� ��-�� ������� ������������� ( ? ), ������� �� ������ �������������� �������� ��� ������ �� ��� ������.
struct BalanceADCtype { enum E
{
    Disable,     // ������������ ���������.
    Settings,    // ������������ �������� ������������, ������� �������� �������������.
    Hand         // ������������ �������� ������������, �������� �������.
};};


// ��� ��������������� ��������
struct RShiftADCtype { enum E
{
    Disable,
    Settings,
    Hand
};};


// ������� ����� ��� ����������� �������.
struct ScaleFFT { enum E
{
    Log,        // ��� �������� �������� ��������������� ����� ������ �������.
    Linear      // ��� �������� �������� �������� ����� ������ �������.
};};


// ��� ���� ��� ��������� ������� ����� �������� �������.
struct WindowFFT { enum E
{
    Rectangle,  // ����� �������� ������� ������ �� ���������� �� ����.
    Hamming,    // ����� �������� ������� ������ ���������� �� ���� ��������.
    Blackman,   // ����� �������� ������� ������ ���������� �� ���� ��������.
    Hann        // ����� �������� ������� ������ ���������� �� ���� �����.
};};


// ����������� �������� �� ������������ ���������� ������� �������.
struct FFTmaxDB { enum E
{
    _40,        // ������������ ���������� �� -40��
    _60,        // ������������ ���������� �� -60��
    _80         // ������������ ���������� �� -80��
};};


// ����� ������� ������������.
struct Function { enum E
{
    Sum,        // ����� ���� �������.
    Mul         // ������������ ���� �������.
};};


// ��� ������������ �������������� ������.
struct ModeDrawMath { enum E
{
    Disable,    // ����������� ��������������� ������� ���������.
    Separate,   // �������������� ������ ������������ �� ��������� ������.
    Together    // �������������� ������ ������������ ������ �������� �������.
};};


// ��� ��������� ����� ���������, ����� �������� �������������� �������.
struct ModeRegSet { enum E
{
    Range,      // ����� ��������� ��������� ��������� �� ����������.
    RShift      // ����� ��������� ��������� ��������� �� ����������.
};};


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
