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

void PageDisplay::OnPress_Accumulation_Clear()
{
    Display::Redraw();
}








DEF_CHOICE_4(mcGrid_Type, PageDisplay::PageGrid::self,
    "���", "Type",
    "����� ���� �����.",
    "Choice like Grid.",
    "��� 1", "Type 1",
    "��� 2", "Type 2",
    "��� 3", "Type 3",
    "��� 4", "Type 4",
    set.display.type_grid, nullptr, nullptr, nullptr
)

static ColorType cTypeGrid = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::GRID.index };

void PageDisplay::OnChanged_Grid_Brightness()
{
    cTypeGrid.SetBrightness(set.display.brightness_grid / 1e2F);
}

static void BeforeDraw_Grid_Brightness()
{
    cTypeGrid.Init();
    set.display.brightness_grid = (int16)(cTypeGrid.brightness * 100.0F);
}

DEF_GOVERNOR(mgGrid_Brightness, PageDisplay::PageGrid::self,
    "�������", "Brightness",
    "������������� ������� �����.",
    "Adjust the brightness of the Grid.",
    set.display.brightness_grid, 0, 100, nullptr, PageDisplay::OnChanged_Grid_Brightness, BeforeDraw_Grid_Brightness
)

DEF_PAGE_2(pageGrid, PageDisplay::self, NamePage::DisplayGrid,
    "�����", "GRID",
    "�������� ��������� ����������� ������������ �����.",
    "Contains settings of display of a coordinate Grid::",
    mcGrid_Type,
    mgGrid_Brightness,
    nullptr, nullptr, nullptr, nullptr
)

static void OnChanged_Settings_Brightness()
{
    Painter::SetBrightnessDisplay(set.display.brightness);
}

DEF_GOVERNOR(mgSettings_Brightness, PageDisplay::PageSettings::self,
    "�������", "Brightness",
    "��������� ������� �������� �������",
    "Setting the brightness of the display",
    set.display.brightness, 0, 100, nullptr, OnChanged_Settings_Brightness, nullptr
)

DEF_GOVERNOR(mgSettings_Levels, PageDisplay::PageSettings::self,
    "������", "Levels",
    "����� �����, � ������� �������� ����� �������� ����� ������� ���������� �� ������ ������� ��������������� ����� ������ ��������",
    "Defines the time during which, after turning the handle visits to the voltage on the screen remains auxiliary label offset level",
    set.display.time_show_levels, 0, 125, nullptr, nullptr, nullptr
)

DEF_GOVERNOR(mgSettings_TimeMessages, PageDisplay::PageSettings::self,
    "�����", "Time",
    "��������� �������, � ������� �������� ��������� ����� ���������� �� ������",
    "Set the time during which the message will be on the screen",
    set.display.time_messages, 1, 99, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcSettings_ShowStringNavigation, PageDisplay::PageSettings::self,
    "������ ����", "Path menu",
    "��� ������ \n����������\n ����� ������ ������ ��������� ������ ���� �� ������� �������� ����",
    "When choosing \nDisplay\n at the top left of the screen displays the full path to the current page menu",
    "��������", "Hide",
    "����������", "Show",
    set.display.show_string_navigation, nullptr, nullptr, nullptr
)

static void OnChanged_Settings_ShowAltMarkers(bool)
{
    Display::ChangedRShiftMarkers();
}

DEF_CHOICE_3(mcSettings_ShowAltMarkers, PageDisplay::PageSettings::self,
    "���. �������", "Alt. markers"
    ,
    "������������� ����� ����������� �������������� �������� ������� �������� � �������������:\n"
    "\"��������\" - �������������� ������� �� ������������,\n"
    "\"����������\" - �������������� ������� ������������ ������,\n"
    "\"����\" - �������������� ������� ������������ � ������� 5 ��� ����� �������� ����� �������� ������ �� ���������� ��� ������ �������������"
    ,
    "Sets the display mode of additional markers levels of displacement and synchronization:\n"
    "\"Hide\" - additional markers are not shown,\n"
    "\"Show\" - additional markers are shown always,\n"
    "\"Auto\" - additional markers are displayed for 5 seconds after turning the handle channel offset voltage or trigger level"
    ,
    "��������", "Hide",
    "����������", "Show",
    "����", "Auto",
    set.display.alt_markers, nullptr, OnChanged_Settings_ShowAltMarkers, nullptr
)

static void OnChanged_Settings_AutoHide(bool autoHide)
{
    Menu::SetAutoHide(autoHide);
}

DEF_CHOICE_6(mcSettings_AutoHide, PageDisplay::PageSettings::self,
    "��������", "Hide",
    "��������� ����� ���������� ������� ������ ��� �������� �����, �� ��������� �������� ���� ������������� ��������� � ������",
    "Installation after the last keystroke or turning the handle, after which the menu will automatically disappear",
    "�������", "Never",
    "����� 5 ���", "Through 5 s",
    "����� 10 ���", "Through 10 s",
    "����� 15 ���", "Through 15 s",
    "����� 30 ���", "Through 30 s",
    "����� 60 ���", "Through 60 s",
    set.display.menu_auto_hide, nullptr, OnChanged_Settings_AutoHide, nullptr
)

DEF_PAGE_7(pageSettings, PageDisplay::self, NamePage::ServiceDisplay,
    "���������", "SETTINGS",
    "�������������� ��������� �������",
    "Additional display settings",
    *PageDisplay::PageSettings::PageColors::self,
    mgSettings_Brightness,
    mgSettings_Levels,
    mgSettings_TimeMessages,
    mcSettings_ShowStringNavigation,
    mcSettings_ShowAltMarkers,
    mcSettings_AutoHide,
    nullptr, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcSettings_Colors_Scheme, PageDisplay::PageSettings::PageColors::self,
    "�������� �����", "Color scheme",
    "����� ������ �����������",
    "Mode of operation of the calibrator",
    "����� 1", "Scheme 1",
    "����� 2", "Scheme 2",
    set.service.colorScheme, nullptr, nullptr, nullptr
)

static ColorType colorT1 = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::DATA_A.index };

DEF_GOVERNOR_COLOR(mgcSettings_Colors_ChannelA, PageDisplay::PageSettings::PageColors::self,
    "����� 1", "Channel 1",
    "",
    "",
    colorT1, nullptr
)

static ColorType colorT2 = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::DATA_B.index };

DEF_GOVERNOR_COLOR(mgcSettings_Colors_ChannelB, PageDisplay::PageSettings::PageColors::self,
    "����� 2", "Channel 2",
    "",
    "",
    colorT2, nullptr
)

DEF_GOVERNOR_COLOR(mgcSettings_Colors_Grid, PageDisplay::PageSettings::PageColors::self,
    "�����", "Grid",
    "������������� ���� �����",
    "Sets the grid color",
    cTypeGrid, nullptr
)

DEF_PAGE_4(pageColors, PageDisplay::PageSettings::self, NamePage::ServiceDisplayColors,
    "�����", "COLORS",
    "����� ������ �������",
    "The choice of colors display",
    mcSettings_Colors_Scheme,
    mgcSettings_Colors_ChannelA,
    mgcSettings_Colors_ChannelB,
    mgcSettings_Colors_Grid,
    nullptr, nullptr, nullptr, nullptr
)

ColorType *PageDisplay::colorTypeGrid = &cTypeGrid;
const Page *PageDisplay::self = &pageDisplay;
const Page *PageDisplay::PageGrid::self = &pageGrid;
const Page *PageDisplay::PageSettings::self = &pageSettings;
const Page *PageDisplay::PageSettings::PageColors::self = &pageColors;
