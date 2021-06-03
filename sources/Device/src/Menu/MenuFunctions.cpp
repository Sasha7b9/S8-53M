// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Sound_.h"
#include "common/Utils/Math_.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"


const Item* Menu::CurrentItem(bool trace)
{
    if(trace) LOG_TRACE_WRITE("1");

    const Item *lastOpened = PageMain::self->RetLastOpened();

    if (trace) LOG_TRACE_WRITE("2");

    if (lastOpened->IsPage())
    {
        if (trace) LOG_TRACE_WRITE("3");
        const Page *page = (const Page *)lastOpened;

        if (trace) LOG_TRACE_WRITE("4");

        int8 pos = page->CurrentItem(trace)->GetPosition();

        if (trace) LOG_TRACE_WRITE("5");

        if (lastOpened->IsPage() && pos >= 0)
        {
            if (trace) LOG_TRACE_WRITE("6");
            return lastOpened->ReinterpretToPage()->GetItem(pos);
        }
    }

    if (trace) LOG_TRACE_WRITE("7");

    return lastOpened;
}


void Menu::CloseOpenedItem()
{
    const Item *item = Item::Opened();

    if(item->IsPage())
    {
        if (((const Page *)item)->IsPageSB())                                               // Для страницы малых кнопок
        {                                                                  // Выполняем функцию нажатия кнопки Key::Menu
            SmallButton *sb = item->ReinterpretToPage()->SmallButonFrom(0);

            if (sb->OwnData()->funcOnPress)                                                             // Если она есть
            {
                sb->OwnData()->funcOnPress();
            }

            if (needClosePageSB)
            {
                item->Keeper()->CloseOpenedItem();
            }

            needClosePageSB = true;
        }
        else
        {
            item->Keeper()->CloseOpenedItem();
        }
     
        if(item == PageMain::self)
        {
            Hide();
        }
    }
    else
    {
        item->Close();
    } 
}


NamePage::E Menu::GetNameOpenedPage()
{
    return Item::Opened()->ReinterpretToPage()->GetName();
}
