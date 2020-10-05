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
};

Measure::E& operator++(Measure::E &measure);



bool Measure_IsActive(int row, int col); 

void Measure_GetActive(int *row, int *col);

void Measure_SetActive(int row, int col);

int Measure_GetDY();

int Measure_GetDX();

const char* Measure_Name(int row, int col);

Measure::E Measure_Type(int row, int col);

int Measure_NumRows();

int Measure_NumCols();

int Measure_GetTopTable();

int Measure_GetDeltaGridLeft();

int Measure_GetDeltaGridBottom();

void Measure_RotateRegSet(int angle);
// ���������� ��� �������� ������� �� ������ ������ ���������.
void Measure_ShorPressOnSmallButtonSettings();

void Measure_ShortPressOnSmallButonMarker();
// ���������� �������� ������ ���������.
void Measure_DrawPageChoice();
