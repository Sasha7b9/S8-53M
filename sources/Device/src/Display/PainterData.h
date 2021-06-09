// 2021/03/17 13:26:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Colors_.h"
#include "common/Settings/SettingsTypes_.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/Data/Storage.h"


struct DataSettings;
struct DataReading;


// ������� ��������� ��� ��������� ������ - ������������������ � ������������
struct DataDrawing
{
friend class PainterData;

    void Prepare();
    void Draw();

    DataReading data;

private:

    Buffer<uint8> points[Channel::Count]; // ����� ������������ �����, ������� � ������ �� �����

    void PrepareChannel(const Channel &ch);

    void DrawChannel(const Channel &ch);
    void DrawChannelPointed(int x, Buffer<uint8> &buffer);
    void DrawChannelLined(int x, Buffer<uint8> &buffer);
};


class PainterData
{
public:

    static void DrawData();
};



/*
    1. ������ ���������:
        - ���������� �����;
        - ���������� � �������������;
        - ���������� � ����������;
        - ����������;
        - ����� ���/����.

    2. ��� �����������:

        2.1 ���������� ������ ����� (�. 3).
        2.2 ���������� ������ ����� (�. 3).

    3. ��������� ������:

        3.1 ���� ����� �� ��������� � ��������� - �����.
        3.2 ����������� ������ ��� ��������� (�. 4).
        3.3 ���������� (�. 5).

    4. ���������� ������ ��� ���������:

        4.1 ����� ������� ������ � ��������� �������������� �����.
        4.2 ��������� ������ ������������ ������� ����������.
        4.3 ����������� �������� � �������� ����������.

    5. ��������� ������

        5.1 ���� ��������� ������� - ���� ������� ������ �����.
        5.2 ���� ��������� ������� - ���������� ������ ����� � ������� �����.

*/
