#pragma once
#include "Utils/Measures.h"


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
    static bool ShowMeasures();
    static int16 PosCursorU(int num);
    static int16 PosCursorT(int num);
};
