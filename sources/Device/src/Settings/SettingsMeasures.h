#pragma once
#include "Utils/Measures.h"


#define MODE_VIEW_SIGNALS               (set.measures.modeViewSignals)                 // SettingsMeasures.modeViewSignals
#define MODE_VIEW_SIGNALS_IS_COMPRESS   (MODE_VIEW_SIGNALS == ModeViewSignals::Compress)

#define MEAS_MARKED                     (set.measures.markedMeasure)                    // SettingsMeasures.markedMeasure
#define MEAS_MARKED_IS_NONE             (MEAS_MARKED == Measure::None)

#define MEASURE(num)                    (set.measures.measures[num])                    // SettingsMeasures.measure
#define MEASURE_IS_MARKED(num)          (MEASURE(num) == MEAS_MARKED)

#define SHOW_MEASURES                   (set.measures.show)                             // SettingsMeasures.show

#define MEAS_POS_CUR_U(num)             (set.measures.posCurU[num])                     // SettingsMeasures.posCurU
#define MEAS_POS_CUR_U0                 (MEAS_POS_CUR_U(0))
#define MEAS_POS_CUR_U1                 (MEAS_POS_CUR_U(1))

#define MEAS_POS_CUR_T(num)             (set.measures.posCurT[num])                     // SettingsMeasures.posCurT
#define MEAS_POS_CUR_T0                 (MEAS_POS_CUR_T(0))
#define MEAS_POS_CUR_T1                 (MEAS_POS_CUR_T(1))

#define MEAS_CURS_CNTRL_U               (set.measures.cntrlU)                           // SettingsMeasures.cntrlU

#define MEAS_CURS_CNTRL_T               (set.measures.cntrlT)                           // SettingsMeasures.cntrlT

// Настройки меню ИЗМЕРЕНИЯ
struct SettingsMeasures
{
    MeasuresNumber::E  number;           // Сколько измерений выводить.
    Channel::E         source;           // Для каких каналов выводить измерения.
    ModeViewSignals::E modeViewSignals;  // Сжимать ли сигналы при выводе измерений.
    Measure::E         measures[15];     // Выбранные для индикации измерения.
    bool               show;             // Показывать ли измерения.
    MeasuresField::E   field;            // Задаёт область, из которой берутся значения для расчёта измерений.
    int16              posCurU[2];       // Позиции курсоров, которые задают область, из которой берутся значения для расчёта измерений при field == MeasuresField::Hand.
    int16              posCurT[2];       // Позиции курсоров, которые задают область, из которой берутся значения для расчёта измерений при field == MeasuresField::Hand.
    CursCntrl::E       cntrlU;           // Активные курсоры напряжения.
    CursCntrl::E       cntrlT;           // Активные курсоры времени.
    CursActive::E      cursActive;       // Какие курсоры активны - по времени или напряжению.
    Measure::E         markedMeasure;    // Измерение, на которое нужно выводить маркеры.

    static Channel::E Source();
    static bool SourceIsA();
    static bool SourceIsB();
    static bool SourceIsAB();
};
