#pragma once


#define FILE_NAME_MASK              (set.memory.fileNameMask)                       // SettingsMemory.fileNameMask

#define FILE_NAME                   (set.memory.fileName)                           // SettingsMemory.fileName

#define INDEX_SYMBOL                (set.memory.indexCurSymbolNameMask)             // SettingsMemory.indexCurSymbolNameMask

#define FLASH_AUTOCONNECT           (set.memory.flashAutoConnect)                   // SettingsMemory.flashAutoConnect

#define MODE_SAVE_SIGNAL            (set.memory.modeSaveSignal)                     // SettingsMemory.modeSaveSignal
#define MODE_SAVE_SIGNAL_IS_BMP     (MODE_SAVE_SIGNAL == ModeSaveSignal::BMP)


struct StructMemoryLast
{
    bool isActiveModeSelect;
};

// ��������� ����->������
struct SettingsMemory
{
#define MAX_SYMBOLS_IN_FILE_NAME 35
    ENUM_POINTS_FPGA::E enum_points_fpga;                       // ����� �����.
    ModeWork::E         modeWork;                               // ����� ������.
    FileNamingMode::E   fileNamingMode;                         // ����� ���������� ������.
    char                fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]; // ����� ����� ��� ��������������� ���������� ������\n
         // ������� ����������.\n
         // %y('\x42') - ���, %m('\x43') - �����, %d('\x44') - ����, %H('\x45') - ����, %M('\x46') - ������, %S('\x47') - �������\n
         // %Nn('\x48''n') - ���������� �����, ������ �������� �� ����� n ���������, ��������, 7 � %3N ����� ������������� � 007\n
         // �������\n
         // name_%4N_%y_%m_%d_%H_%M_%S ����� ������������ ����� ���� name_0043_2014_04_25_14_45_32\n
         // ��� ���� �������� ��������, ��� ���� ������������ %4N ����� ����� ��������� ����������, ��, ������ �����, ���� �������� ����� ������ ����� 0001, �.�. ��� ����������� ������ ��������������� ������ ������� �� ����.
    char                fileName[MAX_SYMBOLS_IN_FILE_NAME];     // ��� ����� ��� ������ ������� �������
    int8                indexCurSymbolNameMask;                 // ������ �������� ������� � ������ ������� ����� ��� ������ �����.
    StructMemoryLast    strMemoryLast;
    ModeShowIntMem::E   modeShowIntMem;                         // ����� ������� ���������� � ������ ����������� ��
    bool                flashAutoConnect;                       // ���� true, ��� ���������� ���� ������������� ��������� NC (������ ���������)
    ModeBtnMemory::E    modeBtnMemory;
    ModeSaveSignal::E   modeSaveSignal;                         // � ����� ���� ��������� ������.

    static int GetNumPoints(bool forCalculate);
    static ENUM_POINTS_FPGA::E IntNumPoints2FPGA_NUM_POINTS(int numPoints);
};
