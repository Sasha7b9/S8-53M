// 2021/03/30 13:42:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Tables.h"


struct Warnings
{
    static void Draw();

    static void ClearFromWarnings();

    static void ShowWarningBad(Warning::E warning);

    static void ShowWarningBad(pchar textRu, pchar textEn);

    static void ShowWarningGood(Warning::E warning);

private:

    static void DrawStringInRectangle(int x, int y, char const *text);

    static void ShowWarn(pchar message);

    static void OnTimerShowWarning();
};
