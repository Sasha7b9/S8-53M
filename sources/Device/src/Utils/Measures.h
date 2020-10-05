#pragma once
#include "defines.h"


// ���� ���������.
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
};

Measure::E& operator++(Measure::E &measure);

int Measure_GetTopTable();

int Measure_GetDeltaGridLeft();

int Measure_GetDeltaGridBottom();

void Measure_RotateRegSet(int angle);
// ���������� ��� �������� ������� �� ������ ������ ���������.
void Measure_ShorPressOnSmallButtonSettings();

void Measure_ShortPressOnSmallButonMarker();
// ���������� �������� ������ ���������.
void Measure_DrawPageChoice();
