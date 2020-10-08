#pragma once
#include <ff.h>
#include "Display/Display.h"
    

// Key configuration
#define KEY_START_ADDRESS   (uint)0x0
#define KEY_PAGE_NUMBER     20
#define KEY_VALUE           0xAAAA5555

// Flash configuration

#define MAIN_PROGRAM_PAGE_NUMBER    21
#define NUM_OF_PAGES                256
#define FLASH_PAGE_SIZE             2048

#define TIME_WAIT   5000    // ����� ������ ��������


struct State { enum E
{
    Start,            // �������� ���������
    Mount,            // ������������ ������
    WrongFlash,       // ������ ����, �� ��������� ������
    RequestAction,    // ��� ������ - ���������� ��� ���
    NotFile,          // ���� ���� �������������, �� ���������� �� �� ����
    Upgrade,          // ������� ��������
    Ok                // ���������� ������ ���������
};};

struct StateDisk { enum E
{
    Idle,
    Start
};};

struct FDrive
{
    FATFS USBDISKFatFS;
    char USBDISKPath[4];
    StateDisk::E state;
    FIL file;
    int connection;
    int active;
};

struct MainStruct
{
    FDrive drive;
    Display display;
    float percentUpdate;
    State::E state;
};


// ������ ��������� ������������ �� ���� ������� ��������� ��� ���������� ������� ���
// ������ ��� �� ������ ���� �������� � ������� malloc � ������ ��������� � ���������� � ������ �������� �� �������� ���������
extern MainStruct *ms; //-V707
