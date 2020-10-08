#pragma once


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
    Latest,     // � ���� ������ ����� ����������� ��������� ����������� ���������
    MemInt,     // � ���� ������ ����� ��������� �� flash-������ ��������� ������������� ����� �����������
};};

// ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������
struct ModeShowIntMem { enum E
{
    Direct,     // ���������� ������ ��������� �������.
    Saved,      // ���������� ����������� ������
    Both        // ���������� ������ ��������� ������� � ����������� ������
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
