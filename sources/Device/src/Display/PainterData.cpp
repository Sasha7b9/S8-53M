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


#define CONVERT_DATA_TO_DISPLAY(out, in)                        \
    (out) = (uint8)(max_y - ((in) - MIN_VALUE) * scale_y);      \
    if((out) < min_y)          { (out) = (uint8)min_y; }        \
    else if ((out) > max_y)    { (out) = (uint8)max_y; };


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

    float scale_y = (float)(max_y - min_y) / (MAX_VALUE - MIN_VALUE);
    float scale_x = Grid::Width() / 280.0f;

    Set2Int p = SettingsDisplay::PointsOnDisplay();

    points[ch].Realloc(281);

    uint8 *data_channel = data.Data(ch);

    if (scale_x == 1.0f)
    {
        for (int i = p.value1; i < p.value2; i++)
        {
            int index = i - p.value1;
            CONVERT_DATA_TO_DISPLAY((points[ch][(uint)index]),
                Math::CalculateFiltr(data_channel, i, SettingsMemory::GetNumPoints(false), (int)Smoothing::NumPoints()));
        }
    }
}


void DataDrawing::DrawChannel(const Channel &ch)
{
    if (!data.Settings().IsEnabled(ch))
    {
        return;
    }

    ch.GetColor().SetAsCurrent();

    int x = Grid::Left();

    for (uint i = 0; i < points[ch].Size(); i++)
    {
        Point().Draw(x++, points[ch][i]);
    }
}
