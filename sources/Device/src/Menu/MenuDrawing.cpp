#include "defines.h"
#include "common/Display/Primitives_c.h"
#include "common/Utils/Math_c.h"
#include "Display/Grid.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include <cstdio>


using namespace Primitives;


static void *itemUnderButton[Key::Count] = {0};


Key::E GetFuncButtonFromY(int _y)
{
    int y = Grid::TOP + Grid::Height() / 12;
    int step = Grid::Height() / 6;
    Key::E button = Key::None;
    for(int i = 0; i < 6; i++)
    {
        if(_y < y)
        {
            return button;
        }
        ++button;
        y += step;
    }
    return  Key::F5;
}


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
    TypeItem::E type = Menu::TypeMenuItem(gItemHint);
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
    if(MenuIsShown() || TypeMenuItem(OpenedItem()) != TypeItem::Page)
    {
        ResetItemsUnderButton();
        void *item = OpenedItem();
        if(MenuIsShown())
        {
            if (TypeMenuItem(item) == TypeItem::Page)
            {
                DrawOpenedPage((Page *)item, 0, Grid::TOP);
            }
            else if(Keeper(item) != &Page::empty)
            {
                DrawOpenedPage(Keeper(item), 0, Grid::TOP);
            }
        }
        else
        {
            if(TypeMenuItem(item) == TypeItem::Choice)
            {
                ((Choice *)item)->Draw(CalculateX(0), Grid::TOP, true);
            }
            else if(TypeMenuItem(item) == TypeItem::Governor)
            {
                ((Governor *)item)->Draw(CalculateX(0), Grid::TOP, true);
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


void Menu::DrawTitlePage(Page *page, int layer, int yTop)
{
    int x = CalculateX(layer);
    if (PageIsSB(page))
    {
        SmallButonFromPage(page, 0)->Draw(LEFT_SB, yTop + 3);
        return;
    }
    int height = HeightOpenedItem(page);
    bool shade = CurrentItemIsOpened(page->GetName());
    Region(MP_TITLE_WIDTH + 2, height + 2).Fill(x - 1, yTop, COLOR_BACK);
    Rectangle(MP_TITLE_WIDTH + 1, height + 1).Draw(x, yTop, ColorBorderMenu(shade));

    if (shade)
    {
        Region(MP_TITLE_WIDTH - 1, MP_TITLE_HEIGHT - 1).Fill(x + 1, yTop + 1, Color::MenuTitleLessBright());
        Region(MP_TITLE_WIDTH - 7, MP_TITLE_HEIGHT - 7).Fill(x + 4, yTop + 4, Color::MENU_TITLE_DARK);
    }
    else
    {
        Painter::DrawVolumeButton(x + 1, yTop + 1, MP_TITLE_WIDTH - 1, MP_TITLE_HEIGHT - 1, 3, ColorMenuTitle(false), ColorMenuTitleBrighter(), Color::MenuTitleLessBright(), shade, false);
    }
    
    VLine().Draw(x, yTop, yTop + HeightOpenedItem(page), ColorBorderMenu(false));
    bool condDrawRSet = NumSubPages(page) > 1 && TypeMenuItem(CurrentItem()) != TypeItem::ChoiceReg && TypeMenuItem(CurrentItem()) != TypeItem::Governor && TypeOpenedItem() == TypeItem::Page;
    int delta = condDrawRSet ? -10 : 0;
    Color::E colorText = shade ? LightShadingTextColor() : Color::BLACK;
    x = Painter::DrawStringInCenterRectC(x, yTop, MP_TITLE_WIDTH + 2 + delta, MP_TITLE_HEIGHT, TitleItem(page), colorText);
    if(condDrawRSet)
    {
        Painter::Draw4SymbolsInRectC(x + 4, yTop + 11, GetSymbolForGovernor(NumCurrentSubPage(page)), colorText);
    }

    itemUnderButton[GetFuncButtonFromY(yTop)] = page;

    delta = 0;
    
    Color::SetCurrent(colorText);
    DrawPagesUGO(page, CalculateX(layer) + MP_TITLE_WIDTH - 3 + delta, yTop + MP_TITLE_HEIGHT - 2 + delta);
}


void Menu::DrawPagesUGO(const Page *page, int right, int bottom)
{
    int size = 4;
    int delta = 2;
    
    int allPages = (NumItemsInPage(page)- 1) / MENU_ITEMS_ON_DISPLAY + 1;
    int currentPage = NumCurrentSubPage(page);

    int left = right - (size + 1) * allPages - delta + (3 - allPages);
    int top = bottom - size - delta;

    for(int p = 0; p < allPages; p++)
    {
        int x = left + p * (size + 2);
        if(p == currentPage)
        {
            Region(size, size).Fill(x, top);
        }
        else
        {
            Rectangle(size, size).Draw(x, top);
        }
    }
}


static void DrawGovernor(void* item, int x, int y)
{
    ((Governor *)item)->Draw(x, y, false);
}


static void DrawIPaddress(void* item, int x, int y)
{
    ((IPaddress *)item)->Draw(x, y, false);
}


static void DrawMACaddress(void* item, int x, int y)
{
    ((MACaddress *)item)->Draw(x, y, false);
}


static void DrawFormula(void* item, int x, int y)
{
    ((Formula *)item)->Draw(x, y, false);
}


static void DrawChoice(void *item, int x, int y)
{
    ((Choice *)item)->Draw(x, y, false);
}


static void DrawSmallButton(void *item, int, int y)
{
    ((SmallButton *)item)->Draw(LEFT_SB, y + 7);
}


static void DrawTime(void *item, int x, int y)
{
    ((TimeItem *)item)->Draw(x, y, false);
}


static void DrawGovernorColor(void *item, int x, int y)
{
    ((GovernorColor *)item)->Draw(x, y, false);
}


static void DrawButton(void *item, int x, int y)
{
    ((Button *)item)->Draw(x, y);
}


static void DrawPage(void *item, int x, int y)
{
    ((Page *)item)->Draw(x, y);
}


void Menu::DrawItemsPage(const Page *page, int layer, int yTop)
{
    void (*funcOfDraw[TypeItem::Count])(void*, int, int) = 
    {  
        EmptyFuncpVII,      // TypeItem::None
        DrawChoice,         // TypeItem::Choice
        DrawButton,         // TypeItem::Button
        DrawPage,           // TypeItem::Page
        DrawGovernor,       // TypeItem::Governor
        DrawTime,           // TypeItem::Time
        DrawIPaddress,      // TypeItem::IP
        EmptyFuncpVII,      // Item_SwitchButton
        DrawGovernorColor,  // TypeItem::GovernorColor
        DrawFormula,        // Item_Formula
        DrawMACaddress,     // Item_Mac
        DrawChoice,         // TypeItem::ChoiceReg
        DrawSmallButton     // TypeItem::SmallButton
    };
    int posFirstItem = PosItemOnTop(page);
    int posLastItem = posFirstItem + MENU_ITEMS_ON_DISPLAY - 1;
    LIMITATION(posLastItem, posLastItem, 0, NumItemsInPage(page) - 1);
    int count = 0;
    for(int posItem = posFirstItem; posItem <= posLastItem; posItem++)
    {
        void *item = GetItem(page, posItem);
        TypeItem::E type = TypeMenuItem(item);
        int top = yTop + MI_HEIGHT * count;
        funcOfDraw[type](item, CalculateX(layer), top);
        count++;
        itemUnderButton[GetFuncButtonFromY(top)] = item;
    }
}


void Menu::DrawOpenedPage(Page *page, int layer, int yTop)
{
    DrawTitlePage(page, layer, yTop);
    DrawItemsPage(page, layer, yTop + MP_TITLE_HEIGHT);
    if (CurrentItemIsOpened(page->GetName()))
    {
        int8 posCurItem = PosCurrentItem(page);
        void *item = GetItem(page, posCurItem);
        for (int i = 0; i < 5; i++)
        {
            if (itemUnderButton[i + Key::F1] != item)
            {
                itemUnderButton[i + Key::F1] = 0;
            }
        }
        TypeItem::E type = TypeMenuItem(item);
        if (type == TypeItem::Choice || type == TypeItem::ChoiceReg)
        {
            ((Choice *)item)->Draw(CalculateX(1), ItemOpenedPosY(item), true);
        }
        else if (type == TypeItem::Governor)
        {
            ((Governor *)item)->Draw(CalculateX(1), ItemOpenedPosY(item), true);
        }
        else if (type == TypeItem::GovernorColor)
        {
            ((GovernorColor *)item)->Draw(CalculateX(1), ItemOpenedPosY(item), true);
        }
        else if (type == TypeItem::Time)
        {
            ((TimeItem *)item)->Draw(CalculateX(1), ItemOpenedPosY(item), true);
        }
        else if (type == TypeItem::IP)
        {
            ((IPaddress *)item)->Draw(CalculateX(1), ItemOpenedPosY(item), true);
        }
        else if (type == TypeItem::MAC)
        {
            ((MACaddress *)item)->Draw(CalculateX(1), ItemOpenedPosY(item), true);
        }
    }

    if (page->OwnData()->funcOnDraw)
    {
        page->OwnData()->funcOnDraw();
    }
}


int Menu::CalculateX(int layer)
{
    return MP_X - layer * Grid::DELTA / 4;
}


bool Menu::IsShade(const void* item)
{
    return CurrentItemIsOpened(Keeper(item)->GetName()) && (item != OpenedItem());
}


bool Menu::IsPressed(const void *item)
{
    return item == ItemUnderKey();
}


void* Menu::ItemUnderButton(Key::E button)
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


int Menu::ItemOpenedPosY(const void *item)
{
    Page *page = Keeper(item);
    int8 posCurItem = PosCurrentItem(page);
    int y = Grid::TOP + (posCurItem % MENU_ITEMS_ON_DISPLAY) * MI_HEIGHT + MP_TITLE_HEIGHT;
    if(y + HeightOpenedItem(item) > Grid::Bottom())
    {
        y = Grid::Bottom() - HeightOpenedItem(item) - 2;
    }
    return y + 1;
}
