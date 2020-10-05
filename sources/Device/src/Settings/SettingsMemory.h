#pragma once
#include "defines.h"
#include "Settings/SettingsTime.h"


/** @addtogroup Settings
 *  @{
 *  @defgroup SettingsMemory
 *  @{
 */



#define ENUM_POINTS                 (set.memory.fpgaNumPoints)                      // SettingsMemory.fpgaNumPoints
#define ENUM_POINTS_IS_281          (ENUM_POINTS == FNP_281)
#define ENUM_POINTS_IS_1024         (ENUM_POINTS == FNP_1024)

#define MODE_WORK                   (set.memory.modeWork)                           // SettingsMemory.modeWork
#define MODE_WORK_IS_DIRECT         (MODE_WORK == ModeWork::Direct)
#define MODE_WORK_IS_LATEST         (MODE_WORK == ModeWork::Latest)
#define MODE_WORK_IS_MEMINT         (MODE_WORK == ModeWork::MemInt)

#define FILE_NAMING_MODE            (set.memory.fileNamingMode)                     // SettingsMemory.fileNamingMode
#define FILE_NAMING_MODE_IS_MASK    (FILE_NAMING_MODE == FileNamingMode::Mask)
#define FILE_NAMING_MODE_IS_HAND    (FILE_NAMING_MODE == FileNamingMode::Manually)

#define FILE_NAME_MASK              (set.memory.fileNameMask)                       // SettingsMemory.fileNameMask

#define FILE_NAME                   (set.memory.fileName)                           // SettingsMemory.fileName

#define INDEX_SYMBOL                (set.memory.indexCurSymbolNameMask)             // SettingsMemory.indexCurSymbolNameMask

#define MODE_SHOW_MEMINT            (set.memory.modeShowIntMem)                     // SettingsMemory.modeShowIntMem
#define MODE_SHOW_MEMINT_IS_SAVED   (MODE_SHOW_MEMINT == ModeShowIntMem::Saved)
#define MODE_SHOW_MEMINT_IS_DIRECT  (MODE_SHOW_MEMINT == ModeShowIntMem::Direct)

#define FLASH_AUTOCONNECT           (set.memory.flashAutoConnect)                   // SettingsMemory.flashAutoConnect

#define MODE_BTN_MEMORY             (set.memory.modeBtnMemory)                      // SettingsMemory.modeBtnMemory
#define MODE_BTN_MEMORY_IS_SAVE     (MODE_BTN_MEMORY == ModeBtnMemory::Save)

#define MODE_SAVE_SIGNAL            (set.memory.modeSaveSignal)                     // SettingsMemory.modeSaveSignal
#define MODE_SAVE_SIGNAL_IS_BMP     (MODE_SAVE_SIGNAL == ModeSaveSignal::BMP)




// ����� ������.
struct ModeWork { enum E
{
    Direct,        // �������� �����.
    Latest,        // � ���� ������ ����� ����������� ��������� ���������� ���������.
    MemInt,        // � ���� ������ ����� ��������� �� flash-������ ��������� ������������� ����� ����������.
};};


// ����� ������������ ������.
struct FileNamingMode { enum E
{
    Mask,        // ��������� �� �����.
    Manually     // ��������� �������.
};};

// ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������.
struct ModeShowIntMem { enum E
{
    Direct,  // ���������� ������ ��������� �������.
    Saved,   // ���������� ���������� ������.
    Both     // ���������� ������ ��������� ������� � ���������� ������.
};};

// ��� ������ ��� ������� ������ ������.
struct ModeBtnMemory { enum E
{
    Menu,     // ����� ������������ ��������������� �������� ����.
    Save      // ���������� ����������� ������ �� ������.
};};

// ��� ��������� ������ �� ������.
struct ModeSaveSignal { enum E
{
    BMP,     // ��������� ������ �� ������ � ������� .bmp.
    TXT      // ��������� ������ �� ������ � ���������� ����.
};};


struct StructMemoryLast
{
    bool isActiveModeSelect;
};

// ��������� ����->������
struct SettingsMemory
{
#define MAX_SYMBOLS_IN_FILE_NAME 35
    ENUM_POINTS_FPGA    fpgaNumPoints;                          // ����� �����.
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
};



int sMemory_GetNumPoints(bool forCalculate);

ENUM_POINTS_FPGA sMemory_IntNumPoints2FPGA_NUM_POINTS(int numPoints);


/** @}  @}
 */
