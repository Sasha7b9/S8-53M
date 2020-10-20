#include "defines.h"
#include "common/Utils/Math_c.h"
#include "Hardware/Sound.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"


// »спользуетс€ дл€ анимации изменени€ значени€ Choice
struct TimeStructChoice
{
    Choice* choice;                 // јдрес Choice, который находитс€ в данный момент в движении. ≈сли 0 - все статичны.
    uint        timeStartMS;        // ¬рем€ начала анимации choice.
    uint        inMoveIncrease : 1;
    uint        inMoveDecrease : 1;
};

static TimeStructChoice tsChoice;


DataItem Item::emptyData = { TypeItem::None, nullptr, nullptr, nullptr, nullptr };
Item Item::empty;
static Item *itemsPage[] = { &Item::empty, nullptr };
static const DataPage emptyDataPage = { NamePage::Empty, &itemsPage[0], nullptr, nullptr };
const char *titleHint[4] = { "", "", "", "" };
static const DataItem emptyDataItem = { TypeItem::None, nullptr, nullptr, titleHint, &emptyDataPage };
Page Page::empty(&emptyDataItem);


Item::Item(const DataItem *const _data) : data(_data)
{
    if (data == nullptr)
    {
        data = &emptyData;
    }
}


bool Item::IsPressed() const
{
    return this == Menu::ItemUnderKey();
}


const char *Choice::NameSubItem(int i) const
{
    return OwnData()->names[(i * 2) + LANG];
}


const char *Choice::NameCurrentSubItem() const
{
    const DataChoice *own = OwnData();

    return (own->cell == 0) ? "" : NameSubItem(*own->cell);
}


const char *Choice::NameNextSubItem() const
{
    const DataChoice *own = OwnData();

    if (own->cell == 0)
    {
        return "";
    }
    int index = *own->cell + 1;
    if (index == NumSubItems())
    {
        index = 0;
    }
    return NameSubItem(index);
}


const char *Choice::NamePrevSubItem() const
{
    const DataChoice *own = OwnData();

    if (own->cell == 0)
    {
        return "";
    }
    int index = *own->cell - 1;
    if (index < 0)
    {
        index = NumSubItems() - 1;
    }
    return NameSubItem(index);
}


int Choice::NumSubItems() const
{
    int i = 0;
    for (; i < MAX_NUM_SUBITEMS_IN_CHOICE; i++)
    {
        if (OwnData()->names[i][set.common.lang] == 0)
        {
            return i;
        }
    }
    return i;
}


void Choice::StartChange(int delta)
{
    if (tsChoice.choice != 0)
    {
        return;
    }
    Sound::GovernorChangedValue();
    if (SHOW_HELP_HINTS)
    {
        SetItemForHint(this);
    }
    else if (!IsActive())
    {
        FuncOnChanged(false);
    }
    else
    {
        tsChoice.choice = this;
        tsChoice.timeStartMS = gTimerMS;
        if (delta > 0)
        {
            tsChoice.inMoveIncrease = 1;
        }
        else if (delta < 0)
        {
            tsChoice.inMoveDecrease = 1;
        }
    }
}


void Choice::FuncOnChanged(bool active) const
{
    if (OwnData()->funcOnChanged)
    {
        OwnData()->funcOnChanged(active);
    }
}


void Choice::FuncForDraw(int x, int y) const
{
    if (OwnData()->funcForDraw)
    {
        OwnData()->funcForDraw(x, y);
    }
}


float Choice::Step() const
{
    const DataChoice *own = OwnData();

    static const float speed = 0.1F;
    static const int numLines = 12;
    if (tsChoice.choice == this)
    {
        float delta = speed * (gTimerMS - tsChoice.timeStartMS);
        if (delta == 0.0F) //-V2550 //-V550
        {
            delta = 0.001F; // “аймер в несколько первых кадров может показать, что прошло 0 мс, но мы возвращаем большее число, потому что ноль будет говорить о том, что движени€ нет
        }
        if (tsChoice.inMoveIncrease == 1)
        {
            if (delta <= numLines)
            {
                return delta;
            }
            CircleIncreaseInt8(own->cell, 0, static_cast<int8>(NumSubItems() - 1));
        }
        else if (tsChoice.inMoveDecrease == 1)
        {
            delta = -delta;

            if (delta >= -numLines)
            {
                return delta;
            }
            CircleDecreaseInt8(own->cell, 0, static_cast<int8>(NumSubItems() - 1));
        }
        tsChoice.choice = 0;
        FuncOnChanged(IsActive());
        Display::Redraw();
        tsChoice.inMoveDecrease = 0;
        tsChoice.inMoveIncrease = 0;
        return 0.0F;
    }
    return 0.0F;
}


void Choice::ChangeValue(int delta)
{
    const DataChoice *own = OwnData();

    if (delta < 0)
    {
        int8 value = (*own->cell == NumSubItems() - 1) ? 0 : (*own->cell + 1);
        *own->cell = value;
    }
    else
    {
        int8 value = static_cast<int8>((*own->cell == 0) ? (NumSubItems() - 1) : (*own->cell - 1));
        *own->cell = value;
    }
    FuncOnChanged(IsActive());
    Sound::GovernorChangedValue();
    Display::Redraw();
}


NamePage::E Page::GetName() const
{
    if (Type() != TypeItem::Page)
    {
        return NamePage::NoPage;
    }
    return OwnData()->name;
}


TypeItem::E Item::Type() const
{
    return data->type;
}




int Page::NumItems() const
{
    if (OwnData()->name == NamePage::MainPage)
    {
        return (SHOW_DEBUG_MENU == 0) ? 10 : 11;
    }
    else if (IsSB())
    {
        return 5;
    }
    else
    {
        for (int i = 0; i < MAX_NUM_ITEMS_IN_PAGE; i++)
        {
            if (GetItem(i) == 0)
            {
                return i;
            }
        }
    }
    return 0;
}


bool Item::IsActive() const
{
    TypeItem::E type = Type();

    /** @todo «десь оптимизировать через битовую маску */

    if (type == TypeItem::Choice || type == TypeItem::Page || type == TypeItem::Button || type == TypeItem::Governor || type == TypeItem::SmallButton)
    {
        pFuncBV func = ((Page *)(this))->data->funcOfActive;

        return func ? func() : true;
    }

    return true;
}


Item *Page::GetItem(int numElement) const
{
    const DataPage *own = OwnData();

    return own->items[numElement + (IsSB() ? 1 : 0)];
}


int Page::NumSubPages() const
{
    return (NumItems() - 1) / MENU_ITEMS_ON_DISPLAY + 1;
}


int Page::NumCurrentSubPage() const
{
    return MenuCurrentSubPage(OwnData()->name);
}


bool Page::IsSB() const
{
    return OwnData()->name >= NamePage::SB_Curs;
}


bool Item::IsShade() const
{
    return Menu::CurrentItemIsOpened(Keeper()->GetName()) && (this != Menu::OpenedItem());
}


int8 Page::PosCurrentItem() const
{
    return MenuPosActItem(OwnData()->name) & 0x7f;
}


Page *Item::Keeper() const
{
    const Page *page = ((Page *)(this))->data->keeper;
    return (Page *)page;
}


void Item::Open(bool open) const
{
    Page *page = Keeper();
    SetMenuPosActItem(page->GetName(), open ? (page->PosCurrentItem() | 0x80) : (page->PosCurrentItem() & 0x7f));
}


void Item::SetCurrent(bool active) const
{
    Page* page = Keeper();
    if (!active)
    {
        SetMenuPosActItem(page->OwnData()->name, 0x7f);
    }
    else
    {
        for (int8 i = 0; i < page->NumItems(); i++)
        {
            if (page->GetItem(i) == this)
            {
                SetMenuPosActItem(page->OwnData()->name, i);
                return;
            }
        }
    }
}


bool Item::IsOpened() const
{
    TypeItem::E type = Type();
    Page* page = Keeper();
    if (type == TypeItem::Page)
    {
        return Menu::CurrentItemIsOpened(page->GetName());
    }
    return (MenuPosActItem(page->OwnData()->name) & 0x80) != 0;
}


void Page::ChangeSubPage(int delta) const
{
    if (delta > 0 && MenuCurrentSubPage(OwnData()->name) < NumSubPages() - 1)
    {
        Sound::RegulatorSwitchRotate();
        SetMenuCurrentSubPage(OwnData()->name, MenuCurrentSubPage(OwnData()->name) + 1);
    }
    else if (delta < 0 && MenuCurrentSubPage(OwnData()->name) > 0)
    {
        Sound::RegulatorSwitchRotate();
        SetMenuCurrentSubPage(OwnData()->name, MenuCurrentSubPage(OwnData()->name) - 1);
    }
}


void Page::ShortPress()
{
    if (OwnData()->funcOnPress)
    {
        OwnData()->funcOnPress();
    }
    if (!IsActive())
    {
        return;
    }
    SetCurrent(true);
    Open(!IsOpened());
}


void Choice::ShortPress()
{
    if (Type() == TypeItem::Choice)
    {
        if (!IsActive())
        {
            FuncOnChanged(false);
        }
        else if (!IsOpened())
        {
            SetCurrent(Menu::CurrentItem() != this);
            StartChange(1);
        }
        else
        {
            ChangeValue(-1);
        }
    }
    else
    {
        if (!IsActive())
        {
            FuncOnChanged(false);
        }
        else if (Menu::OpenedItem() != this)
        {
            SetCurrent(Menu::CurrentItem() != this);
        }
    }
}


void Button::ShortPress()
{
    if (!IsActive())
    {
        return;
    }
    SetCurrent(true);
    OwnData()->funcOnPress();
}


void TimeItem::ShortPress()
{
    if (!IsOpened())
    {
        SetCurrent(true);
        SetOpened();
        Open(true);
    }
    else
    {
        SelectNextPosition();
    }
}


void IPaddress::ShortPress()
{
    if (Menu::OpenedItem() == this)
    {
        NextPosition();
    }
}


void GovernorColor::ShortPress()
{
    if (!IsActive())
    {
        return;
    }
    if (Menu::OpenedItem() == this)
    {
        CircleIncreaseInt8(&(OwnData()->colorType->currentField), 0, 3);
    }
    else
    {
        LongPress();
    }
}


void MACaddress::ShortPress()
{
    if (Menu::OpenedItem() == this)
    {
        CircleIncreaseInt8(&gCurDigit, 0, 5);
    }
}


void SmallButton::ShortPress()
{
    pFuncVV func = OwnData()->funcOnPress;
    if (func)
    {
        func();
        Menu::itemUnderKey = this;
    }
}


void Item::LongPress()
{
    if (Menu::CurrentItem() != this)
    {
        SetCurrent(true);
    }
    Open(!IsOpened());
}


void Button::LongPress()
{
    ShortPress();
}


void TimeItem::LongPress()
{
    if (Menu::CurrentItem() != this)
    {
        SetCurrent(true);
    }
    if (IsOpened() && *OwnData()->curField == iSET)
    {
        SetNewTime();
    }
    Open(!IsOpened());
    SetOpened();
}


Item *Page::RetLastOpened(TypeItem::E *type)
{
    if (Menu::CurrentItemIsOpened(GetName()))
    {
        int8 posActItem = PosCurrentItem();
        Item *item = GetItem(posActItem);
        TypeItem::E typeLocal = GetItem(posActItem)->Type();
        if (typeLocal == TypeItem::Page)
        {
            return ((Page *)item)->RetLastOpened(type);
        }
        else
        {
            return item;
        }
    }
    *type = TypeItem::Page;
    return this;
}


void Item::Change(int delta)
{
    TypeItem::E type = Type();
    if (type == TypeItem::Choice || type == TypeItem::ChoiceReg)
    {
        ((Choice *)this)->StartChange(delta);
    }
    else if (type == TypeItem::Governor)
    {
        Governor *governor = (Governor *)this;
        if (Menu::OpenedItem() != governor)
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
        ((GovernorColor *)this)->ChangeValue(delta);
    }
}


void Page::ShortPressOnItem(int numItem) const
{
    const DataPage *own = OwnData();

    if (Type() != TypeItem::Page)
    {
        return;
    }
    NamePage::E namePage = own->name;
    if (namePage >= NamePage::SB_Curs)
    {
        SmallButton *sb = (SmallButton *)(own->items)[numItem];
        if (sb && own->funcOnPress)
        {
            own->funcOnPress();
        }
    }
}


void Page::OpenAndSetItCurrent() const
{
    SetCurrent(true);
    Open(!IsOpened());
}


bool Item::ChangeOpened(int delta)
{
    if (delta < 2 && delta > -2)
    {
        return false;
    }

    TypeItem::E type = Type();

    if (type == TypeItem::Page)
    {
        ((const Page *)this)->ChangeSubPage(delta);
    }
    else if (type == TypeItem::IP)
    {
        ((IPaddress *)this)->ChangeValue(delta);
    }
    else if (type == TypeItem::MAC)
    {
        ((MACaddress *)this)->ChangeValue(delta);
    }
    else if (type == TypeItem::ChoiceReg || type == TypeItem::Choice)
    {
        ((Choice *)this)->ChangeValue(delta);
    }
    else if (type == TypeItem::Governor)
    {
        ((Governor *)this)->ChangeValue(delta);
    }

    return true;
}


int Item::HeightOpened() const
{
    TypeItem::E type = Type();
    if (type == TypeItem::Page)
    {
        int numItems = ((const Page *)this)->NumItems() - ((Page *)this)->NumCurrentSubPage() * MENU_ITEMS_ON_DISPLAY;
        LIMITATION(numItems, numItems, 0, MENU_ITEMS_ON_DISPLAY);
        return MP_TITLE_HEIGHT + MI_HEIGHT * numItems;
    }
    else if (type == TypeItem::Choice || type == TypeItem::ChoiceReg)
    {
        return MOI_HEIGHT_TITLE + ((Choice *)this)->NumSubItems() * MOSI_HEIGHT - 1;
    }
    return MI_HEIGHT;
}


const char *Item::Title() const
{
    return TITLE((Page *)this);
}


SmallButton *Page::SmallButonFrom(int numButton) const
{
    const DataPage *own = OwnData();

    return (SmallButton *)(own->items)[numButton];
}
