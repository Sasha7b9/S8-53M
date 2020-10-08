#pragma once
#include "common/Settings/cSettingsTypes.h"


// ChannelX

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
struct ModeViewSignals { enum E
{
    AsIs,       // ���������� ������� ��� ����
    Compress    // ������� ����� � ���������
};};


// Math


struct ScaleMath { enum E
{
    Channel0,     // ������� ������ �� ������ 1
    Channel1,     // ������� ������ �� ������ 2
    Hand          // ������� ������� �������.
};};


// �������

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
