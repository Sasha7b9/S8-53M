#include "defines.h"
#include "Hardware/Sound.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"


// ������������ ��� �������� ��������� �������� Choice
struct TimeStructChoice
{
    Choice* choice;                 // ����� Choice, ������� ��������� � ������ ������ � ��������. ���� 0 - ��� ��������.
    uint        timeStartMS;        // ����� ������ �������� choice.
    uint        inMoveIncrease : 1;
    uint        inMoveDecrease : 1;
};

static TimeStructChoice tsChoice;



Control::Control(const ControlStruct *str) : type(str->type), keeper(str->keeper), funcOfActive(str->funcOfActive)
{
    titleHint[0] = str->titleHint[0];
    titleHint[1] = str->titleHint[1];
    titleHint[2] = str->titleHint[2];
    titleHint[3] = str->titleHint[3];
}


Control::Control(TypeItem::E type_, const Page* keeper_, pFuncBV funcOfActive_, const char *titleRU, const char *titleEN, const char *hintRU, const char *hintEN) :
    type(type_), keeper(keeper_), funcOfActive(funcOfActive_)
{
    titleHint[0] = titleRU;
    titleHint[1] = titleEN;
    titleHint[2] = hintRU;
    titleHint[3] = hintEN;
}


Page::Page(const Page *keeper_, pFuncBV funcOfActive_, const char *titleRU, const char *titleEN, const char *hintRU, const char *hintEN, 
           NamePage::E name_, const arrayItems *items_, pFuncVV funcOnPress_, pFuncVV funcOnDraw_, pFuncVI funcRegSetSB_) :
    Control(TypeItem::Page, keeper_, funcOfActive_, titleRU, titleEN, hintRU, hintEN),
    name(name_), items(items_), funcOnPress(funcOnPress_), funcOnDraw(funcOnDraw_), funcRegSetSB(funcRegSetSB_)
{
}


Button::Button(const Page *keeper_, pFuncBV funcOfActive_,
       const char *titleRU, const char *titleEN, const char *hintRU, const char *hintEN, pFuncVV funcOnPress_) :
    Control(TypeItem::Button, keeper_, funcOfActive_, titleRU, titleEN, hintRU, hintEN),
    funcOnPress(funcOnPress_)
{
};


SmallButton::SmallButton(const Page *keeper_, pFuncBV funcOfActive_,
            const char *titleRU, const char *titleEN, const char *hintRU, const char *hintEN,
            pFuncVV funcOnPress_, pFuncVII funcOnDraw_, const arrayHints *hintUGO_) :
    Control(TypeItem::SmallButton, keeper_, funcOfActive_, titleRU, titleEN, hintRU, hintEN),
    funcOnPress(funcOnPress_), funcOnDraw(funcOnDraw_), hintUGO(hintUGO_)
{
}


Governor::Governor(const Page *keeper_, pFuncBV funcOfActive_,
         const char *titleRU, const char *titleEN, const char *hintRU, const char *hintEN,
         int16 *cell_, int16 minValue_, int16 maxValue_, pFuncVV funcOfChanged_, pFuncVV funcBeforeDraw_) :
    Control(TypeItem::Governor, keeper_, funcOfActive_, titleRU, titleEN, hintRU, hintEN),
    cell(cell_), minValue(minValue_), maxValue(maxValue_), funcOfChanged(funcOfChanged_), funcBeforeDraw(funcBeforeDraw_)
{
}


bool Control::IsPressed()
{
    return this == Menu::ItemUnderKey();
}


IPaddress::IPaddress(const IPaddressStruct *str) :
    Control(&str->str), ip0(str->ip0), ip1(str->ip1), ip2(str->ip2), ip3(str->ip3), funcOfChanged(str->funcOfChanged), port(str->port)
{

}


const char *Choice::NameSubItem(int i) const
{
    return names[i][LANG];
}


const char *Choice::NameCurrentSubItem() const
{
    return (cell == 0) ? "" : names[*cell][LANG];
}


const char *Choice::NameNextSubItem() const
{
    if (cell == 0)
    {
        return "";
    }
    int index = *cell + 1;
    if (index == NumSubItems())
    {
        index = 0;
    }
    return names[index][LANG];
}


const char *Choice::NamePrevSubItem() const
{
    if (cell == 0)
    {
        return "";
    }
    int index = *cell - 1;
    if (index < 0)
    {
        index = NumSubItems() - 1;
    }
    return names[index][set.common.lang];
}


int Choice::NumSubItems() const
{
    int i = 0;
    for (; i < MAX_NUM_SUBITEMS_IN_CHOICE; i++)
    {
        if (names[i][set.common.lang] == 0)
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
    if (funcOnChanged)
    {
        funcOnChanged(active);
    }
}


void Choice::FuncForDraw(int x, int y) const
{
    if (funcForDraw)
    {
        funcForDraw(x, y);
    }
}


float Choice::Step() const
{
    static const float speed = 0.1F;
    static const int numLines = 12;
    if (tsChoice.choice == this)
    {
        float delta = speed * (gTimerMS - tsChoice.timeStartMS);
        if (delta == 0.0F) //-V2550 //-V550
        {
            delta = 0.001F; // ������ � ��������� ������ ������ ����� ��������, ��� ������ 0 ��, �� �� ���������� ������� �����, ������ ��� ���� ����� �������� � ���, ��� �������� ���
        }
        if (tsChoice.inMoveIncrease == 1)
        {
            if (delta <= numLines)
            {
                return delta;
            }
            CircleIncreaseInt8(cell, 0, static_cast<int8>(NumSubItems() - 1));
        }
        else if (tsChoice.inMoveDecrease == 1)
        {
            delta = -delta;

            if (delta >= -numLines)
            {
                return delta;
            }
            CircleDecreaseInt8(cell, 0, static_cast<int8>(NumSubItems() - 1));
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
    if (delta < 0)
    {
        int8 value = (*cell == NumSubItems() - 1) ? 0 : (*cell + 1);
        *cell = value;
    }
    else
    {
        int8 value = static_cast<int8>((*cell == 0) ? (NumSubItems() - 1) : (*cell - 1));
        *cell = value;
    }
    FuncOnChanged(Menu::ItemIsActive(this));
    Sound::GovernorChangedValue();
    Display::Redraw();
}
