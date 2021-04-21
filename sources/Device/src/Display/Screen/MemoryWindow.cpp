// 2021/03/30 15:51:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Utils/Math_.h"
#include "common/Utils/Containers/Buffer_.h"
#include "Display/Screen/Grid.h"
#include "Display/Screen/MemoryWindow.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/Data/DataSettings.h"
#include "FPGA/Data/Storage.h"
#include "Settings/Settings.h"


void MemoryWindow::Draw(DataReading &data)
{
    static const int rightXses[3] = { 276, 285, 247 };

    int right = rightXses[set.memory.mode_work];

    if (Cursors::NecessaryDraw())
    {
        right = 68;
    }

    DrawDataInRectangle(right + 2, set.display.last_affected_channel.IsA() ? ChB : ChA, data);
    DrawDataInRectangle(right + 2, set.display.last_affected_channel.IsA() ? ChA : ChB, data);

    int left = 3;

    float scale_x = (float)(right - left + 1) / set.memory.enum_points_fpga.PointsInChannel();
    const int x_rect = left + (int)(set.display.shift_in_memory_in_points * scale_x);

    int width = (int)((right - left) * (282.0f / set.memory.enum_points_fpga.PointsInChannel()));

    Rectangle(width, Grid::TOP - 2).Draw(x_rect, 0, Color::FILL);

    DrawTPos(left, right);

    DrawTShift(left, right);
}


void MemoryWindow::DrawDataInRectangle(int width, const Channel &ch, DataReading &data_channel)
{
    DataSettings &ds = data_channel.Settings();

    if (!ds.IsEnabled(ch))
    {
        return;
    }

    uint8 *data = data_channel.Data(ch);

    float elems_in_column = ds.BytesInChannel() / (float)width;

    const int SIZE_BUFFER = 300;
    uint8 min[SIZE_BUFFER];
    uint8 max[SIZE_BUFFER];

    if (ds.peak_det != 0)                                 // ���� ���. ���. �������
    {
        uint8 *mini = &min[0];
        uint8 *maxi = &max[0];

        for (int col = 0; col < width; col++, mini++, maxi++)
        {
            uint first = (uint)(col * elems_in_column);
            uint last = (uint)(first + elems_in_column - 1);
            *mini = data[first];
            *maxi = data[first];

            for (uint elem = first + 1; elem <= last; elem++)
            {
                uint8 d = data[elem];
                if (d < *mini) { *mini = d; }
                if (d > *maxi) { *maxi = d; }
            }
        }
    }
    else                                                                // ���� ���. ���. ��������
    {
        for (int col = 0; col < width; col++)
        {
            uint first = (uint)(col * elems_in_column);
            uint last = (uint)(first + elems_in_column - 1);
            min[col] = data[first];
            max[col] = data[first];

            for (uint elem = first + 1; elem <= last; elem++)
            {
                uint8 d = data[elem];
                if (d < min[col]) { min[col] = d; }
                if (d > max[col]) { max[col] = d; }
            }
        }
    }

    int mines[SIZE_BUFFER];     // ������ ��� ������������ �������� � ������ ��������
    int maxes[SIZE_BUFFER];     // ������ ��� ����������� �������� � ������ ��������

    float scale = 17.0f / (Value::MAX - Value::MIN);

    mines[0] = Ordinate(max[0], scale);
    maxes[0] = Ordinate(min[0], scale);

    for (int i = 1; i < width; i++)
    {
        maxes[i] = Ordinate((uint8)((max[i] < min[i - 1]) ? min[i - 1] : max[i]), scale);
        mines[i] = Ordinate((uint8)((min[i] > max[i - 1]) ? max[i - 1] : min[i]), scale);
    }

    // ������ ��������� ���������� �����, ������� ����� ����������
    int numPoints = 0;
    for (int i = 0; i < width; ++i)
    {
        if (maxes[i] != -1 && mines[i] != -1) { numPoints++; }
    }

    numPoints--;

    // ������ ���������, � ����� ������� �������� ����� (���� ������ ���� �� �����������, �� ������� ����� ������ �����
    int x = 1;
    for (int i = 0; i < width; ++i)
    {
        if (maxes[i] == -1 && mines[i] == -1) { ++x; }
        else { break; }
    }

    if (numPoints > 1)
    {
        DrawDataInRectangle(ch, x, mines + x, maxes + x, numPoints);
    }
}


void MemoryWindow::DrawTPos(int leftX, int rightX)
{
    int x[] = { leftX, (rightX - leftX) / 2 + leftX, rightX };
    int x0 = x[set.time.t_pos];

    TPos::Draw(x0, 10);
}


void MemoryWindow::DrawTShift(int left, int right)
{
    EnumPointsFPGA::E enum_points = set.memory.enum_points_fpga;
    uint num_points = set.memory.enum_points_fpga.PointsInChannel();

    float scale = (float)(right - left + 1) / num_points;

    int x = (int)(1.5f + (TPos::InPoints(enum_points, set.time.t_pos) - set.time.shift) * scale) - 1;

    if (set.time.peak_det.IsEnabled() && set.time.t_pos.IsRight())
    {
        --x;
    }

    if (enum_points == EnumPointsFPGA::_512)
    {
        ++x;                           /// \todo �������
    }

    Math::LimitAbove(&x, right - 2);

    int x_rect = Math::Limitation(x, 1, right);

    Rectangle(7, 6).Draw(x_rect - 1, 1, Color::BACK);
    Region(5, 5).Fill(x_rect, 1, Color::FILL);

    if (x < left - 3)
    {
        VLine(3).Draw(x_rect + 3, 2, Color::BACK);
        HLine(2).Draw(x_rect + 1, 3);
    }
    else if (x > right - 1)
    {
        VLine(3).Draw(x_rect + 1, 2, Color::BACK);
        HLine(2).Draw(x_rect + 2, 3);
    }
    else
    {
        HLine(3).Draw(x_rect + 1, 2, Color::BACK);
        VLine(2).Draw(x_rect + 2, 3);
    }
}


int MemoryWindow::Ordinate(uint8 x, float scale)
{
    if (x == Value::NONE)
    {
        return -1;
    }

    Math::Limitation<uint8>((uint8)(x - Value::MIN), 0, (Value::MAX - Value::MIN));

    return (int)((17.0f - scale * x) + 0.5f);
}


void MemoryWindow::DrawDataInRectangle(const Channel &ch, int x, int *min, int *max, int width)
{
    Buffer<uint8> buffer((uint)(width * 2));

    uint8 *points = buffer.DataU8();

    for (int i = 0; i < width; i++)
    {
        points[i * 2] = (uint8)max[i];
        points[i * 2 + 1] = (uint8)(min[i] < 0 ? 0 : min[i]);
    }

    VLineArray().Draw(x, (int)width, points, ch.GetColor());
}
