// 2021/05/12 10:18:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/SettingsNRST.h"


static const SettingsNRST defaultNRST =
{
    false,                          // showStats
    false,                          // view altera writting data
    false,                          // view all altera writting data
    0,                              // alt shift
    {
        6,                          // num_ave
        1,                          // num_smooth
        1000,                       // num_measures_for_gates
        0                           // shift_T0
    },
    {
        BalanceADCtype::Settings,   // type_balance
        {0, 5},                     // balance
        StretchADCtype::Settings,   // type_stretch
        {128, 128}                  // stretch
    },
    {
        10,                         // num_srings
        0,                          // size_font
        0                           // in_pause
    },
    {
        false,                  // отображать пред/после запуск
        false, false, false, false, false, false, false, false, false, false, false
    }
};


SettingsNRST setNRST = defaultNRST;


int SettingsNRST::GetSizeFontForConsole()
{
    return setNRST.console.size_font ? 8 : 5;
}
