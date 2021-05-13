// 2021/05/12 10:18:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/SettingsNRST.h"


static const SettingsNRST defaultNRST =
{
    10,                         // numStrings
    0,                          // ������ ������ - 5
    0,                          // consoleInPause
    BalanceADCtype::Settings,   // balanceADC
    {0, 5},                     // shiftADC
    StretchADCtype::Settings,   // stretch
    {128, 128},                 // stretchADC
    1000,                       // numMeasuresForGates
    0,                          // shiftT0
    false,                      // showStats
    6,                          // numAveForRand
    false,                      // view altera writting data
    false,                      // view all altera writting data
    0,                          // alt shift
    1,                          // num_smooth_for_rand
    {
        false,                  // ���������� ����/����� ������
        false, false, false, false, false, false, false, false, false, false, false
    }
};


SettingsNRST setNRST = defaultNRST;


int SettingsNRST::GetSizeFontForConsole()
{
    return setNRST.size_font ? 8 : 5;
}
