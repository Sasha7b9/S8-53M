#pragma once
#include "common/Display/Colors_.h"


#define SHIFT_IN_MEMORY             (set.display.shiftInMemory)                 // SettingsDisplay.shiftInMemory

#define TIME_MESSAGES               (set.display.timeMessages)                  // SettingsDisplay.timeMessages

#define BRIGHTNESS                  (set.display.brightness)                    // SettingsDisplay.brightness

#define BRIGHTNESS_GRID             (set.display.brightnessGrid)                // SettingsDisplay.brightnessGrid

#define DISPLAY_COLOR(num)          (set.display.colors[num])                   // SettingsDisplay.colors

#define MODE_DRAW_SIGNAL            (set.display.modeDrawSignal)                // SettingsDisplay.modeDrawSignal
#define MODE_DRAW_IS_SIGNAL_LINES   (MODE_DRAW_SIGNAL == ModeDrawSignal::Lines) // \c true, если сигнал выводится линиями.

#define ENUM_ACCUM_IS_NONE          (set.display.enum_accum == ENumAccumulation::_1)        // \c true, если накопление выключено.
#define ENUM_ACCUM_IS_INFINITY      (set.display.enum_accum == ENumAccumulation::Infinity)  // \c true, если установлено бесконечное число накоплений.
#define NUM_ACCUM                   (1 << (int)set.display.enumAccumulation)    // Количество накоплений.

#define ENUM_AVE                    (set.display.enumAve)                       // SettingsDisplay.enumAve
#define NUM_AVE                     (1 << ENUM_AVE)                             // Количество усреднений.

#define MODE_AVE                    (set.display.modeAve)                       // SettingsDisplay.modeAve

#define ENUM_MIN_MAX                (::set.display.enumMinMax)                  // SettingsDisplay.enumMinMax
#define NUM_MIN_MAX                 (1 << ENUM_MIN_MAX)                         // Количество измерений сигналов для расчёта минимумов и максимумов.

#define SMOOTHING                   (set.display.smoothing)                     // SettingsDisplay.smoothing
#define SMOOTHING_IS_DISABLE        (SMOOTHING == Smoothing::Disable)           // \c true, если скользящий фильтр выключен.

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
#define LINKING_RSHIFT_IS_VOLTAGE   (LINKING_RSHIFT == LinkingRShift::Voltage)  // \c true, если привязка к абсолютному напряжению.


// Настройки изображения
struct SettingsDisplay
{
    int16               time_show_levels;     // Время, в течение которого нужно показывать уровни смещения.
    int16               shiftInMemory;        // Показывает смещение левого края сетки относительно нулевого байта
                                              // памяти. Нужно для правильного отображения сигнала в окне.
    int16               timeMessages;         // Время в секундах, на которое сообщение остаётся на экране.
    int16               brightness;           // Яркость дисплея.
    int16               brightnessGrid;       // Яркость сетки от 0 до 100.
    uint                colors[16];           // Цвета.
    ModeDrawSignal::E   modeDrawSignal;       // Режим отрисовки сигнала.
    TypeGrid::E         typeGrid;             // Тип сетки.
    ENumAccumulation::E enum_accum;           // Перечисление накоплений сигнала на экране.
    ENumAveraging::E    enumAve;              // Перечисление усреднений сигнала.
    ModeAveraging::E    modeAve;              // Тип усреднений по измерениям.
    ENumMinMax::E       enumMinMax;           // Число измерений для определения минимумов и максимумов.
    Smoothing::E        smoothing;            // Число точек для скользящего фильтра.
    ENumSignalsInSec::E enumSignalsInSec;     // Перечисление считываний сигнала в секунду.
    Channel::E          lastAffectedChannel;  // Здесь хранится номер последнего канала, которым управляли ручками.
                                              // Нужно для того, чтобы знать, какой сигнал рисовать наверху.
    ModeAccumulation::E modeAccumulation;     // Задаёт режим накопления сигналов.
    AltMarkers::E       altMarkers;           // Режим отображения дополнительных боковых маркеров смещений.
    MenuAutoHide::E     menuAutoHide;         // Через сколько времени после последнего нажатия клавиши прятать меню.
    bool                showFullMemoryWindow; // Показывать ли окно памяти вверху экрана. \todo Не используется.
    bool                showStringNavigation; // Показывать ли строку текущего состояния меню..
    LinkingRShift::E    linkingRShift;        // Тип привязки смещения по вертикали.

    // Возвращает адрес первой и последней точки на экране.
    static void PointsOnDisplay(int *firstPoint, int *lastPoint);

    // Если экран разделён на две части и основной сигнал выводится сверху - например, в режиме вывода спектра
    static bool IsSeparate();
};
