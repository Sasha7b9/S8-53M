// 2021/03/18 16:06:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Buffer_.h"


//----------------------------------------------------------------------------------------------------------------------
struct DataStorage
{
friend struct RecordStorage;

    DataStorage();

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
friend class Storage;

private:

    void Fill(const DataStorage &data);

    uint Size() const;

    // ���������� ������ ������, ����� � �� ����� �������� ������ data
    uint Size(const DataStorage &data) const;

    uint8 *Address() const;

    // ���������� ����� ������� �����, ���������� �� �������
    uint8 *End() const;

    DataStorage &Data() const;

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
    static DataStorage &ExtractLast();

    // ������� ������, ����������� �� "����������" from_end �� ��������� ������ (��� from_end ���������� ���������
    // ���������� ������)
    static DataStorage &Extract(uint from_end);

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

    static RecordStorage *addressOldestRecord;   // ����� �������� ����� ������ ������. ���� ���, ����� ���������� ���
                                                // ��������� ������

    static DataStorage *nullData;
};
