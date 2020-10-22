#pragma once
#include "common/Keyboard/Controls_c.h"
#include "Settings/SettingsTypes.h"
#include "Settings/SettingsChannel.h"
#include "Settings/SettingsTime.h"
#include "common/Hardware/HAL/HAL_c.h"


class Item;


#define EXIT_FROM_INT_TO_LAST           (gMemory.exitFromIntToLast)
#define RUN_FPGA_AFTER_SMALL_BUTTONS    (gMemory.runningFPGAbeforeSmallButtons)
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
