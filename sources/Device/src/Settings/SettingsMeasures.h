#pragma once
#include "Utils/Measures.h"


#define MEAS_NUM                        (set.measures.number)                           // SettingsMeasuers.number
#define MEAS_NUM_IS_1                   (MEAS_NUM == MeasuresNumber::_1)
#define MEAS_NUM_IS_1_5                 (MEAS_NUM == MeasuresNumber::_1_5)
#define MEAS_NUM_IS_2_5                 (MEAS_NUM == MeasuresNumber::_2_5)
#define MEAS_NUM_IS_3_5                 (MEAS_NUM == MeasuresNumber::_3_5)
#define MEAS_NUM_IS_6_1                 (MEAS_NUM == MeasuresNumber::_6_1)
#define MEAS_NUM_IS_6_2                 (MEAS_NUM == MeasuresNumber::_6_2)

#define MEAS_SOURCE                     (set.measures.source)                           // SettingsMeasures.source
#define MEAS_SOURCE_IS_A                (MEAS_SOURCE == Channel::A)
#define MEAS_SOURCE_IS_B                (MEAS_SOURCE == Channel::B)
#define MEAS_SOURCE_IS_A_B              (MEAS_SOURCE == Channel::A_B)

#define MODE_VIEW_SIGNALS               (set.measures.modeViewSignals)                 // SettingsMeasures.modeViewSignals
#define MODE_VIEW_SIGNALS_IS_COMPRESS   (MODE_VIEW_SIGNALS == ModeViewSignals::Compress)

#define MEAS_MARKED                     (set.measures.markedMeasure)                    // SettingsMeasures.markedMeasure
#define MEAS_MARKED_IS_NONE             (MEAS_MARKED == Measure::None)

#define MEASURE(num)                    (set.measures.measures[num])                    // SettingsMeasures.measure
#define MEASURE_IS_MARKED(num)          (MEASURE(num) == MEAS_MARKED)

#define SHOW_MEASURES                   (set.measures.show)                             // SettingsMeasures.show

#define MEAS_FIELD                      (set.measures.field)                            // SettingsMeaseres.field
#define MEAS_FIELD_IS_HAND              (MEAS_FIELD == MeasuresField::Hand)

#define MEAS_POS_CUR_U(num)             (set.measures.posCurU[num])                     // SettingsMeasures.posCurU
#define MEAS_POS_CUR_U0                 (MEAS_POS_CUR_U(0))
#define MEAS_POS_CUR_U1                 (MEAS_POS_CUR_U(1))

#define MEAS_POS_CUR_T(num)             (set.measures.posCurT[num])                     // SettingsMeasures.posCurT
#define MEAS_POS_CUR_T0                 (MEAS_POS_CUR_T(0))
#define MEAS_POS_CUR_T1                 (MEAS_POS_CUR_T(1))

#define MEAS_CURS_CNTRL_U               (set.measures.cntrlU)                           // SettingsMeasures.cntrlU

#define MEAS_CURS_CNTRL_T               (set.measures.cntrlT)                           // SettingsMeasures.cntrlT

#define MEAS_CURS_ACTIVE                (set.measures.cursActive)                       // SettingsMeasures.cursActive
#define MEAS_CURS_ACTIVE_IS_T           (MEAS_CURS_ACTIVE == CursActive::T)


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
};
