// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Colors_.h"
#include "Display/Display.h"
#include "Display/Screen/Grid.h"
#include "Menu/Items/MenuItemsDefs.h"


class Page;
class SmallButton;
class TimeItem;
class Governor;
class Choice;


#define MAX_NUM_CHOICE_SMALL_BUTTON 6    // ������������ ���������� ��������� ��������� ������ + 1

                                            
// ������ ���� ������� ����
struct TypeItem { enum E
{
    None,           
    Choice,        // ����� ������ - ��������� ������� ���� �� ���������� �������� ��������.
    Button,        // ������.
    Page,          // ��������.
    Governor,      // ��������� - ��������� ������� ����� ����� �������� �������� �� ������� ��������� ����������.
    Time,          // ��������� ������ �����.
    IP,            // ��������� ������ IP-�����.
    SwitchButton,
    GovernorColor, // ��������� ������� ����.
    Formula,       // ��������� ������� ���� � ������������ ��� �������������� ������� (��������� � ��������)
    MAC,           // MAC-�����
    ChoiceReg,     // ������� ������, � ������� ����� �������������� �� �������, � ������
    SmallButton,   // ������ ��� ������ ����� ������
    Count
};};


// ����� ID ���� �������������� � ���� �������
struct NamePage { enum E
{
    Empty,
    Display,               // ������
    DisplayAccumulation,   // ������� - ����������
    DisplayAverage,        // ������� - ����������
    DisplayGrid,           // ������� - �����
    DisplayRefresh,        // ������� - ����������
    MainPage,              // ������� �������� ����. �� �� ������ ��� ���������
    Debug,                 // �������
    DebugConsole,          // ������� - �������
    DebugADC,              // ������� - ���
    DebugADCbalance,       // ������� - ��� - ������
    DebugADCstretch,       // ������� - ��� - ��������
    DebugADCrShift,        // ������� - ��� - ���. ��������
    DebugADCaltShift,      // ������� - ��� - ���. ����. ���.
    DebugShowRegisters,    // ������� - ��������
    DebugRandomizer,       // ������� - ����-���
    DebugInformation,      // ������� - ����������
    DebugSound,            // ������� - ����
    DebugAltera,           // ������� - �������
    ChannelA,              // ����� 1
    ChannelB,              // ����� 2
    Cursors,               // �������
    Trig,                  // �����.
    Memory,                // ������
    MemoryExt,             // ������ - �����. ��
    Measures,              // ���������
    MeasuresField,         // ��������� - ����
    Service,               // ������
    ServiceDisplay,        // ������ - �������
    ServiceDisplayColors,  // ������ - ������� - �����
    ServiceCalibrator,     // ������ - ����������
    ServiceEthernet,       // ������ - ETHERNET
    Help,                  // ������
    Time,                  // ���������
    Math,                  // ������ - ����������
    MathFFT,
    TrigAuto,              // ����� - ��� �����
    SB_Curs,               // ������� �������� ������� - ���������
    SB_MeasTuneMeas,       // ������� �������� ��������� - ���������
    SB_MeasTuneField,      // ������� �������� ��������� - ���� - ������
    SB_MemLatest,          // ������� �������� ������ - ���������
    SB_MemInt,             // ������� �������� ������ - ����� ��
    SB_MemExtSetMask,      // ������� �������� ������ - ����� �� - �����
    SB_MemExtSetName,      // ���������� ����� ����������� ����� �� ������ ��� ������� ����� ����� ��� ���������������
                           // ����� ����� �� - ��� �����
    SB_FileManager,        // ������� �������� ������ - ����� �� - �����
    SB_Help,               // ������� �������� HELP
    SB_MathCursorsFFT,     // ������� �������� ������ - ���������� - �������
    SB_MathFunction,       // ������ - ���������� - �������
    SB_Information,        // ������ - ����������
    SB_SerialNumber,       // ������ - �/�
    Count,
    NoPage
};};             // ��� ���������� ������� ����� �������� ������������� SettingsMenu � SSettings.c


#define TITLE(item) ((item)->data->titleHint[LANG])
#define HINT(item) ((item)->data->titleHint[2 + LANG])


class StructHelpSmallButton
{
public:
    pFuncVII funcDrawUGO;    // ��������� �� ������� ��������� ����������� �������� ������
    pchar    helpUGO[2];     // ������� � ������� �����������.
};

typedef StructHelpSmallButton arrayHints[MAX_NUM_CHOICE_SMALL_BUTTON];

#include "Menu/Items/Item.h"
#include "Menu/Items/ItemPage.h"
#include "Menu/Items/ItemButton.h"
#include "Menu/Items/ItemSmallButton.h"
#include "Menu/Items/ItemGovernor.h"
#include "Menu/Items/ItemIPaddress.h"
#include "Menu/Items/ItemMACaddress.h"
#include "Menu/Items/ItemFormula.h"
#include "Menu/Items/ItemGovernorColor.h"
#include "Menu/Items/ItemChoice.h"
#include "Menu/Items/ItemTime.h"
