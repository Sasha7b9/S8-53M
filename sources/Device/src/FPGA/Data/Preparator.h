// 2021/03/25 9:02:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// —труктура дл€ хранени€ данных дл€ отрисовки
struct DataDrawingKeeper
{
    DataDrawingKeeper() : data(nullptr) {}
    ~DataDrawingKeeper();
    void Prepare();
    DataDrawing *data;
};


class Preparator
{
public:

    static void PrepareForRead();
};
