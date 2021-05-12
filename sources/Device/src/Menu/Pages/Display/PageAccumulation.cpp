// 2021/05/12 16:40:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


DEF_CHOICE_REG_9(mcNumber, PageDisplay::PageAccumulation::self,
    "����������", "Number"
    ,
    "����� ������������ ���������� ��������� �������� �� ������. ���� � ��������� \"�����\" ������� \"�������������\", ����� ��������� ������ "
    "�������� ������ \"��������\"."
    "\"�������������\" - ������ ��������� ������� �� ������� �� ��� ���, ���� �� ����� ������ ������ \"��������\"."
    ,
    "Sets the maximum quantity of the last signals on the screen. If in control \"Mode\" it is chosen \"Infinity\", the screen is cleared only "
    "by pressing of the button \"Clear\"."
    "\"Infinity\" - each measurement remains on the display until the button \"Clear\" is pressed."
    ,
    DISABLE_RU, DISABLE_EN,
    "2", "2",
    "4", "4",
    "8", "8",
    "16", "16",
    "32", "32",
    "64", "64",
    "128", "128",
    "�������������", "Infinity",
    set.display.enum_accum, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcMode, PageDisplay::PageAccumulation::self,
    "�����", "Mode"
    ,
    "1. \"����������\" - ����� ���������� ��������� ���������� ��������� ���������� ������� �������. ���� ����� ������, ����� ������ �� ������� "
    "��� ���������� ������� ���������� ���������.\n"
    "2. \"�� ����������\" - �� ������� ������ ��������� �������� ��� ������� (� ������ �������� ������) ���������� ���������. ����������� �������� "
    "������� �������������� � ������������� ���������� ��������� ���������� ��������� ��� ���������� ������."
    ,
    "1. \"Dump\" - after accumulation of the set number of measurement there is a cleaning of the Display:: This mode is convenient when memory "
    "isn't enough for preservation of the necessary number of measurements.\n"
    "2. \"Not to dump\" - the number of measurements is always output to the display set or smaller (in case of shortage of memory). Shortcoming "
    "is smaller speed and impossibility of accumulation of the set number of measurements at a lack of memory."
    ,
    "�� ����������", "Not to dump",
    "����������", "Dump",
    set.display.mode_acum, nullptr, nullptr, nullptr
)


static bool IsActive_Clear()
{
    return set.display.enum_accum.IsInfinity();
}


DEF_BUTTON(mbClear, PageDisplay::PageAccumulation::self,
    "��������", "Clear",
    "������� ����� �� ����������� ��������.",
    "Clears the screen of the saved-up signals.",
    IsActive_Clear, PageDisplay::OnPress_Accumulation_Clear
)


static bool IsActive()
{
    return (set.time.base > TBase::_50ns);
}


DEF_PAGE_3(pageAccumulation, PageDisplay::self, NamePage::DisplayAccumulation,
    "����������", "ACCUMULATION",
    "��������� ������ ����������� ��������� �������� �� ������.",
    "Mode setting signals to display the last screen.",
    mcNumber,
    mcMode,
    mbClear,
    IsActive, nullptr, nullptr, nullptr
)

const Page *PageDisplay::PageAccumulation::self = &pageAccumulation;
