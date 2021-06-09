// 2021/03/17 13:27:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Utils/Math_.h"
#include "Display/PainterData.h"
#include "Display/Screen/MemoryWindow.h"
#include "FPGA/FPGA.h"
#include "FPGA/Data/DataSettings.h"
#include "FPGA/Data/Preparator.h"
#include "Menu/Menu.h"
#include "Utils/ProcessingSignal.h"
#include <cstring>


#define CONVERT_DATA_TO_DISPLAY(out, in)                        \
    (out) = (uint8)(max_y - ((in) - Value::MIN) * scale_y);     \
    if((out) < min_y)          { (out) = (uint8)min_y; }        \
    else if ((out) > max_y)    { (out) = (uint8)max_y; };


void PainterData::DrawData()
{
    DataDrawingKeeper keeper;

    Processing::SetSignal(keeper.data->points[ChA], keeper.data->points[ChB], &keeper.data->data.Settings());

    keeper.data->Draw();

    Rectangle(Grid::Width() + 1, Grid::FullHeight() + 1).Draw(Grid::Left(), Grid::TOP, Color::FILL);

    MemoryWindow::Draw(keeper.data->data);

    Measure::DrawAll();
}


void DataDrawing::Prepare()
{
    Storage::ExtractLast(data);

    PrepareChannel(ChA);
    PrepareChannel(ChB);
}


void DataDrawing::Draw()
{
    DrawChannel(ChA);
    DrawChannel(ChB);
}


void DataDrawing::PrepareChannel(const Channel &ch)
{
    points[ch].Free();

    if (!data.Settings().IsEnabled(ch))
    {
        return;
    }

    int min_y = Grid::TOP;
    int max_y = Grid::ChannelBottom();
    float scale_y = (float)(max_y - min_y) / (Value::MAX - Value::MIN);

    BitSet64 p = SettingsDisplay::BytesOnDisplay();

    points[ch].Realloc(data.Settings().IsEnabledPeakDet() ? (281 * 2) : 281U);

    uint8 *data_channel = data.Data(ch);

    for (int i = p.first; i < p.second; i++)
    {
        int index = i - p.first;
        CONVERT_DATA_TO_DISPLAY((points[ch][(uint)index]),
            Math::CalculateFiltr(data_channel, i, (int)set.memory.enum_points_fpga.BytesInChannel(),
            (int)Smoothing::NumPoints()));
    }
}


void DataDrawing::DrawChannel(const Channel &ch)
{
    if (!data.Settings().IsEnabled(ch))
    {
        return;
    }

    if (points[ch].Size() == 0)
    {
        return;
    }

    ch.GetColor().SetAsCurrent();

    if (set.display.mode_draw_signal.IsLines())
    {
        DrawChannelLined(Grid::Left(), points[ch]);
    }
    else
    {
        DrawChannelPointed(Grid::Left(), points[ch]);
    }
}


void DataDrawing::DrawChannelPointed(int x, Buffer<uint8> &buffer)
{
    uint8 *d = buffer.DataU8();
    uint size = buffer.Size();

    if (data.Settings().IsEnabledPeakDet())
    {
        for (uint i = 0; i < size; i += 2)
        {
            Point().Draw(x, *d++);
            Point().Draw(x, *d++);

            x++;
        }
    }
    else
    {
        for (uint i = 0; i < size; i++)
        {
            Point().Draw(x++, *d++);
        }
    }
}


void DataDrawing::DrawChannelLined(int x, Buffer<uint8> &buffer)
{
    if (data.Settings().IsEnabledPeakDet())
    {
        uint size = buffer.Size();
        uint8 *d = buffer.DataU8();

        for (uint i = 0; i < size; i += 2)
        {
            int x1 = *d;
            d++;
            int x2 = *d;
            d++;

            VLine().Draw(x++, x1, x2);
        }
    }
    else
    {
        uint size = buffer.Size() - 1;

        for (uint i = 0; i < size; i++)
        {
            uint8 current = buffer[i];
            uint8 next = buffer[i + 1];

            if (current > next) { VLine().Draw(x++, next + 1, current); }
            else if (current < next) { VLine().Draw(x++, next - 1, current); }
            else { Point().Draw(x++, next); }
        }

        Point().Draw(x, buffer[buffer.Size() - 1]);
    }
}
