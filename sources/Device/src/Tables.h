// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Warning { enum E
{
    LimitChan1_Volts,
    LimitChan2_Volts,
    LimitSweep_Time,
    SignalIsSaved,

    SignalNotFound,
    NeedRebootDevice1,
    NeedRebootDevice2,
    ImpossibleEnableMathFunction,
    Count
};};

struct WarningWithNumber { enum E
{
    ExcessValues        // Превышение значения количества сигналов в "НАКОПЛЕНИЕ", "УСРЕДНЕНИЕ", "Мин Макс"
};};


struct Tables
{
    static pchar GetWarning(Warning::E warning);
    // Выводит строку из таблицы symbolsAlphaBet
    static void DrawStr(int index, int x, int y);
    static pchar symbolsAlphaBet[0x48];
};
