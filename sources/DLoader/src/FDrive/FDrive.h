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
    FATFS USBDISKFatFS;
    char USBDISKPath[4];
    StateDisk::E state;
    FIL file;
    int connected;
    int active;

    static void Init();
};


bool FDrive_Update();
bool FDrive_FileExist(pchar fileName);
int FDrive_OpenFileForRead(pchar fileName);
// ��������� �� ��������� ����� numBytes ����. ���������� ����� ������� ��������� ����
uint FDrive_ReadFromFile(int numBytes, uint8 *buffer);
void FDrive_CloseOpenedFile();


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
    FDrive drive;
    Display display;
    float percentUpdate;
    volatile State::E state;

    // ������ ��������� ������������ �� ���� ������� ��������� ��� ���������� ������� ���
    // ������ ��� �� ������ ���� �������� � ������� malloc � ������ ��������� � ���������� � ������ �������� �� �������� ���������
    static MainStruct *ms;
};
