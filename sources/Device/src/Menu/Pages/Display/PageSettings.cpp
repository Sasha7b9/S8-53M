// 2021/05/12 17:08:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Painter_.h"
#include "common/Display/Painter/Text_.h"
#include "Menu/Menu.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


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

const Page *PageDisplay::PageSettings::self = &pageSettings;
