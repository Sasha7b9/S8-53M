#include "defines.h"
#include "common/Display/Primitives_c.h"
#include "common/Utils/Math_c.h"
#include "Display/Grid.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include <cstdio>


using namespace Primitives;


Item *Menu::itemUnderButton[Key::Count];


static void DrawHintItem(int x, int y, int width)
{
    if (!gItemHint)
    {
        return;
    }

    const char *names[TypeItem::Count][2] =
    {
        {"",            ""},        // TypeItem::None
        {"",            ""},        // TypeItem::Choice
        {"Кнопка",      "Button"},  // TypeItem::Button
        {"Страница",    "Page"},    // TypeItem::Page
        {"",            ""},        // TypeItem::Governor
        {"",            ""},        // TypeItem::Time
        {"",            ""},        // TypeItem::IP
        {"",            ""},        // Item_SwitchButton
        {"",            ""},        // TypeItem::GovernorColor
        {"",            ""},        // Item_Formula
        {"",            ""},        // TypeItem::MAC
        {"",            ""},        // TypeItem::ChoiceReg
        {"Кнопка",      "Button"}   // TypeItem::SmallButton
    };
    TypeItem::E type = gItemHint->Type();
    Language::E lang = set.common.lang;
    Page *item = (Page*)gItemHint;

    const int SIZE = 100;
    char title[SIZE];
    std::snprintf(title, SIZE, "%s \"%s\"", names[type][lang], TITLE(item));

    if (item->data->type == TypeItem::SmallButton)
    {
        y -= 9;
    }
    Painter::DrawStringInCenterRectAndBoundItC(x, y, width, 15, title, COLOR_BACK, COLOR_FILL);
    y = Painter::DrawTextInBoundedRectWithTransfers(x, y + 15, width, HINT(item), COLOR_BACK, COLOR_FILL);
    if (item->data->type == TypeItem::SmallButton)
    {
        Painter::DrawHintsForSmallButton(x, y, width, reinterpret_cast<SmallButton*>(item));
    }
}


void Menu::Draw()
{
    if(MenuIsShown() || OpenedItem()->Type() != TypeItem::Page)
    {
        ResetItemsUnderButton();
        Item *item = OpenedItem();
        if(MenuIsShown())
        {
            if (item->Type() == TypeItem::Page)
            {
                ((Page *)item)->DrawOpened(0, Grid::TOP);
            }
            else if(item->Keeper() != &Page::empty)
            {
                item->Keeper()->DrawOpened(0, Grid::TOP);
            }
        }
        else
        {
            if(item->Type() == TypeItem::Choice)
            {
                ((Choice *)item)->Draw(item->CalculateX(0), Grid::TOP, true);
            }
            else if(item->Type() == TypeItem::Governor)
            {
                ((Governor *)item)->Draw(item->CalculateX(0), Grid::TOP, true);
            }
        }
    }

    if (SHOW_HELP_HINTS)
    {
        int x = 1;
        int y = 0;
        int width = MenuIsMinimize() ? 289 : 220;
        Painter::DrawTextInBoundedRectWithTransfers(x, y, width - 1,
            LANG_RU ?    "Включён режим подсказок. В этом режиме при нажатии на кнопку на экран выводится информация о её назначении. "
                         "Чтобы выключить этот режим, нажмите кнопку ПОМОЩЬ и удерживайте её в течение 0.5с." : 
                         "Mode is activated hints. In this mode, pressing the button displays the information on its purpose. "
                         "To disable this mode, press the button HELP and hold it for 0.5s.",
                         COLOR_BACK, COLOR_FILL);
        y += LANG_RU ? 49 : 40;
        if (gStringForHint)
        {
            Painter::DrawTextInBoundedRectWithTransfers(x, y, width - 1, gStringForHint, COLOR_BACK, COLOR_FILL);
        }
        else if (gItemHint)
        {
            DrawHintItem(x, y, width - 1);
        }
    }
}


Item* Menu::ItemUnderButton(Key::E button)
{
    return itemUnderButton[button];
}


void Menu::ResetItemsUnderButton()
{
    for(int i = 0; i < Key::Count; i++)
    {
        itemUnderButton[i] = 0;
    }
}


int Item::OpenedPosY() const
{
    Page *page = Keeper();
    int8 posCurItem = page->PosCurrentItem();
    int y = Grid::TOP + (posCurItem % MENU_ITEMS_ON_DISPLAY) * MI_HEIGHT + MP_TITLE_HEIGHT;
    if(y + HeightOpened() > Grid::Bottom())
    {
        y = Grid::Bottom() - HeightOpened() - 2;
    }
    return y + 1;
}
