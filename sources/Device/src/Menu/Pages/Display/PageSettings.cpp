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
    "Яркость", "Brightness",
    "Установка яркости свечения дисплея",
    "Setting the brightness of the display",
    set.display.brightness, 0, 100, nullptr, OnChanged_Settings_Brightness, nullptr
)


DEF_GOVERNOR(mgSettings_Levels, PageDisplay::PageSettings::self,
    "Уровни", "Levels",
    "Задаёт время, в течение которого после поворота ручки сещения напряжения на экране остаётся вспомогательная метка уровня смещения",
    "Defines the time during which, after turning the handle visits to the voltage on the screen remains auxiliary label offset level",
    set.display.time_show_levels, 0, 125, nullptr, nullptr, nullptr
)


DEF_GOVERNOR(mgSettings_TimeMessages, PageDisplay::PageSettings::self,
    "Время", "Time",
    "Установка времени, в течение которого сообщения будут находиться на экране",
    "Set the time during which the message will be on the screen",
    set.display.time_messages, 1, 99, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcSettings_ShowStringNavigation, PageDisplay::PageSettings::self,
    "Строка меню", "Path menu",
    "При выборе \nПоказывать\n слева вверху экрана выводится полный путь до текущей страницы меню",
    "When choosing \nDisplay\n at the top left of the screen displays the full path to the current page menu",
    "Скрывать", "Hide",
    "Показывать", "Show",
    set.display.show_string_navigation, nullptr, nullptr, nullptr
)


static void OnChanged_Settings_ShowAltMarkers(bool)
{
    Display::ChangedRShiftMarkers();
}


DEF_CHOICE_3(mcSettings_ShowAltMarkers, PageDisplay::PageSettings::self,
    "Доп. маркеры", "Alt. markers"
    ,
    "Устанавливает режим отображения дополнительных маркеров уровней смещения и синхронизации:\n"
    "\"Скрывать\" - дополнительные маркеры не показываются,\n"
    "\"Показывать\" - дополнительные маркеры показываются всегда,\n"
    "\"Авто\" - дополнительные маркеры показываются в течение 5 сек после поворота ручки смещения канала по напряжению или уровня синхронизации"
    ,
    "Sets the display mode of additional markers levels of displacement and synchronization:\n"
    "\"Hide\" - additional markers are not shown,\n"
    "\"Show\" - additional markers are shown always,\n"
    "\"Auto\" - additional markers are displayed for 5 seconds after turning the handle channel offset voltage or trigger level"
    ,
    "Скрывать", "Hide",
    "Показывать", "Show",
    "Авто", "Auto",
    set.display.alt_markers, nullptr, OnChanged_Settings_ShowAltMarkers, nullptr
)


static void OnChanged_Settings_AutoHide(bool autoHide)
{
    Menu::SetAutoHide(autoHide);
}


DEF_CHOICE_6(mcSettings_AutoHide, PageDisplay::PageSettings::self,
    "Скрывать", "Hide",
    "Установка после последнего нажатия кнопки или поворота ручки, по истечении которого меню автоматически убирается с экрана",
    "Installation after the last keystroke or turning the handle, after which the menu will automatically disappear",
    "Никогда", "Never",
    "Через 5 сек", "Through 5 s",
    "Через 10 сек", "Through 10 s",
    "Через 15 сек", "Through 15 s",
    "Через 30 сек", "Through 30 s",
    "Через 60 сек", "Through 60 s",
    set.display.menu_auto_hide, nullptr, OnChanged_Settings_AutoHide, nullptr
)


DEF_PAGE_7(pageSettings, PageDisplay::self, NamePage::ServiceDisplay,
    "НАСТРОЙКИ", "SETTINGS",
    "Дополнительные настройки дисплея",
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
