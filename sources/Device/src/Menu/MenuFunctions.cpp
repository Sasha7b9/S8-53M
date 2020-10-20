#include "defines.h"
#include "common/Utils/Math_c.h"
#include "Hardware/Sound.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"


extern const Page mainPage;


bool Menu::CurrentItemIsOpened(NamePage::E namePage)
{
    bool retValue = _GET_BIT(MenuPosActItem(namePage), 7) == 1;
    return retValue;
}


TypeItem::E Menu::TypeOpenedItem()
{
    return OpenedItem()->Type();
}


Item* Menu::OpenedItem()
{
    TypeItem::E type = TypeItem::None;
    return ((Page*)&mainPage)->RetLastOpened(&type);
}


Item* Menu::CurrentItem()
{
    TypeItem::E type = TypeItem::None;
    Item *lastOpened = ((Page*)&mainPage)->RetLastOpened(&type);
    int8 pos = ((const Page *)lastOpened)->PosCurrentItem();
    if(type == TypeItem::Page && pos != 0x7f)
    {
        return ((const Page *)lastOpened)->GetItem(pos);
    }
    return lastOpened;
}


int Menu::HeightOpenedItem(const Item *item) 
{
    TypeItem::E type = item->Type();
    if(type == TypeItem::Page)
    {
        int numItems = ((const Page *)item)->NumItems() - ((Page *)item)->NumCurrentSubPage() * MENU_ITEMS_ON_DISPLAY;
        LIMITATION(numItems, numItems, 0, MENU_ITEMS_ON_DISPLAY);
        return MP_TITLE_HEIGHT + MI_HEIGHT * numItems;
    } 
    else if(type == TypeItem::Choice || type == TypeItem::ChoiceReg)
    {
        return MOI_HEIGHT_TITLE + ((Choice *)item)->NumSubItems() * MOSI_HEIGHT - 1;
    }
    return MI_HEIGHT;
}


const char* Menu::TitleItem(const void *item) 
{
    return TITLE((Page*)item);
}


int Menu::PosItemOnTop(const Page *page)
{
    return page->NumCurrentSubPage() * MENU_ITEMS_ON_DISPLAY;
}


bool Menu::IsFunctionalButton(Key::E button)
{
    return (button >= Key::F1) && (button <= Key::F5);
}


void Menu::CloseOpenedItem()
{
    Item *item = OpenedItem();
    if(TypeOpenedItem() == TypeItem::Page)
    {
        if (((const Page *)item)->IsSB())                                   // Для страницы малых кнопок
        {
            SmallButton *sb = SmallButonFromPage((Page *)item, 0);          // Выполняем функцию нажатия кнопки Key::Menu
            if (sb->OwnData()->funcOnPress)                                 // Если она есть
            {
                sb->OwnData()->funcOnPress();
            }
        }
        if(NEED_CLOSE_PAGE_SB == 1)
        {
            NamePage::E namePage = item->Keeper()->OwnData()->name;
            SetMenuPosActItem(namePage, MenuPosActItem(namePage) & 0x7f);   // Сбрасываем бит 7 - "закрываем" активный пункт страницы namePage
        }
        NEED_CLOSE_PAGE_SB = 1;
        if(item == &mainPage)
        {
            ShowMenu(false);
        }
    }
    else
    {
        item->Open(false);
    } 
}


NamePage::E Menu::GetNameOpenedPage()
{
    return ((const Page *)OpenedItem())->GetName();
}


void Menu::OpenPageAndSetItCurrent(Item *page)
{
    page->SetCurrent(true);
    ((Page *)page)->Open(!((Page *)page)->IsOpened());
}


bool Menu::ChangeOpenedItem(Item *item, int delta)
{
    if (delta < 2 && delta > -2)
    {
        return false;
    }

    TypeItem::E type = item->Type();

    if (type == TypeItem::Page)
    {
        ((const Page *)item)->ChangeSubPage(delta);
    }
    else if (type == TypeItem::IP)
    {
        ((IPaddress *)item)->ChangeValue(delta);
    }
    else if (type == TypeItem::MAC)
    {
        ((MACaddress *)item)->ChangeValue(delta);
    }
    else if (type == TypeItem::ChoiceReg || type == TypeItem::Choice)
    {
        ((Choice *)item)->ChangeValue(delta);
    }
    else if (type == TypeItem::Governor)
    {
        ((Governor *)item)->ChangeValue(delta);
    }
    
    return true;
}


void Menu::ShortPressOnPageItem(const Page *page, int numItem)
{
    const DataPage *own = page->OwnData();

    if (page->Type() != TypeItem::Page)
    {
        return;
    }
    NamePage::E namePage = own->name;
    if (namePage >= NamePage::SB_Curs)
    {
        SmallButton *sb = (SmallButton*)(own->items)[numItem];
        if (sb && own->funcOnPress)
        {
            own->funcOnPress();
        }
    }
}


SmallButton* Menu::SmallButonFromPage(const Page *page, int numButton)
{
    const DataPage *own = page->OwnData();

    return (SmallButton *)(own->items)[numButton];
}
