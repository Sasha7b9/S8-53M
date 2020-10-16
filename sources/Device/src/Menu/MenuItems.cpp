#include "defines.h"
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
    else if (!Menu::ItemIsActive(this))
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
        FuncOnChanged(Menu::ItemIsActive(this));
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
    FuncOnChanged(Menu::ItemIsActive(this));
    Sound::GovernorChangedValue();
    Display::Redraw();
}


NamePage::E Page::GetName() const
{
    if (Menu::TypeMenuItem((void *)this) != TypeItem::Page)
    {
        return NamePage::NoPage;
    }
    return OwnData()->name;
}
