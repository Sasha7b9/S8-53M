// 2021/06/01 9:44:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/SettingsNRST.h"


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


DEF_PAGE_1(pageInfo, PageDebug::self, NamePage::DebugInfo,
    "����", "INFO",
    "",
    "",
    cStats,
    nullptr, nullptr, nullptr, nullptr
)


const Page *PageDebug::PageInfo::self = &pageInfo;
