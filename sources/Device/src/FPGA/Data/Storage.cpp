// 2021/03/18 16:06:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "FPGA/Data/DataSettings.h"
#include "FPGA/Data/Storage.h"
#include <cstring>


RecordStorage *Storage::addressOldestRecord = nullptr;


// Записывает по address данные из buffer. Возвращает адрес первого байта после записи
static uint8 *CopyTo(uint8 *address, void *buffer, uint size);


DataReading::DataReading()
{
    CreateFromCurrentSettings();
}


void DataReading::CreateFromCurrentSettings()
{
    DEBUG_POINT_0;
    buffer.Realloc(sizeof(DataSettings) + set.memory.enum_points_fpga.BytesForData());

    DEBUG_POINT_0;
    DataSettings &ds = *(DataSettings *)buffer.DataU8();

    DEBUG_POINT_0;

    ds.Fill();
    DEBUG_POINT_0;
}


void DataReading::CreateFromRecord(RecordStorage *record)
{
    DataSettings &ds = record->Data();

    uint size = sizeof(DataSettings) + ds.BytesInData();

    buffer.Realloc(size);

    std::memcpy(buffer.DataU8(), &ds, size);
}


void DataReading::CreateNull()
{
    DataSettings ds;
    ds.enabled_a = 0;
    ds.enabled_b = 0;

    buffer.Realloc(sizeof(DataSettings));

    std::memcpy(buffer.DataU8(), &ds, sizeof(DataSettings));
}


uint8 *DataReading::Data(const Channel &ch)
{
    const DataSettings &ds = Settings();

    if (!ds.IsEnabled(ch))
    {
        return nullptr;
    }

    uint8 *result = buffer.DataU8() + sizeof(DataSettings);

    if (ch.IsB() && ds.IsEnabled(Channel::A))
    {
        result += ds.BytesInChannel();
    }

    return result;
}


uint8 *DataReading::DataEnd(const Channel &ch)
{
    uint8 *data = Data(ch);

    return (data == nullptr) ? data : data + Settings().BytesInChannel();
}


DataSettings &DataReading::Settings() const
{
    return (DataSettings &)*((DataReading *)this)->buffer.DataU8();
}


uint DataReading::Size()
{
    uint result = sizeof(DataSettings);

    DataSettings &ds = Settings();

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


void RecordStorage::Fill(const DataReading &data)
{
    DataSettings &ds = data.Settings();
    uint length_channel = (uint)ds.BytesInChannel();

    uint8 *address = (uint8 *)this + sizeof(RecordStorage);

    address = CopyTo(address, (void *)&ds, sizeof(DataSettings));

    if (ds.IsEnabled(Channel::A))
    {
        address = CopyTo(address, ((DataReading &)data).Data(Channel::A), length_channel);
    }

    if (ds.IsEnabled(Channel::B))
    {
        CopyTo(address, ((DataReading &)data).Data(Channel::B), length_channel);
    }
}


uint8 *RecordStorage::Address() const
{
    return (uint8 *)this;
}


uint RecordStorage::Size() const
{
    return Size(Data());
}


uint RecordStorage::Size(const DataSettings &data) const
{
    return sizeof(RecordStorage) + sizeof(data) + data.BytesInData();
}


DataSettings &RecordStorage::Data() const
{
    uint8 *address = Address() + sizeof(RecordStorage);

    return *(DataSettings *)address;
}


uint8 *RecordStorage::End() const
{
    return Address() + Size();
}


void Storage::Append(const DataReading &data)
{
    RecordStorage *record = Create(data.Settings());

    record->Fill(data);
}


RecordStorage *Storage::Create(const DataSettings &ds)
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
        uint need_memory = Oldest()->Size(ds);

        if (Oldest() > Newest())
        {
            while (Oldest()->Address() - Newest()->End() < (int)need_memory)
            {
                DeleteOldest();

                if (Oldest() < Newest())
                {
                    break;
                }
            }
        }

        result = (Oldest() < Newest()) ? FindForNewestMoreOldest(need_memory) : (RecordStorage *)Newest()->End();

        result->prev = Newest();
        result->next = nullptr;
        Newest()->next = result;
    }

    return result;
}


RecordStorage *Storage::FindForNewestMoreOldest(uint need_memory)
{
    RecordStorage *result = nullptr;

    uint8 *address = Newest()->End();                                           // Это предполагаемый адрес нашей записи

    if (address + need_memory <= HAL_FMC::ADDR_RAM_END)                               // Если запись поместится в память
    {
        result = (RecordStorage *)address;
    }
    else                                                                          // Если запись не поместится в память
    {                                                                             // то будем записывать в начало памяти
        while (Oldest()->Address() - HAL_FMC::ADDR_RAM_BEGIN < (int)need_memory)
        {
            DeleteOldest();
        }

        result = (RecordStorage *)HAL_FMC::ADDR_RAM_BEGIN;
    }

    return result;
}


void Storage::DeleteOldest()
{
    if (addressOldestRecord == nullptr)
    {
        return;
    }

    Oldest()->next->prev = nullptr;

    addressOldestRecord = Oldest()->next;
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


uint Storage::NumRecords()
{
    if (addressOldestRecord == nullptr)
    {
        return 0;
    }

    RecordStorage *record = Oldest();

    uint result = 1;

    while (record->next)
    {
        result++;
        
        record = record->next;
    }

    return result;
}


bool Storage::ExtractLast(DataReading &data)
{
    return Extract(0, data);
}


bool Storage::Extract(uint from_end, DataReading &data)
{
    if (NumRecords() == 0 || from_end >= NumRecords())
    {
        return CreateNull(data);
    }

    RecordStorage *record = Newest();

    while (from_end != 0)
    {
        if (record == nullptr)
        {
            LOG_ERROR("Ошибка");
            break;
        }

        record = record->prev;
        from_end--;
    }

    data.CreateFromRecord(record);

    return true;
}


bool Storage::CreateNull(DataReading &data)
{
    data.CreateNull();

    return false;
}


static uint8 *CopyTo(uint8 *address, void *buffer, uint size)
{
    std::memcpy(address, buffer, size);

    return address + size;
}


bool Storage::SettingsIsEquals(uint from_end, const DataReading &sample)
{
    DataReading data;

    if (Extract(from_end, data))
    {
        DataSettings &ss = sample.Settings();
        DataSettings &sd = data.Settings();

        return ((ss.tbase == sd.tbase) &&
            (ss.range == sd.range) &&
            (ss.tshift == sd.tshift) &&
            (ss.r_shift_a == sd.r_shift_a) &&
            (ss.r_shift_b == sd.r_shift_b) &&
            (ss.multiplier_a == sd.multiplier_a) &&
            (ss.multiplier_b == sd.multiplier_b) &&
            (ss.enum_points == sd.enum_points));
    }

    return false;
}
