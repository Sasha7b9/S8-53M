#pragma once


// ChannelX

struct Channel { enum E
{
    A,
    B,
    A_B,
    Math
};};

// ����� ������ �� �����.
struct ModeCouple { enum E
{
    DC,      // �������� ����.
    AC,      // �������� ����.
    GND      // ���� �������.
};};

// ��������.
struct Divider { enum E
    {
        _1,
        _10
    };
    static int ToAbs(Divider::E divider);
};

// ������� �� ����������.
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

Range::E& operator++(Range::E &range);
Range::E& operator--(Range::E &range);

#define NumChannels 2

const char *RangeName(Range::E range);



// Time


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

const char *TBaseName(TBase::E tBase);



// Measures


// ������� �� ������� ��� ������ ���������.
enum ModeViewSignals
{
    ModeViewSignals_AsIs,       // ���������� ������� ��� ����
    ModeViewSignals_Compress    // ������� ����� � ���������
};


// Math


enum ScaleMath
{
    ScaleMath_Channel0,     // ������� ������ �� ������ 1
    ScaleMath_Channel1,     // ������� ������ �� ������ 2
    ScaleMath_Hand          // ������� ������� �������.
};


// �������

// ����� �������� ���������.
enum CursCntrl
{
    CursCntrl_1,            // ������.
    CursCntrl_2,            // ������.
    CursCntrl_1_2,          // ������.
    CursCntrl_Disable       // �������.
};


// ������������ ����������� ��������.
enum CursMovement
{
    CursMovement_Points,    // �� ������.
    CursMovement_Percents   // �� ���������.
};

// ����� ������� ������ �������. ����� �������, �� � ����� ������������ �� �������� ����� ���������.
enum CursActive
{
    CursActive_U,
    CursActive_T,
    CursActive_None
};

// ����� �������� ��������.
enum CursLookMode
{
    CursLookMode_None,      // ������� �� ������.
    CursLookMode_Voltage,   // ������� ������ �� ����������� �������������.
    CursLookMode_Time,      // ������� ������ �� �������� �������������.
    CursLookMode_Both       // ������� ������ �� �������� � �����������, � ����������� �� ����, ����� ������� ������� ���������.
};


// Service


// ���� ����
enum Language
{
    Russian,    // �������
    English     // ����������
};


// Debug


// ��� ������������ ��� �������.
// ���� � ���, ��� ������ ��� �� ��������� ��-�� ������� ������������� ( ? ), ������� �� ������ �������������� �������� ��� ������ �� ��� ������.
enum BalanceADCtype
{
    BalanceADC_Disable,     // ������������ ���������.
    BalanceADC_Settings,    // ������������ �������� ������������, ������� �������� �������������.
    BalanceADC_Hand         // ������������ �������� ������������, �������� �������.
};

// ��� �������� ���
enum StretchADCtype
{
    StretchADC_Disable,
    StretchADC_Settings,
    StretchADC_Hand
};

// ��� ��������������� ��������
enum RShiftADCtype
{
    RShiftADC_Disable,
    RShiftADC_Settings,
    RShiftADC_Hand
};
