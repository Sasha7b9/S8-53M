// 2021/03/18 16:06:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Buffer_.h"


struct DataStorage
{
    DataStorage();

    const DataSettings &Settings() const;

    // ���� � ������������ ��������� �� 8-������ ��������, ������� ����� �����, ��� � ������, ���� ������ �����������
    // �� ������� ���, ������ ������� �� 16 ���, �.�. ������ �� �������� ������� � ��� ��������
    uint8 *Data(const Channel &ch) const;

    // ���������� true, ���� � ���������� ������� ���� �� ���� �����
    bool HasData() const;

private:

    // � ������ �������� ��������������� - ���������, ������ ������� ������, ������ ������� ������
    Buffer buffer;

    uint8 *Begin() const;
};


struct RecordStorage
{
    void Fill(const DataStorage &data);
private:
    RecordStorage *prev;        // ����� ���������� (����� ������) ������
    RecordStorage *next;        // ����� ��������� (����� �����) ������
    uint16 *dataStorage;        // 
};


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

    Storage();

    static void Append(const DataStorage &data);

private:

    // ������ ������ �� ������� ��� ��� ����������� data
    static RecordStorage *Create(const DataStorage &data);

    RecordStorage *addressFirstRecord;  // ����� �������� ����� ������ ������. ���� ���, ����� ���������� ���
                                        // ��������� ������
};
