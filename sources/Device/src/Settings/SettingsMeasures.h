#pragma once
#include "Utils/Measures.h"


#define MEAS_CURS_CNTRL_U               (set.measures.cntrlU)                           // SettingsMeasures.cntrlU

#define MEAS_CURS_CNTRL_T               (set.measures.cntrlT)                           // SettingsMeasures.cntrlT

// ��������� ���� ���������
struct SettingsMeasures
{
    MeasuresNumber::E  number;           // ������� ��������� ��������.
    Channel::E         source;           // ��� ����� ������� �������� ���������.
    ModeViewSignals::E modeViewSignals;  // ������� �� ������� ��� ������ ���������.
    Measure::E         measures[15];     // ��������� ��� ��������� ���������.
    bool               show;             // ���������� �� ���������.
    MeasuresField::E   field;            // ����� �������, �� ������� ������� �������� ��� ������� ���������.
    int16              posCurU[2];       // ������� ��������, ������� ������ �������, �� ������� ������� �������� ��� ������� ��������� ��� field == MeasuresField::Hand.
    int16              posCurT[2];       // ������� ��������, ������� ������ �������, �� ������� ������� �������� ��� ������� ��������� ��� field == MeasuresField::Hand.
    CursCntrl::E       cntrlU;           // �������� ������� ����������.
    CursCntrl::E       cntrlT;           // �������� ������� �������.
    CursActive::E      cursActive;       // ����� ������� ������� - �� ������� ��� ����������.
    Measure::E         markedMeasure;    // ���������, �� ������� ����� �������� �������.

    static Channel::E Source();
    static bool SourceIsA();
    static bool SourceIsB();
    static bool SourceIsAB();
    static bool ShowMeasures();
    static int16 PosCursorU(int num);
    static int16 PosCursorT(int num);
};
