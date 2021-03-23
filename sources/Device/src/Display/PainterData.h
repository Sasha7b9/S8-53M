// 2021/03/17 13:26:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Colors_.h"
#include "common/Settings/SettingsTypes_.h"
#include "FPGA/FPGA_Types.h"


struct DataSettings;
struct DataStorage;


// Базовая структура для отрисовки данных - осциллографических и спектральных
struct DataDrawing
{

};


class PainterData
{
public:

    static void DrawData();

private:

    static void DrawDataChannel(DataStorage &data, const Channel &ch, int min_y, int max_y);

    static void DrawSignalLined(puchar data, const DataSettings *ds, int start_i, int end_i, int min_y,
        int max_y, float scale_y, float scale_x, bool calculate_filtr);

    static void DrawSignalPointed(puchar data, DataSettings &ds, int start_i, int end_i, int min_y,
        int max_y, float scale_y, float scale_x);

    static void DrawDataInRect(int x, int width, puchar data, int numElems, const Channel &ch, int shiftForPeakDet);

    // Нарисовать сигнал точками
    // start_x    - координата x первой точки
    // T может быть uint8 - если данные находятся во встроенном ОЗУ, или uint16 - если данные находятся во внешнем ОЗУ
    static void DrawPoints(uint8 *y, const int start_x, int num_points, const Color &color = Color::Count);
    // Аналогично DrawPoints(), только в y идут попарно координаты низа и верха
    static void DrawLines(uint8 *y, const int start_x, int num_lines, const Color &color = Color::Count);
};



/*

    1. Режимы отрисовки:
        - нормальный режим;
        - нормальный в рандомизаторе;
        - нормальный в поточечном;
        - математика;
        - режим мин/макс.

    2. Для реаализации:

        2.1 Нарисовать первый канал (п. 3).
        2.2 Нарисовать второй канал (п. 3).

    3. Отрисовка канала:

        3.1 Если канал не нуждается в отрисовке - выход.
        3.2 Подготовить данные для отрисовки (п. 4).
        3.3 Отрисовать (п. 5).

    4. Подготовка данных для отрисовки:

        4.1 Найти индексы первой и последней отрисовываемой точки.
        4.2 Заполнить массив необходимого размера значениями.
        4.3 Пересчитать значения в экранные координаты.

    5. Отрисовка данных

        5.1 Если отрисовка точками - тупо вывести массив точек.
        5.2 Если отрисовка линиями - рассчитать массив линий и вывести линии.

*/
