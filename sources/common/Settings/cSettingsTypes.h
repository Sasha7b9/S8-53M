#pragma once


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
struct ENUM_POINTS_FPGA { enum E
{
    _281,
    _512,
    _1024
};};


// �������� ������� ��� ����������� �������.
struct SourceFFT { enum E
{
    A,          // ������ � ����������� ������� ������� ������.
    B,          // ������ � ����������� ������� ������� ������.
    Both        // ������ � ����������� �������� ����� �������.
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
