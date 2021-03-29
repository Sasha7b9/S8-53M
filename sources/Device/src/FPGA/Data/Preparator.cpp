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
    data = new DataReading();

    if (Storage::SettingsIsEquals(0, *data))
    {
        return;
    }

    DataReading last;

    if (Storage::ExtractLast(last))
    {
        DataSettings &ds = data->Settings();

        if (ds.IsEnabled(ChA))
        {
            std::memcpy(data->Data(ChA), last.Data(ChA), ds.BytesInChannel());
        }

        if (ds.IsEnabled(ChB))
        {
            std::memcpy(data->Data(ChB), last.Data(ChB), ds.BytesInChannel());
        }
    }
}
