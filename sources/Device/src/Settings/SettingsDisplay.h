#pragma once
#include "defines.h"
#include "SettingsTypes.h"
#include "Display/Colors.h"


/** @addtogroup Settings
 *  @{
 *  @defgroup SettingsDisplay
 *  @{
 */



#define TIME_SHOW_LEVELS            (set.display.timeShowLevels)                // SettingsDisplay.timeShowLevels

#define SHIFT_IN_MEMORY             (set.display.shiftInMemory)                 // SettingsDisplay.shiftInMemory

#define TIME_MESSAGES               (set.display.timeMessages)                  // SettingsDisplay.timeMessages

#define BRIGHTNESS                  (set.display.brightness)                    // SettingsDisplay.brightness

#define BRIGHTNESS_GRID             (set.display.brightnessGrid)                // SettingsDisplay.brightnessGrid

#define DISPLAY_COLOR(num)          (set.display.colors[num])                   // SettingsDisplay.colors

#define MODE_DRAW_SIGNAL            (set.display.modeDrawSignal)                // SettingsDisplay.modeDrawSignal
#define MODE_DRAW_IS_SIGNAL_LINES   (MODE_DRAW_SIGNAL == ModeDrawSignal::Lines)  // \c true, если сигнал выводится линиями.

#define TYPE_GRID                   (set.display.typeGrid)                      // SettingsDisplay.typeGrid
#define TYPE_GRID_IS_1              (TYPE_GRID == TypeGrid::_1)                 // Установлен тип сетки TypeGrid::_1.
#define TYPE_GRID_IS_2              (TYPE_GRID == TypeGrid::_2)                 // Установлен тип сетки TypeGrid_2.
#define TYPE_GRID_IS_3              (TYPE_GRID == TypeGrid::_3)                 // Установлен тип сетки TypeGrid_3.

#define ENUM_ACCUM                  (set.display.enumAccumulation)              // SettingsDisplay.numAccumulation
#define ENUM_ACCUM_IS_NONE          (ENUM_ACCUM == ENumAccumulation::_1)        // \c true, если накопление выключено.
#define ENUM_ACCUM_IS_INFINITY      (ENUM_ACCUM == ENumAccumulation::Infinity)  // \c true, если установлено бесконечное число накоплений.
#define NUM_ACCUM                   (1 << (int)set.display.enumAccumulation)    // Количество накоплений.

#define ENUM_AVE                    (set.display.enumAve)                       // SettingsDisplay.enumAve
#define NUM_AVE                     (1 << ENUM_AVE)                             // Количество усреднений.

#define MODE_AVE                    (set.display.modeAve)                       // SettingsDisplay.modeAve

#define ENUM_MIN_MAX                (set.display.enumMinMax)                    // SettingsDisplay.enumMinMax
#define NUM_MIN_MAX                 (1 << ENUM_MIN_MAX)                         // Количество измерений сигналов для расчёта минимумов и максимумов.

#define SMOOTHING                   (set.display.smoothing)                     // SettingsDisplay.smoothing
#define SMOOTHING_IS_DISABLE        (SMOOTHING == Smoothing::Disable)            // \c true, если скользящий фильтр выключен.

#define ENUM_SIGNALS_IN_SEC         (set.display.enumSignalsInSec)              // SettingsDisplay.enumSignalsInSec

#define LAST_AFFECTED_CHANNEL       (set.display.lastAffectedChannel)           // SettingsDisplay.lastAffectedChannel
#define LAST_AFFECTED_CHANNEL_IS_A  (LAST_AFFECTED_CHANNEL == Channel::A)       // \c true, если последний канал, которым управляли - 1.
#define LAST_AFFECTED_CHANNEL_IS_B  (LAST_AFFECTED_CHANNEL == Channel::B)       // \c true, если последний канал, которым управляли - 2.

#define MODE_ACCUM                  (set.display.modeAccumulation)              // SettingsDisplay.modeAccumulation
#define MODE_ACCUM_IS_RESET         (MODE_ACCUM == ModeAccumulation::Reset)     // \c true, если накопление со сбросом.
#define MODE_ACCUM_IS_NORESET       (MODE_ACCUM == ModeAccumulation::NoReset)   // \c true, если накопление без сброса.

#define ALT_MARKERS                 (set.display.altMarkers)                    // SettingsDisplay.altMarkers
#define ALT_MARKERS_HIDE            (ALT_MARKERS == AltMarkers::Hide)           // \c true, если дополнительные маркеры никогда не надо показывать.

#define MENU_AUTO_HIDE              (set.display.menuAutoHide)                  // SettingsDisplay.menuAutoHide

#define SHOW_STRING_NAVIGATION      (set.display.showStringNavigation)          // SettingsDisplay.showStringNavigation

#define LINKING_RSHIFT              (set.display.linkingRShift)                 // SettingsDisplay.linkingRShift
#define LINKING_RSHIFT_IS_VOLTAGE   (LINKING_RSHIFT == LinkingRShift::Voltage)   // \c true, если привязка к абсолютному напряжению.


// Режим накопления.
struct ModeAccumulation { enum E
{
    NoReset,   // В этом режиме показываются строго N последних измерений.
    Reset      // В этом режиме набираются N последних измерений и потом сбрасываются.
}; };

// Режим отображения дополнительных боковых маркеров смещения по напряжению.
struct AltMarkers { enum E
{
    Hide,        // Никода не выводить.
    Show,        // Всегда выводить.
    AutoHide     // Выводить и прятать через timeShowLevels.
}; };

// Через какое время после последнего нажатия кнопки скрывать меню.
struct MenuAutoHide { enum E
{
    None,   // Никогда.
    _5,     // Через 5 секунд.
    _10,    // Через 10 секунд.
    _15,    // Через 15 секунд.
    _30,    // Через 30 секунд.
    _60     // Через 60 секунд.
    };

    // Возвращает время, через которое меню автоматически скрывается, если не было больше нажатий.
    static int Time();
 };

// Тип привязки к смещению по вертикали
struct LinkingRShift { enum E
{
    Voltage,      // Смещение привязано к напряжению
    Position      // Смещение привязаоно к позиции на экране
}; };



// Настройки изображения
struct SettingsDisplay
{
    int16               timeShowLevels;             // Время, в течение которого нужно показывать уровни смещения.
    int16               shiftInMemory;              // Показывает смещение левого края сетки относительно нулевого байта памяти. Нужно для правильного отображения сигнала в окне.
    int16               timeMessages;               // Время в секундах, на которое сообщение остаётся на экране.
    int16               brightness;                 // Яркость дисплея.
    int16               brightnessGrid;             // Яркость сетки от 0 до 100.
    uint16              colors[16];                 // Цвета.
    ModeDrawSignal::E   modeDrawSignal;             // Режим отрисовки сигнала.
    TypeGrid::E         typeGrid;                   // Тип сетки.
    ENumAccumulation::E enumAccumulation;           // Перечисление накоплений сигнала на экране.
    ENumAveraging::E    enumAve;                    // Перечисление усреднений сигнала.
    ModeAveraging::E    modeAve;                    // Тип усреднений по измерениям.
    ENumMinMax::E       enumMinMax;                 // Число измерений для определения минимумов и максимумов.
    Smoothing::E        smoothing;                  // Число точек для скользящего фильтра.
    ENumSignalsInSec::E enumSignalsInSec;           // Перечисление считываний сигнала в секунду.
    Channel::E          lastAffectedChannel;        // Здесь хранится номер последнего канала, которым управляли ручками. Нужно для того, чтобы знать, какой сигнал рисовать наверху.
    ModeAccumulation::E modeAccumulation;           // Задаёт режим накопления сигналов.
    AltMarkers::E       altMarkers;                 // Режим отображения дополнительных боковых маркеров смещений.
    MenuAutoHide::E     menuAutoHide;               // Через сколько времени после последнего нажатия клавиши прятать меню.
    bool                showFullMemoryWindow;       // Показывать ли окно памяти вверху экрана. \todo Не используется.
    bool                showStringNavigation;       // Показывать ли строку текущего состояния меню..
    LinkingRShift::E    linkingRShift;              // Тип привязки смещения по вертикали.

    // Возвращает адрес первой и последней точки на экране.
    static void PointsOnDisplay(int *firstPoint, int *lastPoint);

    // Если экран разделён на две части и основной сигнал выводится сверху - например, в режиме вывода спектра
    static bool IsSeparate();
};


/** @}  @}
 */
