// 2021/03/30 15:51:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Utils/Math_.h"
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

    DrawDataInRect(right + 2, set.display.last_affected_channel.IsA() ? ChB : ChA, data);
    DrawDataInRect(right + 2, set.display.last_affected_channel.IsA() ? ChA : ChB, data);

    int leftX = 3;
    float scaleX = (float)(right - leftX + 1) / set.memory.enum_points_fpga.ToPoints();
    const int xVert0 = leftX + (int)(set.display.shift_in_memory * scaleX);
    int width = (int)((right - leftX) * (282.0f / set.memory.enum_points_fpga.ToPoints()));

    Rectangle(width - (set.memory.enum_points_fpga == EnumPointsFPGA::_8k ? 1 : 0), Grid::TOP - 2).
        Draw(xVert0, 0, Color::FILL);

    DrawTPos(leftX, right);

    DrawTShift(leftX, right, set.memory.enum_points_fpga.BytesInChannel());
}


void MemoryWindow::DrawDataInRect(int width, const Channel &ch, DataReading &_data)
{
    DataSettings &ds = _data.Settings();

    if (!ds.IsEnabled(ch))
    {
        return;
    }

    uint8 *data = _data.Data(ch);

    float elems_in_column = ds.BytesInChannel() / (float)width;

#undef SIZE_BUFFER
#define SIZE_BUFFER 300

    uint8 min[SIZE_BUFFER];
    uint8 max[SIZE_BUFFER];

    if (ds.peak_det != 0)                                 // Если пик. дет. включен
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
    else                                                                // Если пик. дет. выключён
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

    int mines[SIZE_BUFFER];     // Массив для максимальных значений в каждом столбике
    int maxes[SIZE_BUFFER];     // Массив для минимальных значений в каждом столбике

    float scale = 17.0f / (Value::MAX - Value::MIN);

    mines[0] = Ordinate(max[0], scale);
    maxes[0] = Ordinate(min[0], scale);

    for (int i = 1; i < width; i++)
    {
        maxes[i] = Ordinate((uint8)((max[i] < min[i - 1]) ? min[i - 1] : max[i]), scale);
        mines[i] = Ordinate((uint8)((min[i] > max[i - 1]) ? max[i - 1] : min[i]), scale);
    }

    //*** Теперь определим количество точек, которые нужно нарисовать
    int numPoints = 0;
    for (int i = 0; i < width; ++i)
    {
        if (maxes[i] != -1 && mines[i] != -1) { numPoints++; }
    }

    numPoints--;

    //*** Теперь определим, с какой позиции выводить точки (если сигнал сжат по горизонтали, то вначале будет пустое место
    int x = 1;
    for (int i = 0; i < width; ++i)
    {
        if (maxes[i] == -1 && mines[i] == -1) { ++x; }
        else { break; }
    }

    if (numPoints > 1)
    {
        int delta = x;
        if (numPoints < 256)
        {
            SendToDisplayDataInRect(ch, x, mines + delta, maxes + delta, numPoints);
        }
        else
        {
            SendToDisplayDataInRect(ch, x, mines + delta, maxes + delta, 255);
            numPoints -= 255;
            SendToDisplayDataInRect(ch, x + 255, mines + 255 + delta, maxes + 255 + delta, numPoints);
        }
    }
}


void MemoryWindow::DrawTPos(int leftX, int rightX)
{
    int x[] = { leftX, (rightX - leftX) / 2 + leftX, rightX };
    int x0 = x[set.time.t_pos];

    Region(6, 6).Fill(x0 - 3, 10, Color::BACK);

    Char(Symbol::S8::TPOS_1).Draw(x0 - 3, 10, Color::FILL);
}


void MemoryWindow::DrawTShift(int left, int right, uint numBytes)
{
    float scale = (float)(right - left + 1) / ((float)numBytes - (numBytes == 281 ? 1 : 0));

    int x = (int)(1.5f + (TPos::InPoints(set.time.peak_det, set.memory.enum_points_fpga.ToPoints(), set.time.t_pos) -
        TShift::InPoints(set.time.peak_det)) * scale) - 1;

    if (set.time.peak_det.IsEnabled() && set.time.t_pos.IsRight())
    {
        --x;
    }

    if (set.memory.enum_points_fpga == EnumPointsFPGA::_512)
    {
        ++x;                           /// \todo Костыль
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


void MemoryWindow::SendToDisplayDataInRect(const Channel &ch, int x, int *min, int *max, int width)
{
    Math::LimitAbove(&width, 255);

#undef SIZE_BUFFER
#define SIZE_BUFFER (255 * 2)

    uint8 points[SIZE_BUFFER];

    for (int i = 0; i < width; i++)
    {
        points[i * 2] = (uint8)max[i];
        points[i * 2 + 1] = (uint8)(min[i] < 0 ? 0 : min[i]);
    }

    VLineArray().Draw(x, (int)width, points, ch.GetColor());
}
