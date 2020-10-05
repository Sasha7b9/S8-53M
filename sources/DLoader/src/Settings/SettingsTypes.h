#pragma once


struct ColorScheme { enum E
{
    WhiteLetters,       // � ���� ������ ��������� ��������� ���� ������� ����� - �� ����� ������ ����� �����
    BlackLetters        // � ���� ������ ��������� ��������� ���� ������� ������ - �� ����� ������� ��������
};};

// ����� ��������� �������.
struct ModeDrawSignal { enum E
{
    Lines = 0,   // �������
    Points = 1   // �������
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

// ���������� ����������.
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
    _Infinity
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
};};

// ��� ���������� �� ����������
struct ModeAveraging { enum E
{
    Accurately,   // ��������� �����.
    Around        // ��������� ��������������.
}; };

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
}; };

// ���������� ����� ��� ������� �����������.
struct DisplaySmoothing { enum E
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
};};

// ����������� FPS.
struct ENumSignalsInSec { enum E
{
    _25,
    _10,
    _5,
    _2,
    _1
}; };

struct ModeAccumulation { enum E
{
    NoReset,   // � ���� ������ ������������ ������ N ���������
    Reset      // � ���� ������ ���������� N ��������� � ����� ������������
};};

// ����� ����������� �������������� ������� �������� �������� �� ���������� 
struct AltMarkers { enum E
{
    Hide,        // ������ �� ��������
    Show,        // ������ ��������
    AutoHide     // �������� � ������� ����� timeShowLevels
};};

// ����� ����� ����� ����� ���������� ������� ������ �������� ����
struct MenuAutoHide { enum E
{
    None   = 0,    // �������
    _5     = 5,    // ����� 5 ������
    _10    = 10,   // ����� 10 ������
    _15    = 15,   // ����� 15 ������
    _30    = 30,   // ����� 30 ������
    _60    = 60    // ����� 60 ������
};};


// ChannelX

struct Channel { enum E
{
    A,
    B,
    A_B,
    Math
};};

#define NumChannels 2

// ����� ������ �� �����
struct ModeCouple { enum E
{
    DC,      // �������� ����
    AC,      // �������� ����
    GND      // ���� �������
};};

// ������� �� ����������
struct Range { enum E
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
};};

// ����� �������
struct StartMode { enum E
{
    Auto,     // ��������������
    Wait,     // ������ 
    Single    // �����������
};};

// �������� �������������
struct TrigSource { enum E
{
    A,      // ����� 1
    B,      // ����� 2
    Ext     // �������
};};

// ��� ������������
struct TrigPolarity { enum E
{
    Front,     // �� ������
    Back       // �� �����
};};

// ���� �������������
struct TrigInput { enum E
{
    Full,         // ������ ������
    AC,           // ����������
    LPF,          // ���
    HPF           // ���
};};

struct TrigModeFind { enum E
{
    Hand,          // ������� ������������� ��������������� ������� ��� ������������� - ����������� ��������� ������
    Auto           // ���������� ������ ������������� ������������ ������������� ����� ������� ������ ���������� �������
};};

// ����� ����������� ������� ������ �����
struct ModeLongPressTrig { enum E
{
    Level0,
    Auto
};};


// Time


// ������� ��/���
struct FunctionTime { enum E
{
    Time,
    ShiftInMemory
}; };

// ������� �� �������
struct TBase { enum E
{
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
}; };

// ��������� ����� ������������� �� �������
struct TPos { enum E
{
    Left,      // �������� � ������ ����
    Center,    // �������� � ������
    Right      // �������� � ������� ����
};};

struct PeackDetMode { enum E
{
    Disable,
    Enable,
    Average
};};


// Cursors


// ����� �������� ���������
struct CursCntrl { enum E
{
    _1,        // ������
    _2,        // ������
    _1_2,      // ������
    Disable   // �������
};};

// ������������ ����������� ��������
struct CursMovement { enum E
{
    Points,    // �� ������
    Percents   // �� ���������
};};

// ����� ������� ������ �������. ����� �������, �� � ����� ������������ �� �������� ����� ���������.
struct CursActive { enum E
{
    U,
    T,
    None
};};

// ����� �������� ��������
struct CursLookMode { enum E
{
    None,          // ������� �� ������
    Voltage,       // ������� ������ �� ����������� �������������
    Time,          // ������� ������ �� �������� �������������
    Both           // ������� ������ �� �������� � �����������, � ����������� �� ����, ����� ������� ������� ���������.
};};


// ����� ������.
struct ModeWork { enum E
{
    Direct,     // �������� �����.
    Latest,     // � ���� ������ ����� ����������� ��������� ���������� ���������
    MemInt,     // � ���� ������ ����� ��������� �� flash-������ ��������� ������������� ����� ����������
};};

// ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������
struct ModeShowIntMem { enum E
{
    Direct,     // ���������� ������ ��������� �������.
    Saved,      // ���������� ���������� ������
    Both        // ���������� ������ ��������� ������� � ���������� ������
};};

// ��� ������ ��� ������� ������ ������
struct ModeBtnMemory { enum E
{
    Menu,       // ����� ������������ ��������������� �������� ����
    Save        // ���������� ����������� ������ �� ������
};};

// ����� ������������ ������
struct FileNamingMode { enum E
{
    Mask,        // ��������� �� �����
    Manually     // ��������� �������
};};

// ��� ��������� ������ �� ������
struct ModeSaveSignal { enum E
{
    BMP,
    TXT
};};


// Measures


// ������� �� ������� ��� ������ ���������.
struct ModeViewSignals { enum E
{
    AsIs,       // ���������� ������� ��� ����
    Compress    // ������� ����� � ���������
};};

struct MeasuresNumber { enum E
{
    _1,             // 1 ��������� ����� �����
    _2,             // 2 ��������� ����� �����
    _1_5,           // 1 ������ � 5 �����������
    _2_5,           // 2 ������ �� 5 ���������
    _3_5,           // 3 ������ �� 5 ���������
    _6_1,           // 6 ����� �� 1 ���������
    _6_2            // 6 ����� �� 2 ���������
};};

// ����, �� ������� ��������� ��������
struct MeasuresField { enum E
{
    Sfcreen,        // ��������� ����� ������������� �� ��� ����� �������, ������� ����� �� ������.
    AllMemory,      // ��������� ����� ������������� �� ����� �������.
    Hand            // ��������� ����� ������������� �� ����, ����������� �������������.
};};


struct Function { enum E
{
    Sum,
    Mul
};};

struct ScaleFFT { enum E
{
    Log,           // ��� �������� �������� ��������������� ����� ������ �������
    Linear         // ��� �������� �������� �������� ����� ������ �������
}; };

struct WindowFFT { enum E
{
    Rectangle,
    Hamming,
    Blackman,
    Hann
};};

struct FFTmaxDB { enum E
{
    _40,
    _60,
    _80
};};

struct ModeDrawMath { enum E
{
    Disable,
    Separate,
    Together
};};

struct ModeRegSet { enum E
{
    Range,
    RShift
};};


struct CalibratorMode { enum E
{
    Freq,
    DC,
    GND
};};


// Debug


// ��� ������������ ��� �������.
// ���� � ���, ��� ������ ��� �� ��������� ��-�� ������� ������������� ( ? ), ������� �� ������ �������������� �������� ��� ������ �� ��� ������.
struct BalanceADCtype { enum E
{
    Disable,     // ������������ ���������.
    Settings,    // ������������ �������� ������������, ������� �������� �������������.
    Hand         // ������������ �������� ������������, �������� �������.
};};

// ��� �������� ���
struct StretchADCtype { enum E
{
    Disable,
    Settings,
    Hand
};};

// ��� ��������������� ��������
enum RShiftADCtype
{
    RShiftADC_Disable,
    RShiftADC_Settings,
    RShiftADC_Hand
};
