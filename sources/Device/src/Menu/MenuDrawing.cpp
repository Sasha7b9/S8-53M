#include "defines.h"
#include "common/Display/Painter/Text_.h"
#include "common/Utils/Math_.h"
#include "Menu/Menu.h"
#include "Menu/Pages/HelpContent.h"
#include "Settings/Settings.h"
#include <cstdio>


static void DrawHintItem(int x, int y, int width)
{
    if (!PageHelpContent::itemHint)
    {
        return;
    }

    pchar names[TypeItem::Count][2] =
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

    if (item->IsSmallButton())
    {
        y -= 9;
    }
    Text(title).DrawInCenterRectAndBoundIt(x, y, width, 15, Color::BACK, Color::FILL);
    y = Text(HINT(item)).DrawInBoundedRectWithTransfers(x, y + 15, width, Color::BACK, Color::FILL);
    if (item->IsSmallButton())
    {
        reinterpret_cast<SmallButton *>(item)->DrawHints(x, y, width);
    }
}


void Menu::Draw()
{
    if(Menu::IsShown() || !Item::Opened()->IsPage())
    {
        const Item *item = Item::Opened();

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
            if(item->IsChoice())
            {
                ((Choice *)item)->Draw(Page::X(), Grid::TOP, true);
            }
            else if(item->IsGovernor())
            {
                ((Governor *)item)->Draw(Page::X(), Grid::TOP, true);
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


int Item::OpenedPosY() const
{
    Page *page = Keeper();
    int8 posCurItem = page->GetPositionActItem();
    int y = Grid::TOP + (posCurItem % Menu::ITEMS_ON_DISPLAY) * Item::HEIGHT + Item::TITLE_HEIGHT;
    if(y + HeightOpened() > Grid::Bottom())
    {
        y = Grid::Bottom() - HeightOpened() - 2;
    }
    return y + 1;
}
