// 2021/05/12 11:20:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Settings/SettingsMain.h"


//----------------------------------------------------------------------------------------------------------------------

static void OnChanged_A()
{  
    SettingsNRST::CommonOnChanged();
}


DEF_GOVERNOR(gShiftA, PageDebug::PageADC::PageBalance::self,
    "Смещение 1", "Offset 1",
    "",
    "",
    setNRST.adc.balance[ChA], -125, 125, nullptr, OnChanged_A, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

static void OnChanged_ShiftB()
{
    SettingsNRST::CommonOnChanged();
}


DEF_GOVERNOR(gShiftB, PageDebug::PageADC::PageBalance::self,
    "Смещение 2", "Offset 2",
    "",
    "",
    setNRST.adc.balance[ChB], -125, 125, nullptr, OnChanged_ShiftB, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_PAGE_2(pageBalanceADC, PageDebug::PageADC::self, NamePage::DebugADCbalance,
    "БАЛАНС", "BALANCE",
    "",
    "",
    gShiftA,
    gShiftB,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDebug::PageADC::PageBalance::self = &pageBalanceADC;
