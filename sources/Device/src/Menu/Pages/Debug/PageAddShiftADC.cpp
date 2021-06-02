// 2021/05/12 11:54:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


//----------------------------------------------------------------------------------------------------------------------

static void OnChanged_A()
{
    RShift::Set(ChA, set.chan[ChA].rshift);

    SettingsNRST::CommonOnChanged();
}

static void OnChanged_B()
{
    RShift::Set(ChB, set.chan[ChB].rshift);

    SettingsNRST::CommonOnChanged();
}

//----------------------------------------------------------------------------------------------------------------------

static void OnPress_Reset()
{
    for (int ch = 0; ch < 2; ch++)
    {
        for (int mode = 0; mode < 2; mode++)
        {
            for (int range = 0; range < Range::Count; range++)
            {
                setNRST.chan[ch].rshift[range][mode] = 0;
            }
        }
    }

    RShift::Set(ChA, set.chan[ChA].rshift);
    RShift::Set(ChB, set.chan[ChB].rshift);

    SettingsNRST::CommonOnChanged();
}


DEF_BUTTON(bReset, PageDebug::PageADC::PageAltRShift::self,
    "Ñáðîñ", "Reset",
    "", "",
    nullptr, OnPress_Reset
)

//----------------------------------------------------------------------------------------------------------------------

DEF_GOVERNOR(b2mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "Ñì 1ê 2ìÂ ïîñò", "Shift 1ch 2mV DC",
    "",
    "",
    setNRST.chan[ChA].rshift[Range::_2mV][ModeCouple::DC], -100, 100, nullptr, OnChanged_A, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_GOVERNOR(b2mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "Ñì 2ê 2ìÂ ïîñò", "Shift 2ch 2mV DC",
    "",
    "",
    setNRST.chan[ChB].rshift[Range::_2mV][ModeCouple::DC], -100, 100, nullptr, OnChanged_B, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_GOVERNOR(b5mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "Ñì 1ê 5ìÂ ïîñò", "Shift 1ch 5mV DC",
    "",
    "",
    setNRST.chan[ChA].rshift[Range::_5mV][ModeCouple::DC], -100, 100, nullptr, OnChanged_A, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_GOVERNOR(b5mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "Ñì 2ê 5ìÂ ïîñò", "Shift 2ch 5mV DC",
    "",
    "",
    setNRST.chan[ChB].rshift[Range::_5mV][ModeCouple::DC], -100, 100, nullptr, OnChanged_B, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_GOVERNOR(b10mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "Ñì 1ê 10ìÂ ïîñò", "Shift 1ch 10mV DC",
    "",
    "",
    setNRST.chan[ChA].rshift[Range::_10mV][ModeCouple::DC], -100, 100, nullptr, OnChanged_A, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_GOVERNOR(b10mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "Ñì 2ê 10ìÂ ïîñò", "Shift 2ch 10mV DC",
    "",
    "",
    setNRST.chan[ChB].rshift[Range::_10mV][ModeCouple::DC], -100, 100, nullptr, OnChanged_B, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_PAGE_7(pageAltShift, PageDebug::PageADC::self, NamePage::DebugADCrShift,
    "ÄÎÏ ÑÌÅÙ", "ADD RSHFIT",
    "",
    "",
    bReset,
    b2mV_DC_A,
    b2mV_DC_B,
    b5mV_DC_A,
    b5mV_DC_B,
    b10mV_DC_A,
    b10mV_DC_B,
    nullptr, nullptr, nullptr, nullptr
)


const Page *PageDebug::PageADC::PageAltRShift::self = &pageAltShift;
