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


void Menu::SetCurrentItem(const Item *item, bool active)
{
    if(item != 0)
    {
        Page *page = (item->Keeper());
        if(!active)
        {
            SetMenuPosActItem(page->OwnData()->name, 0x7f);
        }
        else
        {
            for(int8 i = 0; i < page->NumItems(); i++)
            {
                if(page->GetItem(i) == item)
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


Item* Menu::CurrentItem()
{
    TypeItem::E type = TypeItem::None;
    Item *lastOpened = RetLastOpened((Page*)&mainPage, &type);
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


void Menu::ChangeSubPage(const Page *page, int delta)
{
    if (page)
    {
        if (delta > 0 && MenuCurrentSubPage(page->OwnData()->name) < page->NumSubPages() - 1)
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


Item* Menu::RetLastOpened(Page *page, TypeItem::E *type)
{
    if(CurrentItemIsOpened(page->GetName()))
    {
        int8 posActItem = page->PosCurrentItem();
        Item *item = page->GetItem(posActItem);
        TypeItem::E typeLocal = page->GetItem(posActItem)->Type();
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
    Item *item = OpenedItem();
    if(TypeOpenedItem() == TypeItem::Page)
    {
        if (((const Page *)item)->IsSB())                                   // ��� �������� ����� ������
        {
            SmallButton *sb = SmallButonFromPage((Page *)item, 0);          // ��������� ������� ������� ������ Key::Menu
            if (sb->OwnData()->funcOnPress)                                 // ���� ��� ����
            {
                sb->OwnData()->funcOnPress();
            }
        }
        if(NEED_CLOSE_PAGE_SB == 1)
        {
            NamePage::E namePage = item->Keeper()->OwnData()->name;
            SetMenuPosActItem(namePage, MenuPosActItem(namePage) & 0x7f);   // ���������� ��� 7 - "���������" �������� ����� �������� namePage
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


void Menu::OpenItem(const Item *item, bool open)
{
    if(item)
    {
        Page *page = item->Keeper();
        SetMenuPosActItem(page->GetName(), open ? (page->PosCurrentItem() | 0x80) : (page->PosCurrentItem() & 0x7f));
    }
}


bool Menu::ItemIsOpened(const Item *item)
{
    TypeItem::E type = item->Type();
    Page *page = item->Keeper();
    if(type == TypeItem::Page)
    {
        return CurrentItemIsOpened(item->Keeper()->GetName());
    }
    return (MenuPosActItem(page->OwnData()->name) & 0x80) != 0;
}


NamePage::E Menu::GetNameOpenedPage()
{
    return ((const Page *)OpenedItem())->GetName();
}


void Menu::OpenPageAndSetItCurrent(Item *page)
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


SmallButton* Menu::SmallButonFromPage(Page *page, int numButton)
{
    const DataPage *own = page->OwnData();

    return (SmallButton *)(own->items)[numButton];
}
