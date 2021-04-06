#include "defines.h"
#include "common/Display/Colors_.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "Display/Display.h"
#include "FPGA/FPGA.h"
#include "Menu/Items/MenuItems.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"


void CalculateConditions(int16 pos0, int16 pos1, CursCntrl::E cursCntrl, bool *cond0, bool *cond1)
{
    bool zeroLessFirst = pos0 < pos1;
    *cond0 = cursCntrl == CursCntrl::_1_2 || (cursCntrl == CursCntrl::_1 && zeroLessFirst) || (cursCntrl == CursCntrl::_2 && !zeroLessFirst); //-V537
    *cond1 = cursCntrl == CursCntrl::_1_2 || (cursCntrl == CursCntrl::_1 && !zeroLessFirst) || (cursCntrl == CursCntrl::_2 && zeroLessFirst);
}


void DrawSB_Exit(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x2e').Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}


#include "PageHelp.cpp"


DEF_PAGE_11(mainPage, &Page::empty, NamePage::MainPage,
    "лемч", "MENU",
    "",
    "",
    *PageDisplay::self,
    *PageChannelA::self,
    *PageChannelB::self,
    *PageTrig::self,
    *PageTime::self,
    *PageCursors::self,
    *PageMemory::self,
    *PageMeasures::self,
    *PageService::self,
    *PageHelp::self,
    *PageDebug::self,
    nullptr, nullptr, nullptr, nullptr
)



const Page *PageForButton(Key::E button)
{
    static const void *pages[Key::Count] = {  
        (void *)&Page::empty,       // Key::None
        (void *)&Page::empty,       // Key::F1
        (void *)&Page::empty,       // Key::F2
        (void *)&Page::empty,       // Key::F3
        (void *)&Page::empty,       // Key::F4
        (void *)&Page::empty,       // Key::F5
        (void *)PageChannelA::self, // Key::ChannelA
        (void *)PageChannelB::self, // Key::ChannelB
        (void *)PageTime::self,     // Key::Time
        (void *)PageTrig::self,     // Key::Trig
        (void *)PageCursors::self,  // Key::Cursors
        (void *)PageMeasures::self, // Key::Measures
        (void *)PageDisplay::self,  // Key::Display
        (void *)&Page::empty,       // Key::Help
        (void *)&Page::empty,       // Key::Start
        (void *)PageMemory::self,   // Key::Memory
        (void *)PageService::self,  // Key::Service
        (void *)&Page::empty,       // Key::Menu
        (void *)&Page::empty,       // Key::Power
        (void *)&Page::empty,       // Key::RangeA
        (void *)&Page::empty,       // Key::RangeB
        (void *)&Page::empty,       // Key::RShiftA
        (void *)&Page::empty,       // Key::RShiftB
        (void *)&Page::empty,       // Key::TBase
        (void *)&Page::empty,       // Key::TShift
        (void *)&Page::empty,       // Key::TrigLev
        (void *)&Page::empty        // Key::Setting
    };

    return reinterpret_cast<const Page *>(pages[button]);
}


const Page *PageMain::self = &mainPage;


bool IsMainPage(const void *item)
{
    return item == &mainPage;
}
