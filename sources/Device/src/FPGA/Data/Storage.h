// 2021/03/18 16:06:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Buffer_.h"


struct Channel;
struct DataDrawing;
struct DataSettings;
struct RecordStorage;


struct DataDrawingKeeper
{
    DataDrawingKeeper() : data(nullptr) {}
    ~DataDrawingKeeper();
    DataDrawing *data;
};


//----------------------------------------------------------------------------------------------------------------------
struct DataStorage
{
friend struct RecordStorage;

    DataStorage();

    void CreateFromCurrentSettings();

    void CreateFromRecord(RecordStorage *record);

    void CreateNull();

    DataSettings &Settings() const;

    // ���� � ������������ ��������� �� 8-������ ��������, ������� ����� �����, ��� � ������, ���� ������ �����������
    // �� ������� ���, ������ ������� �� 16 ���, �.�. ������ �� �������� ������� � ��� ��������
    uint8 *Data(const Channel &ch);

private:

    // � ������ �������� ��������������� - ���������, ������ ������� ������, ������ ������� ������
    Buffer buffer;

    uint Size();
};


//----------------------------------------------------------------------------------------------------------------------
struct RecordStorage
{
    /*
    *  ������ ������������ ���. ������� ��� ���������� RecordStorage, ����� DataSettings, � ����� ������ ������� � ���-
    *  ���� �������
    */

friend class Storage;
friend struct DataStorage;

private:

    void Fill(const DataStorage &data);

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
    *  � ���������� addressFirstRecord �������� ����� ������ ������. ������ ������������ ����� ������� ���� DataStorage.
    *  ������ ������ ����������� ������ ��������� �� ��������� ������ ���������� ������.
    *  ��� ���������� ����� ��� ��������� ������ (����� ������) �������, � �� ��� ����� ������������ ��������� (�����
    *  �����. �� ������ ���������� ��������� �� ������ �
    */

public:

    static void Append(const DataStorage &data);

    // ������� ��������� ���������� ������
    static bool ExtractLast(DataStorage &data);

    // ������� ������, ����������� �� "����������" from_end �� ��������� ������ (��� from_end ���������� ���������
    // ���������� ������)
    static bool Extract(uint from_end, DataStorage &data);

    // ���������� ���������� ������� � ���������
    static uint NumRecords();

    static void PrepareDataForDrawing(DataDrawingKeeper &keeper);

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
    static bool CreateNull(DataStorage &data);

    // ����� ������ ��� ������, ����� ����� ������ ����� ������� �����, ��� ������
    static RecordStorage *FindForNewestMoreOldest(uint need_memory);

    static RecordStorage *addressOldestRecord;   // ����� �������� ����� ������ ������. ���� ���, ����� ���������� ���
                                                // ��������� ������
};
