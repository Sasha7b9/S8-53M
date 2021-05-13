// 2021/05/12 10:18:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/SettingsNRST.h"


static const SettingsNRST defaultNRST =
{
    10,                         // numStrings
    0,                          // размер шрифта - 5
    0,                          // consoleInPause
    BalanceADCtype::Settings,   // balanceADC
    {0, 5},                     // shiftADC
    StretchADCtype::Settings,   // stretch
    {128, 128},                 // stretchADC
    false,                      // showStats
    6,                          // numAveForRand
    false,                      // view altera writting data
    false,                      // view all altera writting data
    0,                          // alt shift
    {
        1,                      // num_smooth
        1000,                   // num_measures_for_gates
        0                       // shift_T0
    },
    {
        false,                  // отображать пред/после запуск
        false, false, false, false, false, false, false, false, false, false, false
    }
};


SettingsNRST setNRST = defaultNRST;


int SettingsNRST::GetSizeFontForConsole()
{
    return setNRST.size_font ? 8 : 5;
}
