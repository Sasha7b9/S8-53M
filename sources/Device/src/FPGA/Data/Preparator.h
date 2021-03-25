// 2021/03/25 9:02:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// ��������� ��� �������� ������ ��� ���������
struct DataDrawingKeeper
{
    DataDrawingKeeper();
    ~DataDrawingKeeper();
    DataDrawing *data;
private:
    void Prepare();
};


// ��������� ��� �������� ������� ����������� ������
struct DataReadingKeeper
{
    DataReadingKeeper();
    ~DataReadingKeeper();
    DataReading *data;
private:
    void Prepare();
};
