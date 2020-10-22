#include "defines.h"
#include "common/Display/Painter_c.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


DEF_CHOICE_2(mcMapping, PageDisplay::self,
    "Отображение", "View",
    "Задаёт режим отображения сигнала.",
    "Sets the display mode signal.",
    "Вектор", "Vector",
    "Точки", "Points",
    MODE_DRAW_SIGNAL, nullptr, nullptr, nullptr
)

static bool IsActive_Accumulation(void)
{
    return SET_TBASE > TBase::_50ns;
}

DEF_CHOICE_REG_9(mcAccumulation_Number, PageDisplay::PageAccumulation::self,
    "Количество", "Number"
    ,
    "Задаёт максимальное количество последних сигналов на экране. Если в настройке \"Режим\" выбрано \"Бесконечность\", экран очищается только "
    "нажатием кнопки \"Очистить\"."
    "\"Бесконечность\" - каждое измерение остаётся на дисплее до тех пор, пока не будет нажата кнопка \"Очистить\"."
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
    "Бесконечность", "Infinity",
    ENUM_ACCUM, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcAccumulation_Mode, PageDisplay::PageAccumulation::self,
    "Режим", "Mode"
    ,
    "1. \"Сбрасывать\" - после накопления заданного количества измерения происходит очистка дисплея. Этот режим удобен, когда памяти не хватает "
    "для сохранения нужного количества измерений.\n"
    "2. \"Не сбрасывать\" - на дисплей всегда выводится заданное или меньшее (в случае нехватки памяти) количество измерений. Недостатком является "
    "меньшее быстродействие и невозможность накопления заданного количества измерений при недостатке памяти."
    ,
    "1. \"Dump\" - after accumulation of the set number of measurement there is a cleaning of the Display:: This mode is convenient when memory "
    "isn't enough for preservation of the necessary number of measurements.\n"
    "2. \"Not to dump\" - the number of measurements is always output to the display set or smaller (in case of shortage of memory). Shortcoming "
    "is smaller speed and impossibility of accumulation of the set number of measurements at a lack of memory."
    ,
    "Не сбрасывать", "Not to dump",
    "Сбрасывать", "Dump",
    MODE_ACCUM, nullptr, nullptr, nullptr
)

static bool IsActive_Accumulation_Clear()
{
    return ENUM_ACCUM_IS_INFINITY;
}

DEF_BUTTON(mbAccumulation_Clear, PageDisplay::PageAccumulation::self,
    "Очистить", "Clear",
    "Очищает экран от накопленных сигналов.",
    "Clears the screen of the saved-up signals.",
    IsActive_Accumulation_Clear, PageDisplay::OnPress_Accumulation_Clear
)

DEF_PAGE_3(pageAccumulation, PageDisplay::self, NamePage::DisplayAccumulation,
    "НАКОПЛЕНИЕ", "ACCUMULATION",
    "Настройки режима отображения последних сигналов на экране.",
    "Mode setting signals to display the last screen.",
    mcAccumulation_Number,
    mcAccumulation_Mode,
    mbAccumulation_Clear,
    IsActive_Accumulation, nullptr, nullptr, nullptr
)

static bool IsActive_MinMax() //-V524
{
    return SET_TBASE > TBase::_50ns;
}

static void OnChanged_MinMax(bool)
{
}

DEF_CHOICE_REG_8(mcMinMax, PageDisplay::self,
    "Мин Макс", "Min Max",
    "Задаёт количество последних измерений, по которым строятся ограничительные линии, огибающие минимумы и максимумы измерений.",
    "Sets number of the last measurements on which the limiting lines which are bending around minima and maxima of measurements are under construction.",
    DISABLE_RU, DISABLE_EN,
    "2", "2",
    "4", "4",
    "8", "8",
    "16", "16",
    "32", "32",
    "64", "64",
    "128", "128",
    ENUM_MIN_MAX, IsActive_MinMax, OnChanged_MinMax, nullptr
)

DEF_CHOICE_REG_10(mcSmoothing, PageDisplay::self,
    "Сглаживание", "Smoothing",
    "Устанавливает количество точек для расчёта сглаженного по соседним точкам сигнала.",
    "Establishes quantity of points for calculation of the signal smoothed on the next points.",
    DISABLE_RU, DISABLE_EN,
    "2 точки", "2 points",
    "3 точки", "3 points",
    "4 точки", "4 points",
    "5 точек", "5 points",
    "6 точек", "6 points",
    "7 точек", "7 points",
    "8 точек", "8 points",
    "9 точек", "9 points",
    "10 точек", "10 points",
    SMOOTHING, nullptr, nullptr, nullptr
)

void PageDisplay::OnChanged_RefreshFPS(bool)
{
    FPGA::SetNumSignalsInSec(ENumSignalsInSec::NumSignalsInS());
}

DEF_CHOICE_5(mcRefreshFPS, PageDisplay::self,
    "Частота обновл", "Refresh rate",
    "Задаёт максимальное число выводимых в секунду кадров.",
    "Sets the maximum number of the shots removed in a second.",
    "25", "25",
    "10", "10",
    "5", "5",
    "2", "2",
    "1", "1",
    ENUM_SIGNALS_IN_SEC, nullptr, PageDisplay::OnChanged_RefreshFPS, nullptr
)

DEF_CHOICE_2(mcTypeShift, PageDisplay::self,
    "Смещение", "Offset",
    "Задаёт режим удержания смещения по вертикали\n1. \"Напряжение\" - сохраняется напряжение смещения.\n2. \"Деления\" - сохраняется положение смещения на экране.",
    "Sets the mode of retaining the vertical displacement\n1. \"Voltage\" - saved dressing bias.\n2. \"Divisions\" - retained the position of the offset on the screen.",
    "Напряжение", "Voltage",
    "Деления", "Divisions",
    LINKING_RSHIFT, nullptr, nullptr, nullptr
)

DEF_PAGE_9(pageDisplay, PageMain::self, NamePage::Display,
    "ДИСПЛЕЙ", "DISPLAY",
    "Содержит настройки отображения дисплея.",
    "Contains settings of display of the Display::",
    mcMapping,
    pageAccumulation,
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

DEF_CHOICE_REG_10(mcAveraging_Number, PageDisplay::PageAveraging::self,
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
    ENUM_AVE, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcAveraging_Mode, PageDisplay::PageAveraging::self,
    "Режим", "Mode"
    ,
    "1. \"Точно\" - точный режим усреднения, когда в расчёте участвуют только последние сигналы.\n"
    "2. \"Приблизительно\" - приблизительный режим усреднения. Имеет смысл использовать, когда задано количество измерений большее, чем может "
    "поместиться в памяти."
    ,
    "1. \"Accurately\" - the exact mode of averaging when only the last signals participate in calculation.\n"
    "2. \"Around\" - approximate mode of averaging. It makes sense to use when the number of measurements bigger is set, than can be located in "
    "memory.",
    "Точно", "Accurately",
    "Приблизительно", "Around",
    MODE_AVE, nullptr, nullptr, nullptr
)

static bool IsActive_Averaging(void)
{
    return true;
}

DEF_PAGE_2(pageAveraging, PageDisplay::self, NamePage::DisplayAverage,
    "УСРЕДНЕНИЕ", "AVERAGE",
    "Настройки режима усреднения по последним измерениям.",
    "Settings of the mode of averaging on the last measurements.",
    mcAveraging_Number,
    mcAveraging_Mode,
    IsActive_Averaging, nullptr, nullptr, nullptr
)

DEF_CHOICE_4(mcGrid_Type, PageDisplay::PageGrid::self,
    "Тип", "Type",
    "Выбор типа сетки.",
    "Choice like Grid.",
    "Тип 1", "Type 1",
    "Тип 2", "Type 2",
    "Тип 3", "Type 3",
    "Тип 4", "Type 4",
    TYPE_GRID, nullptr, nullptr, nullptr
)

static ColorType cTypeGrid = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::GRID };

void PageDisplay::OnChanged_Grid_Brightness(void)
{
    Color_SetBrightness(&cTypeGrid, BRIGHTNESS_GRID / 1e2F);
}

static void BeforeDraw_Grid_Brightness(void)
{
    Color_Init(&cTypeGrid);
    BRIGHTNESS_GRID = (int16)(cTypeGrid.brightness * 100.0F);
}

DEF_GOVERNOR(mgGrid_Brightness, PageDisplay::PageGrid::self,
    "Яркость", "Brightness",
    "Устанавливает яркость сетки.",
    "Adjust the brightness of the Grid.",
    BRIGHTNESS_GRID, 0, 100, nullptr, PageDisplay::OnChanged_Grid_Brightness, BeforeDraw_Grid_Brightness
)

DEF_PAGE_2(pageGrid, PageDisplay::self, NamePage::DisplayGrid,
    "СЕТКА", "GRID",
    "Содержит настройки отображения координатной сетки.",
    "Contains settings of display of a coordinate Grid::",
    mcGrid_Type,
    mgGrid_Brightness,
    nullptr, nullptr, nullptr, nullptr
)

static void OnChanged_Settings_Brightness()
{
    Painter::SetBrightnessDisplay(BRIGHTNESS);
}

DEF_GOVERNOR(mgSettings_Brightness, PageDisplay::PageSettings::self,
    "Яркость", "Brightness",
    "Установка яркости свечения дисплея",
    "Setting the brightness of the display",
    BRIGHTNESS, 0, 100, nullptr, OnChanged_Settings_Brightness, nullptr
)

DEF_GOVERNOR(mgSettings_Levels, PageDisplay::PageSettings::self,
    "Уровни", "Levels",
    "Задаёт время, в течение которого после поворота ручки сещения напряжения на экране остаётся вспомогательная метка уровня смещения",
    "Defines the time during which, after turning the handle visits to the voltage on the screen remains auxiliary label offset level",
    TIME_SHOW_LEVELS, 0, 125, nullptr, nullptr, nullptr
)

DEF_GOVERNOR(mgSettings_TimeMessages, PageDisplay::PageSettings::self,
    "Время", "Time",
    "Установка времени, в течение которого сообщения будут находиться на экране",
    "Set the time during which the message will be on the screen",
    TIME_MESSAGES, 1, 99, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcSettings_ShowStringNavigation, PageDisplay::PageSettings::self,
    "Строка меню", "Path menu",
    "При выборе \nПоказывать\n слева вверху экрана выводится полный путь до текущей страницы меню",
    "When choosing \nDisplay\n at the top left of the screen displays the full path to the current page menu",
    "Скрывать", "Hide",
    "Показывать", "Show",
    SHOW_STRING_NAVIGATION, nullptr, nullptr, nullptr
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
    ALT_MARKERS, nullptr, OnChanged_Settings_ShowAltMarkers, nullptr
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
    MENU_AUTO_HIDE, nullptr, OnChanged_Settings_AutoHide, nullptr
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

DEF_CHOICE_2(mcSettings_Colors_Scheme, PageDisplay::PageSettings::PageColors::self,
    "Цветовая схема", "Color scheme",
    "Режим работы калибратора",
    "Mode of operation of the calibrator",
    "Схема 1", "Scheme 1",
    "Схема 2", "Scheme 2",
    COLOR_SCHEME, nullptr, nullptr, nullptr
)

static ColorType colorT1 = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::DATA_A };

DEF_GOVERNOR_COLOR(mgcSettings_Colors_ChannelA, PageDisplay::PageSettings::PageColors::self,
    "Канал 1", "Channel 1",
    "",
    "",
    colorT1, nullptr
)

static ColorType colorT2 = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::DATA_B };

DEF_GOVERNOR_COLOR(mgcSettings_Colors_ChannelB, PageDisplay::PageSettings::PageColors::self,
    "Канал 2", "Channel 2",
    "",
    "",
    colorT2, nullptr
)

DEF_GOVERNOR_COLOR(mgcSettings_Colors_Grid, PageDisplay::PageSettings::PageColors::self,
    "Сетка", "Grid",
    "Устанавливает цвет сетки",
    "Sets the grid color",
    cTypeGrid, nullptr
)

DEF_PAGE_4(pageColors, PageDisplay::PageSettings::self, NamePage::ServiceDisplayColors,
    "ЦВЕТА", "COLORS",
    "Выбор цветов дисплея",
    "The choice of colors display",
    mcSettings_Colors_Scheme,
    mgcSettings_Colors_ChannelA,
    mgcSettings_Colors_ChannelB,
    mgcSettings_Colors_Grid,
    nullptr, nullptr, nullptr, nullptr
)

ColorType *PageDisplay::colorTypeGrid = &cTypeGrid;
const Page *PageDisplay::self = &pageDisplay;
const Page *PageDisplay::PageAccumulation::self = &pageAccumulation;
const Page *PageDisplay::PageAveraging::self = &pageAveraging;
const Page *PageDisplay::PageGrid::self = &pageGrid;
const Page *PageDisplay::PageSettings::self = &pageSettings;
const Page *PageDisplay::PageSettings::PageColors::self = &pageColors;
