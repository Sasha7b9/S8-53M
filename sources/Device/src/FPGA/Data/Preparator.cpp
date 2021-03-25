// 2021/03/24 8:41:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/PainterData.h"
#include "FPGA/Data/Preparator.h"


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
}
