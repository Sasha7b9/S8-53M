// 2021/03/24 8:41:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/PainterData.h"
#include "FPGA/Data/DataSettings.h"
#include "FPGA/Data/Preparator.h"
#include <cstring>


DataDrawingKeeper::DataDrawingKeeper() : data(nullptr)
{
    Prepare();
}


DataDrawingKeeper::~DataDrawingKeeper()
{
    delete data;
}
 

void DataDrawingKeeper::Prepare()
{
    data = new DataDrawing();

    data->Prepare();
}


DataReadingKeeper::DataReadingKeeper() : data(nullptr)
{
    Prepare();
}


DataReadingKeeper::~DataReadingKeeper()
{
    delete data;
}


void DataReadingKeeper::Prepare()
{
    DEBUG_POINT_0;
    data = new DataReading();
    DEBUG_POINT_0;

    if (Storage::SettingsIsEquals(0, *data))
    {
        DEBUG_POINT_0;
        return;
    }
    DEBUG_POINT_0;

    DataReading last;

    DEBUG_POINT_0;

    if (Storage::ExtractLast(last))
    {
        DEBUG_POINT_0;
        DataSettings &ds = data->Settings();
        DEBUG_POINT_0;
        if (ds.IsEnabled(ChA))
        {
            DEBUG_POINT_0;
            std::memcpy(data->Data(ChA), last.Data(ChA), ds.BytesInChannel());
        }
        DEBUG_POINT_0;
        if (ds.IsEnabled(ChB))
        {
            DEBUG_POINT_0;
            std::memcpy(data->Data(ChB), last.Data(ChB), ds.BytesInChannel());
        }
        DEBUG_POINT_0;
        ds.is_clean = 0;
    }
    DEBUG_POINT_0;
}
