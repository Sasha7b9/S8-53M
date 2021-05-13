// 2021/05/12 16:45:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/SettingsCommon.h"


DEF_CHOICE_REG_10(mcNumber, PageDisplay::PageAveraging::self,
    "Количество", "Number",
    "Задаёт количество последних измерений, по которым производится усреднение.",
    "Sets number of the last measurements on which averaging is made.",
    DISABLE_RU, DISABLE_EN,
    "2", "2",
    "4", "4",
    "8", "8",
    "16", "16",
    "32", "32",
    "64", "64",
    "128", "128",
    "256", "256",
    "512", "512",
    set.display.enum_ave, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcMode, PageDisplay::PageAveraging::self,
    "Режим", "Mode"
    ,
    "1. \"Точно\" - точный режим усреднения, когда в расчёте участвуют только последние сигналы.\n"
    "2. \"Приблизительно\" - приблизительный режим усреднения. Имеет смысл использовать, когда задано количество "
    "измерений большее, чем может поместиться в памяти."
    ,
    "1. \"Accurately\" - the exact mode of averaging when only the last signals participate in calculation.\n"
    "2. \"Around\" - approximate mode of averaging. It makes sense to use when the number of measurements bigger is "
    "set, than can be located in memory.",
    "Точно", "Accurately",
    "Приблизительно", "Around",
    set.display.mode_ave, nullptr, nullptr, nullptr
)


static bool IsActive()
{
    return true;
}


DEF_PAGE_2(pageAveraging, PageDisplay::self, NamePage::DisplayAverage,
    "УСРЕДНЕНИЕ", "AVERAGE",
    "Настройки режима усреднения по последним измерениям.",
    "Settings of the mode of averaging on the last measurements.",
    mcNumber,
    mcMode,
    IsActive, nullptr, nullptr, nullptr
)

const Page *PageDisplay::PageAveraging::self = &pageAveraging;
