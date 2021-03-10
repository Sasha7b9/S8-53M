#include "defines.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "Menu/Menu.h"
#include "Menu/Pages/HelpContent.h"
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
static const Item *itemsPage[] = { &Item::empty, nullptr };
static const DataPage emptyDataPage = { NamePage::Empty, &itemsPage[0], nullptr, nullptr };
const char *titleHint[4] = { "", "", "", "" };
static const DataItem emptyDataItem = { TypeItem::None, nullptr, nullptr, titleHint, &emptyDataPage };
static int8 subPageEmpty = 0;
static int8 actItemEmpty = -1;
static bool actItemIsOpenedEmpty = false;
Page Page::empty(&emptyDataItem, &subPageEmpty, &actItemEmpty, &actItemIsOpenedEmpty);

bool Governor::inMoveIncrease = false;
bool Governor::inMoveDecrease = false;
Item *Governor::address = 0;
uint Governor::timeStartMS = 0;

uint8 Item::positionGovernor = 0;

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
    while (OwnData()->names[i] != nullptr)
    {
        i++;
    }
    return i / 2;
}


void Choice::StartChange(int delta)
{
    if (tsChoice.choice != 0)
    {
        return;
    }
    Sound::GovernorChangedValue();
    if (Menu::showHelpHints)
    {
        PageHelpContent::SetItemForHint(this);
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


void Item::CalculatePositionGovernor(int delta)
{
    if (IsChoice() || IsChoiceReg())
    {
        if (delta < 0)
        {
            if (positionGovernor == 0) { positionGovernor = Item::NUM_STATES_GOVERNOR - 1; }
            else                       { positionGovernor--; }
        }
        else
        {
            positionGovernor++;
            positionGovernor %= Item::NUM_STATES_GOVERNOR;
        }
    }
}


void Choice::ChangeValue(int delta)
{
    CalculatePositionGovernor(delta);

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
        return !Menu::showDebugMenu ? 10 : 11;
    }
    else if (IsPageSB())
    {
        return 5;
    }
    else
    {
        for (int i = 0; ; i++)
        {
            if (GetItem(i) == nullptr)
            {
                return i;
            }
        }
    }
}


bool Item::IsActive() const
{
    if (IsChoice() || IsPage() || IsButton() || IsGovernor() || IsSmallButton())
    {
        pFuncBV func = ((Page *)(this))->data->funcOfActive;

        return func ? func() : true;
    }

    return true;
}


const Item *Page::GetItem(int numElement) const
{
    return OwnData()->items[numElement + (IsPageSB() ? 1 : 0)];
}


int Page::NumSubPages() const
{
    return (NumItems() - 1) / Menu::ITEMS_ON_DISPLAY + 1;
}


bool Page::IsPageSB() const
{
    return OwnData()->name >= NamePage::SB_Curs;
}


Page *Item::Keeper() const
{
    const Page *page = ((Page *)(this))->data->keeper;
    return (Page *)page;
}


void Item::Open(bool open) const
{
    Page *page = Keeper();

    if (open)
    {
        page->OpenActItem();
    }
    else
    {
        page->CloseOpenedItem();
    }
}


void Item::SetCurrent(bool active) const
{
    Page* page = Keeper();
    if (!active)
    {
        page->SetPositionActItem(-1);
    }
    else
    {
        for (int8 i = 0; i < page->NumItems(); i++)
        {
            if (page->GetItem(i) == this)
            {
                page->SetPositionActItem(i);
                return;
            }
        }
    }
}


bool Item::IsOpened() const
{
    Page* page = Keeper();

    if (IsPage())
    {
        return page->CurrentItemIsOpened();
    }

    return page->CurrentItemIsOpened(); //-V523
}


void Page::ChangeSubPage(int delta) const
{
    if (delta > 0 && GetCurrentSubPage() < NumSubPages() - 1)
    {
        Sound::RegulatorSwitchRotate();
        SetCurrentSubPage(GetCurrentSubPage() + 1);
    }
    else if (delta < 0 && GetCurrentSubPage() > 0)
    {
        Sound::RegulatorSwitchRotate();
        SetCurrentSubPage(GetCurrentSubPage() - 1);
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
    if (IsChoice())
    {
        if (!IsActive())
        {
            FuncOnChanged(false);
        }
        else if (!IsOpened())
        {
            SetCurrent(!Menu::IsCurrentItem(this));
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
        else if (!IsOpened())
        {
            SetCurrent(!Menu::IsCurrentItem(this));
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
    if (IsOpened())
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
    if (IsOpened())
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
    if (IsOpened())
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
    }
}


void Item::LongPress()
{
    if (!Menu::IsCurrentItem(this))
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
    if (!Menu::IsCurrentItem(this))
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


const Item *Page::RetLastOpened() const
{
    if (CurrentItemIsOpened())
    {
        int8 pActItem = GetPositionActItem();
        const Item *item = GetItem(pActItem);
        if (GetItem(pActItem)->IsPage())
        {
            return ((Page *)item)->RetLastOpened();
        }
        else
        {
            return item;
        }
    }
    return this;
}


void Item::Change(int delta) const
{
    if (IsChoice() || IsChoiceReg())
    {
        ((Choice *)this)->StartChange(delta);
    }
    else if (IsGovernor())
    {
        Governor *governor = (Governor *)this;
        if (!governor->IsOpened())
        {
            governor->StartChange(delta);
        }
        else
        {
            governor->ChangeValue(delta);
        }
    }
    else if (IsGovernorColor())
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


bool Item::ChangeOpened(int delta) const
{
    if (delta < 2 && delta > -2)
    {
        return false;
    }

    if (IsPage())
    {
        ((const Page *)this)->ChangeSubPage(delta);
    }
    else if (IsIP())
    {
        ((IPaddress *)this)->ChangeValue(delta);
    }
    else if (IsMAC())
    {
        ((MACaddress *)this)->ChangeValue(delta);
    }
    else if (IsChoiceReg() || IsChoice())
    {
        ((Choice *)this)->ChangeValue(delta);
    }
    else if (IsGovernor())
    {
        ((Governor *)this)->ChangeValue(delta);
    }

    return true;
}


int Item::HeightOpened() const
{
    if (IsPage())
    {
        int numItems = ((const Page *)this)->NumItems() - ((Page *)this)->GetCurrentSubPage() * Menu::ITEMS_ON_DISPLAY;
        LIMITATION(numItems, numItems, 0, Menu::ITEMS_ON_DISPLAY);
        return Item::TITLE_HEIGHT + Item::HEIGHT * numItems;
    }
    else if (IsChoice() || IsChoiceReg())
    { 
        return Item::OPENED_HEIGHT_TITLE + ((Choice *)this)->NumSubItems() * Item::OPENED_HEIGHT - 1;
    }
    return Item::HEIGHT;
}


pString Item::Title() const
{
    return TITLE((Page *)this);
}


SmallButton *Page::SmallButonFrom(int numButton) const
{
    const DataPage *own = OwnData();

    return (SmallButton *)(own->items)[numButton];
}


int Page::GetCurrentSubPage() const
{
    return *currentSubPage;
}


void Page::SetCurrentSubPage(int posSubPage) const
{
    *currentSubPage = (int8)posSubPage;
}


bool Page::CurrentItemIsOpened() const
{
    return *actItemIsOpened;
}


void Page::SetPositionActItem(int8 pos)
{
    *posActItem = pos;
}


void Page::CloseOpenedItem()
{
    *actItemIsOpened = false;
}


void Page::OpenActItem()
{
    if (*posActItem >= 0)
    {
        *actItemIsOpened = true;
    }
}


int8 Page::GetPositionActItem() const
{
    return *posActItem;
}


int Page::PosItemOnTop() const
{
    return GetCurrentSubPage() * Menu::ITEMS_ON_DISPLAY;
}
