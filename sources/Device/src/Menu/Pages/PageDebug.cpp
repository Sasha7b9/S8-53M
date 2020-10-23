#include "defines.h"
#include "common/Display/Painter_c.h"
#include "common/Display/Primitives_c.h"
#include "common/Display/Text_c.h"
#include "common/Hardware/Sound_c.h"
#include "common/Hardware/HAL/HAL_c.h"
#include "Display/Grid.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/EPROM.h"
#include "Hardware/RAM.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include <cstdio>
#include <cstdlib>


using namespace Primitives;

static int16 shiftADCA;
static int16 shiftADCB;
static int8 size = 0;


// � ���� ��������� ����� ��������� ������ ��������� ������ ��� �������� �������� ppSerialNumer
struct StructForSN
{
    int number;     // ��������������, ���������� �����.
    int year;       // ��������������, ���.
    int curDigt;    // ��������������, ������� (0) ��� ����� (1) ��������� ����� ���������.
};


DEF_CHOICE_2(mcStats, PageDebug::self,
    "����������", "Statistics",
    "����������/�� ���������� �����/����, ������ � �������, ���������� �������� � ���������� ����������� � ������/���������� ����������� � ������ ��������",
    "To show/not to show a time/shot, frames per second, quantity of signals with the last settings in memory/quantity of the signals kept in memory",
    "�� ����������", "Hide",
    "����������", "Show",
    SHOW_STATS, nullptr, nullptr, nullptr
)

DEF_GOVERNOR(mgConsole_NumStrings, PageDebug::PageConsole::self,
    "����� �����", "Number strings",
    "",
    "",
    NUM_STRINGS, 0, 33, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_SizeFont, PageDebug::PageConsole::self,
    "������ ������", "Size font",
    "",
    "",
    "5", "5",
    "8", "8",
    SIZE_FONT_CONSOLE, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_ShowAll, PageDebug::PageConsole::PageRegisters::self,
    "���������� ���", "Show all",
    "���������� ��� ��������, ���������� � ��������",
    "To show all values transferred in registers",
    "���", "No",
    "��", "Yes",
    IS_SHOW_REGISTERS_ALL, nullptr, nullptr, nullptr
)

static bool IsActive_Console_Registers_RD_FL(void)
{
    return !IS_SHOW_REGISTERS_ALL;
}

DEF_CHOICE_2(mcConsole_Registers_RD_FL, PageDebug::PageConsole::PageRegisters::self,
    "RD_FL", "RD_FL",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.debug.showRegisters.flag, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_RShiftA, PageDebug::PageConsole::PageRegisters::self,
    "U ��. 1�", "U shift 1ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.debug.showRegisters.rShiftA, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_RShiftB, PageDebug::PageConsole::PageRegisters::self,
    "U ��. 2�", "U shift 2ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.debug.showRegisters.rShiftB, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_TrigLev, PageDebug::PageConsole::PageRegisters::self,
    "U �����.", "U trig.",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.debug.showRegisters.trigLev, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_RangeA, PageDebug::PageConsole::PageRegisters::self,
    "�����/��� 1", "Range 1",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.debug.showRegisters.range[0], IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_RangeB, PageDebug::PageConsole::PageRegisters::self,
    "�����/��� 2", "Range 2",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.debug.showRegisters.range[1], IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_TrigParam, PageDebug::PageConsole::PageRegisters::self,
    "�����. �����.", "Trig param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.debug.showRegisters.trigParam, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_ChanParamA, PageDebug::PageConsole::PageRegisters::self,
    "�����. ���. 1", "Chan 1 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.debug.showRegisters.chanParam[0], IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_ChanParamB, PageDebug::PageConsole::PageRegisters::self,
    "�����. ���. 2", "Chan 2 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.debug.showRegisters.chanParam[1], IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_TBase, PageDebug::PageConsole::PageRegisters::self,
    "�����/���", "TBase",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.debug.showRegisters.tBase, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_tShift, PageDebug::PageConsole::PageRegisters::self,
    "� ��.", "tShift",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.debug.showRegisters.tShift, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

DEF_PAGE_12(pageRegisters, PageDebug::PageConsole::self, NamePage::DebugShowRegisters,
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

DEF_PAGE_3(pageConsole, PageDebug::self, NamePage::DebugConsole,
    "�������", "CONSOLE",
    "",
    "",
    mgConsole_NumStrings,                           // ������� - ������� - ����� �����
    mcConsole_SizeFont,                             // ������� - ������� - ������ ������
    *PageDebug::PageConsole::PageRegisters::self,   // ������� - ������� - ��������
    nullptr, nullptr, nullptr, nullptr
);

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

static void OnChanged_ADC_Balance_Mode(bool)
{
    Draw_ADC_Balance_Mode(0, 0);

    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)shiftADCA, false);
    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)shiftADCB, false);
}

DEF_CHOICE_3(mcADC_Balance_Mode, PageDebug::PageADC::PageBalance::self,
    "�����", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "��������", "Real",
    "������", "Manual",
    BALANCE_ADC_TYPE, nullptr, OnChanged_ADC_Balance_Mode, Draw_ADC_Balance_Mode
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

DEF_GOVERNOR(mgADC_Balance_ShiftA, PageDebug::PageADC::PageBalance::self,
    "�������� 1", "Offset 1",
    "",
    "",
    shiftADCA, -125, 125, IsActive_ADC_Balance_Shift, OnChanged_ADC_Balance_ShiftA, nullptr
)

static void OnChanged_ADC_Balance_ShiftB(void)
{
    BALANCE_ADC_B = shiftADCB;
    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)BALANCE_ADC_B, false);
}

DEF_GOVERNOR(mgADC_Balance_ShiftB, PageDebug::PageADC::PageBalance::self,
    "�������� 2", "Offset 2",
    "",
    "",
    shiftADCB, -125, 125, IsActive_ADC_Balance_Shift, OnChanged_ADC_Balance_ShiftB, nullptr
)

DEF_PAGE_3(pageBalanceADC, PageDebug::PageADC::self, NamePage::DebugADCbalance,
    "������", "BALANCE",
    "",
    "",
    mcADC_Balance_Mode,     // ������� - ��� - ������ - �����
    mgADC_Balance_ShiftA,   // ������� - ��� - ������ - �������� 1
    mgADC_Balance_ShiftB,   // ������� - ��� - ������ - �������� 2
    nullptr, nullptr, nullptr, nullptr
)

void PageDebug::LoadStretchADC(Channel::E chan)
{
    if (DEBUG_STRETCH_ADC_TYPE_IS_DISABLED)
    {
        FPGA::WriteToHardware(chan == Channel::A ? WR_CAL_A : WR_CAL_B, 0x80, true);
    }
    else if (DEBUG_STRETCH_ADC_TYPE_IS_HAND)
    {
        FPGA::WriteToHardware(chan == Channel::A ? WR_CAL_A : WR_CAL_B, (uint8)DEBUG_STRETCH_ADC(chan), true);
    }
    else if (DEBUG_STRETCH_ADC_TYPE_IS_SETTINGS)
    {
        FPGA::LoadKoeffCalibration(chan);
    }
}

static void OnChanged_ADC_Stretch_Mode(bool active)
{
    if (active)
    {
        PageDebug::LoadStretchADC(Channel::A);
        PageDebug::LoadStretchADC(Channel::B);
    }
}

DEF_CHOICE_3(mcADC_Stretch_Mode, PageDebug::PageADC::PageStretch::self,
    "�����", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "��������", "Real",
    "������", "Manual",
    DEBUG_STRETCH_ADC_TYPE, nullptr, OnChanged_ADC_Stretch_Mode, nullptr
)

static void OnChanged_ADC_Stretch_ADC_A(void)
{
    FPGA::WriteToHardware(WR_CAL_A, (uint8)DEBUG_STRETCH_ADC_A, true);
}

static bool IsActive_ADC_Stretch_ADC(void)
{
    return DEBUG_STRETCH_ADC_TYPE_IS_HAND;
}

DEF_GOVERNOR(mgADC_Stretch_ADC_A, PageDebug::PageADC::PageStretch::self,
    "�����. 1�", "Koeff. 1ch",
    "",
    "",
    DEBUG_STRETCH_ADC_A, 0, 255, IsActive_ADC_Stretch_ADC, OnChanged_ADC_Stretch_ADC_A, nullptr
)

static void OnChanged_ADC_Stretch_ADC_B(void)
{
    FPGA::WriteToHardware(WR_CAL_B, (uint8)DEBUG_STRETCH_ADC_B, true);
}

DEF_GOVERNOR(mgADC_Stretch_ADC_B, PageDebug::PageADC::PageStretch::self,
    "�����. 2�", "Koeff. 2ch",
    "",
    "",
    DEBUG_STRETCH_ADC_B, 0, 255, IsActive_ADC_Stretch_ADC, OnChanged_ADC_Stretch_ADC_B, nullptr
)

DEF_PAGE_3(pageStretchADC, PageDebug::PageADC::self, NamePage::DebugADCstretch,
    "��������", "STRETCH",
    "",
    "",
    mcADC_Stretch_Mode,     // ������� - ��� - �������� - �����
    mgADC_Stretch_ADC_A,    // ������� - ��� - �������� - �����. 1�
    mgADC_Stretch_ADC_B,    // ������� - ��� - �������� - �����. 2�    
    nullptr, nullptr, nullptr, nullptr
)

static void OnPress_ADC_AltRShift_Reset(void)
{
    for (int chan = 0; chan < 2; chan++)
    {
        for (int mode = 0; mode < 2; mode++)
        {
            for (int range = 0; range < Range::Count; range++)
            {
                RSHIFT_ADD(chan, range, mode) = 0;
            }
        }
    }
    FPGA::SetRShift(Channel::A, SET_RSHIFT_A);
    FPGA::SetRShift(Channel::B, SET_RSHIFT_B);
}

DEF_BUTTON(mbADC_AltRShift_Reset, PageDebug::PageADC::PageAltRShift::self,
    "�����", "Reset",
    "", "",
    nullptr, OnPress_ADC_AltRShift_Reset
)

static void OnChanged_ADC_AltRShift_A(void)
{
    FPGA::SetRShift(Channel::A, SET_RSHIFT_A);
}

DEF_GOVERNOR(mbADC_AltRShift_2mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "�� 1� 2�� ����", "Shift 1ch 2mV DC",
    "",
    "",
    set.chan[Channel::A].rShiftAdd[Range::_2mV][ModeCouple::DC], -100, 100, nullptr, OnChanged_ADC_AltRShift_A, nullptr
)

static void OnChanged_ADC_AltRShift_B(void)
{
    FPGA::SetRShift(Channel::B, SET_RSHIFT_B);
}

DEF_GOVERNOR(mbADC_AltRShift_2mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "�� 2� 2�� ����", "Shift 2ch 2mV DC",
    "",
    "",
    RSHIFT_ADD(Channel::B, Range::_2mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_B, nullptr
)

DEF_GOVERNOR(mbADC_AltRShift_5mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "�� 1� 5�� ����", "Shift 1ch 5mV DC",
    "",
    "",
    RSHIFT_ADD(Channel::A, Range::_5mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_A, nullptr
)

DEF_GOVERNOR(mbADC_AltRShift_5mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "�� 2� 5�� ����", "Shift 2ch 5mV DC",
    "",
    "",
    RSHIFT_ADD(Channel::B, Range::_5mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_B, nullptr
)

DEF_GOVERNOR(mbADC_AltRShift_10mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "�� 1� 10�� ����", "Shift 1ch 10mV DC",
    "",
    "",
    RSHIFT_ADD(Channel::A, Range::_10mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_A, nullptr
)

DEF_GOVERNOR(mbADC_AltRShift_10mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "�� 2� 10�� ����", "Shift 2ch 10mV DC",
    "",
    "",
    RSHIFT_ADD(Channel::B, Range::_10mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_B, nullptr
)

DEF_PAGE_7(pageAltShiftADC, PageDebug::PageADC::self, NamePage::DebugADCrShift,
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

DEF_PAGE_3(pageADC, PageDebug::self, NamePage::DebugADC,
    "���", "ADC",
    "",
    "",
    *PageDebug::PageADC::PageBalance::self,     // ������� - ��� - ������
    *PageDebug::PageADC::PageStretch::self,     // ������� - ��� - ��������
    *PageDebug::PageADC::PageAltRShift::self,   // ������� - ��� - ��� ����
    nullptr, nullptr, nullptr, nullptr
)

static void OnChanged_Randomizer_SamplesForGates(void)
{
    FPGA::SetNumberMeasuresForGates(NUM_MEAS_FOR_GATES);
}

DEF_GOVERNOR(mgRandomizer_SamplesForGates, PageDebug::PageRandomizer::self,
    "���-�/������", "Samples/gates",
    "",
    "",
    NUM_MEAS_FOR_GATES, 1, 2500, nullptr, OnChanged_Randomizer_SamplesForGates, nullptr
)

static void OnChanged_Randomizer_AltTShift0(void)
{
    FPGA::SetDeltaTShift(ADD_SHIFT_T0);
}

DEF_GOVERNOR(mgRandomizer_AltTShift0, PageDebug::PageRandomizer::self,
    "tShift ���.", "tShift alt.",
    "",
    "",
    ADD_SHIFT_T0, 0, 510, nullptr, OnChanged_Randomizer_AltTShift0, nullptr
)

DEF_GOVERNOR(mgRandomizer_Average, PageDebug::PageRandomizer::self,
    "������.", "Average",
    "",
    "",
    NUM_AVE_FOR_RAND, 1, 32, nullptr, nullptr, nullptr
)

DEF_PAGE_3(pageRandomizer, PageDebug::self, NamePage::DebugRandomizer,
    "����-���", "RANDOMIZER",
    "",
    "",
    mgRandomizer_SamplesForGates,   // ������� - ����-��� - ���-�/������
    mgRandomizer_AltTShift0,        // ������� - ����-��� - tShift ���.
    mgRandomizer_Average,           // ������� - ����-��� - ������.
    nullptr, nullptr, nullptr, nullptr
)

static void OnDraw_SizeSettings(int x, int y)
{
    Text("������ %d", sizeof(Settings)).Draw(x + 5, y + 21, Color::BLACK);
}

DEF_CHOICE_2(mcSizeSettings, PageDebug::self,
    "������ ��������", "Size settings",
    "����� ������� ��������� Settings",
    "Show size of struct Settings",
    "������", "Size",
    "������", "Size",
    size, nullptr, nullptr, OnDraw_SizeSettings
)

static bool IsActive_SaveFirmware()
{
    return FDrive::isConnected;
}

static void OnPress_SaveFirmware()
{
    StructForWrite structForWrite;

    FDrive::OpenNewFileForWrite("S8-53.bin", &structForWrite);

    uint8 *address = (uint8 *)0x08020000; //-V566
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

DEF_BUTTON(mbSaveFirmware, PageDebug::self,
    "����. ��������", "Save firmware",
    "���������� �������� - �������� 5, 6, 7 ����� ������� 3 � 128 ��, ��� �������� ���������",
    "Saving firmware - sectors 5, 6, 7 with a total size of 3 x 128 kB, where the program is stored",
    IsActive_SaveFirmware, OnPress_SaveFirmware
)

static void OnPress_EraseData()
{
    EPROM::EraseData();
}

DEF_BUTTON(bEraseData, PageDebug::self,
    "������� ������", "Erase data",
    "������� ������� � �������",
    "Erases data sectors",
    nullptr, OnPress_EraseData
)

DEF_PAGE_7(pageDebug, PageMain::self, NamePage::Debug,
    "�������", "DEBUG",
    "",
    "",
    mcStats,                            // ������� - ����������
    *PageDebug::PageConsole::self,      // ������� - �������
    *PageDebug::PageADC::self,          // ������� - ���
    *PageDebug::PageRandomizer::self,   // ������� - ����-���
    mcSizeSettings,                     // ������� - ������ ��������
    mbSaveFirmware,                     // ������� - ����. ��������
    bEraseData,                         // ������� - ������� ������
    nullptr, nullptr, nullptr, nullptr
);

static void OnPress_SerialNumber_Exit(void)
{
    Display::RemoveAddDrawFunction();
    FREE_EXTRAMEM();
}

DEF_SMALL_BUTTON(bSerialNumber_Exit, PageDebug::PageSerialNumber::self,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu",
    nullptr, OnPress_SerialNumber_Exit, DrawSB_Exit, nullptr
)

static void OnPress_SerialNumber_Change(void)
{
    ACCESS_EXTRAMEM(StructForSN, s);
    ++s->curDigt;
    s->curDigt %= 2;
    Color::ResetFlash();
}

static void Draw_SerialNumber_Change(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::TAB).Draw4SymbolsInRect(x + 2, y + 2);
    Font::Set(TypeFont::S8);
}

DEF_SMALL_BUTTON(bSerialNumber_Change, PageDebug::PageSerialNumber::self,
    "��������", "Insert",
    "��������� �������� ������",
    "Inserts the chosen symbol",
    nullptr, OnPress_SerialNumber_Change, Draw_SerialNumber_Change, nullptr
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
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::SAVE_TO_MEM).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}

DEF_SMALL_BUTTON(bSerialNumber_Save, PageDebug::PageSerialNumber::self,
    "���������", "Save",
    "���������� �������� ����� � OTP",
    "Records the serial number in OTP",
    nullptr, OnPress_SerialNumber_Save, Draw_SerialNumber_Save, nullptr
)

static void Draw_EnterSerialNumber(void)
{
    int x0 = Grid::Left() + 40;
    int y0 = Grid::TOP + 20;
    int width = Grid::Width() - 80;
    int height = 160;

    Primitives::Rectangle(width, height).Draw(x0, y0, Color::FILL);
    Region(width - 2, height - 2).Fill(x0 + 1, y0 + 1, Color::BACK);

    int deltaX = 10;

    ACCESS_EXTRAMEM(StructForSN, s);

    bool selNumber = s->curDigt == 0;

    char buffer[20];
    std::snprintf(buffer, 19, "%02d", s->number);

    Color::E colorText = Color::FILL;
    Color::E colorBackground = Color::BACK;

    if (selNumber)
    {
        colorText = Color::FLASH_01;
        colorBackground = Color::FLASH_10;
    }

    int y = y0 + 50;

    Color::SetCurrent(colorText);
    int x = Text(buffer).DrawOnBackground(x0 + deltaX, y, colorBackground);

    colorText = Color::FLASH_01;
    colorBackground = Color::FLASH_10;

    if (selNumber)
    {
        colorText = Color::FILL;
        colorBackground = Color::BACK;
    }

    std::snprintf(buffer, 19, "%04d", s->year);

    Color::SetCurrent(colorText);
    Text(buffer).DrawOnBackground(x + 5, y, colorBackground);

    // ������ ������� ���������� �� ���������� ����� � OTP-������ ��� ������

    int allShots = HAL_OTP::GetSerialNumber(buffer);

    Text("������� ���������� ����� %s", buffer[0] == 0 ? "-- ----" : buffer).Draw(x0 + deltaX, y0 + 130, Color::FILL);

    Text("�������� ����� ��� %d �������", allShots).Draw(x0 + deltaX, y0 + 100, Color::FILL);
}

static void OnPress_SerialNumber(void)
{
    PageDebug::PageSerialNumber::self->OpenAndSetItCurrent();
    Display::SetAddDrawFunction(Draw_EnterSerialNumber);
    MALLOC_EXTRAMEM(StructForSN, s);
    s->number = 1; //-V522
    s->year = 2017;
    s->curDigt = 0;
} //-V773

static void OnRegSet_SerialNumber(int angle)
{
    typedef int(*pFunc)(int *, int, int);

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

DEF_PAGE_6(pageSerialNumber, PageDebug::self, NamePage::SB_SerialNumber,
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

const Page *PageDebug::PageSerialNumber::self = &pageSerialNumber;
const Page *PageDebug::self = &pageDebug;
const Page *PageDebug::PageConsole::self = &pageConsole;
const Page *PageDebug::PageConsole::PageRegisters::self = &pageRegisters;
const Page *PageDebug::PageADC::self = &pageADC;
const Page *PageDebug::PageADC::PageBalance::self = &pageBalanceADC;
const Page *PageDebug::PageADC::PageStretch::self = &pageStretchADC;
const Page *PageDebug::PageADC::PageAltRShift::self = &pageAltShiftADC;
const Page *PageDebug::PageRandomizer::self = &pageRandomizer;
