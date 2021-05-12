// 2021/05/12 11:06:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Settings/SettingsNRST.h"


DEF_CHOICE_2(mcLaunchFPGA, PageDebug::PageConsole::PageRegisters::self,
    "������", "Launch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.launch, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcRD_FL, PageDebug::PageConsole::PageRegisters::self,
    "RD_FL", "RD_FL",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.flag, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcRShiftA, PageDebug::PageConsole::PageRegisters::self,
    "U ��. 1�", "U shift 1ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.rShiftA, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcRShiftB, PageDebug::PageConsole::PageRegisters::self,
    "U ��. 2�", "U shift 2ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.rShiftB, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcTrigLev, PageDebug::PageConsole::PageRegisters::self,
    "U �����.", "U trig.",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.trigLev, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcRangeA, PageDebug::PageConsole::PageRegisters::self,
    "�����/��� 1", "Range 1",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.rangeA, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcRangeB, PageDebug::PageConsole::PageRegisters::self,
    "�����/��� 2", "Range 2",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.rangeB, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mTrigParam, PageDebug::PageConsole::PageRegisters::self,
    "�����. �����.", "Trig param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.trigParam, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcChanParamA, PageDebug::PageConsole::PageRegisters::self,
    "�����. ���. 1", "Chan 1 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.chanParamA, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcChanParamB, PageDebug::PageConsole::PageRegisters::self,
    "�����. ���. 2", "Chan 2 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.chanParamB, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcTBase, PageDebug::PageConsole::PageRegisters::self,
    "�����/���", "TBase",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.tBase, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcTShift, PageDebug::PageConsole::PageRegisters::self,
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
    mcRD_FL,
    mcRShiftA,
    mcRShiftB,
    mcTrigLev,
    mcRangeA,
    mcRangeB,
    mTrigParam,
    mcChanParamA,
    mcChanParamB,
    mcTBase,
    mcTShift,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDebug::PageConsole::PageRegisters::self = &pageRegisters;
