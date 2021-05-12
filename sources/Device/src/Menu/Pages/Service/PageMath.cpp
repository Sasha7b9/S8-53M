// 2021/05/12 15:27:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"


DEF_PAGE_2(pageMath, PageService::self, NamePage::Math,
    "МАТЕМАТИКА", "MATH",
    "Математические функции и БПФ",
    "Mathematical functions and FFT",
    *PageService::PageMath::PageFunction::self,
    *PageService::PageMath::PageFFT::self,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageService::PageMath::self = &pageMath;
