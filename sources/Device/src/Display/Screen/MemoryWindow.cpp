// 2021/03/30 15:51:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Primitives_.h"
#include "Display/Screen/Grid.h"
#include "Display/Screen/MemoryWindow.h"
#include "Settings/Settings.h"


void MemoryWindow::Draw(DataReading &data)
{
    static const int rightXses[3] = { 276, 285, 247 };
    int rightX = rightXses[set.memory.mode_work];
    if (Cursors::NecessaryDraw())
    {
        rightX = 68;
    }

    DrawDataInRect(rightX + 2, set.display.last_affected_channel.IsA() ? ChB : ChA, data);
    DrawDataInRect(rightX + 2, set.display.last_affected_channel.IsA() ? ChA : ChB, data);

    int leftX = 3;
    float scaleX = (float)(rightX - leftX + 1) / set.memory.enum_points_fpga.ToPoints();
    const int xVert0 = leftX + (int)(set.display.shift_in_memory * scaleX);
    int width = (int)((rightX - leftX) * (282.0f / set.memory.enum_points_fpga.ToPoints()));

    Rectangle(width - (set.memory.enum_points_fpga == ENUM_POINTS_FPGA::_8k ? 1 : 0), Grid::TOP - 2).
        Draw(xVert0, 0, Color::FILL);

    DrawTPos(leftX, rightX);

    DrawTShift(leftX, rightX, set.memory.enum_points_fpga.BytesInChannel());
}


void MemoryWindow::DrawDataInRect(int width, const Channel &ch, DataReading &data)
{
    if (!dataStruct->needDraw[ch])
    {
        return;
    }

    uint8 *data = OUT(ch);

    float elemsInColumn = NUM_BYTES_SET / (float)width;

#undef SIZE_BUFFER
#define SIZE_BUFFER 300
    uint8 min[SIZE_BUFFER];
    uint8 max[SIZE_BUFFER];

    if (PEAKDET_DS != PeakDet_Disabled)                                 // Если пик. дет. включен
    {
        uint8 *iMin = &min[0];
        uint8 *iMax = &max[0];

        for (int col = 0; col < width; col++, iMin++, iMax++)
        {
            uint firstElem = (uint)(col * elemsInColumn);
            uint lastElem = (uint)(firstElem + elemsInColumn - 1);
            *iMin = data[firstElem];
            *iMax = data[firstElem];
            for (uint elem = firstElem + 1; elem <= lastElem; elem++)
            {
                SET_MIN_IF_LESS(data[elem], *iMin);
                SET_MAX_IF_LARGER(data[elem], *iMax);
            }
        }
    }
    else                                                                // Если пик. дет. выключён
    {
        for (int col = 0; col < width; col++)
        {
            uint firstElem = (uint)(col * elemsInColumn);
            uint lastElem = (uint)(firstElem + elemsInColumn - 1);
            min[col] = data[firstElem];
            max[col] = data[firstElem];
            for (uint elem = firstElem + 1; elem <= lastElem; elem++)
            {
                SET_MIN_IF_LESS(data[elem], min[col]);
                SET_MAX_IF_LARGER(data[elem], max[col]);
            }
        }
    }

    int mines[SIZE_BUFFER];     // Массив для максимальных значений в каждом столбике
    int maxes[SIZE_BUFFER];     // Массив для минимальных значений в каждом столбике

    float scale = 17.0f / (MAX_VALUE - MIN_VALUE);

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
    int x0 = x[TPOS];
    Painter::FillRegion(x0 - 3, 10, 6, 6, Color::BACK);
    Painter::DrawChar(x0 - 3, 10, SYMBOL_TPOS_1, Color::FILL);
}


void MemoryWindow::DrawTShift(int leftX, int rightX, int numBytes)
{
    float scale = (float)(rightX - leftX + 1) / ((float)numBytes - (numBytes == 281 ? 1 : 0));
    int xShift = (int)(1.5f + (TPOS_IN_BYTES - TSHIFT_IN_POINTS) * scale) - 1;
    if (SET_PEAKDET_EN && TPOS_IS_RIGHT)
    {
        --xShift;
    }
    if (FPGA_POINTS_512)
    {
        ++xShift;                           /// \todo Костыль
    }
    LIMIT_ABOVE(xShift, rightX - 2);

    int dX01 = 1, dX02 = 2, dX11 = 3, dY11 = 7, dY12 = 6;

    if (xShift < leftX - 2)
    {
        xShift = leftX - 2;
        dX01 = 3; dX02 = 1; dY12 = 6;
    }
    else if (xShift > rightX - 1)
    {
        xShift = rightX - 2;
        dX11 = 1;
    }
    else
    {
        dY11 = 5; dY12 = 7;
    }

    Painter::FillRegion((int)xShift - 1, 1, 6, 6, Color::BACK);
    Painter::FillRegion((int)xShift, 2, 4, 4, Color::FILL);
    Painter::DrawLine((int)xShift + dX01, 3, (int)xShift + dX11, dY11 - 2, Color::BACK);
    Painter::DrawLine((int)xShift + dX02, 4, (int)xShift + 2, dY12 - 2);
}
