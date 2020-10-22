#pragma once
#include "common/Keyboard/Controls_c.h"
#include "Settings/SettingsTypes.h"
#include "Settings/SettingsChannel.h"
#include "Settings/SettingsTime.h"
#include "common/Hardware/HAL/HAL_c.h"


class Item;


#define NEED_OPEN_FILE_MANAGER      (gBF.needOpenFileMananger)
#define FLASH_DRIVE_IS_CONNECTED    (gBF.flashDriveIsConnected)
#define NEED_CLOSE_PAGE_SB          (gBF.needClosePageSB)
#define AUTO_FIND_IN_PROGRESS       (gBF.FPGAautoFindInProgress)
#define NEED_RESET_SETTINGS         (gBF.needResetSettings)
#define NEED_FINISH_REDRAW          (gBF.needFinishDraw)
#define NUM_DRAWING_SIGNALS         (gBF.numDrawingSignals)
#define FRAMES_ELAPSED              (gBF.framesElapsed)
#define FPGA_CAN_READ_DATA          (gBF.FPGAcanReadData)
#define FPGA_CRITICAL_SITUATION     (gBF.FPGAcritiacalSituation)
#define TRIG_AUTO_FIND              (gBF.FPGAtrigAutoFind)
#define FPGA_TEMPORARY_PAUSE        (gBF.FPGAtemporaryPause)
#define FPGA_IN_PROCESS_READ        (gBF.FPGAinProcessingOfRead)
#define FPGA_FIRST_AFTER_WRITE      (gBF.FPGAfirstAfterWrite)
#define NEED_REDRAW_FILEMANAGER     (gBF.needRedrawFileManager)
#define CURSORS_IN_DIRS             (gBF.cursorInDirs)
#define SHOW_HELP_HINTS             (gBF.showHelpHints)
#define SHOW_DEBUG_MENU             (gBF.showDebugMenu)


struct BitField
{
    // ��� ��������� : Display::c
    uint needFinishDraw             : 1;    // ���� 1, �� ������� ��������� � �����������
    uint framesElapsed              : 1;
    uint numDrawingSignals          : 8;    // ����� ������������ �������� ��� ������ ����������

    // FPGA
    uint FPGAtrigAutoFind           : 1;    // ������������� � 1 �������� ��������, ��� ����� ����������� �������������� ����� �������������, ���� 
                                            // ������� ��������������� ���������.
    uint FPGAautoFindInProgress     : 1;
    uint FPGAtemporaryPause         : 1;
    uint FPGAinProcessingOfRead     : 1;
    uint FPGAcanReadData            : 1;
    uint FPGAcritiacalSituation     : 1;
    uint FPGAfirstAfterWrite        : 1;    // ������������ � ������ �������������. ����� ������ ������ ��������� � �������
                                            // ����� �� ������������ ������ ��������� ������ � ���, ������ ��� ��� �������� � ������ ������
    // Flash drive
    uint flashDriveIsConnected      : 1;
    uint cursorInDirs               : 1;    // ���� 1, �� ����� ��������� ���������� �� ���������
    uint needRedrawFileManager      : 2;    // ���� 1, �� ����-�������� ��������� � ������ �����������
                                            // ���� 2, �� ������������ ������ ��������
                                            // ���� 3, �� ������������ ������ �����

    uint showHelpHints              : 1;    // ���� 1, �� ��� ������� ������ ������ ���������� � ������ ��������� ��������� � � ����������

    uint showDebugMenu              : 1;
    
    uint needOpenFileMananger       : 1;    // ���� 1, �� ����� ������� �������� �������� (��������� ���������������)
    uint needClosePageSB            : 1;    // ���� 1, ����� ��������� �������� ����� ������
    uint needResetSettings          : 1;    // ���� 1, �� ��������� ����� ��������
};


extern BitField gBF;

#define CURRENT_NUM_LATEST_SIGNAL       (gMemory.currentNumLatestSignal)
#define CURRENT_NUM_INT_SIGNAL          (gMemory.currentNumIntSignal)
#define EXIT_FROM_INT_TO_LAST           (gMemory.exitFromIntToLast)
#define RUN_FPGA_AFTER_SMALL_BUTTONS    (gMemory.runningFPGAbeforeSmallButtons)
#define INT_SHOW_ALWAYS                 (gMemory.showAlways)
#define NEED_SAVE_TO_DRIVE              (gMemory.needForSaveToFlashDrive)

#define EXIT_FROM_SET_NAME_TO           (gMemory.exitFromModeSetNameTo)
#define RETURN_TO_MAIN_MENU             0U
#define RETURN_TO_LAST_MEM              1U
#define RETURN_TO_INT_MEM               2U
#define RETURN_TO_DISABLE_MENU          3U
#define EXIT_FROM_SET_NAME_TO_MAIN_MENU (EXIT_FROM_SET_NAME_TO == RETURN_TO_MAIN_MENU)
#define EXIT_FROM_SET_NAME_TO_LAST      (EXIT_FROM_SET_NAME_TO == RETURN_TO_LAST_MEM)
#define EXIT_FROM_SET_NAME_TO_INT       (EXIT_FROM_SET_NAME_TO == RETURN_TO_INT_MEM)
#define EXIT_FROM_SET_NAME_TO_DIS_MENU  (EXIT_FROM_SET_NAME_TO == RETURN_TO_DISABLE_MENU)

struct GMemory
{
    int16   currentNumLatestSignal;                 // ������� ����� ���������� ������� � ������ ������ - ���������
    int8    currentNumIntSignal;                    // ������� ����� �������, ������������ � ����
    uint    showAlways                      : 1;    // ���� 1, �� ���������� ������ ��������� � ������ "�����. ��" ������
    uint    runningFPGAbeforeSmallButtons   : 1;    // ����� ����������� ���������� � ���, �������� �� ���� ����� ��������� � ����� ������ � �������
    uint    exitFromIntToLast               : 1;    // ���� 1, �� �������� �� �������� ���������� ������ ����� �� ����������, � � ���� ���������
    uint    exitFromModeSetNameTo           : 2;    // ���� ������������ �� ���� ��������� ����� ��� ���������� : 0 - � �������� ����, 1 - � ���� 
                                                    // ���������, 2 - � ���� ����� ��, 3 - � ������� ���� � ����������� ����
    uint    needForSaveToFlashDrive         : 1;    // ���� 1, �� ����� ��������� ����� ��������� �� ������.
};


extern GMemory gMemory;


struct StateCalibration { enum E
{
    None,
    ADCinProgress,
    RShift0start,
    RShift0inProgress,
    RShift1start,
    RShift1inProgress,
    ErrorCalibration0,
    ErrorCalibration1
};};

struct StateWorkFPGA { enum E
{
    Stop,    // ���� - �� ���������� ����������� ����������.
    Wait,    // ��� ����������� ��������������.
    Work,    // ��� ������.
    Pause    // ��� ���������, ����� �������� ������������� ������, ��������, ��� ������ ������ ��� ��� 
             // ������ �������� ���������.
};};


struct StateFPGA
{
    bool needCalibration;				        // ������������� � true �������� ��������, ��� ���������� ���������� ����������.
    StateWorkFPGA::E stateWorkBeforeCalibration;
    StateCalibration::E stateCalibration;          // ������� ��������� ����������. ������������ � �������� ����������.
};

struct DataSettings
{
    void*         addrNext;               // ����� ��������� ������.
    void*         addrPrev;               // ����� ���������� ������.
    uint          rShiftCh0       : 10;   // �������� �� ����������
    uint          rShiftCh1       : 10;
    uint          trigLevCh0      : 10;   // ������� �������������
    int16         tShift;                 // �������� �� �������
    ModeCouple::E modeCouple1     : 2;
    Range::E      range[2];               // ������� �� ���������� ����� �������.

    uint          trigLevCh1      : 10;
    uint          length1channel  : 11;   // ������� �������� � ������ ����� ��������� ������ ������
    TBase::E      tBase           : 5;    // ������� �� �������
    ModeCouple::E modeCouple0     : 2;    // ����� ������ �� �����
    uint          peakDet         : 2;    // ������� �� ������� ��������
    uint          enableCh0       : 1;    // ������� �� ����� 0
    uint          enableCh1       : 1;    // ������� �� ����� 1

    uint          inverseCh0      : 1;
    uint          inverseCh1      : 1;
    Divider::E    multiplier0     : 1;
    Divider::E    multiplier1     : 1;
    PackedTime    time;
};


void SetItemForHint(Item *item);

extern StateFPGA gStateFPGA;

extern uint8        *gData0;    // ��������� �� ������ ������� ������, ������� ���� �������� �� ������
extern uint8        *gData1;    // ��������� �� ������ ������� ������, ������� ���� �������� �� ������
extern DataSettings *gDSet;     // ��������� �� ��������� �������� ��������

extern DataSettings *gDSmemInt;       //--
extern uint8        *gData0memInt;    // | ����� ������ �� ����, ������� ������ ���������� �� �����
extern uint8        *gData1memInt;    //-/

extern DataSettings *gDSmemLast;
extern uint8        *gData0memLast;
extern uint8        *gData1memLast;

extern void *extraMEM;      // ��� ����������� ���������. ������������ ��� ��������� ������ ����������, ������� �� ����� �� ����� ��������� ���������,
                            // �� ����� ������ ��� � ����� �������. ����� �������������� � ������� ������ malloc() ���������� ����������� ����������
                            // ������, ������� ����� ������������� ������ free()

#define MALLOC_EXTRAMEM(NameStruct, name)   extraMEM = std::malloc(sizeof(NameStruct));    \
                                            NameStruct *name = (NameStruct*)extraMEM
#define ACCESS_EXTRAMEM(NameStruct, name)   NameStruct *name = (NameStruct*)extraMEM //-V2546 //-V1003
#define FREE_EXTRAMEM()                     std::free(extraMEM)

extern int transmitBytes;
