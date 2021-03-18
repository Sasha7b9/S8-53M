// 2021/03/18 16:06:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/DataSettings.h"
#include "FPGA/FPGA.h"
#include "FPGA/Storage.h"
#include <cstring>


DataStorage::DataStorage()
{
    buffer.Realloc((int)sizeof(DataSettings) + FPGA::SET::BytesForData());

    DataSettings &ds = *(DataSettings *)Begin();

    ds.Fill();
}


uint8 *DataStorage::Data(const Channel &ch) const
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


void RecordStorage::Fill(const DataStorage &data)
{
    const DataSettings &ds = data.Settings();
    uint length_channel = (uint)ds.BytesInChannel();

    uint8 *address = (uint8 *)this;

    std::memcpy(address, &ds, sizeof(DataSettings));

    address += sizeof(DataSettings);

    if (ds.IsEnabled(Channel::A))
    {
        std::memcpy(address, data.Data(Channel::A), length_channel);
        address += length_channel;
    }

    if (ds.IsEnabled(Channel::B))
    {
        std::memcpy(address, data.Data(Channel::B), length_channel);
    }
}


Storage::Storage() : addressFirstRecord(nullptr)
{

}


void Storage::Append(const DataStorage &data)
{
    RecordStorage *record = Create(data);

    record->Fill(data);
}


RecordStorage *Storage::Create(const DataStorage &data)
{
    return nullptr;
}
