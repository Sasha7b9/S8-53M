#include "defines.h"
#include "common/Hardware/Sound_c.h"
#include "common/Utils/Math_c.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


const Item* Menu::OpenedItem()
{
    return PageMain::self->RetLastOpened();
}


const Item* Menu::CurrentItem()
{
    const Item *lastOpened = PageMain::self->RetLastOpened();
    int8 pos = ((const Page *)lastOpened)->GetPositionActItem(); //-V2533
    if(lastOpened->IsPage() && pos != -1)
    {
        return lastOpened->ReinterpretToPage()->GetItem(pos);
    }
    return lastOpened;
}


bool Menu::IsCurrentItem(const Item *item)
{
    return CurrentItem() == item;
}


void Menu::CloseOpenedItem()
{
    const Item *item = OpenedItem();
    if(item->IsPage())
    {
        if (((const Page *)item)->IsPageSB())                                       // Для страницы малых кнопок //-V2533
        {
            SmallButton *sb = item->ReinterpretToPage()->SmallButonFrom(0);         // Выполняем функцию нажатия кнопки Key::Menu
            if (sb->OwnData()->funcOnPress)                                         // Если она есть
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
    return OpenedItem()->ReinterpretToPage()->GetName();
}
