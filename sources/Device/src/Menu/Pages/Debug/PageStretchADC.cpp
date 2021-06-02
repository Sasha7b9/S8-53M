// 2021/05/12 11:37:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/SettingsNRST.h"


//----------------------------------------------------------------------------------------------------------------------

static bool IsActive()
{
    return StretchADCtype::IsHand();
}

//----------------------------------------------------------------------------------------------------------------------

static void OnChanged_Mode(bool active)
{
    if (active)
    {
        PageDebug::LoadStretchADC(ChA);
        PageDebug::LoadStretchADC(ChB);

        SettingsNRST::ExecuteOnChanged();
    }
}


DEF_CHOICE_3(cMode, PageDebug::PageADC::PageStretch::self,
    "Ðåæèì", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "Ðåàëüíûé", "Real",
    "Ðó÷íîé", "Manual",
    setNRST.adc.type_stretch, nullptr, OnChanged_Mode, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

static void OnChanged_A()
{
    //    FPGA::WriteToHardware(WR_CAL_A, (uint8)DEBUG_STRETCH_ADC_A, true);

    SettingsNRST::ExecuteOnChanged();
}


DEF_GOVERNOR(gADC_A, PageDebug::PageADC::PageStretch::self,
    "Êîýôô. 1ê", "Koeff. 1ch",
    "",
    "",
    setNRST.chan[ChA].stretch_hand, 0, 255, IsActive, OnChanged_A, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

static void OnChanged_B()
{
    //    FPGA::WriteToHardware(WR_CAL_B, (uint8)DEBUG_STRETCH_ADC_B, true);

    SettingsNRST::ExecuteOnChanged();
}


DEF_GOVERNOR(gADC_B, PageDebug::PageADC::PageStretch::self,
    "Êîýôô. 2ê", "Koeff. 2ch",
    "",
    "",
    setNRST.chan[ChB].stretch_hand, 0, 255, IsActive, OnChanged_B, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_PAGE_3(pageStretchADC, PageDebug::PageADC::self, NamePage::DebugADCstretch,
    "ÐÀÑÒßÆÊÀ", "STRETCH",
    "",
    "",
    cMode,
    gADC_A,
    gADC_B,
    nullptr, nullptr, nullptr, nullptr
)


const Page *PageDebug::PageADC::PageStretch::self = &pageStretchADC;


void PageDebug::LoadStretchADC(const Channel &ch)
{
    if (StretchADCtype::IsDisabled())
    {
        //        FPGA::WriteToHardware(ch == ChA ? WR_CAL_A : WR_CAL_B, 0x80, true);
    }
    else if (StretchADCtype::IsHand())
    {
        //        FPGA::WriteToHardware(ch == ChA ? WR_CAL_A : WR_CAL_B, (uint8)DEBUG_STRETCH_ADC(ch), true);
    }
    else if (StretchADCtype::IsSettings())
    {
        FPGA::Calibrator::LoadKoeff(ch);
    }
}
