// 2021/03/18 16:06:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/DataSettings.h"
#include "FPGA/FPGA.h"
#include "FPGA/Storage.h"
#include <cstring>


RecordStorage *Storage::addressOldestRecord = nullptr;


// ���������� �� address ������ �� buffer. ���������� ����� ������� ����� ����� ������
static uint8 *CopyTo(uint8 *address, void *buffer, uint size);


DataStorage::DataStorage()
{
    CreateFromCurrentSettings();
}


void DataStorage::CreateFromCurrentSettings()
{
    buffer.Realloc(sizeof(DataSettings) + FPGA::SET::BytesForData());

    DataSettings &ds = *(DataSettings *)Begin();

    ds.Fill();
}


void DataStorage::CreateFromRecord(RecordStorage *record)
{
    DataSettings &ds = record->Data();

    uint size = sizeof(DataSettings) + ds.BytesInData();

    buffer.Realloc(size);

    std::memcpy(buffer.Data(), &ds, size);
}


void DataStorage::CreateNull()
{
    DataSettings ds;
    ds.enabled_a = 0;
    ds.enabled_b = 0;

    buffer.Realloc(sizeof(DataSettings));

    std::memcpy(buffer.Data(), &ds, sizeof(DataSettings));
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

    uint8 *address = (uint8 *)this + sizeof(RecordStorage);

    address = CopyTo(address, (void *)&ds, sizeof(DataSettings));

    if (ds.IsEnabled(Channel::A))
    {
        address = CopyTo(address, data.Data(Channel::A), length_channel);
    }

    if (ds.IsEnabled(Channel::B))
    {
        CopyTo(address, data.Data(Channel::B), length_channel);
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
        uint need_memory = Oldest()->Size(data.Settings());

        if (Newest() >= Oldest())          // ���� ������ ���� � "������" ������� - ����� ��������� ������ ������ ������
        {                                  // (��� ��������, ��� ������ ��� �� ��������� ���� �� ��������� ���������)
            uint8 *address = Newest()->End();                                   // ��� �������������� ����� ����� ������

            if (address + need_memory <= HAL_FMC::ADDR_RAM_END)                       // ���� ������ ���������� � ������
            {
                result = (RecordStorage *)address;
            }
            else                                                                  // ���� ������ �� ���������� � ������
            {                                                                     // �� ����� ���������� � ������ ������
                while (Oldest()->Address() - HAL_FMC::ADDR_RAM_BEGIN < (int)need_memory)
                {
                    DeleteOldest();
                }

                result = (RecordStorage *)HAL_FMC::ADDR_RAM_BEGIN;
            }

            Oldest()->next = result;
            result->prev = Newest();
            result->next = nullptr;
        }
        else
        {
            while (Oldest()->Address() - Newest()->End() < (int)need_memory)
            {
                DeleteOldest();
            }

            result = (RecordStorage *)Newest()->End();

            Newest()->next = result;
            result->prev = Newest();
            result->next = nullptr;
        }
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


bool Storage::ExtractLast(DataStorage &data)
{
    return Extract(0, data);
}


bool Storage::Extract(uint from_end, DataStorage &data)
{
    if (NumRecords() == 0 || from_end >= NumRecords())
    {
        return CreateNull(data);
    }

    RecordStorage *record = Newest();

    while (from_end != 0)
    {
        record = record->prev;
        from_end--;
    }

    data.CreateFromRecord(record);

    return true;
}


bool Storage::CreateNull(DataStorage &data)
{
    data.CreateNull();

    return false;
}


static uint8 *CopyTo(uint8 *address, void *buffer, uint size)
{
    std::memcpy(address, buffer, size);

    return address + size;
}
