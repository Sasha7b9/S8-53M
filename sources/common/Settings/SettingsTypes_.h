// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Colors_.h"
#include "common/Utils/Containers/String_.h"
#include "common/Utils/Containers/Values_.h"


//#define NumChannels 2


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
struct EnumPointsFPGA
{
    enum E
    {
        _281,
        _512,
        _1k,
        _2k,
        _4k,
        _8k,
        _16k,
        Count
    } value;

    EnumPointsFPGA(E v = _1k) : value(v) {}

    static uint ToPoints(E);
    static E FromPoints(uint num_points);

    uint PointsInChannel();
    uint BytesInChannel();
    // ������� ���� ����� ��� ���������� ������ ����� �������
    uint BytesForData();

    operator E() const { return value; }
};


// �������� ������� ��� ����������� �������.
struct SourceFFT {
    enum E
    {
        A_,          // ������ � ����������� ������� ������� ������.
        B_,          // ������ � ����������� ������� ������� ������.
        Both_        // ������ � ����������� �������� ����� �������.
    } value;
};


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
        Points,     // ������ �������� �������.
        Count
    } value;

    ModeDrawSignal(E v = Lines) : value(v) {}

    bool IsLines() const { return value == Lines; };

    operator E() const { return value; }
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
    } value;

    bool Is1() const { return value == _1; };
    bool Is2() const { return value == _2; };
    bool Is3() const { return value == _3; };

    operator E() const { return value; }
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
    } value;

    ENumAccumulation(E v = _1) : value(v) {}

    bool IsNone() const     { return value == _1; };
    bool IsInfinity() const { return value == Infinity; };

    operator E() const { return value; }
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
    } value;

    ENumAveraging(E v = _1) : value(v) {}

    int NumAverages();
    int ToAbs();

    operator E() const { return value; }
};


// ��� ���������� �� ����������.
struct ModeAveraging
{
    enum E
    {
        Accurately,   // ��������� �����.
        Around        // ��������� ��������������.
    } value;

    ModeAveraging(E v = Accurately) : value(v) {}

    // ���������� ����� ����������
    static ModeAveraging::E Current();

    operator E() const { return value; }
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
    } value;

    ENumMinMax(E v = _1) : value(v) {}

    int ToAbs();

    operator E() const { return value; }
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
    } value;

    Smoothing(E v = Disable) : value(v) {}

    // ���������� ����� ����� ������������� ������� (����� ������� - �����������).
    static uint NumPoints();

    bool IsDisable() const { return value == Disable; }

    operator E() const { return value; }
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
    } value;

    ENumSignalsInSec(E v = _25) : value(v) {}

    // ���������� ����������� ������� ������.
    int NumSignalsInS();

    // ���������� ���������� ������� ����� �����������
    uint DeltaTime();

    operator E() const { return value; }
};


// ����� ����������.
struct ModeAccumulation
{
    enum E
    {
        NoReset,   // � ���� ������ ������������ ������ N ��������� ���������.
        Reset      // � ���� ������ ���������� N ��������� ��������� � ����� ������������.
    } value;

    ModeAccumulation(E v = NoReset) : value(v) {}

    bool IsNoReset() const { return value == NoReset; };
    bool IsReset()   const { return value == Reset; };

    operator E() const { return value; }
};


// ����� ����������� �������������� ������� �������� �������� �� ����������.
struct AltMarkers
{
    enum E
    {
        Hide,        // ������ �� ��������.
        Show,        // ������ ��������.
        AutoHide     // �������� � ������� ����� timeShowLevels.
    } value;

    AltMarkers(E v = Hide) : value(v) {}

    bool IsHide() const { return value == Hide; };
};


// ����� ����� ����� ����� ���������� ������� ������ �������� ����.
struct MenuAutoHide
{
    enum E
    {
        None,   // �������.
        _5,     // ����� 5 ������.
        _10,    // ����� 10 ������.
        _15,    // ����� 15 ������.
        _30,    // ����� 30 ������.
        _60     // ����� 60 ������.
    } value;

    MenuAutoHide(E v = None) : value(v) {}

    // ���������� �����, ����� ������� ���� ������������� ����������, ���� �� ���� ������ �������.
    int Time();
};


// ��� �������� � �������� �� ���������
struct LinkingRShift
{
    enum E
    {
        Voltage,      // �������� ��������� � ����������
        Position      // �������� ���������� � ������� �� ������
    } value;

    bool IsVoltage() const { return value == Voltage; };
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
        Latest,        // � ���� ������ ����� ����������� ��������� ���������� ���������.
        MemInt,        // � ���� ������ ����� ��������� �� flash-������ ��������� ������������� ����� ����������.
    } value;

    ModeWork(E v = Direct) : value(v) {}

    bool IsDirect() const { return value == Direct; };
    bool IsLatest() const { return value == Latest; };
    bool IsMemInt() const { return value == MemInt; };

    operator E() const { return value; }
};


// ����� ������������ ������.
struct FileNamingMode
{
    enum E
    {
        Mask,        // ��������� �� �����.
        Manually     // ��������� �������.
    } value;

    FileNamingMode(E v = Mask) : value(v) {}

    bool IsMask() const { return value == Mask; };
    bool IsHand() const { return value == Manually; };
};


// ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������.
struct ModeShowIntMem
{
    enum E
    {
        Direct,  // ���������� ������ ��������� �������.
        Saved,   // ���������� ���������� ������.
        Both     // ���������� ������ ��������� ������� � ���������� ������.
    } value;

    ModeShowIntMem(E v = Direct) : value(v) {}

    bool IsDirect() const { return value == Direct; }
    bool IsSaved() const { return value == Saved; }
    bool IsBoth() const { return value == Both; }
};


// ��� ������ ��� ������� ������ ������.
struct ModeBtnMemory
{
    enum E
    {
        Menu,     // ����� ������������ ��������������� �������� ����.
        Save      // ���������� ����������� ������ �� ������.
    } value;

    ModeBtnMemory(E v = Menu) : value(v) {}

    bool IsSave() const { return value == Save; };
};


// ��� ��������� ������ �� ������.
struct ModeSaveSignal
{
    enum E
    {
        BMP,     // ��������� ������ �� ������ � ������� .bmp.
        TXT      // ��������� ������ �� ������ � ���������� ����.
    } value;

    ModeSaveSignal(E v = BMP) : value(v) {}

    bool IsBMP() const { return value == BMP; }
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
        Average,
        Count
    } value;

    PeackDetMode(E v = Disable) : value(v) {}

    static void Set(PeackDetMode::E mode);
    static PeackDetMode::E Get();
    static bool IsEnabled();

    operator E() const { return value; }
};


// �������� ������������� � ������.
struct TPos
{
    enum E
    {
        Left,      // ������������� ��������� � ������ ������.
        Center,    // ������������� ��������� � ������ ������.
        Right,     // ������������� ��������� � ����� ������.
        Count
    } value;

    TPos(E v = Center) : value(v) {}

    static bool IsLeft();
    static bool IsCenter();
    static bool IsRight();

    // ������ �������� ���������� ������ ������������� � ������
    static int InPoints(EnumPointsFPGA::E enum_points, E t_pos);

    static void Draw();

    static void Draw(int x, int y);

    operator E() const { return value; }
};


struct Channel
{
    enum E
    {
        A,
        B,
        A_B,
        Math,
        Count = 2
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

    // ������ ����� ������ ����� ������������ �������
    static Channel &FirstDrawing();

    // ������ ����� ������ ����� ������������ ����������
    static Channel &SecondDrawing();
};

extern Channel ChA;
extern Channel ChB;


// ��������.
struct Divider
{
    enum E
    {
        _1,
        _10
    } value;

    Divider(E v = _1) : value(v) {}

    operator E() const { return value; }

    static int ToAbs(Divider::E divider);
    static int ToAbs(const Channel &ch);
};

// ����� ������ �� �����.
struct ModeCouple
{
    enum E
    {
        DC,      // �������� ����.
        AC,      // �������� ����.
        GND      // ���� �������.
    } value;

    ModeCouple(E v = DC) : value(v) {}

    operator E() const { return value; }

    static void Set(const Channel &ch, ModeCouple::E mode);
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
    } value;

    Range(E v = _2mV) : value(v) {}

    operator E() const { return value; }

    static void Load(const Channel &ch);

    static void Set(const Channel &ch, Range::E range);

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

    static int16 GetMath();

    static void Load(const Channel &ch);

    static int ToRel(float rshift_abs, Range::E range);

    static Voltage ToAbs(int16 rshift, Range::E range);

    static Voltage ToAbs(int16 rshift, Range::E range, Divider::E multiplier);

    static void Draw();

    static void DrawCursors();

    static bool show_level[Channel::Count];    // ����� �� �������� �������������� ����� ������ �������� ������� ������
    static bool draw_markers;

    static const int16 MIN = 20;    // ����������� �������� �������� ������ �� ����������, ���������� � ���������� �����
                                    // ������������� �������� 10 ������ ���� �� ����������� �����
    static const int16 ZERO = 500;  // ������������� ������������ ������ �� ������ ������.
    static const int16 MAX = 980;   // ������������� �������� 10 ������ ����� �� ����������� �����.

    static int16 STEP();            // �� ������� ������ ����� �������� �������� ��������, ����� ������ �������� ��
                                    // ���������� ������������ �� ���� �����

private:

    static void Draw(const Channel &ch);

    static void OnChanged(const Channel &ch);

    static void FuncOnTimerDisableShowA();
    static void FuncOnTimerDisableShowB();

    // ��������� ��������������� ����� ������� �������� �� ����������
    static void DisableShowLevel(const Channel &ch);

    static const float abs_step[Range::Count];
};


// ����� �������.
struct StartMode {
    enum E
    {
        Auto,             // ��������������.
        Wait,             // ������.
        Single            // �����������.
    } value;

    StartMode(E v = Auto) : value(v) {}

    bool IsAuto() const { return value == Auto; };
    bool IsSingle() const { return value == Single; }

    operator E() const { return value; }
};

// �������� �������������.
struct TrigSource
{
    enum E
    {
        A,      // ����� 1.
        B,      // ����� 2.
        Ext     // �������.
    } value;

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
    } value;

    TrigPolarity(E v = Front) : value(v) {};

    static void Set(E polarity);

    bool IsFront() const { return value == Front; };

    // ������������ ���������� - ��������������� ������ ����� ����������
    static void Switch();

    static void Load();

    operator E() const { return value; }

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
    } value;

    static void Set(E input);

    static E Get();

    static bool IsAC();
};


// ����� ����������� ������� ������ �����.
struct ModeLongPressTrig {
    enum E
    {
        Level0,  // ����� ������ ������������� � 0.
        Auto     // �������������� ����� ������������� - ������� ��������������� ���������� ����� ���������� � ���������
    } value;

    static E Get();

    static bool IsLevel0();
};


// ����� ��������� �������������.
struct TrigModeFind {
    enum E
    {
        Hand,   // ������� ������������� ��������������� �������.
        Auto    // ���������� ������ ������������� ������������ ������������� ����� ������� ������ ���������� �������
    } value;

    static E Get();

    static bool IsHand();
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
    } value;

    TBase(E v = TBase::_1ns) : value(v) {}

    static void Set(TBase::E tbase);

    static void Load();

    static void Decrease();

    static void Increase();

    // ���������� true, ���� ����������� ��������������� ��������
    static bool IsRandomize();

    // ���������� true, ���� ����������� ���������� ��������
    static bool IsP2P();

    // ���������� true, ���� �������� ����� �������� ���������
    static bool IsPeakDetModeAllowed();

    static pchar Name(TBase::E tbase);

    static pchar ToString(TBase::E tbase);
    static pchar ToStringEN(TBase::E tbase);

    static E MIN_P2P;       // � ���� �������� ���������� ���������� �����
    static E MIN_PEAK_DET;  // � ���� �������� ����� �������� ������� ��������
    static E MAX_RANOMIZE;  // ������������ ��������, �� ������� �������� ������������

    // ���������� ��� ����� ������� ��� ������������� ��� ������������� ��������
    static int StepRand();

    operator E() const { return value; }
};


struct TShift
{
    static void Set(int tshift);

    static void Draw();

    // ���������� ���������� �������� �� ������� ��� ������ �������������. � ������ �������� ��� �������� ������ ����
    // ������.
    static void SetDelta(int16 shift);

    // ���������� ������������� �������� �� ������� � ��������� ����, ��������� ��� ������ �� �����
    static String ToString(int tshift_rel);

    // ����������� �������� �� �������, ������� ����� ���� �������� � ���������� �����.
    static int16 Min();

    // �������� �� �������, ��������������� ������� TPos.
    static int16 Zero();

    // �������� �� �������, ������������ ��� ������� � ����/�����- �������
    static int ForLaunchFPGA();

    // tshift ����� ��������� ������� �������� � ���������� �������
    static Time ToAbs(float shift, TBase::E base);
    static Time ToAbs(int shift, TBase::E base);

    static float ToRel(float tshift_abs, TBase::E tbase);

    static const int NULL_VALUE = 1000000;

    static const int16 MAX = 16000;

    static const float abs_step[TBase::Count];

    static int add_rand;                // �������������� �������� � ������ �������������
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
    } value;

    bool IsCompress() const { return value == Compress; };
};


struct ScaleMath { enum E
{
    Channel0,     // ������� ������ �� ������ 1
    Channel1,     // ������� ������ �� ������ 2
    Hand          // ������� ������� �������.
};};


// ��� ������������ ��� �������.
// ���� � ���, ��� ������ ��� �� ��������� ��-�� ������� ������������� ( ? ), ������� �� ������ �������������� ��������
// ��� ������ �� ��� ������.
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
struct ScaleFFT
{
    enum E
    {
        Log,        // ��� �������� �������� ��������������� ����� ������ �������.
        Linear      // ��� �������� �������� �������� ����� ������ �������.
    } value;

    bool IsLog()    const { return value == Log; };
    bool IsLinear() const { return value == Linear; };
};


// ��� ���� ��� ��������� ������� ����� �������� �������.
struct WindowFFT
{
    enum E
    {
        Rectangle,  // ����� �������� ������� ������ �� ���������� �� ����.
        Hamming,    // ����� �������� ������� ������ ���������� �� ���� ��������.
        Blackman,   // ����� �������� ������� ������ ���������� �� ���� ��������.
        Hann        // ����� �������� ������� ������ ���������� �� ���� �����.
    } value;

    bool IsHamming()  const { return value == Hamming; };
    bool IsBlackman() const { return value == Blackman; };
    bool IsHann()     const { return value == Hann; };
};


// ����������� �������� �� ������������ ���������� ������� �������.
struct MaxDbFFT
{
    enum E
    {
        _40,        // ������������ ���������� �� -40��
        _60,        // ������������ ���������� �� -60��
        _80         // ������������ ���������� �� -80��
    } value;

    float GetReal();
    operator E() const { return value; }
};


// ����� ������� ������������.
struct Function
{
    enum E
    {
        Sum,        // ����� ���� �������.
        Mul         // ������������ ���� �������.
    } value;

    bool IsSum() const { return value == Sum; };
    bool IsMul() const { return value == Mul; };
    operator E() const { return value; }
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

    static void DrawCursor();

    static void WriteValue();

    static bool need_auto_find;   // ���� true, �� ����� ���������� ����� �������������

    static const int16 MIN = RShift::MIN;
    static const int16 MAX = RShift::MAX;
    static const int16 ZERO = RShift::ZERO;     // ������� �������� ������ �������������, ���������� � ������.
                          // ������ ������������� ��� ���� ���������� �� ����� ������ � �������� �������� �� ����������.

    static bool exist_impulse;          // ���� true, ����� ���������� ������ ������� ��������������

    static bool show_level;             // ����� �� �������� �������������� ����� ������ �������� ������ �������������
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


// ��������� ��� ���������� ����/�����- ���������
struct LaunchFPGA
{
    // ��������� � ���������� ����� ����- � �����- �������
    static void Load();

    static uint16 Pred() { return (uint16)pred; }

    // ���������� ��������, ������� ��� ������ � ����
    static uint16 PredForWrite();
    static uint16 PostForWrite();

    // �� ������� ������ ����� ������� ������
    static int DeltaReadAddress();

    // �������������� �������� ��� ������ ����������� ����� (����� ��� ����, ����� ������, ��� � ������ �������������
    // ��������� tShift �������� ����������� �� ������ �� �� 1 �������)
    static int AdditionalOffsetIndexFirst();

private:

    static void Calculate();

    static void CalculateReal();

    static void CalculateRandomize();

    static int pred;
    static int post;

    static const int8 d_pred[TBase::Count];   // �������������� �������� ��� �����������
    static const int8 d_post[TBase::Count];   // �������������� �������� ��� ������������
    static const int8 d_read[TBase::Count];   // �������������� �������� ��� ������ ������
};


struct CalibrationMode
{
    enum E
    {
        x1,
        x10,
        Disable
    };
};
