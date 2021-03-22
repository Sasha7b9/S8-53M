#pragma once
#include "Utils/Measures.h"


#define SHOW_MEASURES                   (set.measures.show)                             // SettingsMeasures.show

#define MEAS_POS_CUR_U(num)             (set.measures.posCurU[num])                     // SettingsMeasures.posCurU
#define MEAS_POS_CUR_U0                 (MEAS_POS_CUR_U(0))
#define MEAS_POS_CUR_U1                 (MEAS_POS_CUR_U(1))

#define MEAS_POS_CUR_T(num)             (set.measures.posCurT[num])                     // SettingsMeasures.posCurT
#define MEAS_POS_CUR_T0                 (MEAS_POS_CUR_T(0))
#define MEAS_POS_CUR_T1                 (MEAS_POS_CUR_T(1))

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
};
