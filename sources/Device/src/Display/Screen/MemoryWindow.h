// 2021/03/30 15:51:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct DataReading;
struct Channel;


class MemoryWindow
{
public:

    static void Draw(DataReading &data);

private:

    static void DrawDataInRect(int width, const Channel &ch, DataReading &data);

    static void DrawTPos(int left_x, int right_x);

    static void DrawTShift(int left_x, int right_x, uint num_points);

    // Возвращает точку в экранной координате. Если точка не считана (Value::NONE), возвращает -1.
    static int Ordinate(uint8 x, float scale);

    // \node Процедура ограничивает width числом 255
    static void SendToDisplayDataInRect(const Channel &ch, int x, int *min, int *max, int width);
};
