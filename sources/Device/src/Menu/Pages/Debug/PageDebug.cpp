// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/Memory/ROM_.h"
#include "Display/Warnings.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include <cstdio>
#include <cstdlib>


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
    setNRST.show_stats, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcLaunchFPGA, PageDebug::PageConsole::PageRegisters::self,
    "������", "Launch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.launch, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcConsole_Registers_RD_FL, PageDebug::PageConsole::PageRegisters::self,
    "RD_FL", "RD_FL",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.flag, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_RShiftA, PageDebug::PageConsole::PageRegisters::self,
    "U ��. 1�", "U shift 1ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.rShiftA, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_RShiftB, PageDebug::PageConsole::PageRegisters::self,
    "U ��. 2�", "U shift 2ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.rShiftB, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_TrigLev, PageDebug::PageConsole::PageRegisters::self,
    "U �����.", "U trig.",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.trigLev, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_RangeA, PageDebug::PageConsole::PageRegisters::self,
    "�����/��� 1", "Range 1",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.rangeA, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_RangeB, PageDebug::PageConsole::PageRegisters::self,
    "�����/��� 2", "Range 2",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.rangeB, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_TrigParam, PageDebug::PageConsole::PageRegisters::self,
    "�����. �����.", "Trig param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.trigParam, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_ChanParamA, PageDebug::PageConsole::PageRegisters::self,
    "�����. ���. 1", "Chan 1 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.chanParamA, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_ChanParamB, PageDebug::PageConsole::PageRegisters::self,
    "�����. ���. 2", "Chan 2 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.chanParamB, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_TBase, PageDebug::PageConsole::PageRegisters::self,
    "�����/���", "TBase",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.tBase, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcConsole_Registers_tShift, PageDebug::PageConsole::PageRegisters::self,
    "� ��.", "tShift",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.tShift, nullptr, nullptr, nullptr
)

DEF_PAGE_12(pageRegisters, PageDebug::PageConsole::self, NamePage::DebugShowRegisters,
    "��������", "REGISTERS",
    "",
    "",
    mcLaunchFPGA,
    mcConsole_Registers_RD_FL,
    mcConsole_Registers_RShiftA,
    mcConsole_Registers_RShiftB,
    mcConsole_Registers_TrigLev,
    mcConsole_Registers_RangeA,
    mcConsole_Registers_RangeB,
    mcConsole_Registers_TrigParam,
    mcConsole_Registers_ChanParamA,
    mcConsole_Registers_ChanParamB,
    mcConsole_Registers_TBase,
    mcConsole_Registers_tShift,
    nullptr, nullptr, nullptr, nullptr
)



static void Draw_ADC_Balance_Mode(int, int)
{
    int8 shift[2][3] =
    {
        {0, set.chan[ChA].balance_shift_ADC, (int8)setNRST.balance_ADC[ChA]},
        {0, set.chan[ChB].balance_shift_ADC, (int8)setNRST.balance_ADC[ChB]}
    };

    shiftADCA = shift[0][BalanceADCtype::Get()];
    shiftADCB = shift[1][BalanceADCtype::Get()];
}

static void OnChanged_ADC_Balance_Mode(bool)
{
    Draw_ADC_Balance_Mode(0, 0);

//    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)shiftADCA, false);
//    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)shiftADCB, false);
}

DEF_CHOICE_3(mcADC_Balance_Mode, PageDebug::PageADC::PageBalance::self,
    "�����", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "��������", "Real",
    "������", "Manual",
    setNRST.balance_ADC_type, nullptr, OnChanged_ADC_Balance_Mode, Draw_ADC_Balance_Mode
)

static void OnChanged_ADC_Balance_ShiftA()
{
    setNRST.balance_ADC[ChA] = shiftADCA;
//    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)BALANCE_ADC_A, false);
}

static bool IsActive_ADC_Balance_Shift()
{
    return (BalanceADCtype::Get() == BalanceADCtype::Hand);
}

DEF_GOVERNOR(mgADC_Balance_ShiftA, PageDebug::PageADC::PageBalance::self,
    "�������� 1", "Offset 1",
    "",
    "",
    shiftADCA, -125, 125, IsActive_ADC_Balance_Shift, OnChanged_ADC_Balance_ShiftA, nullptr
)

static void OnChanged_ADC_Balance_ShiftB()
{
    setNRST.balance_ADC[ChB] = shiftADCB;
//    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)BALANCE_ADC_B, false);
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

void PageDebug::LoadStretchADC(const Channel &ch)
{
    if (StretchADCtype::IsDisabled())
    {
//        FPGA::WriteToHardware(ch == ChA ? WR_CAL_A : WR_CAL_B, 0x80, true);
    }
    else if (StretchADCtype::IsHand())
    {
//        FPGA::WriteToHardware(ch == ChA ? WR_CAL_A : WR_CAL_B, (uint8)DEBUG_STRETCH_ADC(ch), true);
    }
    else if (StretchADCtype::IsSettings())
    {
        FPGA::Calibrator::LoadKoeff(ch);
    }
}

static void OnChanged_ADC_Stretch_Mode(bool active)
{
    if (active)
    {
        PageDebug::LoadStretchADC(ChA);
        PageDebug::LoadStretchADC(ChB);
    }
}

DEF_CHOICE_3(mcADC_Stretch_Mode, PageDebug::PageADC::PageStretch::self,
    "�����", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "��������", "Real",
    "������", "Manual",
    setNRST.stretch_ADC_type, nullptr, OnChanged_ADC_Stretch_Mode, nullptr
)

static void OnChanged_ADC_Stretch_ADC_A()
{
//    FPGA::WriteToHardware(WR_CAL_A, (uint8)DEBUG_STRETCH_ADC_A, true);
}

static bool IsActive_ADC_Stretch_ADC()
{
    return StretchADCtype::IsHand();
}

DEF_GOVERNOR(mgADC_Stretch_ADC_A, PageDebug::PageADC::PageStretch::self,
    "�����. 1�", "Koeff. 1ch",
    "",
    "",
    setNRST.stretch_ADC[ChA], 0, 255, IsActive_ADC_Stretch_ADC, OnChanged_ADC_Stretch_ADC_A, nullptr
)

static void OnChanged_ADC_Stretch_ADC_B()
{
//    FPGA::WriteToHardware(WR_CAL_B, (uint8)DEBUG_STRETCH_ADC_B, true);
}

DEF_GOVERNOR(mgADC_Stretch_ADC_B, PageDebug::PageADC::PageStretch::self,
    "�����. 2�", "Koeff. 2ch",
    "",
    "",
    setNRST.stretch_ADC[ChB], 0, 255, IsActive_ADC_Stretch_ADC, OnChanged_ADC_Stretch_ADC_B, nullptr
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

static void OnPress_ADC_AltRShift_Reset()
{
    for (int ch = 0; ch < 2; ch++)
    {
        for (int mode = 0; mode < 2; mode++)
        {
            for (int range = 0; range < Range::Count; range++)
            {
                RSHIFT_ADD(ch, range, mode) = 0;
            }
        }
    }
    RShift::Set(ChA, set.chan[ChA].rshift);
    RShift::Set(ChB, set.chan[ChB].rshift);
}

DEF_BUTTON(mbADC_AltRShift_Reset, PageDebug::PageADC::PageAltRShift::self,
    "�����", "Reset",
    "", "",
    nullptr, OnPress_ADC_AltRShift_Reset
)

static void OnChanged_ADC_AltRShift_A()
{
    RShift::Set(ChA, set.chan[ChA].rshift);
}

DEF_GOVERNOR(mbADC_AltRShift_2mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "�� 1� 2�� ����", "Shift 1ch 2mV DC",
    "",
    "",
    set.chan[ChA].rshift_add[Range::_2mV][ModeCouple::DC], -100, 100, nullptr, OnChanged_ADC_AltRShift_A, nullptr
)

static void OnChanged_ADC_AltRShift_B()
{
    RShift::Set(ChB, set.chan[ChB].rshift);
}

DEF_GOVERNOR(mbADC_AltRShift_2mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "�� 2� 2�� ����", "Shift 2ch 2mV DC",
    "",
    "",
    RSHIFT_ADD(ChB, Range::_2mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_B, nullptr
)

DEF_GOVERNOR(mbADC_AltRShift_5mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "�� 1� 5�� ����", "Shift 1ch 5mV DC",
    "",
    "",
    RSHIFT_ADD(ChA, Range::_5mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_A, nullptr
)

DEF_GOVERNOR(mbADC_AltRShift_5mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "�� 2� 5�� ����", "Shift 2ch 5mV DC",
    "",
    "",
    RSHIFT_ADD(ChB, Range::_5mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_B, nullptr
)

DEF_GOVERNOR(mbADC_AltRShift_10mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "�� 1� 10�� ����", "Shift 1ch 10mV DC",
    "",
    "",
    RSHIFT_ADD(ChA, Range::_10mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_A, nullptr
)

DEF_GOVERNOR(mbADC_AltRShift_10mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "�� 2� 10�� ����", "Shift 2ch 10mV DC",
    "",
    "",
    RSHIFT_ADD(ChB, Range::_10mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_B, nullptr
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

static void OnChanged_Randomizer_SamplesForGates()
{
    FPGA::Randomizer::SetNumberMeasuresForGates(setNRST.num_measures_for_gates);
}

DEF_GOVERNOR(mgRandomizer_SamplesForGates, PageDebug::PageRandomizer::self,
    "���-�/������", "Samples/gates",
    "",
    "",
    setNRST.num_measures_for_gates, 1, 2500, nullptr, OnChanged_Randomizer_SamplesForGates, nullptr
)

static void OnChanged_Randomizer_AltTShift0()
{
    TShift::SetDelta(setNRST.shift_T0);
}

DEF_GOVERNOR(mgRandomizer_AltTShift0, PageDebug::PageRandomizer::self,
    "tShift ���.", "tShift alt.",
    "",
    "",
    setNRST.shift_T0, 0, 510, nullptr, OnChanged_Randomizer_AltTShift0, nullptr
)

DEF_GOVERNOR(mgRandomizer_Average, PageDebug::PageRandomizer::self,
    "������.", "Average",
    "",
    "",
    setNRST.num_ave_for_rand, 1, 32, nullptr, nullptr, nullptr
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

    uint8 *address = (uint8 *)0x08020000;
    uint8 *endAddress = address + 128 * 1024 * 3;

    int sizeBlock = 512;

    while (address < endAddress)
    {
        FDrive::WriteToFile(address, sizeBlock, &structForWrite);
        address += sizeBlock;
    }

    FDrive::CloseFile(&structForWrite);

    Warnings::ShowWarningGood(Warning::FirmwareSaved);
}

DEF_BUTTON(mbSaveFirmware, PageDebug::self,
    "����. ��������", "Save firmware",
    "���������� �������� - �������� 5, 6, 7 ����� ������� 3 � 128 ��, ��� �������� ���������",
    "Saving firmware - sectors 5, 6, 7 with a total size of 3 x 128 kB, where the program is stored",
    IsActive_SaveFirmware, OnPress_SaveFirmware
)

static void OnPress_EraseData()
{
    ROM::Data::EraseAll();
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

// ��� ��, ��� �������� �� "�������" S8-53
    mcStats,                            // ������� - ����������
    *PageDebug::PageConsole::self,      // ������� - �������
    *PageDebug::PageADC::self,          // ������� - ���
    *PageDebug::PageRandomizer::self,   // ������� - ����-���
    mcSizeSettings,                     // ������� - ������ ��������
    mbSaveFirmware,                     // ������� - ����. ��������
    bEraseData,                         // ������� - ������� ������
    nullptr, nullptr, nullptr, nullptr
);


const Page *PageDebug::self = &pageDebug;
const Page *PageDebug::PageConsole::PageRegisters::self = &pageRegisters;
const Page *PageDebug::PageADC::self = &pageADC;
const Page *PageDebug::PageADC::PageBalance::self = &pageBalanceADC;
const Page *PageDebug::PageADC::PageStretch::self = &pageStretchADC;
const Page *PageDebug::PageADC::PageAltRShift::self = &pageAltShiftADC;
const Page *PageDebug::PageRandomizer::self = &pageRandomizer;
