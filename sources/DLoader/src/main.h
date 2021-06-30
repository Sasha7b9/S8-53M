// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <ff.h>
#include "Display/Display.h"
#include "FDrive/FDrive.h"
    

// Key configuration
#define KEY_START_ADDRESS   (uint)0x0
#define KEY_PAGE_NUMBER     20
#define KEY_VALUE           0xAAAA5555

// Flash configuration

#define MAIN_PROGRAM_PAGE_NUMBER    21
#define NUM_OF_PAGES                256
#define FLASH_PAGE_SIZE             2048

#define TIME_WAIT   10000    // ����� ������ ��������


struct State { enum E {
    NoDrive,            // ��������� ��������. ���� ���, �� �� ���������� ������
    Start,              // �������� ���������
    NeedMount,          // ��������� ������������ ������
    Mounted,            // ������ ��������������
    WrongFlash,         // ������ ����, �� ��������� ������
    RequestAction,      // ��� ������ - ���������� ��� ���
    NotFile,            // ���� ���� �������������, �� ���������� �� �� ����
    Upgrade,            // ������� ��������
    Ok                  // ���������� ������ ���������
}; };


struct MainStruct
{
    static float percentUpdate;
    static State::E state;
};
