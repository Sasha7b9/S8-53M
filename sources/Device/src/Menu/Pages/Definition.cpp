#include "defines.h"
#include "PageMemory.h"
#include "Menu/Pages/PageCursors.h"
#include "PageHelp.h"
#include "FPGA/FPGA.h"
#include "Display/Colors.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include "Settings/SettingsTypes.h"
#include "Panel/Panel.h"


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
        Painter::DrawVLine(x0 + i, y0, y1);
    }
    for(int i = 0; i < (right ? 3 : 1); i++)
    {
        Painter::DrawVLine(x1 - i, y0, y1);
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
        Painter::DrawHLine(y0 + i, x0, x1);
    }
    for(int i = 0; i < (bottom ? 3 : 1); i++)
    {
        Painter::DrawHLine(y1 - i, x0, x1);
    }
}


void DrawSB_Exit(int x, int y)
{
    Painter::SetFont(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x2e');
    Painter::SetFont(TypeFont::_8);
}



extern const Page mainPage;

//#include "PageTrig.c"
//#include "PageTime.c"
#include "PageHelp.cpp"

extern const Page pDisplay;
extern const Page pChanA;
extern const Page pChanB;
extern const Page pMeasures;
extern const Page pDebug;
extern const Page pService;
extern const Page pTime;
extern const Page pTrig;



static const arrayItems itemsMainPage =
{
    (void*)&pDisplay,
    (void*)&pChanA,
    (void*)&pChanB,
    (void*)&pTrig,
    (void*)&pTime,
    PageCursors::GetPointer(),
    PageMemory::GetPointer(),
    (void*)&pMeasures,
    (void*)&pService,
    (void*)&mpHelp,
    (void*)&pDebug
};

const Page mainPage
(
    0, 0,
    "лемч", "MENU",
    "",
    "",
    NamePage::MainPage, &itemsMainPage
);



const void *PageForButton(PanelButton::E button)
{
    static const void *pages[] = {  
        0,                          // PanelButton::Empty
        (void*)&pChanA,             // PanelButton::Channel0
        (void*)&pService,           // PanelButton::Service
        (void*)&pChanB,             // PanelButton::Channel1
        (void*)&pDisplay,           // PanelButton::Display
        (void*)&pTime,              // PanelButton::Time
        PageMemory::GetPointer(),   // PanelButton::Memory
        (void*)&pTrig,              // PanelButton::Trig
        0,                          // PanelButton::Start
        PageCursors::GetPointer(),  // PanelButton::Cursors
        (void*)&pMeasures,          // PanelButton::Measures
        0,                          // B_Power
        0,                          // PanelButton::Help
        0,                          // PanelButton::Menu
        0,                          // PanelButton::F1
        0,                          // PanelButton::F2
        0,                          // PanelButton::F3
        0,                          // PanelButton::F4
        0,                          // PanelButton::F5
    };

    return pages[button];
}



bool IsMainPage(const void *item)
{
    return item == &mainPage;
}
