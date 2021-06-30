// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "main.h"


typedef enum
{
    APPLICATION_IDLE = 0,
    APPLICATION_START,
    APPLICATION_RUNNING,
}MSC_ApplicationTypeDef;


extern MSC_ApplicationTypeDef Appli_state;


struct StateDisk { enum E {
    Idle,
    Start
}; };


struct FDrive
{
    static FATFS USBDISKFatFS;
    static char USBDISKPath[4];
    static StateDisk::E state;
    static FIL file;
    static bool connected;
    static bool active;

    static void Init();

    static bool Update();

    static bool FileExist(pchar fileName);

    static int OpenFileForRead(pchar fileName);

    // ��������� �� ��������� ����� numBytes ����. ���������� ����� ������� ��������� ����
    static uint ReadFromFile(int numBytes, uint8 *buffer);

    static void CloseOpenedFile();
};


struct State { enum E {
    Start,            // �������� ���������
    Mount,            // ������������ ������
    WrongFlash,       // ������ ����, �� ��������� ������
    RequestAction,    // ��� ������ - ���������� ��� ���
    NotFile,          // ���� ���� �������������, �� ���������� �� �� ����
    Upgrade,          // ������� ��������
    Ok                // ���������� ������ ���������
}; };


struct MainStruct
{
    float percentUpdate;
    volatile State::E state;

    // ������ ��������� ������������ �� ���� ������� ��������� ��� ���������� ������� ���
    // ������ ��� �� ������ ���� �������� � ������� malloc � ������ ��������� � ���������� � ������ �������� �� �������� ���������
    static MainStruct *ms;
};
