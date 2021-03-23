// 2021/03/17 13:27:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Log_.h"
#include "common/Display/Painter_.h"
#include "common/Display/Primitives_.h"
#include "common/Display/Text_.h"
#include "common/Utils/GlobalFunctions_.h"
#include "common/Utils/Math_.h"
#include "Display/Grid.h"
#include "Display/PainterData.h"
#include "FPGA/FPGA_Reader.h"
#include "FPGA/MathFPGA.h"
#include "FPGA/Storage.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Utils/ProcessingSignal.h"
#include <cstring>


using namespace Primitives;


void PainterData::DrawData()
{
    DataDrawing data;

    data.Prepare();

    data.Draw();

    Primitives::Rectangle(Grid::Width(), Grid::FullHeight()).Draw(Grid::Left(), Grid::TOP, Color::FILL);
}


#define CONVERT_DATA_TO_DISPLAY(out, in)                    \
    out = (uint8)(max_y - ((in) - MIN_VALUE) * scale_y);    \
    if(out < min_y)          { out = (uint8)min_y; }        \
    else if (out > max_y)    { out = (uint8)max_y; };


// shiftForPeakDet - если рисуем информацию с пикового детектора - то через shiftForPeakDet точек расположена
// иниформация о максимумах.
void PainterData::DrawDataInRect(int x, int width, puchar data, int numElems, const Channel &ch, int shiftForPeakDet)
{
    if (numElems == 0)
    {
        return;
    }

    width--;
    float elemsInColumn = (float)numElems / (float)width;
    uint8 min[300];
    uint8 max[300];


    if ((TBase::Get() >= TBase::_20ms) && PeackDetMode::IsEnabled())
    {
        for (int col = 0; col < width; col++)
        {
            float firstElem = col * elemsInColumn;
            float lastElem = firstElem + elemsInColumn - 1;
            min[col] = data[(int)firstElem];
            max[col] = data[(int)firstElem + 1];
            for (int elem = (int)(firstElem) + 2; elem <= lastElem; elem += 2)
            {
                SET_MIN_IF_LESS(data[elem], min[col]);
                SET_MAX_IF_LARGER(data[elem + 1], max[col]);
            }
        }
    }
    else if (shiftForPeakDet == 0)
    {
        uint8 *iMin = &min[0];
        uint8 *iMax = &max[0];

        for (int col = 0; col < width; col++, iMin++, iMax++)
        {
            int firstElem = (int)(col * elemsInColumn);
            int lastElem = firstElem + (int)(elemsInColumn) - 1;
            *iMin = data[firstElem];
            *iMax = data[firstElem];
            for (int elem = firstElem + 1; elem <= lastElem; elem++)
            {
                SET_MIN_IF_LESS(data[elem], *iMin);
                SET_MAX_IF_LARGER(data[elem], *iMax);
            }
        }
    }
    else        // А здесь будет, если пик. дет. включен
    {
        for (int col = 0; col < width; col++)
        {
            float firstElem = col * elemsInColumn;
            float lastElem = firstElem + elemsInColumn - 1;
            min[col] = data[(int)firstElem];
            max[col] = data[(int)firstElem + shiftForPeakDet];
            for (int elem = (int)(firstElem) + 1; elem <= lastElem; elem++)
            {
                SET_MIN_IF_LESS(data[elem], min[col]);
                SET_MAX_IF_LARGER(data[elem + shiftForPeakDet], max[col]);
            }
        }
    }

    int bottom = 16;
    int height = 14;
    float scale = (float)height / (float)(MAX_VALUE - MIN_VALUE);

#define ORDINATE(x) bottom - scale * Math::Limitation<int>((x) - MIN_VALUE, 0, 200)

#define NUM_POINTS (300 * 2)
    uint8 points[NUM_POINTS];

    points[0] = (uint8)(ORDINATE(max[0]));
    points[1] = (uint8)(ORDINATE(min[0]));

    for (int i = 1; i < width; i++)
    {
        int value0 = min[i] > max[i - 1] ? max[i - 1] : min[i];
        int value1 = max[i] < min[i - 1] ? min[i - 1] : max[i];
        points[i * 2] = (uint8)(ORDINATE(value1));
        points[i * 2 + 1] = (uint8)(ORDINATE(value0));
    }
    if (width < 256)
    {
        VLineArray().Draw(x, width, points, ch.GetColor());
    }
    else
    {
        VLineArray().Draw(x, 255, points, ch.GetColor());
        VLineArray().Draw(x + 255, width - 255, points + 255 * 2, ch.GetColor());
    }
}


void DataDrawing::Prepare()
{
    Storage::ExtractLast(data);
}


void DataDrawing::Draw()
{
    DrawDataChannel(data, Channel::A, Grid::TOP, Grid::ChannelBottom());
    DrawDataChannel(data, Channel::B, Grid::TOP, Grid::ChannelBottom());
}


void DataDrawing::DrawDataChannel(DataStorage &data, const Channel &ch, int min_y, int max_y)
{
    if (!data.Settings().IsEnabled(ch))
    {
        return;
    }

    float scaleY = static_cast<float>(max_y - min_y) / (MAX_VALUE - MIN_VALUE);
    float scaleX = Grid::Width() / 280.0F;

    int firstPoint = 0;
    int lastPoint = 0;

    SettingsDisplay::PointsOnDisplay(&firstPoint, &lastPoint);

    ch.GetColor().SetAsCurrent();

    DrawSignalPointed(data.Data(ch), data.Settings(), firstPoint, lastPoint, min_y, max_y, scaleY, scaleX);
}


void DataDrawing::DrawSignalPointed(puchar data, DataSettings &ds, int start_i, int end_i, int min_y,
    int max_y, float scale_y, float scale_x)
{
    int numPoints = SettingsMemory::GetNumPoints(false);
    uint numSmoothing = Smoothing::NumPoints();

    uint8 dataCD[281];

    if (scale_x == 1.0F)
    {
        for (int i = start_i; i < end_i; i++)
        {
            int index = i - start_i;
            CONVERT_DATA_TO_DISPLAY(dataCD[index], Math::CalculateFiltr(data, i, numPoints, (int)numSmoothing));
        }

        DrawPoints(dataCD, Grid::Left(), 281);

        if (ds.IsEnabledPeakDet())
        {
            int size = end_i - start_i;
            start_i += numPoints;
            end_i = start_i + size;;

            for (int i = start_i; i < end_i; i++)
            {
                int index = i - start_i;
                CONVERT_DATA_TO_DISPLAY(dataCD[index], Math::CalculateFiltr(data, i, numPoints, (int)numSmoothing));
            }

            DrawPoints(dataCD, Grid::Left(), 281);
        }
    }
    else
    {
        for (int i = start_i; i < end_i; i++)
        {
            int index = i - start_i;
            int dat = 0;
            CONVERT_DATA_TO_DISPLAY(dat, Math::CalculateFiltr(data, i, numPoints, (int)numSmoothing));
            Point().Draw(Grid::Left() + (int)(index * scale_x), dat);
        }
    }
}


void DataDrawing::DrawSignalLined(puchar data, const DataSettings *ds, int start_i, int end_i, int min_y,
    int max_y, float scale_y, float scale_x, bool calculate_filtr)
{
    if (end_i < start_i)
    {
        return;
    }
    uint8 dataCD[281];

    int gridLeft = Grid::Left();
    int gridRight = Grid::Right();

    int numPoints = SettingsMemory::GetNumPoints(false);
    uint numSmoothing = Smoothing::NumPoints();

    if (ds->peakDet == PeackDetMode::Disable)
    {
        for (int i = start_i; i < end_i; i++)
        {
            float x0 = gridLeft + (i - start_i) * scale_x;
            if (x0 >= gridLeft && x0 <= gridRight)
            {
                int index = i - start_i;
                int y = calculate_filtr ? Math::CalculateFiltr(data, i, numPoints, (int)numSmoothing) : data[i];
                CONVERT_DATA_TO_DISPLAY(dataCD[index], y);
            }
        }
    }
    else
    {
        int shift = (int)(ds->BytesInChannel());

        int yMinNext = -1;
        int yMaxNext = -1;

        for (int i = start_i; i < end_i; i++)
        {
            float x = gridLeft + (i - start_i) * scale_x;
            if (x >= gridLeft && x <= gridRight)
            {
                int yMin = yMinNext;
                if (yMin == -1)
                {
                    CONVERT_DATA_TO_DISPLAY(yMin, data[i + shift]);
                }
                int yMax = yMaxNext;
                if (yMax == -1)
                {
                    CONVERT_DATA_TO_DISPLAY(yMax, data[i]);
                }

                CONVERT_DATA_TO_DISPLAY(yMaxNext, data[i + 1]);
                if (yMaxNext < yMin)
                {
                    yMin = yMaxNext + 1;
                }

                CONVERT_DATA_TO_DISPLAY(yMinNext, data[i + shift + 1]);
                if (yMinNext > yMax)
                {
                    yMax = yMinNext - 1;
                }

                HLine().Draw((int)(x), yMin, yMax);
            }
        }
    }

    if (end_i - start_i < 281)
    {
        int _numPoints = 281 - (end_i - start_i);
        for (int i = 0; i < _numPoints; i++)
        {
            int index = end_i - start_i + i;
            CONVERT_DATA_TO_DISPLAY(dataCD[index], MIN_VALUE);
        }
    }

    if (ds->peakDet == PeackDetMode::Disable)
    {
        CONVERT_DATA_TO_DISPLAY(dataCD[280], data[end_i]);
        DrawPoints(dataCD, Grid::Left(), 281);
    }
}

void DataDrawing::DrawPoints(uint8 *y, const int start_x, int num_points, const Color &color)
{
    color.SetAsCurrent();

    int x = start_x;

    while (num_points-- > 0)
    {
        Point().Draw(x++, *y++);
    }
}
