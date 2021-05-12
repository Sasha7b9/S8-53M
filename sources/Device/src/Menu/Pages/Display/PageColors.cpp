// 2021/05/12 17:14:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


DEF_CHOICE_2(mcScheme, PageDisplay::PageSettings::PageColors::self,
    "Цветовая схема", "Color scheme",
    "Режим работы калибратора",
    "Mode of operation of the calibrator",
    "Схема 1", "Scheme 1",
    "Схема 2", "Scheme 2",
    set.service.colorScheme, nullptr, nullptr, nullptr
)


static ColorType colorT1 = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::DATA_A.index };

DEF_GOVERNOR_COLOR(mgcChannelA, PageDisplay::PageSettings::PageColors::self,
    "Канал 1", "Channel 1",
    "",
    "",
    colorT1, nullptr
)


static ColorType colorT2 = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::DATA_B.index };

DEF_GOVERNOR_COLOR(mgcChannelB, PageDisplay::PageSettings::PageColors::self,
    "Канал 2", "Channel 2",
    "",
    "",
    colorT2, nullptr
)


DEF_GOVERNOR_COLOR(mgcGrid, PageDisplay::PageSettings::PageColors::self,
    "Сетка", "Grid",
    "Устанавливает цвет сетки",
    "Sets the grid color",
    PageDisplay::PageGrid::typeGrid, nullptr
)


DEF_PAGE_4(pageColors, PageDisplay::PageSettings::self, NamePage::ServiceDisplayColors,
    "ЦВЕТА", "COLORS",
    "Выбор цветов дисплея",
    "The choice of colors display",
    mcScheme,
    mgcChannelA,
    mgcChannelB,
    mgcGrid,
    nullptr, nullptr, nullptr, nullptr
)


const Page *PageDisplay::PageSettings::PageColors::self = &pageColors;
