#include "defines.h"
#include "common/Display/Painter_.h"
#include "common/Display/Primitives_.h"
#include "common/Display/Text_.h"
#include "common/Utils/Math_.h"
#include "Display/Grid.h"
#include "Menu/Menu.h"
#include "Menu/Pages/HelpContent.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include <cstdio>


using namespace Primitives;


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
    Page *item = (Page*)PageHelpContent::itemHint; //-V2533

    const int SIZE = 100;
    char title[SIZE];
    std::snprintf(title, SIZE, "%s \"%s\"", names[type][LANG], TITLE(item)); //-V2563

    if (item->IsSmallButton())
    {
        y -= 9;
    }
    Text(title).DrawInCenterRectAndBoundIt(x, y, width, 15, Color::BACK, Color::FILL);
    y = Text(HINT(item)).DrawInBoundedRectWithTransfers(x, y + 15, width, Color::BACK, Color::FILL); //-V2563
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
                ((Page *)item)->DrawOpened(Grid::TOP); //-V2533 //-V2567
            }
            else if(item->Keeper()->IsExist()) //-V2516
            {
                item->Keeper()->DrawOpened(Grid::TOP);
            }
        }
        else
        {
            if(item->IsChoice())
            {
                ((Choice *)item)->Draw(Page::X(), Grid::TOP, true); //-V2533 //-V2567
            }
            else if(item->IsGovernor()) //-V2516
            {
                ((Governor *)item)->Draw(Page::X(), Grid::TOP, true); //-V2533 //-V2567
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
        else if (PageHelpContent::itemHint) //-V2516
        {
            DrawHintItem(x, y, width - 1);
        }
    }
}


Item* Menu::ItemUnderButton(Key::E)
{
    return nullptr;
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
