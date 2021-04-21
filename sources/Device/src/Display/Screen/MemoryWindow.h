// 2021/03/30 15:51:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct DataReading;
struct Channel;


class MemoryWindow
{
public:

    static void Draw(DataReading &data);

private:

    static void DrawDataInRectangle(int width, const Channel &ch, DataReading &data);

    static void DrawTPos(int left_x, int right_x);

    static void DrawTShift(int left_x, int right_x);

    // ���������� ����� � �������� ����������. ���� ����� �� ������� (Value::NONE), ���������� -1.
    static int Ordinate(uint8 x, float scale);

    static void DrawDataInRectangle(const Channel &ch, int x, int *min, int *max, int width);

    static void Calculate(Buffer<uint8> &buffer_min, Buffer<uint8> &buffer_max, uint bytes_in_channel, uint8 *data);
};
