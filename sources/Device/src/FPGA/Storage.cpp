// 2021/03/18 16:06:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/DataSettings.h"
#include "FPGA/FPGA.h"
#include "FPGA/Storage.h"


DataStorage::DataStorage()
{
    buffer.Realloc((int)sizeof(DataSettings) + FPGA::SET::BytesForData());

    DataSettings &ds = *(DataSettings *)Begin();

    ds.Fill();
}


uint8 *DataStorage::Data(const Channel &ch)
{
    const DataSettings &ds = Settings();

    if (!ds.IsEnabled(ch))
    {
        return nullptr;
    }

    uint8 *result = Begin() + sizeof(DataSettings);

    if (ch.IsB() && ds.IsEnabled(Channel::A))
    {
        result += ds.BytesInChannel();
    }

    return result;
}


const DataSettings &DataStorage::Settings() const
{
    return (const DataSettings &)*Begin();
}


bool DataStorage::HasData() const
{
    const DataSettings &ds = Settings();

    return ds.IsEnabled(Channel::A) || ds.IsEnabled(Channel::B);
}


uint8 *DataStorage::Begin() const
{
    return (uint8 *)((Buffer)buffer).Data();
}


void Storage::Append(const DataStorage &data)
{

}
