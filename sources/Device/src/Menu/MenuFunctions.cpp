#include "defines.h"
#include "common/Hardware/Sound_c.h"
#include "common/Utils/Math_c.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


Item* Menu::OpenedItem()
{
    TypeItem::E type = TypeItem::None;
    return const_cast<Page *>(PageMain::self)->RetLastOpened(&type);
}


Item* Menu::CurrentItem()
{
    TypeItem::E type = TypeItem::None;
    Item *lastOpened = ((Page *)PageMain::self)->RetLastOpened(&type);
    int8 pos = ((const Page *)lastOpened)->GetPositionActItem();
    if(type == TypeItem::Page && pos != -1)
    {
        return ((const Page *)lastOpened)->GetItem(pos);
    }
    return lastOpened;
}


bool Menu::IsCurrentItem(const Item *item)
{
    return CurrentItem() == item;
}


void Menu::CloseOpenedItem()
{
    Item *item = OpenedItem();
    if(OpenedItem()->Type() == TypeItem::Page)
    {
        if (((const Page *)item)->IsPageSB())                                   // Для страницы малых кнопок
        {
            SmallButton *sb = ((Page *)item)->SmallButonFrom(0);          // Выполняем функцию нажатия кнопки Key::Menu
            if (sb->OwnData()->funcOnPress)                                 // Если она есть
            {
                sb->OwnData()->funcOnPress();
            }
        }
        if(Menu::needClosePageSB)
        {
            item->Keeper()->CloseOpenedItem();
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


bool Menu::IsOpenedItem(const Item *item)
{
    return (item == OpenedItem());
}


NamePage::E Menu::GetNameOpenedPage()
{
    return ((const Page *)OpenedItem())->GetName();
}
