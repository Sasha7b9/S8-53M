// 2021/06/01 9:44:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/SettingsMain.h"


//----------------------------------------------------------------------------------------------------------------------

DEF_CHOICE_2(cStats, PageDebug::self,
    "Статистика", "Statistics"
    ,
    "Показывать/не показывать время/кадр, кадров в секунду, количество сигналов с последними настройками в "
    "памяти/количество сохраняемых в памяти сигналов"
    ,
    "To show/not to show a time/shot, frames per second, quantity of signals with the last settings in memory/quantity "
    "of the signals kept in memory"
    ,
    "Не показывать", "Hide",
    "Показывать", "Show",
    setNRST.show_stats, nullptr, SettingsNRST::CommonOnChanged, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

static int8 size = 0;

static void OnDraw_SizeSettings(int x, int y)
{
    Text("Размер %d", sizeof(SettingsMain)).Draw(x + 5, y + 21, Color::BLACK);
}

DEF_CHOICE_2(cSizeSettings, PageDebug::self,
    "Размер настроек", "Size settings",
    "Вывод размера структуры Settings",
    "Show size of struct Settings",
    "", "",
    "", "",
    size, nullptr, nullptr, OnDraw_SizeSettings
)

//----------------------------------------------------------------------------------------------------------------------

static int8 sizeNRST = 0;

static void OnDraw_SizeSettingsNRST(int x, int y)
{
    Text("Размер %d", sizeof(SettingsNRST)).Draw(x + 5, y + 21, Color::BLACK);
}

DEF_CHOICE_2(cSizeSettingsNRST, PageDebug::self,
    "Размер NRST", "Size NRST",
    "Вывод размера структуры несбрасываемых настроек",
    "Outputting the size of the non-resettable settings structure",
    "", "",
    "", "",
    sizeNRST, nullptr, nullptr, OnDraw_SizeSettingsNRST
)

//----------------------------------------------------------------------------------------------------------------------

DEF_PAGE_3(pageInfo, PageDebug::self, NamePage::DebugInfo,
    "ИНФО", "INFO",
    "",
    "",
    cStats,
    cSizeSettings,
    cSizeSettingsNRST,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDebug::PageInfo::self = &pageInfo;
