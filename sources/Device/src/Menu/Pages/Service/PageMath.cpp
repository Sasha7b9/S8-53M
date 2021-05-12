// 2021/05/12 15:27:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"


DEF_PAGE_2(pageMath, PageService::self, NamePage::Math,
    "ÌÀÒÅÌÀÒÈÊÀ", "MATH",
    "Ìàòåìàòè÷åñêèå ôóíêöèè è ÁÏÔ",
    "Mathematical functions and FFT",
    *PageService::PageMath::PageFunction::self,     // ÑÅĞÂÈÑ - ÌÀÒÅÌÀÒÈÊÀ - ÔÓÍÊÖÈß
    *PageService::PageMath::PageFFT::self,          // ÑÅĞÂÈÑ - ÌÀÒÅÌÀÒÈÊÀ - ÑÏÅÊÒĞ
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageService::PageMath::self = &pageMath;
