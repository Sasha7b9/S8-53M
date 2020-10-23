#include "defines.h"
#include "common/Hardware/Sound_c.h"
#include "common/Utils/Math_c.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


bool Menu::CurrentItemIsOpened(NamePage::E namePage)
{
    bool retValue = _GET_BIT(Menu::GetPosActItem(namePage), 7) == 1;
    return retValue;
}


Item* Menu::OpenedItem()
{
    TypeItem::E type = TypeItem::None;
    return const_cast<Page *>(PageMain::self)->RetLastOpened(&type);
}


Item* Menu::CurrentItem()
{
    TypeItem::E type = TypeItem::None;
    Item *lastOpened = ((Page *)PageMain::self)->RetLastOpened(&type);
    int8 pos = ((const Page *)lastOpened)->PosCurrentItem();
    if(type == TypeItem::Page && pos != 0x7f)
    {
        return ((const Page *)lastOpened)->GetItem(pos);
    }
    return lastOpened;
}


int Menu::PosItemOnTop(const Page *page)
{
    return page->GetCurrentSubPage() * Menu::ITEMS_ON_DISPLAY;
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
        if (((const Page *)item)->IsSB())                                   // Для страницы малых кнопок
        {
            SmallButton *sb = ((Page *)item)->SmallButonFrom(0);          // Выполняем функцию нажатия кнопки Key::Menu
            if (sb->OwnData()->funcOnPress)                                 // Если она есть
            {
                sb->OwnData()->funcOnPress();
            }
        }
        if(Menu::needClosePageSB)
        {
            NamePage::E namePage = item->Keeper()->OwnData()->name;
            SetPosActItem(namePage, GetPosActItem(namePage) & 0x7f);   // Сбрасываем бит 7 - "закрываем" активный пункт страницы namePage
        }
        Menu::needClosePageSB = true;
        if(item == PageMain::self)
        {
            Menu::Show(false);
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
