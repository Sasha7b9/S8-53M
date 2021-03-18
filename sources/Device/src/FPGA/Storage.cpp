// 2021/03/18 16:06:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/DataSettings.h"
#include "FPGA/FPGA.h"
#include "FPGA/Storage.h"


DataStorage::DataStorage()
{
    buffer.Realloc((int)sizeof(DataSettings) + FPGA::SET::BytesForData());
}
