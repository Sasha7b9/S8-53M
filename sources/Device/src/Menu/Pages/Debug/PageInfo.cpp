// 2021/06/01 9:44:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/SettingsMain.h"


//----------------------------------------------------------------------------------------------------------------------


DEF_CHOICE_2(cStats, PageDebug::self,
    "����������", "Statistics"
    ,
    "����������/�� ���������� �����/����, ������ � �������, ���������� �������� � ���������� ����������� � "
    "������/���������� ����������� � ������ ��������"
    ,
    "To show/not to show a time/shot, frames per second, quantity of signals with the last settings in memory/quantity "
    "of the signals kept in memory"
    ,
    "�� ����������", "Hide",
    "����������", "Show",
    setNRST.show_stats, nullptr, SettingsNRST::CommonOnChanged, nullptr
)


//----------------------------------------------------------------------------------------------------------------------

static int8 size = 0;

static void OnDraw_SizeSettings(int x, int y)
{
    Text("������ %d", sizeof(SettingsMain)).Draw(x + 5, y + 21, Color::BLACK);
}


DEF_CHOICE_2(cSizeSettings, PageDebug::self,
    "������ ��������", "Size settings",
    "����� ������� ��������� Settings",
    "Show size of struct Settings",
    "", "",
    "", "",
    size, nullptr, SettingsNRST::CommonOnChanged, OnDraw_SizeSettings
)


//----------------------------------------------------------------------------------------------------------------------


DEF_PAGE_2(pageInfo, PageDebug::self, NamePage::DebugInfo,
    "����", "INFO",
    "",
    "",
    cStats,
    cSizeSettings,
    nullptr, nullptr, nullptr, nullptr
)


const Page *PageDebug::PageInfo::self = &pageInfo;
