// 2021/05/12 11:20:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"


static int16 shiftADCA;
static int16 shiftADCB;


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
    "Режим", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "Реальный", "Real",
    "Ручной", "Manual",
    setNRST.balance_ADC_type, nullptr, OnChanged_ADC_Balance_Mode, Draw_ADC_Balance_Mode
)


static bool IsActive_ADC_Balance_Shift()
{
    return (BalanceADCtype::Get() == BalanceADCtype::Hand);
}


static void OnChanged_ADC_Balance_ShiftA()
{
    setNRST.balance_ADC[ChA] = shiftADCA;
    //    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)BALANCE_ADC_A, false);
}


DEF_GOVERNOR(mgADC_Balance_ShiftA, PageDebug::PageADC::PageBalance::self,
    "Смещение 1", "Offset 1",
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
    "Смещение 2", "Offset 2",
    "",
    "",
    shiftADCB, -125, 125, IsActive_ADC_Balance_Shift, OnChanged_ADC_Balance_ShiftB, nullptr
)


DEF_PAGE_3(pageBalanceADC, PageDebug::PageADC::self, NamePage::DebugADCbalance,
    "БАЛАНС", "BALANCE",
    "",
    "",
    mcADC_Balance_Mode,     // ОТЛАДКА - АЦП - БАЛАНС - Режим
    mgADC_Balance_ShiftA,   // ОТЛАДКА - АЦП - БАЛАНС - Смещение 1
    mgADC_Balance_ShiftB,   // ОТЛАДКА - АЦП - БАЛАНС - Смещение 2
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDebug::PageADC::PageBalance::self = &pageBalanceADC;
