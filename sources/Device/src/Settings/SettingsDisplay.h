#pragma once
#include "common/Display/Colors_.h"


// Настройки изображения
struct SettingsDisplay
{
    int16               time_show_levels;       // Время, в течение которого нужно показывать уровни смещения.
    int16               shift_in_memory;        // Показывает смещение левого края сетки относительно нулевого байта
                                                // памяти. Нужно для правильного отображения сигнала в окне.
    int16               time_messages;          // Время в секундах, на которое сообщение остаётся на экране.
    int16               brightness;             // Яркость дисплея.
    int16               brightness_grid;        // Яркость сетки от 0 до 100.
    uint                colors[16];             // Цвета.
    ModeDrawSignal::E   mode_draw_signal;       // Режим отрисовки сигнала.
    TypeGrid::E         typeGrid;               // Тип сетки.
    ENumAccumulation::E enum_accum;             // Перечисление накоплений сигнала на экране.
    ENumAveraging::E    enum_ave;               // Перечисление усреднений сигнала.
    ModeAveraging::E    mode_ave;               // Тип усреднений по измерениям.
    ENumMinMax::E       enum_min_max;           // Число измерений для определения минимумов и максимумов.
    Smoothing::E        smoothing;              // Число точек для скользящего фильтра.
    ENumSignalsInSec::E enum_fps;               // Перечисление считываний сигнала в секунду.
    Channel::E          last_affected_channel;  // Здесь хранится номер последнего канала, которым управляли ручками.
                                                // Нужно для того, чтобы знать, какой сигнал рисовать наверху.
    ModeAccumulation::E mode_acum;              // Задаёт режим накопления сигналов.
    AltMarkers::E       alt_markers;            // Режим отображения дополнительных боковых маркеров смещений.
    MenuAutoHide::E     menu_auto_hide;         // Через сколько времени после последнего нажатия клавиши прятать меню.
    bool                showFullMemoryWindow;   // Показывать ли окно памяти вверху экрана. \todo Не используется.
    bool                show_string_navigation; // Показывать ли строку текущего состояния меню..
    LinkingRShift::E    linking_rshift;         // Тип привязки смещения по вертикали.

    // Возвращает адрес первой и последней точки на экране.
    static void PointsOnDisplay(int *firstPoint, int *lastPoint);

    // Если экран разделён на две части и основной сигнал выводится сверху - например, в режиме вывода спектра
    static bool IsSeparate();
};
