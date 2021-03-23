// 2021/03/17 13:26:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Colors_.h"
#include "common/Settings/SettingsTypes_.h"
#include "FPGA/FPGA_Types.h"


struct DataSettings;
struct DataStorage;


// ������� ��������� ��� ��������� ������ - ������������������ � ������������
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

    // ���������� ������ �������
    // start_x    - ���������� x ������ �����
    // T ����� ���� uint8 - ���� ������ ��������� �� ���������� ���, ��� uint16 - ���� ������ ��������� �� ������� ���
    static void DrawPoints(uint8 *y, const int start_x, int num_points, const Color &color = Color::Count);
    // ���������� DrawPoints(), ������ � y ���� ������� ���������� ���� � �����
    static void DrawLines(uint8 *y, const int start_x, int num_lines, const Color &color = Color::Count);
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
