// 2021/05/12 11:37:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/SettingsNRST.h"


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


const Page *PageDebug::PageADC::PageStretch::self = &pageStretchADC;
