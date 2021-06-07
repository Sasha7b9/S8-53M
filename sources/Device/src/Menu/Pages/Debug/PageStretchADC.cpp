// 2021/05/12 11:37:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/SettingsNRST.h"


//----------------------------------------------------------------------------------------------------------------------

DEF_CHOICE_2(cMode, PageDebug::PageADC::PageStretch::self,
    "�����", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "��������", "Real",
    setNRST.adc.type_stretch, nullptr, nullptr, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_PAGE_1(pageStretchADC, PageDebug::PageADC::self, NamePage::DebugADCstretch,
    "��������", "STRETCH",
    "",
    "",
    cMode,
    nullptr, nullptr, nullptr, nullptr
)


const Page *PageDebug::PageADC::PageStretch::self = &pageStretchADC;
