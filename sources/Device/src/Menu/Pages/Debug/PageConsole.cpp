// 2021/05/12 10:55:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/SettingsNRST.h"


DEF_GOVERNOR(mgNumStrings, PageDebug::PageConsole::self,
    "����� �����", "Number strings",
    "",
    "",
    setNRST.num_srings, 0, 33, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcSizeFont, PageDebug::PageConsole::self,
    "������ ������", "Size font",
    "",
    "",
    "5", "5",
    "8", "8",
    setNRST.size_font, nullptr, nullptr, nullptr
)


DEF_PAGE_3(pageConsole, PageDebug::self, NamePage::DebugConsole,
    "�������", "CONSOLE",
    "",
    "",
    mgNumStrings,
    mcSizeFont,
    *PageDebug::PageConsole::PageRegisters::self,
    nullptr, nullptr, nullptr, nullptr
);


const Page *PageDebug::PageConsole::self = &pageConsole;
