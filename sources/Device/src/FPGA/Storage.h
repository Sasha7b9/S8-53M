// 2021/03/18 16:06:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Buffer_.h"


//----------------------------------------------------------------------------------------------------------------------
struct DataStorage
{
friend struct RecordStorage;

    DataStorage();

    void CreateFromCurrentSettings();

    void CreateFromRecord(RecordStorage *record);

    void CreateNull();

    const DataSettings &Settings() const;

    // ���� � ������������ ��������� �� 8-������ ��������, ������� ����� �����, ��� � ������, ���� ������ �����������
    // �� ������� ���, ������ ������� �� 16 ���, �.�. ������ �� �������� ������� � ��� ��������
    uint8 *Data(const Channel &ch) const;

private:

    // � ������ �������� ��������������� - ���������, ������ ������� ������, ������ ������� ������
    Buffer buffer;

    uint8 *Begin() const;

    uint Size() const;
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

private:

    // ������ ������ �� ������� ��� ��� ����������� data
    static RecordStorage *Create(const DataStorage &data);

    // ����� ������ ������ (�������� ������)
    static RecordStorage *Oldest();

    // ����� ����� ������ (�������� ���������)
    static RecordStorage *Newest();

    // ������� ����� ������ ������
    static void DeleteOldest();

    // ���������� false � ������ data (� ������� ��� ������ ���������, ��������� ��������� �� ����������)
    static bool CreateNull(DataStorage &data);

    static RecordStorage *addressOldestRecord;   // ����� �������� ����� ������ ������. ���� ���, ����� ���������� ���
                                                // ��������� ������
};
