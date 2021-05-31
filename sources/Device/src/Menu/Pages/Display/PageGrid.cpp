// 2021/05/12 16:56:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/SettingsMain.h"


ColorType PageDisplay::PageGrid::colorType = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::GRID.index };


DEF_CHOICE_4(mcType, PageDisplay::PageGrid::self,
    "Тип", "Type",
    "Выбор типа сетки.",
    "Choice like Grid.",
    "Тип 1", "Type 1",
    "Тип 2", "Type 2",
    "Тип 3", "Type 3",
    "Тип 4", "Type 4",
    set.display.type_grid, nullptr, nullptr, nullptr
)


void PageDisplay::PageGrid::OnChangedBrightness()
{
    PageDisplay::PageGrid::colorType.SetBrightness(set.display.brightness_grid / 1e2F);
}


static void BeforeDraw_Grid_Brightness()
{
    PageDisplay::PageGrid::colorType.Init();
    set.display.brightness_grid = (int16)(PageDisplay::PageGrid::colorType.brightness * 100.0F);
}


DEF_GOVERNOR(mgBrightness, PageDisplay::PageGrid::self,
    "Яркость", "Brightness",
    "Устанавливает яркость сетки.",
    "Adjust the brightness of the Grid.",
    set.display.brightness_grid, 0, 100, nullptr, PageDisplay::PageGrid::OnChangedBrightness, BeforeDraw_Grid_Brightness
)


DEF_PAGE_2(pageGrid, PageDisplay::self, NamePage::DisplayGrid,
    "СЕТКА", "GRID",
    "Содержит настройки отображения координатной сетки.",
    "Contains settings of display of a coordinate Grid::",
    mcType,
    mgBrightness,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDisplay::PageGrid::self = &pageGrid;
