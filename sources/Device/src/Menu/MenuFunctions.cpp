#include "defines.h"
#include "common/Utils/Math_c.h"
#include "Hardware/Sound.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"


extern const Page mainPage;


//TypeItem::E Menu::TypeMenuItem(const void *address) 
//{
//    if (address)
//    {
//        Item *item = (Item *)(address);
//        return item->data->type;
//    }
//    else
//    {
//        return TypeItem::None;
//    }
//}


bool Menu::CurrentItemIsOpened(NamePage::E namePage)
{
    bool retValue = _GET_BIT(MenuPosActItem(namePage), 7) == 1;
    return retValue;
}


int8 Menu::PosCurrentItem(const Page *page)
{
    return MenuPosActItem(page->OwnData()->name) & 0x7f;
}


void Menu::SetCurrentItem(const void *item, bool active)
{
    if(item != 0)
    {
        Page *page = (Keeper(item));
        if(!active)
        {
            SetMenuPosActItem(page->OwnData()->name, 0x7f);
        }
        else
        {
            for(int8 i = 0; i < page->NumItems(); i++)
            {
                if(GetItem(page, i) == item)
                {
                    SetMenuPosActItem(page->OwnData()->name, i);
                    return;
                }
            }
        }
    }
}


TypeItem::E Menu::TypeOpenedItem()
{
    return OpenedItem()->Type();
}


Item* Menu::OpenedItem()
{
    TypeItem::E type = TypeItem::None;
    return RetLastOpened((Page*)&mainPage, &type);
}


Item* Menu::GetItem(const Page *page, int numElement)
{
    const DataPage *own = page->OwnData();

    return own->items[numElement + (PageIsSB(page) ? 1 : 0)];
}


Item* Menu::CurrentItem()
{
    TypeItem::E type = TypeItem::None;
    Item *lastOpened = RetLastOpened((Page*)&mainPage, &type);
    int8 pos = PosCurrentItem((const Page *)lastOpened);
    if(type == TypeItem::Page && pos != 0x7f)
    {
        return GetItem((const Page *)lastOpened, pos);
    }
    return lastOpened;
}


int Menu::HeightOpenedItem(const Item *item) 
{
    TypeItem::E type = item->Type();
    if(type == TypeItem::Page)
    {
        int numItems = ((const Page *)item)->NumItems() - NumCurrentSubPage((Page *)item) * MENU_ITEMS_ON_DISPLAY;
        LIMITATION(numItems, numItems, 0, MENU_ITEMS_ON_DISPLAY);
        return MP_TITLE_HEIGHT + MI_HEIGHT * numItems;
    } 
    else if(type == TypeItem::Choice || type == TypeItem::ChoiceReg)
    {
        return MOI_HEIGHT_TITLE + ((Choice *)item)->NumSubItems() * MOSI_HEIGHT - 1;
    }
    return MI_HEIGHT;
}


int Menu::NumCurrentSubPage(const Page *page)
{
    return MenuCurrentSubPage(page->OwnData()->name);
}


const char* Menu::TitleItem(const void *item) 
{
    return TITLE((Page*)item);
}


int Menu::PosItemOnTop(const Page *page)
{
    return NumCurrentSubPage(page) * MENU_ITEMS_ON_DISPLAY;
}


bool Menu::IsFunctionalButton(Key::E button)
{
    return (button >= Key::F1) && (button <= Key::F5);
}


void Menu::ChangeSubPage(const Page *page, int delta)
{
    if (page)
    {
        if (delta > 0 && MenuCurrentSubPage(page->OwnData()->name) < NumSubPages(page) - 1)
        {
            Sound::RegulatorSwitchRotate();
            SetMenuCurrentSubPage(page->OwnData()->name, MenuCurrentSubPage(page->OwnData()->name) + 1);
        }
        else if (delta < 0 && MenuCurrentSubPage(page->OwnData()->name) > 0)
        {
            Sound::RegulatorSwitchRotate();
            SetMenuCurrentSubPage(page->OwnData()->name, MenuCurrentSubPage(page->OwnData()->name) - 1);
        }
    }
}


int Menu::NumSubPages(const Page *page)
{
    return (page->NumItems() - 1) / MENU_ITEMS_ON_DISPLAY + 1;
}


Item* Menu::RetLastOpened(Page *page, TypeItem::E *type)
{
    if(CurrentItemIsOpened(page->GetName()))
    {
        int8 posActItem = PosCurrentItem(page);
        Item *item = GetItem(page, posActItem);
        TypeItem::E typeLocal = GetItem(page, posActItem)->Type();
        if(typeLocal == TypeItem::Page)
        {
            return RetLastOpened((Page *)item, type);
        }
        else
        {
            return item;
        }
    }
    *type = TypeItem::Page;
    return page;
}


void Menu::CloseOpenedItem()
{
    void *item = OpenedItem();
    if(TypeOpenedItem() == TypeItem::Page)
    {
        if (PageIsSB((const Page *)item))                                   // Для страницы малых кнопок
        {
            SmallButton *sb = SmallButonFromPage((Page *)item, 0);          // Выполняем функцию нажатия кнопки Key::Menu
            if (sb->OwnData()->funcOnPress)                                 // Если она есть
            {
                sb->OwnData()->funcOnPress();
            }
        }
        if(NEED_CLOSE_PAGE_SB == 1)
        {
            NamePage::E namePage = Keeper(item)->OwnData()->name;
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
        OpenItem(item, false);
    } 
}


void Menu::OpenItem(const void *item, bool open)
{
    if(item)
    {
        Page *page = Keeper(item);
        SetMenuPosActItem(page->GetName(), open ? (PosCurrentItem(page) | 0x80) : (PosCurrentItem(page) & 0x7f));
    }
}


bool Menu::ItemIsOpened(const Item *item)
{
    TypeItem::E type = item->Type();
    Page *page = Keeper(item);
    if(type == TypeItem::Page)
    {
        return CurrentItemIsOpened(Keeper(item)->GetName());
    }
    return (MenuPosActItem(page->OwnData()->name) & 0x80) != 0;
}


Page* Menu::Keeper(const void *item)
{
    const Page* page = ((Page*)(item))->data->keeper;
    return (Page *)page;
}


NamePage::E Menu::GetNameOpenedPage()
{
    return ((const Page *)OpenedItem())->GetName();
}


void Menu::OpenPageAndSetItCurrent(void *page)
{
    SetCurrentItem(page, true);
    OpenItem((Page *)page, !ItemIsOpened((Page *)page));
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
        ChangeSubPage((const Page *)item, delta);
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


void Menu::ChangeItem(Item *item, int delta)
{
    TypeItem::E type = item->Type();
    if (type == TypeItem::Choice || type == TypeItem::ChoiceReg)
    {
        ((Choice *)item)->StartChange(delta);
    }
    else if (type == TypeItem::Governor)
    {
        Governor *governor = (Governor*)item;
        if (OpenedItem() != governor)
        {
            governor->StartChange(delta);
        }
        else
        {
            governor->ChangeValue(delta);
        }
    }
    else if (type == TypeItem::GovernorColor)
    {
        ((GovernorColor *)item)->ChangeValue(delta);
    }
}


void Menu::ShortPressOnPageItem(Page *page, int numItem)
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


Page* Menu::PagePointerFromName(NamePage::E)
{
    return 0;
}


bool Menu::PageIsSB(const Page *page)
{
    return page->OwnData()->name >= NamePage::SB_Curs;
}


SmallButton* Menu::SmallButonFromPage(Page *page, int numButton)
{
    const DataPage *own = page->OwnData();

    return (SmallButton *)(own->items)[numButton];
}
