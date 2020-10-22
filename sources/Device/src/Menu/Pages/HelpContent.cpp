#include "defines.h"
#include "common/Display/Primitives_c.h"
#include "common/Display/Text_c.h"
#include "common/Hardware/HAL/HAL_c.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Menu/Pages/HelpContent.h"
#include "Menu/Pages/HelpContentPages.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"


using namespace Primitives;

const char *PageHelpContent::stringForHint = 0;
Item *PageHelpContent::itemHint = 0;

const PageHelpItem PageHelpItem::empty(&Item::emptyData);


int currentParagraph = 0;   // ���� TypePage(currentPage) == TypePageHelp::Content, �� ��������� �� ������� ������ ����������
const PageHelpItem *currentPage = PageHelpContent::PageMain::self;


static const int WIDTH = 295;


static void DrawPageContent()
{
    int y = 50;
    Text(TITLE(currentPage)).DrawInCenterRect(0, y, WIDTH, 10);

    int numPage = 0;

    y += 40;

    while(currentPage->OwnData()->pages[numPage])
    {
        Page *page = (Page*)currentPage->OwnData()->pages[numPage];
        const char *title = TITLE(page);
        if(currentParagraph == numPage)
        {
            Painter::DrawStringInCenterRectOnBackgroundC(0, y, WIDTH, 10, title, Color::BACK, 2, Color::FILL);
        }
        else
        {
            Text(title).DrawInCenterRect(0, y, WIDTH, 10, Color::FILL);
        }
        y += 16;
        numPage++;
    }
}

static void DrawPageDescription()
{
    Text(TITLE(currentPage)).DrawInCenterRect(0, 3, WIDTH, 10);
    Painter::DrawTextInRectWithTransfers(2, 15, WIDTH - 5, 240, HINT(currentPage));
}


void PageHelpContent::Draw()
{
    uint startTime = gTimerMS;
    Region(319 - Grid::Right(), 20).Fill(Grid::Right(), 0, Color::BACK);
    Region(319 - Grid::Right(), 21).Fill(Grid::Right(), 219);
    Region(WIDTH, 237).Fill(1, 1);
    Rectangle(WIDTH + 2, 239).Draw(0, 0, Color::FILL);

    /*
    uint16 *addr1 = (uint16*)(0x08000000 + (rand() % 65535));
    uint8 *addr2 = (uint8*)(0x08000000 + (rand() % 65535));

    Color::SetCurrent(Color::FILL);
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
    Painter::DrawFormatText(2, 230, Color::FILL, "%d", gTimerMS - startTime);
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

void PageHelpContent::NextParagraph()
{
    if(currentPage->OwnData()->type == TypePageHelp::Content)
    {
        CircleIncreaseInt(&currentParagraph, 0, NumParagraphs(currentPage) - 1);
    }
}

void PageHelpContent::PrevParagraph()
{
    if(currentPage->OwnData()->type == TypePageHelp::Content)
    {
        CircleDecreaseInt(&currentParagraph, 0, NumParagraphs(currentPage) - 1);
    }
}

void PageHelpContent::EnterParagraph()
{
    if(currentPage->OwnData()->type == TypePageHelp::Content)
    {
        currentPage = (const PageHelpItem *)currentPage->OwnData()->pages[currentParagraph];
    }
    currentParagraph = 0;
}

void PageHelpContent::LeaveParagraph()
{
    currentParagraph = 0;
    if(currentPage->OwnData()->parent)
    {
        currentPage = (const PageHelpItem *)currentPage->OwnData()->parent;
    }
}

bool PageHelpContent::LeaveParagraphIsActive()
{
    return currentPage->OwnData()->parent != 0;
}

bool PageHelpContent::EnterParagraphIsActive()
{
    return currentPage->OwnData()->type == TypePageHelp::Content;
}

void PageHelpContent::SetItemForHint(Item *item)
{
    stringForHint = 0;
    itemHint = item;
}
