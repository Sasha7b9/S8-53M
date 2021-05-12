// 2021/05/12 17:14:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


DEF_CHOICE_2(mcSettings_Colors_Scheme, PageDisplay::PageSettings::PageColors::self,
    "�������� �����", "Color scheme",
    "����� ������ �����������",
    "Mode of operation of the calibrator",
    "����� 1", "Scheme 1",
    "����� 2", "Scheme 2",
    set.service.colorScheme, nullptr, nullptr, nullptr
)


static ColorType colorT1 = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::DATA_A.index };

DEF_GOVERNOR_COLOR(mgcSettings_Colors_ChannelA, PageDisplay::PageSettings::PageColors::self,
    "����� 1", "Channel 1",
    "",
    "",
    colorT1, nullptr
)


static ColorType colorT2 = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::DATA_B.index };

DEF_GOVERNOR_COLOR(mgcSettings_Colors_ChannelB, PageDisplay::PageSettings::PageColors::self,
    "����� 2", "Channel 2",
    "",
    "",
    colorT2, nullptr
)


DEF_GOVERNOR_COLOR(mgcSettings_Colors_Grid, PageDisplay::PageSettings::PageColors::self,
    "�����", "Grid",
    "������������� ���� �����",
    "Sets the grid color",
    PageDisplay::PageGrid::typeGrid, nullptr
)


DEF_PAGE_4(pageColors, PageDisplay::PageSettings::self, NamePage::ServiceDisplayColors,
    "�����", "COLORS",
    "����� ������ �������",
    "The choice of colors display",
    mcSettings_Colors_Scheme,
    mgcSettings_Colors_ChannelA,
    mgcSettings_Colors_ChannelB,
    mgcSettings_Colors_Grid,
    nullptr, nullptr, nullptr, nullptr
)


const Page *PageDisplay::PageSettings::PageColors::self = &pageColors;
