// 2021/05/12 11:37:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/SettingsNRST.h"


static void OnChanged_Mode(bool active)
{
    if (active)
    {
        PageDebug::LoadStretchADC(ChA);
        PageDebug::LoadStretchADC(ChB);
    }
}


DEF_CHOICE_3(mcMode, PageDebug::PageADC::PageStretch::self,
    "Ðåæèì", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "Ðåàëüíûé", "Real",
    "Ðó÷íîé", "Manual",
    setNRST.adc.type_stretch, nullptr, OnChanged_Mode, nullptr
)


static void OnChanged_A()
{
    //    FPGA::WriteToHardware(WR_CAL_A, (uint8)DEBUG_STRETCH_ADC_A, true);
}


static bool IsActive()
{
    return StretchADCtype::IsHand();
}


DEF_GOVERNOR(mgADC_A, PageDebug::PageADC::PageStretch::self,
    "Êîýôô. 1ê", "Koeff. 1ch",
    "",
    "",
    setNRST.chan[ChA].stretch_hand, 0, 255, IsActive, OnChanged_A, nullptr
)


static void OnChanged_B()
{
    //    FPGA::WriteToHardware(WR_CAL_B, (uint8)DEBUG_STRETCH_ADC_B, true);
}


DEF_GOVERNOR(mgADC_B, PageDebug::PageADC::PageStretch::self,
    "Êîýôô. 2ê", "Koeff. 2ch",
    "",
    "",
    setNRST.chan[ChB].stretch_hand, 0, 255, IsActive, OnChanged_B, nullptr
)


DEF_PAGE_3(pageStretchADC, PageDebug::PageADC::self, NamePage::DebugADCstretch,
    "ÐÀÑÒßÆÊÀ", "STRETCH",
    "",
    "",
    mcMode,
    mgADC_A,
    mgADC_B,
    nullptr, nullptr, nullptr, nullptr
)


const Page *PageDebug::PageADC::PageStretch::self = &pageStretchADC;
