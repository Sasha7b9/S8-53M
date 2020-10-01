#pragma once
#include <ff.h>
    

// Key configuration
#define KEY_START_ADDRESS   (uint)0x0
#define KEY_PAGE_NUMBER     20
#define KEY_VALUE           0xAAAA5555

// Flash configuration

#define MAIN_PROGRAM_PAGE_NUMBER    21
#define NUM_OF_PAGES                256
#define FLASH_PAGE_SIZE             2048

#define TIME_WAIT   5000    // ����� ������ ��������


enum State
{
    State_Start,            // �������� ���������
    State_Mount,            // ������������ ������
    State_WrongFlash,       // ������ ����, �� ��������� ������
    State_RequestAction,    // ��� ������ - ���������� ��� ���
    State_NotFile,          // ���� ���� �������������, �� ���������� �� �� ����
    State_Upgrade,          // ������� ��������
    State_Ok                // ���������� ������ ���������
};

enum StateDisk
{
    StateDisk_Idle,
    StateDisk_Start
};

struct Display
{
    bool isRun;
    float value;
    float direction;
    uint timePrev;
};

struct FDrive
{
    FATFS USBDISKFatFS;
    char USBDISKPath[4];
    StateDisk state;
    FIL file;
    int connection;
    int active;
};

struct MainStruct
{
    FDrive drive;
    Display display;
    float percentUpdate;
    State state;
};


// ������ ��������� ������������ �� ���� ������� ��������� ��� ���������� ������� ���
// ������ ��� �� ������ ���� �������� � ������� malloc � ������ ��������� � ���������� � ������ �������� �� �������� ���������
extern MainStruct *ms; //-V707
