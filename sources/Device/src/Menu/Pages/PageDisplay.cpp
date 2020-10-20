#include "defines.h"
#include "Menu/MenuItems.h"
#include "defines.h"
#include "Menu/Menu.h"
#include "Tables.h"
#include "Display/Painter.h"
#include "FPGA/FPGA.h"
#include "Settings/SettingsTypes.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"


extern const Page pDisplay;


extern const Choice mcMapping;                              //     ДИСПЛЕЙ - Отображение

extern const Page mspAccumulation;                          //     ДИСПЛЕЙ - НАКОПЛЕНИЕ
static bool IsActive_Accumulation();                        // Активна ли страница ДИСПЛЕЙ-НАКОПЛЕНИЕ
extern const Choice mcAccumulation_Number;                  // ДИСПЛЕЙ - НАКОПЛЕНИЕ - Количество
extern const Choice mcAccumulation_Mode;                    // ДИСПЛЕЙ - НАКОПЛЕНИЕ - Режим
extern const Button mbAccumulation_Clear;                   // ДИСПЛЕЙ - НАКОПЛЕНИЕ - Очистить
static bool IsActive_Accumulation_Clear();                  // Активна ли кнопка ДИСПЛЕЙ-НАКОПЛЕНИЕ-Очистить
void OnPress_Accumulation_Clear();                          // Обработка нажатия ДИСПЛЕЙ-НАКОПЛЕНИЕ-Очистить

extern const Page mspAveraging;                             //     ДИСПЛЕЙ - УСРЕДНЕНИЕ
static bool IsActive_Averaging();                           // Активна ли страница ДИСПЛЕЙ-УСРЕДНЕНИЕ
extern const Choice mcAveraging_Number;                     // ДИСПЛЕЙ - УСРЕДНЕНИЕ - Количество
extern const Choice mcAveraging_Mode;                       // ДИСПЛЕЙ - УСРЕДНЕНИЕ - Режим

extern const Choice mcMinMax;                               //     ДИСПЛЕЙ - Мин Макс
static bool IsActive_MinMax();                              // Активна ли настройка ДИСПЛЕЙ-Мин Макс
static void OnChanged_MinMax(bool active);                  // Реакция на изменение ДИСПЛЕЙ-Мим Макс

extern const Choice mcSmoothing;                            //     ДИСПЛЕЙ - Сглаживание

extern const Choice mcRefreshFPS;                           //     ДИСПЛЕЙ - Частота обновл
void OnChanged_RefreshFPS(bool active);                     // Реакция на изменение ДИСПЛЕЙ-Частота обновл
extern const Page mspGrid;                                  //     ДИСПЛЕЙ - СЕТКА

extern const Choice mcGrid_Type;                            //     ДИСПЛЕЙ - СЕТКА - Тип
extern const Governor mgGrid_Brightness;                    // ДИСПЛЕЙ - СЕТКА - Яркость
void OnChanged_Grid_Brightness();                           // Реакция на изменение ДИСПЛЕЙ-СЕТКА-Яркость
static void BeforeDraw_Grid_Brightness();                   // Вызывается перед изменением ДИСПЛЕЙ-СЕТКА-Яркость

extern const Choice mcTypeShift;                            //     ДИСПЛЕЙ - Смещение

extern const Page mspSettings;                              //     ДИСПЛЕЙ - НАСТРОЙКИ
extern const Page mspSettings_Colors;                       // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА
extern const Choice mcSettings_Colors_Scheme;               // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Цветовая схема
extern const GovernorColor mgcSettings_Colors_ChannelA;     // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 1
extern const GovernorColor mgcSettings_Colors_ChannelB;     // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 2
extern const GovernorColor mgcSettings_Colors_Grid;         // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сетка
extern const Governor mgSettings_Brightness;                // ДИСПЛЕЙ - НАСТРОЙКИ - Яркость
static void OnChanged_Settings_Brightness();                // Вызыватеся при изменении ДИСПЛЕЙ-НАСТРОЙКИ-Яркость
extern const Governor mgSettings_Levels;                    // ДИСПЛЕЙ - НАСТРОЙКИ - Уровни
extern const Governor mgSettings_TimeMessages;              // ДИСПЛЕЙ - НАСТРОЙКИ - Время
extern const Choice mcSettings_ShowStringNavigation;        // ДИСПЛЕЙ - НАСТРОЙКИ - Строка меню
extern const Choice mcSettings_ShowAltMarkers;              // ДИСПЛЕЙ - НАСТРОЙКИ - Доп. маркеры
static void OnChanged_Settings_ShowAltMarkers(bool);
extern const Choice mcSettings_AutoHide;                    // ДИСПЛЕЙ - НАСТРОЙКИ - Скрывать
static void OnChanged_Settings_AutoHide(bool autoHide);     // Вызывается при изменении ДИСПЛЕЙ-НАСТРОЙКИ-Скрывать


// ДИСПЛЕЙ /////////////////////////
extern const Page mainPage;

DEF_PAGE_9(pDisplay, mainPage, NamePage::Display,
    "ДИСПЛЕЙ", "DISPLAY",
    "Содержит настройки отображения дисплея.",
    "Contains settings of display of the Display::",
    mcMapping,
    mspAccumulation,
    mspAveraging,
    mcMinMax,
    mcSmoothing,
    mcRefreshFPS,
    mspGrid,
    mcTypeShift,
    mspSettings,
    nullptr, nullptr, nullptr, nullptr
)


// ДИСПЛЕЙ - Отображение -----------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcMapping, pDisplay,
    "Отображение", "View",
    "Задаёт режим отображения сигнала.",
    "Sets the display mode signal.",
    "Вектор", "Vector",
    "Точки",  "Points",
    MODE_DRAW_SIGNAL, nullptr, nullptr, nullptr
)


// ДИСПЛЕЙ - НАКОПЛЕНИЕ ////////////
DEF_PAGE_3(mspAccumulation, pDisplay, NamePage::DisplayAccumulation,
    "НАКОПЛЕНИЕ", "ACCUMULATION",
    "Настройки режима отображения последних сигналов на экране.",
    "Mode setting signals to display the last screen.",
    mcAccumulation_Number,
    mcAccumulation_Mode,
    mbAccumulation_Clear,
    IsActive_Accumulation, nullptr, nullptr, nullptr
)

static bool IsActive_Accumulation(void)
{
    return SET_TBASE > TBase::_50ns;
}

// ДИСПЛЕЙ - НАКОПЛЕНИЕ - Количество -----------------------------------------------------------------------------------------------------------------
DEF_CHOICE_REG_9(mcAccumulation_Number, mspAccumulation,
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
    DISABLE_RU,      DISABLE_EN,
    "2",             "2",
    "4",             "4",
    "8",             "8",
    "16",            "16",
    "32",            "32",
    "64",            "64",
    "128",           "128",
    "Бесконечность", "Infinity",
    ENUM_ACCUM, nullptr, nullptr, nullptr
)


// ДИСПЛЕЙ - НАКОПЛЕНИЕ - Режим ----------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcAccumulation_Mode, mspAccumulation,
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
    "Сбрасывать",    "Dump",
    MODE_ACCUM, nullptr, nullptr, nullptr
)


// ДИСПЛЕЙ - НАКОПЛЕНИЕ - Очистить /
DEF_BUTTON(mbAccumulation_Clear, mspAccumulation,
    "Очистить", "Clear",
    "Очищает экран от накопленных сигналов.",
    "Clears the screen of the saved-up signals.",
    IsActive_Accumulation_Clear, OnPress_Accumulation_Clear
)


static bool IsActive_Accumulation_Clear(void)
{
    return ENUM_ACCUM_IS_INFINITY;
}

void OnPress_Accumulation_Clear(void)
{
    Display::Redraw();
}


// ДИСПЛЕЙ - УСРЕДНЕНИЕ ////////////
DEF_PAGE_2(mspAveraging, pDisplay, NamePage::DisplayAverage,
    "УСРЕДНЕНИЕ", "AVERAGE",
    "Настройки режима усреднения по последним измерениям.",
    "Settings of the mode of averaging on the last measurements.",
    mcAveraging_Number,
    mcAveraging_Mode,
    IsActive_Averaging, nullptr, nullptr, nullptr
)

static bool IsActive_Averaging(void)
{
    return true;
}


// ДИСПЛЕЙ - УСРЕДНЕНИЕ - Количество -----------------------------------------------------------------------------------------------------------------
DEF_CHOICE_REG_10(mcAveraging_Number, mspAveraging,
    "Количество", "Number",
    "Задаёт количество последних измерений, по которым производится усреднение.",
    "Sets number of the last measurements on which averaging is made.",
    DISABLE_RU,    DISABLE_EN,
    "2",           "2",
    "4",           "4",
    "8",           "8",
    "16",          "16",
    "32",          "32",
    "64",          "64",
    "128",         "128",
    "256",         "256",
    "512",         "512",
    ENUM_AVE, nullptr, nullptr, nullptr
)

// ДИСПЛЕЙ - УСРЕДНЕНИЕ - Режим ----------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcAveraging_Mode, mspAveraging,
    "Режим", "Mode"
    ,
    "1. \"Точно\" - точный режим усреднения, когда в расчёте участвуют только последние сигналы.\n"
    "2. \"Приблизительно\" - приблизительный режим усреднения. Имеет смысл использовать, когда задано количество измерений большее, чем может "
    "поместиться в памяти."
    ,
    "1. \"Accurately\" - the exact mode of averaging when only the last signals participate in calculation.\n"
    "2. \"Around\" - approximate mode of averaging. It makes sense to use when the number of measurements bigger is set, than can be located in "
    "memory.",
    "Точно",          "Accurately",
    "Приблизительно", "Around",
    MODE_AVE, nullptr, nullptr, nullptr
)


// ДИСПЛЕЙ - Мин Макс --------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_REG_8(mcMinMax, pDisplay,
    "Мин Макс", "Min Max",
    "Задаёт количество последних измерений, по которым строятся ограничительные линии, огибающие минимумы и максимумы измерений.",
    "Sets number of the last measurements on which the limiting lines which are bending around minima and maxima of measurements are under construction.",
    DISABLE_RU, DISABLE_EN,
    "2",        "2",
    "4",        "4",
    "8",        "8",
    "16",       "16",
    "32",       "32",
    "64",       "64",
    "128",      "128",
    ENUM_MIN_MAX, IsActive_MinMax, OnChanged_MinMax, nullptr
)

static bool IsActive_MinMax(void) //-V524
{
    return SET_TBASE > TBase::_50ns;
}

static void OnChanged_MinMax(bool)
{
    /*
    int maxMeasures = DS_NumberAvailableEntries();  
    int numMinMax = sDisplay_NumberMinMax();

    if (maxMeasures < numMinMax)
    {
        Display::ShowWarningWithNumber(ExcessValues, maxMeasures);
    }
    */
}



// ДИСПЛЕЙ - Сглаживание -----------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_REG_10(mcSmoothing, pDisplay,
    "Сглаживание", "Smoothing",
    "Устанавливает количество точек для расчёта сглаженного по соседним точкам сигнала.",
    "Establishes quantity of points for calculation of the signal smoothed on the next points.",
    DISABLE_RU, DISABLE_EN,
    "2 точки",  "2 points",
    "3 точки",  "3 points",
    "4 точки",  "4 points",
    "5 точек",  "5 points",
    "6 точек",  "6 points",
    "7 точек",  "7 points",
    "8 точек",  "8 points",
    "9 точек",  "9 points",
    "10 точек", "10 points",
    SMOOTHING, nullptr, nullptr, nullptr
)


// ДИСПЛЕЙ - Частота обновл --------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_5(mcRefreshFPS, pDisplay,
    "Частота обновл", "Refresh rate",
    "Задаёт максимальное число выводимых в секунду кадров.",
    "Sets the maximum number of the shots removed in a second.",
    "25", "25",
    "10", "10",
    "5",  "5",
    "2",  "2",
    "1",  "1",
    ENUM_SIGNALS_IN_SEC, nullptr, OnChanged_RefreshFPS, nullptr
)

void OnChanged_RefreshFPS(bool)
{
    FPGA::SetNumSignalsInSec(ENumSignalsInSec::NumSignalsInS());
}


// ДИСПЛЕЙ - СЕТКА /////////////////
DEF_PAGE_2(mspGrid, pDisplay, NamePage::DisplayGrid,
    "СЕТКА", "GRID",
    "Содержит настройки отображения координатной сетки.",
    "Contains settings of display of a coordinate Grid::",
    mcGrid_Type,
    mgGrid_Brightness,
    nullptr, nullptr, nullptr, nullptr
)


// ДИСПЛЕЙ - СЕТКА - Тип -----------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(mcGrid_Type, mspGrid,
    "Тип", "Type",
    "Выбор типа сетки.",
    "Choice like Grid.",
    "Тип 1", "Type 1",
    "Тип 2", "Type 2",
    "Тип 3", "Type 3",
    "Тип 4", "Type 4",
    TYPE_GRID, nullptr, nullptr, nullptr
)


// ДИСПЛЕЙ - СЕТКА - Яркость -------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgGrid_Brightness, mspGrid,
    "Яркость", "Brightness",
    "Устанавливает яркость сетки.",
    "Adjust the brightness of the Grid.",
    BRIGHTNESS_GRID, 0, 100, nullptr, OnChanged_Grid_Brightness, BeforeDraw_Grid_Brightness
)


ColorType colorTypeGrid = {0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::GRID};

void OnChanged_Grid_Brightness(void)
{
    Color_SetBrightness(&colorTypeGrid, BRIGHTNESS_GRID / 1e2F);
}

static void BeforeDraw_Grid_Brightness(void)
{
    Color_Init(&colorTypeGrid);
    BRIGHTNESS_GRID = (int16)(colorTypeGrid.brightness * 100.0F);
}


// ДИСПЛЕЙ - Смещение --------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcTypeShift, pDisplay,
    "Смещение", "Offset",
    "Задаёт режим удержания смещения по вертикали\n1. \"Напряжение\" - сохраняется напряжение смещения.\n2. \"Деления\" - сохраняется положение смещения на экране.",
    "Sets the mode of retaining the vertical displacement\n1. \"Voltage\" - saved dressing bias.\n2. \"Divisions\" - retained the position of the offset on the screen.",
    "Напряжение", "Voltage",
    "Деления",    "Divisions",
    LINKING_RSHIFT, nullptr, nullptr, nullptr
)


// ДИСПЛЕЙ - НАСТРОЙКИ /////////////
DEF_PAGE_7(mspSettings, pDisplay, NamePage::ServiceDisplay,
    "НАСТРОЙКИ", "SETTINGS",
    "Дополнительные настройки дисплея",
    "Additional display settings",
    mspSettings_Colors,
    mgSettings_Brightness,
    mgSettings_Levels,
    mgSettings_TimeMessages,
    mcSettings_ShowStringNavigation,
    mcSettings_ShowAltMarkers,
    mcSettings_AutoHide,
    nullptr, nullptr, nullptr, nullptr
)


// ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА /////
DEF_PAGE_4(mspSettings_Colors, mspSettings, NamePage::ServiceDisplayColors,
    "ЦВЕТА", "COLORS",
    "Выбор цветов дисплея",
    "The choice of colors display",
    mcSettings_Colors_Scheme,
    mgcSettings_Colors_ChannelA,
    mgcSettings_Colors_ChannelB,
    mgcSettings_Colors_Grid,
    nullptr, nullptr, nullptr, nullptr
)


// ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Цветовая схема ------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcSettings_Colors_Scheme, mspSettings_Colors,
    "Цветовая схема", "Color scheme",
    "Режим работы калибратора",
    "Mode of operation of the calibrator",
    "Схема 1", "Scheme 1",
    "Схема 2", "Scheme 2",
    COLOR_SCHEME, nullptr, nullptr, nullptr
)

static ColorType colorT1 = {0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::DATA_A};

// ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 1 -------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR_COLOR(mgcSettings_Colors_ChannelA, mspSettings_Colors,
    "Канал 1", "Channel 1",
    "",
    "",
    colorT1, nullptr
)


static ColorType colorT2 = {0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::DATA_B};

// ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 2 -------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR_COLOR(mgcSettings_Colors_ChannelB, mspSettings_Colors,
    "Канал 2", "Channel 2",
    "",
    "",
    colorT2, nullptr
)


// ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сетка ---------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR_COLOR(mgcSettings_Colors_Grid, mspSettings_Colors,
    "Сетка", "Grid",
    "Устанавливает цвет сетки",
    "Sets the grid color",
    colorTypeGrid, nullptr
)


// ДИСПЛЕЙ - НАСТРОЙКИ - Яркость ---------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgSettings_Brightness, mspSettings,
    "Яркость", "Brightness",
    "Установка яркости свечения дисплея",
    "Setting the brightness of the display",
    BRIGHTNESS, 0, 100, nullptr, OnChanged_Settings_Brightness, nullptr
)

static void OnChanged_Settings_Brightness(void)
{
    Painter::SetBrightnessDisplay(BRIGHTNESS);
}


// ДИСПЛЕЙ - НАСТРОЙКИ - Уровни ----------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgSettings_Levels, mspSettings,
    "Уровни", "Levels",
    "Задаёт время, в течение которого после поворота ручки сещения напряжения на экране остаётся вспомогательная метка уровня смещения",
    "Defines the time during which, after turning the handle visits to the voltage on the screen remains auxiliary label offset level",
    TIME_SHOW_LEVELS, 0, 125, nullptr, nullptr, nullptr
)

// ДИСПЛЕЙ - НАСТРОЙКИ - Время -----------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgSettings_TimeMessages, mspSettings,
    "Время", "Time",
    "Установка времени, в течение которого сообщения будут находиться на экране",
    "Set the time during which the message will be on the screen",
    TIME_MESSAGES, 1, 99, nullptr, nullptr, nullptr
)


// ДИСПЛЕЙ - НАСТРОЙКИ - Строка меню -----------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcSettings_ShowStringNavigation, mspSettings,
    "Строка меню", "Path menu",
    "При выборе \nПоказывать\n слева вверху экрана выводится полный путь до текущей страницы меню",
    "When choosing \nDisplay\n at the top left of the screen displays the full path to the current page menu",
    "Скрывать",   "Hide",
    "Показывать", "Show",
    SHOW_STRING_NAVIGATION, nullptr, nullptr, nullptr
)


// ДИСПЛЕЙ - НАСТРОЙКИ - Доп. маркеры ----------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(mcSettings_ShowAltMarkers, mspSettings,
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
    "Скрывать",   "Hide",
    "Показывать", "Show",
    "Авто",       "Auto",
    ALT_MARKERS, nullptr, OnChanged_Settings_ShowAltMarkers, nullptr
)

static void OnChanged_Settings_ShowAltMarkers(bool)
{
    Display::ChangedRShiftMarkers();
}


// ДИСПЛЕЙ - НАСТРОЙКИ - Скрывать --------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_6(mcSettings_AutoHide, mspSettings,
    "Скрывать", "Hide",
    "Установка после последнего нажатия кнопки или поворота ручки, по истечении которого меню автоматически убирается с экрана",
    "Installation after the last keystroke or turning the handle, after which the menu will automatically disappear",
    "Никогда",      "Never",
    "Через 5 сек",  "Through 5 s",
    "Через 10 сек", "Through 10 s",
    "Через 15 сек", "Through 15 s",
    "Через 30 сек", "Through 30 s",
    "Через 60 сек", "Through 60 s",
    MENU_AUTO_HIDE, nullptr, OnChanged_Settings_AutoHide, nullptr
)

static void OnChanged_Settings_AutoHide(bool autoHide)
{
    Menu::SetAutoHide(autoHide);
}


// ДИСПЛЕЙ - Окно памяти ///////////
/*
static const Choice mcDisplMemoryWindow =
{
    TypeItem::Choice, &pDisplay, 0,
    {
        "Окно памяти", "Window memory",
        "1. \"Стандартное\" - в верхней части экрана выводится содержимое памяти.\n2. \"Упрощённое\" - выводится положение видимого окна в памяти.",
        "1. \"Standard\" - in the top part of the screen memory contents are removed.\n2. \"Simplified\" - shows the position of the visible window in memory."
    },
    {
        {"Упрощённое",  "Simplified"},
        {"Стандартное", "Standard"}
    },
    (int8*)&set.display.showFullMemoryWindow
};
*/
