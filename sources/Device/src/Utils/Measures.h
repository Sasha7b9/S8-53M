#pragma once


// Виды измерений.
struct Measure { enum E
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
    };

    static char GetChar(Measure::E measure);

    static bool IsActive(int row, int col);

    static void GetActive(int *row, int *col);

    static void SetActive(int row, int col);

    static int GetDY();

    static int GetDX();

    static const char *Name(int row, int col);

    static Measure::E Type(int row, int col);

    static int NumRows();

    static int NumCols();
    
    static int GetTopTable();

    static int GetDeltaGridLeft();

    static int GetDeltaGridBottom();

    static void RotateRegSet(int angle);
    // Вызывается при коротком нажатии на кнопку выбора измерения.
    static void ShorPressOnSmallButtonSettings();

    static void ShortPressOnSmallButonMarker();
    // Нарисовать страницу выбора измерений.
    static void DrawPageChoice();
};

Measure::E& operator++(Measure::E &measure);
