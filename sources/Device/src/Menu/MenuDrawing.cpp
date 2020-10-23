#include "defines.h"
#include "common/Display/Painter_c.h"
#include "common/Display/Primitives_c.h"
#include "common/Display/Text_c.h"
#include "common/Utils/Math_c.h"
#include "Display/Grid.h"
#include "Menu/Menu.h"
#include "Menu/Pages/HelpContent.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include <cstdio>


using namespace Primitives;


Item *Menu::itemUnderButton[Key::Count];


static void DrawHintItem(int x, int y, int width)
{
    if (!PageHelpContent::itemHint)
    {
        return;
    }

    const char *names[TypeItem::Count][2] =
    {
        {"",            ""},        // TypeItem::None
        {"",            ""},        // TypeItem::Choice
        {"������",      "Button"},  // TypeItem::Button
        {"��������",    "Page"},    // TypeItem::Page
        {"",            ""},        // TypeItem::Governor
        {"",            ""},        // TypeItem::Time
        {"",            ""},        // TypeItem::IP
        {"",            ""},        // Item_SwitchButton
        {"",            ""},        // TypeItem::GovernorColor
        {"",            ""},        // Item_Formula
        {"",            ""},        // TypeItem::MAC
        {"",            ""},        // TypeItem::ChoiceReg
        {"������",      "Button"}   // TypeItem::SmallButton
    };
    TypeItem::E type = PageHelpContent::itemHint->Type();
    Page *item = (Page*)PageHelpContent::itemHint;

    const int SIZE = 100;
    char title[SIZE];
    std::snprintf(title, SIZE, "%s \"%s\"", names[type][LANG], TITLE(item));

    if (item->data->type == TypeItem::SmallButton)
    {
        y -= 9;
    }
    Text(title).DrawInCenterRectAndBoundIt(x, y, width, 15, Color::BACK, Color::FILL);
    y = Text(HINT(item)).DrawInBoundedRectWithTransfers(x, y + 15, width, Color::BACK, Color::FILL);
    if (item->data->type == TypeItem::SmallButton)
    {
        reinterpret_cast<SmallButton *>(item)->DrawHints(x, y, width);
    }
}


void Menu::Draw()
{
    if(Menu::IsShown() || OpenedItem()->Type() != TypeItem::Page)
    {
        ResetItemsUnderButton();
        Item *item = OpenedItem();
        if(Menu::IsShown())
        {
            if (item->IsPage())
            {
                ((Page *)item)->DrawOpened(Grid::TOP);
            }
            else if(item->Keeper()->IsExist())
            {
                item->Keeper()->DrawOpened(Grid::TOP);
            }
        }
        else
        {
            if(item->Type() == TypeItem::Choice)
            {
                ((Choice *)item)->Draw(MP_X, Grid::TOP, true);
            }
            else if(item->Type() == TypeItem::Governor)
            {
                ((Governor *)item)->Draw(MP_X, Grid::TOP, true);
            }
        }
    }

    if (Menu::showHelpHints)
    {
        int x = 1;
        int y = 0;
        int width = Menu::IsMinimize() ? 289 : 220;

        Text(LANG_RU ? "������� ����� ���������. � ���� ������ ��� ������� �� ������ �� ����� ��������� ���������� � � ����������. "
            "����� ��������� ���� �����, ������� ������ ������ � ����������� � � ������� 0.5�."
            :
            "Mode is activated hints. In this mode, pressing the button displays the information on its purpose. "
            "To disable this mode, press the button HELP and hold it for 0.5s.").DrawInBoundedRectWithTransfers(x, y, width - 1, Color::BACK, Color::FILL);

        y += LANG_RU ? 49 : 40;
        if (PageHelpContent::stringForHint)
        {
            Text(PageHelpContent::stringForHint).DrawInBoundedRectWithTransfers(x, y, width - 1, Color::BACK, Color::FILL);
        }
        else if (PageHelpContent::itemHint)
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
    int y = Grid::TOP + (posCurItem % Menu::ITEMS_ON_DISPLAY) * MI_HEIGHT + MP_TITLE_HEIGHT;
    if(y + HeightOpened() > Grid::Bottom())
    {
        y = Grid::Bottom() - HeightOpened() - 2;
    }
    return y + 1;
}
