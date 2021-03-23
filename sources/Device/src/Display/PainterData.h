// 2021/03/17 13:26:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Colors_.h"
#include "common/Settings/SettingsTypes_.h"
#include "common/Utils/Buffer_.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/Storage.h"


struct DataSettings;
struct DataStorage;


// ������� ��������� ��� ��������� ������ - ������������������ � ������������
struct DataDrawing
{
    void Prepare();
    void Draw();

private:

    DataStorage data;
    Buffer points[NumChannels]; // ����� ������������ �����, ������� � ������ �� �����

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
