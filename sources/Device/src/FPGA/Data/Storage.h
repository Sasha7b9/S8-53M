// 2021/03/18 16:06:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Containers/Buffer_.h"


struct Channel;
struct DataDrawing;
struct DataSettings;
struct RecordStorage;


//----------------------------------------------------------------------------------------------------------------------
struct DataReading
{
friend struct RecordStorage;
friend class Storage;

    DataReading();

    void CreateFromCurrentSettings();

    void CreateNull();

    DataSettings &Settings() const;

    // ���� � ������������ ��������� �� 8-������ ��������, ������� ����� �����, ��� � ������, ���� ������ �����������
    // �� ������� ���, ������ ������� �� 16 ���, �.�. ������ �� �������� ������� � ��� ��������
    uint8 *Data(const Channel &ch);

private:

    // � ������ �������� ��������������� - ���������, ������ ������� ������, ������ ������� ������
    Buffer buffer;

    uint Size();

    void CreateFromRecord(RecordStorage *record);
};


//----------------------------------------------------------------------------------------------------------------------
struct RecordStorage
{
    /*
    *  ������ ������������ ���. ������� ��� ���������� RecordStorage, ����� DataSettings, � ����� ������ ������� � ���-
    *  ���� �������
    */

friend class Storage;
friend struct DataReading;

private:

    void Fill(const DataReading &data);

    uint Size() const;

    // ���������� ������ ������, ����� � �� ����� �������� ������ data
    uint Size(const DataSettings &data) const;

    uint8 *Address() const;

    // ���������� ����� ������� �����, ���������� �� �������
    uint8 *End() const;

    DataSettings &Data() const;

    RecordStorage *prev;        // ����� ���������� (����� ������) ������
    RecordStorage *next;        // ����� ��������� (����� �����) ������
};


//----------------------------------------------------------------------------------------------------------------------
class Storage
{
    /*
    *  ����� ���������� �� ���������� ������������ �������� ��� ��������� ������, ������ � ������� �������� ���.
    *  � ���������� addressFirstRecord �������� ����� ������ ������. ������ ������������ ����� ������� ���� DataReading.
    *  ������ ������ ����������� ������ ��������� �� ��������� ������ ���������� ������.
    *  ��� ���������� ����� ��� ��������� ������ (����� ������) �������, � �� ��� ����� ������������ ��������� (�����
    *  �����. �� ������ ���������� ��������� �� ������ �
    */

public:

    static void Append(const DataReading &data);

    // ������� ��������� ���������� ������
    static bool ExtractLast(DataReading &data);

    // ������� ������, ����������� �� "����������" from_end �� ��������� ������ (��� from_end ���������� ���������
    // ���������� ������)
    static bool Extract(uint from_end, DataReading &data);

    // ���������� ���������� ������� � ���������
    static uint NumRecords();

    // ���������� true, ���� ��������� � from-end-� ������ � data ��������� (����� ��� �������������)
    static bool SettingsIsEquals(uint from_end, const DataReading &data);

private:

    // ������ ������ �� ������� ��� ��� ����������� data
    static RecordStorage *Create(const DataSettings &data);

    // ����� ������ ������ (�������� ������)
    static RecordStorage *Oldest();

    // ����� ����� ������ (�������� ���������)
    static RecordStorage *Newest();

    // ������� ����� ������ ������
    static void DeleteOldest();

    // ���������� false � ������ data (� ������� ��� ������ ���������, ��������� ��������� �� ����������)
    static bool CreateNull(DataReading &data);

    // ����� ������ ��� ������, ����� ����� ������ ����� ������� �����, ��� ������
    static RecordStorage *FindForNewestMoreOldest(uint need_memory);

    static RecordStorage *addressOldestRecord;   // ����� �������� ����� ������ ������. ���� ���, ����� ���������� ���
                                                // ��������� ������
};
