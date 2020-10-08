#pragma once


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
