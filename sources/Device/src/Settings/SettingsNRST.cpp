// 2021/05/12 10:18:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Memory/ROM_.h"
#include "Settings/SettingsNRST.h"
#include <cstring>


static const SettingsNRST defaultNRST =
{
    0,
    false,                          // show_stats
    {
        6,                          // num_ave
        1,                          // num_smooth
        1000,                       // num_measures_for_gates
        0,                          // shift_T0
        false                       // show_info
    },
    {
        BalanceADCtype::Settings,   // type_balance
        StretchADCtype::Settings,   // type_stretch
    },
    {
        {
            0,                      // balance_hand
            0,                      // balance_auto
            128,                    // stretch_hand
            1.0f,                   // stretch_auto
            { { 0 }, { 0 } }        // rshift_add
        },
        {
            0,                      // balance_hand
            0,                      // balance_auto
            128,                    // stretch_hand
            1.0f,                   // stretch_auto
            { { 0 }, { 0 } }        // rshift_add
        }
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


int SettingsNRST::SettingsConsole::GetSizeFontForConsole()
{
    return setNRST.console.size_font ? 8 : 5;
}


void SettingsNRST::Init()
{

}


void SettingsNRST::Save()
{
    SettingsNRST *saved = ROM::NRST::GetSaved();

    size = sizeof(*this);

    if (!saved ||               // ≈сли нет сохранЄнных настроек
        saved->size != size ||  // или размер структуры не совпадают
        *saved != setNRST)      // или записанные и текущие настройки не совпадают
    {
        ROM::NRST::Save(this);
    }
}


void SettingsNRST::CommonOnChanged()
{
    setNRST.Save();
}


void SettingsNRST::CommonOnChanged(bool)
{
    CommonOnChanged();
}


bool SettingsNRST::operator!=(const SettingsNRST &rhs)
{
    return std::memcmp(this, &rhs, sizeof(*this)) != 0;
}
