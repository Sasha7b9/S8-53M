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

static void OnPress_ResetBalance()
{
    for (int ch = 0; ch < 2; ch++)
    {
        for (int range = 0; range < Range::Count; range++)
        {
            for (int mode = 0; mode < 2; mode++)
            {

            }
        }
    }
}

DEF_BUTTON(mbResetBalance, PageDebug::PageADC::PageAltRShift::self,
    "—брос баланс", "Reset balance",
    "", "",
    nullptr, OnPress_ResetBalance
)

//----------------------------------------------------------------------------------------------------------------------

static void OnPress_Reset()
{
    for (int ch = 0; ch < 2; ch++)
    {
        for (int mode = 0; mode < 2; mode++)
        {
            for (int range = 0; range < Range::Count; range++)
            {
                setNRST.chan[ch].rshift_add[range][mode] = 0;
            }
        }
    }
    RShift::Set(ChA, set.chan[ChA].rshift);
    RShift::Set(ChB, set.chan[ChB].rshift);

    SettingsNRST::CommonOnChanged();
}


DEF_BUTTON(mbReset, PageDebug::PageADC::PageAltRShift::self,
    "—брос", "Reset",
    "", "",
    nullptr, OnPress_Reset
)

//----------------------------------------------------------------------------------------------------------------------

DEF_GOVERNOR(mb2mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "—м 1к 2м¬ пост", "Shift 1ch 2mV DC",
    "",
    "",
    setNRST.chan[ChA].rshift_add[Range::_2mV][ModeCouple::DC], -100, 100, nullptr, OnChanged_A, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_GOVERNOR(mb2mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "—м 2к 2м¬ пост", "Shift 2ch 2mV DC",
    "",
    "",
    setNRST.chan[ChB].rshift_add[Range::_2mV][ModeCouple::DC], -100, 100, nullptr, OnChanged_B, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_GOVERNOR(mb5mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "—м 1к 5м¬ пост", "Shift 1ch 5mV DC",
    "",
    "",
    setNRST.chan[ChA].rshift_add[Range::_5mV][ModeCouple::DC], -100, 100, nullptr, OnChanged_A, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_GOVERNOR(mb5mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "—м 2к 5м¬ пост", "Shift 2ch 5mV DC",
    "",
    "",
    setNRST.chan[ChB].rshift_add[Range::_5mV][ModeCouple::DC], -100, 100, nullptr, OnChanged_B, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_GOVERNOR(mb10mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "—м 1к 10м¬ пост", "Shift 1ch 10mV DC",
    "",
    "",
    setNRST.chan[ChA].rshift_add[Range::_10mV][ModeCouple::DC], -100, 100, nullptr, OnChanged_A, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_GOVERNOR(mb10mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "—м 2к 10м¬ пост", "Shift 2ch 10mV DC",
    "",
    "",
    setNRST.chan[ChB].rshift_add[Range::_10mV][ModeCouple::DC], -100, 100, nullptr, OnChanged_B, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_PAGE_7(pageAltShift, PageDebug::PageADC::self, NamePage::DebugADCrShift,
    "ƒќѕ —ћ≈ў", "ADD RSHFIT",
    "",
    "",
//    mbResetBalance,
    mbReset,
    mb2mV_DC_A,
    mb2mV_DC_B,
    mb5mV_DC_A,
    mb5mV_DC_B,
    mb10mV_DC_A,
    mb10mV_DC_B,
    nullptr, nullptr, nullptr, nullptr
)


const Page *PageDebug::PageADC::PageAltRShift::self = &pageAltShift;
