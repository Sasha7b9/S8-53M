#include "defines.h"
#include "Menu/Pages/PageMemory.h"
#include "Menu/Pages/PageCursors.h"
#include "PageHelp.h"
#include "FPGA/FPGA.h"
#include "common/Display/Colors_c.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "common/Display/Primitives_c.h"
#include "Settings/Settings.h"
#include "Settings/SettingsTypes.h"
#include "Panel/Panel.h"


using namespace Primitives;


extern Choice mcCursorsSource;
extern Choice mcCursorsU;
extern Choice mcCursorsT;


void CalculateConditions(int16 pos0, int16 pos1, CursCntrl::E cursCntrl, bool *cond0, bool *cond1)
{
    bool zeroLessFirst = pos0 < pos1;
    *cond0 = cursCntrl == CursCntrl::_1_2 || (cursCntrl == CursCntrl::_1 && zeroLessFirst) || (cursCntrl == CursCntrl::_2 && !zeroLessFirst); //-V537
    *cond1 = cursCntrl == CursCntrl::_1_2 || (cursCntrl == CursCntrl::_1 && !zeroLessFirst) || (cursCntrl == CursCntrl::_2 && zeroLessFirst);
}



int CalculateYforCurs(int y, bool top)
{
    return top ? y + MI_HEIGHT / 2 + 4 : y + MI_HEIGHT - 2;
}



int CalculateXforCurs(int x, bool left)
{
    return left ? x + MI_WIDTH - 20 : x + MI_WIDTH - 5;
}



void CalculateXY(int *x0, int *x1, int *y0, int *y1)
{
    *x0 = CalculateXforCurs(*x0, true);
    *x1 = CalculateXforCurs(*x1, false);
    *y0 = CalculateYforCurs(*y0, true);
    *y1 = CalculateYforCurs(*y1, false);
}



void DrawMenuCursTime(int x, int y, bool left, bool right)
{
    x -= 58;
    y -= 16;
    int x0 = x, x1 = x, y0 = y, y1 = y;
    CalculateXY(&x0, &x1, &y0, &y1);
    for(int i = 0; i < (left ? 3 : 1); i++)
    {
        HLine().Draw(x0 + i, y0, y1);
    }
    for(int i = 0; i < (right ? 3 : 1); i++)
    {
        HLine().Draw(x1 - i, y0, y1);
    }
}



void DrawMenuCursVoltage(int x, int y, bool top, bool bottom)
{
    x -= 65;
    y -= 21;
    int x0 = x, x1 = x, y0 = y, y1 = y;
    CalculateXY(&x0, &x1, &y0, &y1);
    for(int i = 0; i < (top ? 3 : 1); i++)
    {
        HLine().Draw(y0 + i, x0, x1);
    }
    for(int i = 0; i < (bottom ? 3 : 1); i++)
    {
        HLine().Draw(y1 - i, x0, x1);
    }
}


void DrawSB_Exit(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x2e');
    Font::Set(TypeFont::_8);
}



extern const Page mainPage;

//#include "PageTrig.c"
//#include "PageTime.c"
#include "PageHelp.cpp"

extern const Page pDisplay;
extern const Page pMeasures;
extern const Page pDebug;
extern const Page pService;
extern const Page pTime;
extern const Page pTrig;



DEF_PAGE_11(mainPage, Page::empty, NamePage::MainPage,
    "лемч", "MENU",
    "",
    "",
    pDisplay,
    *PageChannelA::self,
    *PageChannelB::self,
    pTrig,
    pTime,
    PageCursors::self,
    PageMemory::self,
    pMeasures,
    pService,
    mpHelp,
    pDebug,
    nullptr, nullptr, nullptr, nullptr
)



const Page *PageForButton(Key::E button)
{
    static const void *pages[] = {  
        0,                          // Key::None
        0,                          // Key::F1
        0,                          // Key::F2
        0,                          // Key::F3
        0,                          // Key::F4
        0,                          // Key::F5
        (void *)PageChannelA::self, // Key::ChannelA
        (void *)PageChannelB::self, // Key::ChannelB
        (void *)&pTime,             // Key::Time
        (void *)&pTrig,             // Key::Trig
        PageCursors::GetPointer(),  // Key::Cursors
        (void *)&pMeasures,         // Key::Measures
        (void *)&pDisplay,          // Key::Display
        0,                          // Key::Help
        0,                          // Key::Start
        PageMemory::GetPointer(),   // Key::Memory
        (void *)&pService,          // Key::Service
        0,                          // Key::Menu
        0                           // B_Power
    };

    return reinterpret_cast<const Page *>(pages[button]);
}



bool IsMainPage(const void *item)
{
    return item == &mainPage;
}
