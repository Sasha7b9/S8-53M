// 2021/03/17 13:26:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Colors_.h"
#include "common/Settings/SettingsTypes_.h"
#include "common/Utils/Buffer_.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/Storage.h"


struct DataSettings;
struct DataStorage;


// Базовая структура для отрисовки данных - осциллографических и спектральных
struct DataDrawing
{
    void Prepare();
    void Draw();

private:

    DataStorage data;
    Buffer points[NumChannels]; // Здесь рассчитанные точки, готовые к выводу на экран

    void PrepareChannel(const Channel &ch);

    void DrawChannel(const Channel &ch);
    void DrawChannelPointed(int x, Buffer &buffer);
    void DrawChannelLined(int x, Buffer &buffer);
};


class PainterData
{
public:

    static void DrawData();
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
