// 2021/05/12 16:40:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


DEF_CHOICE_REG_9(mcNumber, PageDisplay::PageAccumulation::self,
    "Количество", "Number"
    ,
    "Задаёт максимальное количество последних сигналов на экране. Если в настройке \"Режим\" выбрано "
    "\"Бесконечность\", экран очищается только "
    "нажатием кнопки \"Очистить\"."
    "\"Бесконечность\" - каждое измерение остаётся на дисплее до тех пор, пока не будет нажата кнопка \"Очистить\"."
    ,
    "Sets the maximum quantity of the last signals on the screen. If in control \"Mode\" it is chosen \"Infinity\", "
    "the screen is cleared only "
    "by pressing of the key \"Clear\"."
    "\"Infinity\" - each measurement remains on the display until the key \"Clear\" is pressed."
    ,
    DISABLE_RU, DISABLE_EN,
    "2", "2",
    "4", "4",
    "8", "8",
    "16", "16",
    "32", "32",
    "64", "64",
    "128", "128",
    "Бесконечность", "Infinity",
    set.display.enum_accum, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcMode, PageDisplay::PageAccumulation::self,
    "Режим", "Mode"
    ,
    "1. \"Сбрасывать\" - после накопления заданного количества измерения происходит очистка дисплея. Этот режим "
    "удобен, когда памяти не хватает для сохранения нужного количества измерений.\n"
    "2. \"Не сбрасывать\" - на дисплей всегда выводится заданное или меньшее (в случае нехватки памяти) количество "
    "измерений. Недостатком является меньшее быстродействие и невозможность накопления заданного количества измерений "
    "при недостатке памяти."
    ,
    "1. \"Dump\" - after accumulation of the set number of measurement there is a cleaning of the Display:: This mode "
    "is convenient when memory isn't enough for preservation of the necessary number of measurements.\n"
    "2. \"Not to dump\" - the number of measurements is always output to the display set or smaller (in case of "
    "shortage of memory). Shortcoming is smaller speed and impossibility of accumulation of the set number of "
    "measurements at a lack of memory."
    ,
    "Не сбрасывать", "Not to dump",
    "Сбрасывать", "Dump",
    set.display.mode_acum, nullptr, nullptr, nullptr
)


static bool IsActive_Clear()
{
    return set.display.enum_accum.IsInfinity();
}


DEF_BUTTON(mbClear, PageDisplay::PageAccumulation::self,
    "Очистить", "Clear",
    "Очищает экран от накопленных сигналов.",
    "Clears the screen of the saved-up signals.",
    IsActive_Clear, PageDisplay::PageAccumulation::OnPress_Clear
)


static bool IsActive()
{
    return (set.time.base > TBase::_50ns);
}


DEF_PAGE_3(pageAccumulation, PageDisplay::self, NamePage::DisplayAccumulation,
    "НАКОПЛЕНИЕ", "ACCUMULATION",
    "Настройки режима отображения последних сигналов на экране.",
    "Mode setting signals to display the last screen.",
    mcNumber,
    mcMode,
    mbClear,
    IsActive, nullptr, nullptr, nullptr
)

const Page *PageDisplay::PageAccumulation::self = &pageAccumulation;


void PageDisplay::PageAccumulation::OnPress_Clear()
{
    Display::Redraw();
}
