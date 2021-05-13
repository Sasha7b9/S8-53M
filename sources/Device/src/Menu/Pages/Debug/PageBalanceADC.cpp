// 2021/05/12 11:20:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"


static int16 shiftADCA;
static int16 shiftADCB;


//----------------------------------------------------------------------------------------------------------------------

static bool IsActive_Shift()
{
    return (BalanceADCtype::Get() == BalanceADCtype::Hand);
}

//----------------------------------------------------------------------------------------------------------------------

static void Draw_Mode(int, int)
{
    int8 shift[2][3] =
    {
        {0, setNRST.chan[ChA].balance_auto, (int8)setNRST.chan[ChA].balance_hand},
        {0, setNRST.chan[ChB].balance_auto, (int8)setNRST.chan[ChB].balance_hand}
    };

    shiftADCA = shift[0][BalanceADCtype::Get()];
    shiftADCB = shift[1][BalanceADCtype::Get()];
}


static void OnChanged_Mode(bool)
{
    Draw_Mode(0, 0);

    //    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)shiftADCA, false);
    //    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)shiftADCB, false);

    SettingsNRST::CommonOnChanged();
}


DEF_CHOICE_3(mcMode, PageDebug::PageADC::PageBalance::self,
    "�����", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "��������", "Real",
    "������", "Manual",
    setNRST.adc.type_balance, nullptr, OnChanged_Mode, Draw_Mode
)

//----------------------------------------------------------------------------------------------------------------------

static void OnChanged_ShiftA()
{
    setNRST.chan[ChA].balance_hand = shiftADCA;
    //    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)BALANCE_ADC_A, false);

    SettingsNRST::CommonOnChanged();
}


DEF_GOVERNOR(mgShiftA, PageDebug::PageADC::PageBalance::self,
    "�������� 1", "Offset 1",
    "",
    "",
    shiftADCA, -125, 125, IsActive_Shift, OnChanged_ShiftA, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

static void OnChanged_ShiftB()
{
    setNRST.chan[ChB].balance_hand = shiftADCB;
    //    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)BALANCE_ADC_B, false);

    SettingsNRST::CommonOnChanged();
}


DEF_GOVERNOR(mgShiftB, PageDebug::PageADC::PageBalance::self,
    "�������� 2", "Offset 2",
    "",
    "",
    shiftADCB, -125, 125, IsActive_Shift, OnChanged_ShiftB, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_PAGE_3(pageBalanceADC, PageDebug::PageADC::self, NamePage::DebugADCbalance,
    "������", "BALANCE",
    "",
    "",
    mcMode,
    mgShiftA,
    mgShiftB,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDebug::PageADC::PageBalance::self = &pageBalanceADC;
