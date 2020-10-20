#include "defines.h"
#include "common/Display/Primitives_c.h"
#include "common/Hardware/HAL/HAL_c.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/EPROM.h"
#include "Hardware/Sound.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include <cstdio>
#include <cstdlib>


using namespace Primitives;


extern const Page pDebug;


extern const Choice     mcStats;                            // ������� - ����������
extern const Page       mpConsole;                          // ������� - �������
extern const Governor   mgConsole_NumStrings;               // ������� - ������� - ����� �����
extern const Choice     mcConsole_SizeFont;                 // ������� - ������� - ������ ������
extern const Page       mpConsole_Registers;                // ������� - ������� - ��������
extern const Choice     mcConsole_Registers_ShowAll;        // ������� - ������� - �������� - ���������� ���
extern const Choice     mcConsole_Registers_RD_FL;          // ������� - ������� - �������� - RD_FL
static bool      IsActive_Console_Registers_RD_FL();
extern const Choice     mcConsole_Registers_RShiftA;        // ������� - ������� - �������� - U ��. 1�
extern const Choice     mcConsole_Registers_RShiftB;        // ������� - ������� - �������� - U ��. 2�
extern const Choice     mcConsole_Registers_TrigLev;        // ������� - ������� - �������� - U �����.
extern const Choice     mcConsole_Registers_RangeA;         // ������� - ������� - �������� - �����/��� 1
extern const Choice     mcConsole_Registers_RangeB;         // ������� - ������� - �������� - �����/��� 2
extern const Choice     mcConsole_Registers_TrigParam;      // ������� - ������� - �������� - �����. �����.
extern const Choice     mcConsole_Registers_ChanParamA;     // ������� - ������� - �������� - �����. ���. 1
extern const Choice     mcConsole_Registers_ChanParamB;     // ������� - ������� - �������� - �����. ���. 2
extern const Choice     mcConsole_Registers_TBase;          // ������� - ������� - �������� - �����/���
extern const Choice     mcConsole_Registers_tShift;         // ������� - ������� - �������� - � ��.
extern const Page       mpADC;                              // ������� - ���
extern const Page       mpADC_Balance;                      // ������� - ��� - ������
extern const Choice     mcADC_Balance_Mode;                 // ������� - ��� - ������ - �����
static void     OnChanged_ADC_Balance_Mode(bool active);
static void          Draw_ADC_Balance_Mode(int x, int y);
extern const Governor   mgADC_Balance_ShiftA;               // ������� - ��� - ������ - �������� 1
static bool      IsActive_ADC_Balance_Shift();
static void     OnChanged_ADC_Balance_ShiftA();
extern const Governor   mgADC_Balance_ShiftB;               // ������� - ��� - ������ - �������� 2
static void     OnChanged_ADC_Balance_ShiftB();
extern const Page       mpADC_Stretch;                      // ������� - ��� - ��������
extern const Choice     mcADC_Stretch_Mode;                 // ������� - ��� - �������� - �����
static void     OnChanged_ADC_Stretch_Mode(bool active);
void LoadStretchADC(Channel::E chan);
extern const Governor   mgADC_Stretch_ADC_A;                // ������� - ��� - �������� - �����. 1�
static bool      IsActive_ADC_Stretch_ADC();
static void     OnChanged_ADC_Stretch_ADC_A();
extern const Governor   mgADC_Stretch_ADC_B;                // ������� - ��� - �������� - �����. 2�
static void     OnChanged_ADC_Stretch_ADC_B();
extern const Page       mpADC_AltRShift;                    // ������� - ��� - ��� ����
extern const Button     mbADC_AltRShift_Reset;              // ������� - ��� - ��� ���� - �����
static void       OnPress_ADC_AltRShift_Reset();
extern const Governor   mbADC_AltRShift_2mV_DC_A;           // ������� - ��� - ��� ���� - �� 1� 2�� ����
static void     OnChanged_ADC_AltRShift_A();
extern const Governor   mbADC_AltRShift_2mV_DC_B;           // ������� - ��� - ��� ���� - �� 2� 2�� ����
static void     OnChanged_ADC_AltRShift_B();
extern const Governor   mbADC_AltRShift_5mV_DC_A;           // ������� - ��� - ��� ���� - �� 1� 5�� ����
extern const Governor   mbADC_AltRShift_5mV_DC_B;           // ������� - ��� - ��� ���� - �� 2� 5�� ����
extern const Governor   mbADC_AltRShift_10mV_DC_A;          // ������� - ��� - ��� ���� - �� 1� 10�� ����
extern const Governor   mbADC_AltRShift_10mV_DC_B;          // ������� - ��� - ��� ���� - �� 2� 10�� ����
extern const Page       mpRandomizer;                       // ������� - ����-���
extern const Governor   mgRandomizer_SamplesForGates;       // ������� - ����-��� - ���-�/������
static void     OnChanged_Randomizer_SamplesForGates();
extern const Governor   mgRandomizer_AltTShift0;            // ������� - ����-��� - tShift ���.
static void     OnChanged_Randomizer_AltTShift0();
extern const Governor   mgRandomizer_Average;               // ������� - ����-��� - ������.
extern const Choice     mcSizeSettings;                     // ������� - ������ ��������
static void        OnDraw_SizeSettings(int x, int y);
extern const Button     mbSaveFirmware;                     // ������� - ����. ��������
static bool      IsActive_SaveFirmware();
static void       OnPress_SaveFirmware();
extern const       Page ppSerialNumber;                     // ������� - �/�
static void       OnPress_SerialNumber();
static void Draw_EnterSerialNumber();
static void      OnRegSet_SerialNumber(int);
extern const SmallButton bSerialNumber_Exit;                // ������� - �/� - �����
static void       OnPress_SerialNumber_Exit();
extern const SmallButton bSerialNumber_Change;              // ������� - �/� - �������
static void       OnPress_SerialNumber_Change();
static void          Draw_SerialNumber_Change(int, int);
extern const  SmallButton bSerialNumber_Save;               // ������� - �/� - ���������
static void        OnPress_SerialNumber_Save();
static void           Draw_SerialNumber_Save(int, int);
extern const       Button bEraseData;                       // ������� - ������� ������
static void       OnPress_EraseData();


// � ���� ��������� ����� ��������� ������ ��������� ������ ��� �������� �������� ppSerialNumer
struct StructForSN
{
    int number;     // ��������������, ���������� �����.
    int year;       // ��������������, ���.
    int curDigt;    // ��������������, ������� (0) ��� ����� (1) ��������� ����� ���������.
};


const Page *PageDebug::SerialNumber::self = &ppSerialNumber;


// ������� /////////////////////////
DEF_PAGE_7(pDebug, PageMain::self, NamePage::Debug,
    "�������", "DEBUG",
    "",
    "",
    mcStats,            // ������� - ����������
    mpConsole,          // ������� - �������
    mpADC,              // ������� - ���
    mpRandomizer,       // ������� - ����-���
    mcSizeSettings,     // ������� - ������ ��������
    mbSaveFirmware,     // ������� - ����. ��������
    bEraseData,         // ������� - ������� ������
    nullptr, nullptr, nullptr, nullptr
);


const Page *PageDebug::self = &pDebug;


// ������� - ���������� ------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcStats, &pDebug,
    "����������", "Statistics",
    "����������/�� ���������� �����/����, ������ � �������, ���������� �������� � ���������� ����������� � ������/���������� ����������� � ������ ��������",
    "To show/not to show a time/shot, frames per second, quantity of signals with the last settings in memory/quantity of the signals kept in memory",
    "�� ����������", "Hide",
    "����������",    "Show",
    SHOW_STATS, nullptr, nullptr, nullptr
)


// ������� - ������� ///////////////
DEF_PAGE_3(mpConsole, pDebug, NamePage::DebugConsole,
    "�������", "CONSOLE",
    "",
    "",
    mgConsole_NumStrings,   // ������� - ������� - ����� �����
    mcConsole_SizeFont,     // ������� - ������� - ������ ������
    mpConsole_Registers,    // ������� - ������� - ��������
    nullptr, nullptr, nullptr, nullptr
);

// ������� - ������� - ����� ����� -------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgConsole_NumStrings, mpConsole,
    "����� �����", "Number strings",
    "",
    "",
    NUM_STRINGS,  0, 33, nullptr, nullptr, nullptr
)

// ������� - ������� - ������ ������ -----------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_SizeFont, &mpConsole,
    "������ ������", "Size font",
     "",
     "",
     "5", "5",
     "8", "8",
    SIZE_FONT_CONSOLE, nullptr, nullptr, nullptr
)


// ������� - ������� - �������� ////
DEF_PAGE_12(mpConsole_Registers, mpConsole, NamePage::DebugShowRegisters,
    "��������", "REGISTERS",
    "",
    "",
    mcConsole_Registers_ShowAll,    // ������� - ������� - �������� - ���������� ���
    mcConsole_Registers_RD_FL,      // ������� - ������� - �������� - RD_FL
    mcConsole_Registers_RShiftA,    // ������� - ������� - �������� - U ��. 1�
    mcConsole_Registers_RShiftB,    // ������� - ������� - �������� - U ��. 2�
    mcConsole_Registers_TrigLev,    // ������� - ������� - �������� - U �����.
    mcConsole_Registers_RangeA,     // ������� - ������� - �������� - �����/��� 1
    mcConsole_Registers_RangeB,     // ������� - ������� - �������� - �����/��� 2
    mcConsole_Registers_TrigParam,  // ������� - ������� - �������� - �����. �����.
    mcConsole_Registers_ChanParamA, // ������� - ������� - �������� - �����. ���. 1
    mcConsole_Registers_ChanParamB, // ������� - ������� - �������� - �����. ���. 2
    mcConsole_Registers_TBase,      // ������� - ������� - �������� - �����/���
    mcConsole_Registers_tShift,      // ������� - ������� - �������� - � ��.
    nullptr, nullptr, nullptr, nullptr
)

// ������� - ������� - �������� - ���������� ��� -----------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_ShowAll, &mpConsole_Registers,
    "���������� ���", "Show all",
    "���������� ��� ��������, ���������� � ��������",
    "To show all values transferred in registers",
    "���", "No",
    "��",  "Yes",
    IS_SHOW_REGISTERS_ALL, nullptr, nullptr, nullptr
)

// ������� - ������� - �������� - RD_FL --------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_RD_FL, &mpConsole_Registers, 
    "RD_FL", "RD_FL",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.flag, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

static bool IsActive_Console_Registers_RD_FL(void)
{
    return !IS_SHOW_REGISTERS_ALL;
}

// ������� - ������� - �������� - U ��. 1� -----------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_RShiftA, &mpConsole_Registers,
    "U ��. 1�", "U shift 1ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.rShiftA, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

// ������� - ������� - �������� - U ��. 2� -----------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_RShiftB, &mpConsole_Registers,
    "U ��. 2�", "U shift 2ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.rShiftB, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

// ������� - ������� - �������� - U �����. -----------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_TrigLev, &mpConsole_Registers,
    "U �����.", "U trig.",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.trigLev, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

// ������� - ������� - �������� - �����/��� 1 --------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_RangeA, &mpConsole_Registers,
    "�����/��� 1", "Range 1",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.range[0], IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

// ������� - ������� - �������� - �����/��� 2 --------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_RangeB, &mpConsole_Registers,
    "�����/��� 2", "Range 2",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.range[1], IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

// ������� - ������� - �������� - �����. �����. ------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_TrigParam, &mpConsole_Registers,
    "�����. �����.", "Trig param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.trigParam, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

// ������� - ������� - �������� - �����. ���. 1 ------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_ChanParamA, &mpConsole_Registers,
    "�����. ���. 1",  "Chan 1 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.chanParam[0], IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

// ������� - ������� - �������� - �����. ���. 2 ------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_ChanParamB, &mpConsole_Registers,
    "�����. ���. 2", "Chan 2 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.chanParam[1], IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

// ������� - ������� - �������� - �����/��� ----------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_TBase, &mpConsole_Registers,
    "�����/���", "TBase",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.tBase, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

// ������� - ������� - �������� - � ��. --------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_tShift, &mpConsole_Registers,
    "� ��.", "tShift",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.tShift, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)


// ������� - ��� ///////////////////
DEF_PAGE_3(mpADC, pDebug, NamePage::DebugADC,
    "���", "ADC",
    "",
    "",
    mpADC_Balance,      // ������� - ��� - ������
    mpADC_Stretch,      // ������� - ��� - ��������
    mpADC_AltRShift,    // ������� - ��� - ��� ����
    nullptr, nullptr, nullptr, nullptr
)

// ������� - ��� - ������ //////////
DEF_PAGE_3(mpADC_Balance, mpADC, NamePage::DebugADCbalance,
    "������", "BALANCE",
    "",
    "",
    mcADC_Balance_Mode,     // ������� - ��� - ������ - �����
    mgADC_Balance_ShiftA,   // ������� - ��� - ������ - �������� 1
    mgADC_Balance_ShiftB,   // ������� - ��� - ������ - �������� 2
    nullptr, nullptr, nullptr, nullptr
)

// ������� - ��� - ������ - ����� --------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(mcADC_Balance_Mode, &mpADC_Balance,
    "�����", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "��������", "Real",
    "������",   "Manual",
    BALANCE_ADC_TYPE, nullptr, OnChanged_ADC_Balance_Mode, Draw_ADC_Balance_Mode
)

static int16 shiftADCA;
static int16 shiftADCB;

static void OnChanged_ADC_Balance_Mode(bool)
{
    Draw_ADC_Balance_Mode(0, 0);

    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)shiftADCA, false);
    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)shiftADCB, false);
}

static void Draw_ADC_Balance_Mode(int, int)
{
    int8 shift[2][3] =
    {
        {0, SET_BALANCE_ADC_A, static_cast<int8>(BALANCE_ADC_A)},
        {0, SET_BALANCE_ADC_B, static_cast<int8>(BALANCE_ADC_B)}
    };

    shiftADCA = shift[0][BALANCE_ADC_TYPE];
    shiftADCB = shift[1][BALANCE_ADC_TYPE];
}

// ������� - ��� - ������ - �������� 1 ---------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgADC_Balance_ShiftA, mpADC_Balance, 
    "�������� 1", "Offset 1",
    "",
    "",
    shiftADCA, -125, 125, IsActive_ADC_Balance_Shift, OnChanged_ADC_Balance_ShiftA, nullptr
)

static void OnChanged_ADC_Balance_ShiftA(void)
{
    BALANCE_ADC_A = shiftADCA;
    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)BALANCE_ADC_A, false);
}

static bool IsActive_ADC_Balance_Shift(void)
{
    return BALANCE_ADC_TYPE_IS_HAND;
}

// ������� - ��� - ������ - �������� 2----------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgADC_Balance_ShiftB, mpADC_Balance,
    "�������� 2", "Offset 2",
    "",
    "",
    shiftADCB, -125, 125, IsActive_ADC_Balance_Shift, OnChanged_ADC_Balance_ShiftB, nullptr
)

static void OnChanged_ADC_Balance_ShiftB(void)
{
    BALANCE_ADC_B = shiftADCB;
    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)BALANCE_ADC_B, false);
}


// ������� - ��� - �������� ////////
DEF_PAGE_3(mpADC_Stretch, mpADC, NamePage::DebugADCstretch,
    "��������", "STRETCH",
    "",
    "",
    mcADC_Stretch_Mode,     // ������� - ��� - �������� - �����
    mgADC_Stretch_ADC_A,    // ������� - ��� - �������� - �����. 1�
    mgADC_Stretch_ADC_B,    // ������� - ��� - �������� - �����. 2�    
    nullptr, nullptr, nullptr, nullptr
)

// ������� - ��� - �������� - ����� ------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(mcADC_Stretch_Mode, &mpADC_Stretch,
    "�����", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "��������", "Real",
    "������",   "Manual",
    DEBUG_STRETCH_ADC_TYPE, nullptr, OnChanged_ADC_Stretch_Mode, nullptr
)

static void OnChanged_ADC_Stretch_Mode(bool active)
{
    if(active)
    {
        LoadStretchADC(Channel::A);
        LoadStretchADC(Channel::B);
    }
}

void LoadStretchADC(Channel::E chan)
{
    if(DEBUG_STRETCH_ADC_TYPE_IS_DISABLED)
    {
        FPGA::WriteToHardware(chan == Channel::A ? WR_CAL_A : WR_CAL_B, 0x80, true);
    }
    else if(DEBUG_STRETCH_ADC_TYPE_IS_HAND)
    {
        FPGA::WriteToHardware(chan == Channel::A ? WR_CAL_A : WR_CAL_B, (uint8)DEBUG_STRETCH_ADC(chan), true);
    }
    else if(DEBUG_STRETCH_ADC_TYPE_IS_SETTINGS)
    {
        FPGA::LoadKoeffCalibration(chan);
    }
}

// ������� - ��� - �������� - �����. 1� --------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgADC_Stretch_ADC_A, mpADC_Stretch,
    "�����. 1�", "Koeff. 1ch",
    "",
    "",
    DEBUG_STRETCH_ADC_A, 0, 255, IsActive_ADC_Stretch_ADC, OnChanged_ADC_Stretch_ADC_A, nullptr
)

static bool IsActive_ADC_Stretch_ADC(void)
{
    return DEBUG_STRETCH_ADC_TYPE_IS_HAND;
}

static void OnChanged_ADC_Stretch_ADC_A(void)
{
    FPGA::WriteToHardware(WR_CAL_A, (uint8)DEBUG_STRETCH_ADC_A, true);
}


// ������� - ��� - �������� - �����. 2� --------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgADC_Stretch_ADC_B, mpADC_Stretch,
    "�����. 2�", "Koeff. 2ch",
    "",
    "",
    DEBUG_STRETCH_ADC_B, 0, 255, IsActive_ADC_Stretch_ADC, OnChanged_ADC_Stretch_ADC_B, nullptr
)

static void OnChanged_ADC_Stretch_ADC_B(void)
{
    FPGA::WriteToHardware(WR_CAL_B, (uint8)DEBUG_STRETCH_ADC_B, true);
}

// ������� - ��� - ��� ���� ////////
DEF_PAGE_7(mpADC_AltRShift, &mpADC, NamePage::DebugADCrShift,
    "��� ����", "ADD RSHFIT",
    "",
    "",
    mbADC_AltRShift_Reset,          // ������� - ��� - ��� ���� - �����
    mbADC_AltRShift_2mV_DC_A,       // ������� - ��� - ��� ���� - �� 1� 2�� ����
    mbADC_AltRShift_2mV_DC_B,       // ������� - ��� - ��� ���� - �� 2� 2�� ����
    mbADC_AltRShift_5mV_DC_A,       // ������� - ��� - ��� ���� - �� 1� 5�� ����
    mbADC_AltRShift_5mV_DC_B,       // ������� - ��� - ��� ���� - �� 2� 5�� ����
    mbADC_AltRShift_10mV_DC_A,      // ������� - ��� - ��� ���� - �� 1� 10�� ����
    mbADC_AltRShift_10mV_DC_B,      // ������� - ��� - ��� ���� - �� 2� 10�� ����
    nullptr, nullptr, nullptr, nullptr
)

// ������� - ��� - ��� ���� - ����� ------------------------------------------------------------------------------------------------------------------
DEF_BUTTON(mbADC_AltRShift_Reset, mpADC_AltRShift,
    "�����", "Reset",
    "", "",
    nullptr, OnPress_ADC_AltRShift_Reset
)

static void OnPress_ADC_AltRShift_Reset(void)
{
    for(int chan = 0; chan < 2; chan++)
    {
        for(int mode = 0; mode < 2; mode++)
        {
            for(int range = 0; range < Range::Count; range++)
            {
                RSHIFT_ADD(chan, range, mode) = 0;
            }
        }
    }
    FPGA::SetRShift(Channel::A, SET_RSHIFT_A);
    FPGA::SetRShift(Channel::B, SET_RSHIFT_B);
}

// ������� - ��� - ��� ���� - �� 1� 2�� ���� ---------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mbADC_AltRShift_2mV_DC_A, mpADC_AltRShift,
    "�� 1� 2�� ����", "Shift 1ch 2mV DC",
    "",
    "",
    set.chan[Channel::A].rShiftAdd[Range::_2mV][ModeCouple::DC], -100, 100, nullptr, OnChanged_ADC_AltRShift_A, nullptr
)

static void OnChanged_ADC_AltRShift_A(void)
{
    FPGA::SetRShift(Channel::A, SET_RSHIFT_A);
}

// ������� - ��� - ��� ���� - �� 2� 2�� ���� ---------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mbADC_AltRShift_2mV_DC_B, mpADC_AltRShift,
    "�� 2� 2�� ����", "Shift 2ch 2mV DC",
    "",
    "",
    RSHIFT_ADD(Channel::B, Range::_2mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_B, nullptr
)

static void OnChanged_ADC_AltRShift_B(void)
{
    FPGA::SetRShift(Channel::B, SET_RSHIFT_B);
}

// ������� - ��� - ��� ���� - �� 1� 5�� ���� ---------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mbADC_AltRShift_5mV_DC_A, mpADC_AltRShift,
    "�� 1� 5�� ����", "Shift 1ch 5mV DC",
    "",
    "",
    RSHIFT_ADD(Channel::A, Range::_5mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_A, nullptr
)

// ������� - ��� - ��� ���� - �� 2� 5�� ���� ---------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mbADC_AltRShift_5mV_DC_B, mpADC_AltRShift,
    "�� 2� 5�� ����", "Shift 2ch 5mV DC",
    "",
    "",
    RSHIFT_ADD(Channel::B, Range::_5mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_B, nullptr
)

// ������� - ��� - ��� ���� - �� 1� 10�� ���� --------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mbADC_AltRShift_10mV_DC_A, mpADC_AltRShift,
    "�� 1� 10�� ����", "Shift 1ch 10mV DC",
    "",
    "",
    RSHIFT_ADD(Channel::A, Range::_10mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_A, nullptr
)

// ������� - ��� - ��� ���� - �� 2� 10�� ���� --------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mbADC_AltRShift_10mV_DC_B, mpADC_AltRShift,
    "�� 2� 10�� ����", "Shift 2ch 10mV DC",
    "",
    "",
    RSHIFT_ADD(Channel::B, Range::_10mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_B, nullptr
)


// ������� - ����-��� //////////////
DEF_PAGE_3(mpRandomizer, pDebug, NamePage::DebugRandomizer,
    "����-���", "RANDOMIZER",
    "",
    "",
    mgRandomizer_SamplesForGates,   // ������� - ����-��� - ���-�/������
    mgRandomizer_AltTShift0,        // ������� - ����-��� - tShift ���.
    mgRandomizer_Average,           // ������� - ����-��� - ������.
    nullptr, nullptr, nullptr, nullptr
)

// ������� - ����-��� - ���-�/������ -----------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgRandomizer_SamplesForGates, mpRandomizer,
    "���-�/������", "Samples/gates",
    "",
    "",
    NUM_MEAS_FOR_GATES, 1, 2500, nullptr, OnChanged_Randomizer_SamplesForGates, nullptr
)

static void OnChanged_Randomizer_SamplesForGates(void)
{
    FPGA::SetNumberMeasuresForGates(NUM_MEAS_FOR_GATES);
}

// ������� - ����-��� - tShift ���. ------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgRandomizer_AltTShift0, mpRandomizer,
    "tShift ���.", "tShift alt.",
    "",
    "",
    ADD_SHIFT_T0, 0, 510, nullptr, OnChanged_Randomizer_AltTShift0, nullptr
)

static void OnChanged_Randomizer_AltTShift0(void)
{
    FPGA::SetDeltaTShift(ADD_SHIFT_T0);
}

// ������� - ����-��� - ������. ----------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgRandomizer_Average, mpRandomizer,
    "������.", "Average",
    "",
    "",
    NUM_AVE_FOR_RAND, 1, 32, nullptr, nullptr, nullptr
)


// ������� - ������ ��������  ------------------------------------------------------------------------------------------------------------------------
static int8 size = 0;
DEF_CHOICE_2(mcSizeSettings, &pDebug,
    "������ ��������", "Size settings",
    "����� ������� ��������� Settings",
    "Show size of struct Settings",
    "������", "Size",
    "������", "Size",
    size, nullptr, nullptr, OnDraw_SizeSettings
)

static void OnDraw_SizeSettings(int x, int y)
{
    Painter::DrawFormatText(x + 5, y + 21, Color::BLACK, "������ %d", sizeof(Settings));
}


// ������� - ����. �������� --------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON(mbSaveFirmware, pDebug,
    "����. ��������", "Save firmware",
    "���������� �������� - �������� 5, 6, 7 ����� ������� 3 � 128 ��, ��� �������� ���������",
    "Saving firmware - sectors 5, 6, 7 with a total size of 3 x 128 kB, where the program is stored",
    IsActive_SaveFirmware, OnPress_SaveFirmware
)

static bool IsActive_SaveFirmware()
{
    return FLASH_DRIVE_IS_CONNECTED == 1;
}

static void OnPress_SaveFirmware()
{
    StructForWrite structForWrite;

    FDrive::OpenNewFileForWrite("S8-53.bin", &structForWrite);

    uint8 *address = (uint8*)0x08020000; //-V566
    uint8 *endAddress = address + 128 * 1024 * 3;

    int sizeBlock = 512;

    while (address < endAddress)
    {
        FDrive::WriteToFile(address, sizeBlock, &structForWrite);
        address += sizeBlock;
    }

    FDrive::CloseFile(&structForWrite);

    Display::ShowWarningGood(Warning::FirmwareSaved);
}


DEF_BUTTON(bEraseData, pDebug,
    "������� ������", "Erase data",
    "������� ������� � �������",
    "Erases data sectors",
    nullptr, OnPress_EraseData
)

static void OnPress_EraseData()
{
    EPROM::EraseData();
}


// ������� - �/� ///////////////////
DEF_PAGE_6(ppSerialNumber, &pDebug, NamePage::SB_SerialNumber,
    "�/�", "S/N",
    "������ ��������� ������ � OTP-������. ��������!!! ��P-������ - ������ � ����������� �������.",
    "Serial number recording in OTP-memory. ATTENTION!!! OTP memory is a one-time programming memory.",
    bSerialNumber_Exit,     // ������� - �/� - �����
    bSerialNumber_Change,   // ������� - �/� - �������
    Item::empty,
    Item::empty,
    Item::empty,
    bSerialNumber_Save,      // ������� - �/� - ���������    
    nullptr, OnPress_SerialNumber, nullptr, OnRegSet_SerialNumber
)

static void OnPress_SerialNumber(void)
{
    PageDebug::SerialNumber::self->OpenAndSetItCurrent();
    Display::SetAddDrawFunction(Draw_EnterSerialNumber);
    MALLOC_EXTRAMEM(StructForSN, s);
    s->number = 1; //-V522
    s->year = 2017;
    s->curDigt = 0;
}

static void Draw_EnterSerialNumber(void)
{
    int x0 = Grid::Left() + 40;
    int y0 = Grid::TOP + 20;
    int width = Grid::Width() - 80;
    int height = 160;

    Primitives::Rectangle(width, height).Draw(x0, y0, COLOR_FILL);
    Region(width - 2, height - 2).Fill(x0 + 1, y0 + 1, COLOR_BACK);

    int deltaX = 10;

    ACCESS_EXTRAMEM(StructForSN, s);

    bool selNumber = s->curDigt == 0;

    char buffer[20];
    std::snprintf(buffer, 19, "%02d", s->number);

    Color::E colorText = COLOR_FILL;
    Color::E colorBackground = COLOR_BACK;

    if (selNumber)
    {
        colorText = Color::FLASH_01;
        colorBackground = Color::FLASH_10;
    }

    int y = y0 + 50;

    Color::SetCurrent(colorText);
    int x = Painter::DrawTextOnBackground(x0 + deltaX, y, buffer, colorBackground);

    colorText = Color::FLASH_01;
    colorBackground = Color::FLASH_10;

    if (selNumber)
    {
        colorText = COLOR_FILL;
        colorBackground = COLOR_BACK;
    }

    std::snprintf(buffer, 19, "%04d", s->year);

    Color::SetCurrent(colorText);
    Painter::DrawTextOnBackground(x + 5, y, buffer, colorBackground);

    // ������ ������� ���������� �� ���������� ����� � OTP-������ ��� ������

    int allShots = HAL_OTP::GetSerialNumber(buffer);

    Painter::DrawFormatText(x0 + deltaX, y0 + 130, COLOR_FILL, "������� ���������� ����� %s", buffer[0] == 0 ? "-- ----" : buffer);

    Painter::DrawFormatText(x0 + deltaX, y0 + 100, COLOR_FILL, "�������� ����� ��� %d �������", allShots);
}

static void OnRegSet_SerialNumber(int angle)
{
    typedef int(*pFunc)(int*, int, int);

    pFunc p = angle > 0 ? CircleIncreaseInt : CircleDecreaseInt;

    ACCESS_EXTRAMEM(StructForSN, s);

    if (s->curDigt == 0)
    {
        p(&s->number, 1, 99);
    }
    else
    {
        p(&s->year, 2014, 2050);
    }
    Sound::GovernorChangedValue();
}

// ������� - �/� - ����� -----------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(bSerialNumber_Exit, ppSerialNumber,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu",
    nullptr, OnPress_SerialNumber_Exit, DrawSB_Exit, nullptr
)

static void OnPress_SerialNumber_Exit(void)
{
    Display::RemoveAddDrawFunction();
    FREE_EXTRAMEM();
}

// ������� - �/� - �������� --------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(bSerialNumber_Change, ppSerialNumber,
    "��������", "Insert",
    "��������� �������� ������",
    "Inserts the chosen symbol",
    nullptr, OnPress_SerialNumber_Change, Draw_SerialNumber_Change, nullptr
)

static void OnPress_SerialNumber_Change(void)
{
    ACCESS_EXTRAMEM(StructForSN, s);
    ++s->curDigt;
    s->curDigt %= 2;
    Painter::ResetFlash();
}

static void Draw_SerialNumber_Change(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, SYMBOL_TAB);
    Font::Set(TypeFont::_8);
}

// ������� - �/� - ��������� -------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(bSerialNumber_Save, ppSerialNumber,
    "���������", "Save",
    "���������� �������� ����� � OTP",
    "Records the serial number in OTP",
    nullptr, OnPress_SerialNumber_Save, Draw_SerialNumber_Save, nullptr
)

static void OnPress_SerialNumber_Save(void)
{
    ACCESS_EXTRAMEM(StructForSN, s);

    char stringSN[20];

    std::snprintf(stringSN, 19, "%02d %04d", s->number, s->year);

    if (!HAL_OTP::SaveSerialNumber(stringSN))
    {
        Display::ShowWarningBad(Warning::FullyCompletedOTP);
    }
}

static void Draw_SerialNumber_Save(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_SAVE_TO_MEM);
    Font::Set(TypeFont::_8);
}



/** @}  @}
 */

        /*
        static void OnChangeRShiftADC(bool active)
        {
            FPGA::SetRShift(Channel::A, SET_RSHIFT_A);
            FPGA::SetRShift(Channel::B, SET_RSHIFT_B);
        }

        const Choice mcDebugADCrShiftMode =
        {
            TypeItem::Choice, &mpADC_AltRShift, {"�����",      "Mode"},
            {
                "",
                ""
            },
            EmptyFuncBV,
            {                                    {DISABLE_RU,   DISABLE_EN},
                                                 {"��������",   "Real"},
                                                 {"������",     "Hand"}
            },
            (int8*)&set.debug.rShiftADCtype, OnChangeRShiftADC
        };

        // ������� - ��� - ���. �������� - �������� 1� 2��
        static TimeStructGovernor tsRShiftADC;
        static bool IsActiveRShiftADC(void)
        {
            return set.debug.rShiftADCtype == RShiftADC_Hand;
        }

        static void OnChangeRShiftADC0(void)
        {
            FPGA::SetRShift(Channel::A, SET_RSHIFT_A);
        }

        const Governor mgDebugRShiftADC00 =
        {
            &mpADC_AltRShift,
            {
                "���� 1� 2��", "Shift 1ch"
            },
            {
                "",
                ""
            },
            IsActiveRShiftADC,
            &set.debug.rShiftADC[0][0], -100, 100, &tsRShiftADC, OnChangeRShiftADC0
        };

        static TimeStructGovernor tsRShiftADC01;
        const Governor mgDebugRShiftADC01 =
        {
            &mpADC_AltRShift,
            {
                "���� 1� 500��", "Shift 1ch 500mV"
            },
            {
                "",
                ""
            },
            IsActiveRShiftADC,
            &set.debug.rShiftADC[1][0], -100, 100, &tsRShiftADC01, OnChangeRShiftADC0
        };

        static void OnChangeRShiftADC1(void)
        {
            FPGA::SetRShift(Channel::B, SET_RSHIFT_B);
        }

        // ������� - ��� - ���. ����. ���.
const Page mspDebugADCaltShift;

// ������� - ��� - ���. ����. ���. - ��������
const Governor mgDebugADCaltShift =
{
    &mspDebugADCaltShift, 0,
    {
        "��������", "Value",
        "",
        ""
    },
    &set.debug.altShift, -2, 2
};

// ������� - ��� - ��� ���� ��� //////
const Page mspDebugADCaltShift =
{
    TypeItem::Page, &mpADC, 0,
    {
        "��� ���� ���", "ALT SHIFT MEM",
        "",
        ""
    },
    NamePage::DebugADCaltShift,
    {
        (void*)&mgDebugADCaltShift
    }
};

const Governor mgDebugInfoNumber =
{
    &mspDebugInformation,
    {
        "�����", "Number"
    },
    {
        "��������� ������ ��������� ������",
        "Installation numbers of the serial number"
    },
    EmptyFuncBV,
    &set.debug.infoNumber, 0, 100
};

const Governor mgDebugInfoYear =
{
    &mspDebugInformation,
    {
        "���", "Year"
    },
    {
        "��������� ���� ��������� ������",
        "Installation year serial number"
    },
    EmptyFuncBV,
    &set.debug.infoYear, 2015, 2050
};

const Governor mgDebugInfoVersion =
{
    &mspDebugInformation,
    {
        "������", "Version"
    },
    {
        "��������� ������ ������",
        "Setting the version number"
    },
    EmptyFuncBV,
    &set.debug.infoVer, 10, 100
};

// ������� - ���������� /////////////////////
const Page mspDebugInformation =
{
    TypeItem::Page, &pDebug,
    {
        "����������", "INFORMATION",
    },
    {
        "��������� ������ ���������� ��� ���� ������-����������",
        "It allows you to enter information for SERVICE INFORMATION menu"
    },
    EmptyFuncBV, NamePage::DebugInformation,
    {
        (void*)&mgDebugInfoNumber,
        (void*)&mgDebugInfoYear,
        (void*)&mgDebugInfoVersion
    }
};
*/
