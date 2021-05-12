// 2021/04/27 11:09:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Containers/String_.h"


struct DataSettings;
struct SettingsNRST;


struct ROM
{
	static const uint SIZE = 2 * 1024 * 1024;

    static void Init();

    // ������� ��� ����������/�������������� ��������
    struct Settings
    {
        // ��������� ������ � ��������� ��������� ������
        static void CheckMemory();

        static void Save();

        static bool Load();

        static void Erase();
    };

    // ������� ��� ����������/�������� �������������� ������������� ��������
    struct NRST
    {
        // ���������� ��������� �� ���������� � ROM �������������� ���������. nullptr � ������, ���� �������� ��� ���
        static SettingsNRST *GetSaved();
        
        // ��������� ��������� �� ������ nrst � ROM
        static void Save(SettingsNRST *nrst);

        static void Erase();
    };

    // ������� ��� ����������/�������������� ������
    struct Data
    {
        // \brief ����� ����������� �� ���������� ���� ���������. ���� ���������� ����������� �����������, ������� �����
        // ������� � ���� ����� ����� �����.
        static const uint MAX_NUM_SAVED_WAVES = 23;  

        // ���� ������� ����, ��������������� ������� ������� ����� true.
        static void GetInfo(bool existData[MAX_NUM_SAVED_WAVES]);

        static void Save(uint numInROM, const DataSettings *ds);
        
        // �������� ������ � ������� ����� num. ���� ������� ���, � ds ������������ 0
        static const DataSettings *Read(uint numInROM);

        static void Erase(uint numInROM);

        static void EraseAll();
    };
};



struct OTP
{
    static bool SaveSerialNumber(char *servialNumber);
    
    // ���������� ��������� ������������� ��������� ������. � freeForWrite ���������� ��������� ���� ��� ������
    static String GetSerialNumber(int *freeForWrite);
};

