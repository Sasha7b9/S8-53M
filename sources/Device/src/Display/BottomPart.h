// 2021/03/24 15:17:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Channel;


struct BottomPart
{
    // �������� ���������� ��� ������ - � ������ ����� �������.
    static void Draw();

private:

    static void WriteTextVoltage(const Channel &ch, int x, int y);

    static void WriteStringAndNumber(pchar text, int x, int y, int number);

    // ������� ������� �����.
    static void DrawTime(int x, int y);
};
