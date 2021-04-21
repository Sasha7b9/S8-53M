// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Painter/Text_.h"


// ¬иды измерений.
struct Measure
{ 
    enum E
    {
        None,
        VoltageMax,
        VoltageMin,
        VoltagePic,
        VoltageMaxSteady,
        VoltageMinSteady,
        VoltageAmpl,
        VoltageAverage,
        VoltageRMS,
        VoltageVybrosPlus,
        VoltageVybrosMinus,
        Period,
        Freq,
        TimeNarastaniya,
        TimeSpada,
        DurationPlus,
        DurationMinus,
        SkvaznostPlus,
        SkvaznostMinus,
        DelayPlus,
        DelayMinus,
        PhazaPlus,
        PhazaMinus,
        Count
    } value;

    Measure(E v = None) : value(v) {}

    operator E() { return value; }

    bool IsNone() const { return value == None; }

    bool Is(E v) const { return value == v; }

    static E Get(int num);

    static bool IsMarked(int num);

    static char GetChar(Measure::E measure);

    static bool IsActive(int row, int col);

    static void GetActive(int *row, int *col);

    static void SetActive(int row, int col);

    static int GetDY();

    static int GetDX();

    static Text Name(int row, int col);

    static Measure::E Type(int row, int col);

    static int NumRows();

    static int NumCols();
    
    static int GetTopTable();

    static int GetDeltaGridLeft();

    static int GetDeltaGridBottom();

    static void RotateRegSet(int angle);

    // ¬ызываетс€ при коротком нажатии на кнопку выбора измерени€.
    static void ShorPressOnSmallButtonSettings();

    static void ShortPressOnSmallButonMarker();

    // Ќарисовать страницу выбора измерений.
    static void DrawPageChoice();

    // ¬ывести значени€ автоматических измерений.
    static void DrawAll();

    static int top_measures;    // ¬ерх таблицы вывода измерений. Ёто значение нужно дл€ нормального вывода сообщений на
                                // экран - чтобы они ничего не перекрывали
};

Measure::E& operator++(Measure::E &measure);
