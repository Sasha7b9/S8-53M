// 2021/05/12 11:06:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Settings/SettingsNRST.h"


DEF_CHOICE_2(mcLaunchFPGA, PageDebug::PageConsole::PageRegisters::self,
    "Запуск", "Launch",
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
    "U см. 1к", "U shift 1ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.rShiftA, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcConsole_Registers_RShiftB, PageDebug::PageConsole::PageRegisters::self,
    "U см. 2к", "U shift 2ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.rShiftB, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcConsole_Registers_TrigLev, PageDebug::PageConsole::PageRegisters::self,
    "U синхр.", "U trig.",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.trigLev, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcConsole_Registers_RangeA, PageDebug::PageConsole::PageRegisters::self,
    "ВОЛЬТ/ДЕЛ 1", "Range 1",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.rangeA, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcConsole_Registers_RangeB, PageDebug::PageConsole::PageRegisters::self,
    "ВОЛЬТ/ДЕЛ 2", "Range 2",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.rangeB, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcConsole_Registers_TrigParam, PageDebug::PageConsole::PageRegisters::self,
    "Парам. синхр.", "Trig param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.trigParam, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcConsole_Registers_ChanParamA, PageDebug::PageConsole::PageRegisters::self,
    "Парам. кан. 1", "Chan 1 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.chanParamA, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcConsole_Registers_ChanParamB, PageDebug::PageConsole::PageRegisters::self,
    "Парам. кан. 2", "Chan 2 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.chanParamB, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcConsole_Registers_TBase, PageDebug::PageConsole::PageRegisters::self,
    "ВРЕМЯ/ДЕЛ", "TBase",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.tBase, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcConsole_Registers_tShift, PageDebug::PageConsole::PageRegisters::self,
    "Т см.", "tShift",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.tShift, nullptr, nullptr, nullptr
)


DEF_PAGE_12(pageRegisters, PageDebug::PageConsole::self, NamePage::DebugShowRegisters,
    "РЕГИСТРЫ", "REGISTERS",
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

const Page *PageDebug::PageConsole::PageRegisters::self = &pageRegisters;
