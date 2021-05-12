// 2021/05/12 11:54:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


static void OnPress_Reset()
{
    for (int ch = 0; ch < 2; ch++)
    {
        for (int mode = 0; mode < 2; mode++)
        {
            for (int range = 0; range < Range::Count; range++)
            {
                RSHIFT_ADD(ch, range, mode) = 0;
            }
        }
    }
    RShift::Set(ChA, set.chan[ChA].rshift);
    RShift::Set(ChB, set.chan[ChB].rshift);
}


DEF_BUTTON(mbReset, PageDebug::PageADC::PageAltRShift::self,
    "Ñáðîñ", "Reset",
    "", "",
    nullptr, OnPress_Reset
)


static void OnChanged_A()
{
    RShift::Set(ChA, set.chan[ChA].rshift);
}


DEF_GOVERNOR(mb2mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "Ñì 1ê 2ìÂ ïîñò", "Shift 1ch 2mV DC",
    "",
    "",
    set.chan[ChA].rshift_add[Range::_2mV][ModeCouple::DC], -100, 100, nullptr, OnChanged_A, nullptr
)


static void OnChanged_B()
{
    RShift::Set(ChB, set.chan[ChB].rshift);
}


DEF_GOVERNOR(mb2mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "Ñì 2ê 2ìÂ ïîñò", "Shift 2ch 2mV DC",
    "",
    "",
    RSHIFT_ADD(ChB, Range::_2mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_B, nullptr
)


DEF_GOVERNOR(mb5mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "Ñì 1ê 5ìÂ ïîñò", "Shift 1ch 5mV DC",
    "",
    "",
    RSHIFT_ADD(ChA, Range::_5mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_A, nullptr
)


DEF_GOVERNOR(mb5mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "Ñì 2ê 5ìÂ ïîñò", "Shift 2ch 5mV DC",
    "",
    "",
    RSHIFT_ADD(ChB, Range::_5mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_B, nullptr
)


DEF_GOVERNOR(mb10mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "Ñì 1ê 10ìÂ ïîñò", "Shift 1ch 10mV DC",
    "",
    "",
    RSHIFT_ADD(ChA, Range::_10mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_A, nullptr
)


DEF_GOVERNOR(mb10mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "Ñì 2ê 10ìÂ ïîñò", "Shift 2ch 10mV DC",
    "",
    "",
    RSHIFT_ADD(ChB, Range::_10mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_B, nullptr
)


DEF_PAGE_7(pageAltShiftADC, PageDebug::PageADC::self, NamePage::DebugADCrShift,
    "ÄÎÏ ÑÌÅÙ", "ADD RSHFIT",
    "",
    "",
    mbReset,          // ÎÒËÀÄÊÀ - ÀÖÏ - ÄÎÏ ÑÌÅÙ - Ñáðîñ
    mb2mV_DC_A,       // ÎÒËÀÄÊÀ - ÀÖÏ - ÄÎÏ ÑÌÅÙ - Ñì 1ê 2ìÂ ïîñò
    mb2mV_DC_B,       // ÎÒËÀÄÊÀ - ÀÖÏ - ÄÎÏ ÑÌÅÙ - Ñì 2ê 2ìÂ ïîñò
    mb5mV_DC_A,       // ÎÒËÀÄÊÀ - ÀÖÏ - ÄÎÏ ÑÌÅÙ - Ñì 1ê 5ìÂ ïîñò
    mb5mV_DC_B,       // ÎÒËÀÄÊÀ - ÀÖÏ - ÄÎÏ ÑÌÅÙ - Ñì 2ê 5ìÂ ïîñò
    mb10mV_DC_A,      // ÎÒËÀÄÊÀ - ÀÖÏ - ÄÎÏ ÑÌÅÙ - Ñì 1ê 10ìÂ ïîñò
    mb10mV_DC_B,      // ÎÒËÀÄÊÀ - ÀÖÏ - ÄÎÏ ÑÌÅÙ - Ñì 2ê 10ìÂ ïîñò
    nullptr, nullptr, nullptr, nullptr
)


const Page *PageDebug::PageADC::PageAltRShift::self = &pageAltShiftADC;
