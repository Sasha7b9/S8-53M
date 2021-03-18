// 2021/03/18 16:06:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/DataSettings.h"
#include "FPGA/FPGA.h"
#include "FPGA/Storage.h"
#include <cstring>


RecordStorage *Storage::addressOldestRecord = nullptr;


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


uint DataStorage::Size() const
{
    uint result = sizeof(DataSettings);

    const DataSettings &ds = Settings();

    if (ds.IsEnabled(Channel::A))
    {
        result += ds.BytesInChannel();
    }

    if (ds.IsEnabled(Channel::B))
    {
        result += ds.BytesInChannel();
    }

    return result;
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


uint8 *RecordStorage::Address() const
{
    return (uint8 *)this;
}


void Storage::Append(const DataStorage &data)
{
    RecordStorage *record = Create(data);

    record->Fill(data);
}


RecordStorage *Storage::Create(const DataStorage &data)
{
    RecordStorage *result = nullptr;

    if (addressOldestRecord == nullptr)
    {
        addressOldestRecord = (RecordStorage *)HAL_FMC::ADDR_RAM_BEGIN;
        result = addressOldestRecord;
        result->prev = nullptr;
        result->next = nullptr;
    }
    else
    {
        if (Newest() >= Oldest())       // Если записи идут в "прямом" порядке - адрес последней больше адреса первой
        {                               // (Это означает, что память ещё не заполнена либо же заполнена полностью)
            
        }
        else
        {

        }
    }

    return result;
}


RecordStorage *Storage::Oldest()
{
    return addressOldestRecord;
}


RecordStorage *Storage::Newest()
{
    if (addressOldestRecord == nullptr)
    {
        return nullptr;
    }

    RecordStorage *record = addressOldestRecord;

    while (record->next)
    {
        record = record->next;
    }

    return record;
}
