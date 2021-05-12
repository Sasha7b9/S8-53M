// 2021/05/12 11:17:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"


DEF_PAGE_3(pageADC, PageDebug::self, NamePage::DebugADC,
    "¿÷œ", "ADC",
    "",
    "",
    *PageDebug::PageADC::PageBalance::self,
    *PageDebug::PageADC::PageStretch::self,
    *PageDebug::PageADC::PageAltRShift::self,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDebug::PageADC::self = &pageADC;
