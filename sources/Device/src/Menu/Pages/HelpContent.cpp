#include "defines.h"
#include "HelpContentPages.h"
#include "HelpContent.h"
#include "Display/Painter.h"
#include "common/Display/Primitives_c.h"
#include "Display/Grid.h"
#include "common/Display/Font/Font_c.h"
#include "Utils/GlobalFunctions.h"
#include "common/Utils/Math_c.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Definition.h"
#include "common/Hardware/Timer_c.h"
#include "common/Hardware/HAL/HAL_c.h"


using namespace Primitives;

const char *PageHelpContent::stringForHint = 0;
Item *PageHelpContent::itemHint = 0;

const PageHelpItem PageHelpItem::empty(&Item::emptyData);


int currentParagraph = 0;   // ≈сли TypePage(currentPage) == TypePageHelp::Content, то указывает не текущий раздел оглавлени€
const PageHelpItem *currentPage = PageHelpContent::PageMain::self;


static const int WIDTH = 295;


static void DrawPageContent()
{
    int y = 50;
    Painter::DrawStringInCenterRect(0, y, WIDTH, 10, TITLE(currentPage));

    int numPage = 0;

    y += 40;

    while(currentPage->OwnData()->pages[numPage])
    {
        Page *page = (Page*)currentPage->OwnData()->pages[numPage];
        const char *title = TITLE(page);
        if(currentParagraph == numPage)
        {
            Painter::DrawStringInCenterRectOnBackgroundC(0, y, WIDTH, 10, title, COLOR_BACK, 2, COLOR_FILL);
        }
        else
        {
            Painter::DrawStringInCenterRectC(0, y, WIDTH, 10, title, COLOR_FILL);
        }
        y += 16;
        numPage++;
    }
}

static void DrawPageDescription()
{
    Painter::DrawStringInCenterRect(0, 3, WIDTH, 10, TITLE(currentPage));
    Painter::DrawTextInRectWithTransfers(2, 15, WIDTH - 5, 240, HINT(currentPage));
}


void HelpContent_Draw()
{
    uint startTime = gTimerMS;
    Region(319 - Grid::Right(), 20).Fill(Grid::Right(), 0, COLOR_BACK);
    Region(319 - Grid::Right(), 21).Fill(Grid::Right(), 219);
    Region(WIDTH, 237).Fill(1, 1);
    Rectangle(WIDTH + 2, 239).Draw(0, 0, COLOR_FILL);

    /*
    uint16 *addr1 = (uint16*)(0x08000000 + (rand() % 65535));
    uint8 *addr2 = (uint8*)(0x08000000 + (rand() % 65535));

    Color::SetCurrent(COLOR_FILL);
    for (int i = 0; i < 10000; i++)
    {
        Painter::SetPoint((*addr1) % WIDTH, Math_LimitationInt(*addr2, 0, 239));
        addr1++;
        addr2++;
    }
    */

    if(currentPage->OwnData()->type == TypePageHelp::Content)
    {
        DrawPageContent();
    }
    else if(currentPage->OwnData()->type == TypePageHelp::Description)
    {
        DrawPageDescription();
    }
    Painter::DrawFormatText(2, 230, COLOR_FILL, "%d", gTimerMS - startTime);
}

static int NumParagraphs(const PageHelpItem *page)
{
    int retValue = 0;
    while(page->OwnData()->pages[retValue])
    {
        retValue++;
    }
    return retValue;
}

void HelpContent_NextParagraph()
{
    if(currentPage->OwnData()->type == TypePageHelp::Content)
    {
        CircleIncreaseInt(&currentParagraph, 0, NumParagraphs(currentPage) - 1);
    }
}

void HelpContent_PrevParagraph()
{
    if(currentPage->OwnData()->type == TypePageHelp::Content)
    {
        CircleDecreaseInt(&currentParagraph, 0, NumParagraphs(currentPage) - 1);
    }
}

void HelpContent_EnterParagraph()
{
    if(currentPage->OwnData()->type == TypePageHelp::Content)
    {
        currentPage = (const PageHelpItem *)currentPage->OwnData()->pages[currentParagraph];
    }
    currentParagraph = 0;
}

void HelpContent_LeaveParagraph()
{
    currentParagraph = 0;
    if(currentPage->OwnData()->parent)
    {
        currentPage = (const PageHelpItem *)currentPage->OwnData()->parent;
    }
}

bool HelpContent_LeaveParagraphIsActive()
{
    return currentPage->OwnData()->parent != 0;
}

bool HelpContent_EnterParagraphIsActive()
{
    return currentPage->OwnData()->type == TypePageHelp::Content;
}
