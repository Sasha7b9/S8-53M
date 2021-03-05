#include "Utils/GlobalFunctions.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Pages/HelpContent.h"
#include "Definition.h"


void DrawSB_Help_ParagraphEnter(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x4a').Draw4SymbolsInRect(x + 2, y + 2);
    Font::Set(TypeFont::S8);
}

void DrawSB_Help_ParagraphLeave(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x48').Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}

void DrawSB_Help_ParagraphPrev(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x4c').Draw4SymbolsInRect(x + 2, y + 5);
    Font::Set(TypeFont::S8);
}

void DrawSB_Help_ParagraphNext(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x4e').Draw4SymbolsInRect(x + 2, y + 5);
    Font::Set(TypeFont::S8);
}

void OnHelpRegSet(int)
{

}

DEF_SMALL_BUTTON(sbHelpParagraphEnter, PageHelp::self,
    "Открыть", "Open",
    "Открывает раздел справки",
    "Opens the section of the reference",
    PageHelpContent::EnterParagraphIsActive, PageHelpContent::EnterParagraph, DrawSB_Help_ParagraphEnter, nullptr
)

DEF_SMALL_BUTTON(sbHelpParagraphLeave, PageHelp::self,
    "Закрыть", "Close",
    "Закрывает раздел справки",
    "Closes the section of the reference",
    PageHelpContent::LeaveParagraphIsActive, PageHelpContent::LeaveParagraph, DrawSB_Help_ParagraphLeave, nullptr
)

DEF_SMALL_BUTTON(sbHelpParagraphPrev, PageHelp::self,
    "Предыдущий раздел", "Previous section",
    "Выбрать предыдущий раздел справки",
    "To choose the previous section of the reference",
    nullptr, PageHelpContent::PrevParagraph, DrawSB_Help_ParagraphPrev, nullptr
)

DEF_SMALL_BUTTON(sbHelpParagraphNext, PageHelp::self,
    "", "",
    "",
    "",
    nullptr, PageHelpContent::NextParagraph, DrawSB_Help_ParagraphNext, nullptr
)

static void PressSB_Help_Exit()
{
    Display::RemoveAddDrawFunction();
}

DEF_SMALL_BUTTON(sbExitHelp, PageHelp::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, PressSB_Help_Exit, DrawSB_Exit, nullptr
)

DEF_PAGE_6(pageHelp, PageMain::self, NamePage::SB_Help,
    "ПОМОЩЬ", "HELP",
    "Открыть разделы помощи",
    "To open sections of the help",
    sbExitHelp,
    sbHelpParagraphEnter,
    sbHelpParagraphLeave,
    Item::empty,
    sbHelpParagraphPrev,
    sbHelpParagraphNext,
    nullptr, nullptr,  PageHelpContent::Draw, OnHelpRegSet
);


const Page *PageHelp::self = &pageHelp;
