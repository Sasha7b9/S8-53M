// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Painter_.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


DEF_CHOICE_2(mcMapping, PageDisplay::self,
    "�����������", "View",
    "����� ����� ����������� �������.",
    "Sets the display mode signal.",
    "������", "Vector",
    "�����", "Points",
    set.display.mode_draw_signal, nullptr, nullptr, nullptr
)


static bool IsActive_MinMax()
{
    return (set.time.base > TBase::_50ns);
}


static void OnChanged_MinMax(bool)
{
}


DEF_CHOICE_REG_8(mcMinMax, PageDisplay::self,
    "��� ����", "Min Max",
    "����� ���������� ��������� ���������, �� ������� �������� ��������������� �����, ��������� �������� � ��������� ���������.",
    "Sets number of the last measurements on which the limiting lines which are bending around minima and maxima of measurements are under construction.",
    DISABLE_RU, DISABLE_EN,
    "2", "2",
    "4", "4",
    "8", "8",
    "16", "16",
    "32", "32",
    "64", "64",
    "128", "128",
    set.display.enum_min_max, IsActive_MinMax, OnChanged_MinMax, nullptr
)


DEF_CHOICE_REG_10(mcSmoothing, PageDisplay::self,
    "�����������", "Smoothing",
    "������������� ���������� ����� ��� ������� ����������� �� �������� ������ �������.",
    "Establishes quantity of points for calculation of the signal smoothed on the next points.",
    DISABLE_RU, DISABLE_EN,
    "2 �����", "2 points",
    "3 �����", "3 points",
    "4 �����", "4 points",
    "5 �����", "5 points",
    "6 �����", "6 points",
    "7 �����", "7 points",
    "8 �����", "8 points",
    "9 �����", "9 points",
    "10 �����", "10 points",
    set.display.smoothing, nullptr, nullptr, nullptr
)


void PageDisplay::OnChanged_RefreshFPS(bool)
{
    FPGA::SetNumSignalsInSec(set.display.enum_fps.NumSignalsInS());
}


DEF_CHOICE_5(mcRefreshFPS, PageDisplay::self,
    "������� ������", "Refresh rate",
    "����� ������������ ����� ��������� � ������� ������.",
    "Sets the maximum number of the shots removed in a second.",
    "25", "25",
    "10", "10",
    "5", "5",
    "2", "2",
    "1", "1",
    set.display.enum_fps, nullptr, PageDisplay::OnChanged_RefreshFPS, nullptr
)


DEF_CHOICE_2(mcTypeShift, PageDisplay::self,
    "��������", "Offset",
    "����� ����� ��������� �������� �� ���������\n1. \"����������\" - ����������� ���������� ��������.\n2. \"�������\" - ����������� ��������� �������� �� ������.",
    "Sets the mode of retaining the vertical displacement\n1. \"Voltage\" - saved dressing bias.\n2. \"Divisions\" - retained the position of the offset on the screen.",
    "����������", "Voltage",
    "�������", "Divisions",
    set.display.linking_rshift, nullptr, nullptr, nullptr
)


DEF_PAGE_9(pageDisplay, PageMain::self, NamePage::Display,
    "�������", "DISPLAY",
    "�������� ��������� ����������� �������.",
    "Contains settings of display of the Display::",
    mcMapping,
    *PageDisplay::PageAccumulation::self,
    *PageDisplay::PageAveraging::self,
    mcMinMax,
    mcSmoothing,
    mcRefreshFPS,
    *PageDisplay::PageGrid::self,
    mcTypeShift,
    *PageDisplay::PageSettings::self,
    nullptr, nullptr, nullptr, nullptr
)


void PageDisplay::PageAccumulation::OnPress_Clear()
{
    Display::Redraw();
}


const Page *PageDisplay::self = &pageDisplay;
