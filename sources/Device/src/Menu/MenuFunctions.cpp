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
    if(OpenedItem()->Type() == TypeItem::Page)
    {
        if (((const Page *)item)->IsSB())                                   // ��� �������� ����� ������
        {
            SmallButton *sb = ((Page *)item)->SmallButonFrom(0);          // ��������� ������� ������� ������ Key::Menu
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
        item->Open(false);
    } 
}


NamePage::E Menu::GetNameOpenedPage()
{
    return ((const Page *)OpenedItem())->GetName();
}
