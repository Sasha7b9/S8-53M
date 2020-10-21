#include "Utils/GlobalFunctions.h"
#include "Globals.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/HelpContent.h"
#include "Definition.h"


void DrawSB_Help_ParagraphEnter(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, '\x4a');
    Font::Set(TypeFont::_8);
}

void DrawSB_Help_ParagraphLeave(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x48');
    Font::Set(TypeFont::_8);
}

void DrawSB_Help_ParagraphPrev(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 5, '\x4c');
    Font::Set(TypeFont::_8);
}

void DrawSB_Help_ParagraphNext(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 5, '\x4e');
    Font::Set(TypeFont::_8);
}

void OnHelpRegSet(int)
{

}

DEF_SMALL_BUTTON(sbHelpParagraphEnter, PageHelp::self,
    "Открыть", "Open",
    "Открывает раздел справки",
    "Opens the section of the reference",
    HelpContent_EnterParagraphIsActive, HelpContent_EnterParagraph, DrawSB_Help_ParagraphEnter, nullptr
)

DEF_SMALL_BUTTON(sbHelpParagraphLeave, PageHelp::self,
    "Закрыть", "Close",
    "Закрывает раздел справки",
    "Closes the section of the reference",
    HelpContent_LeaveParagraphIsActive, HelpContent_LeaveParagraph, DrawSB_Help_ParagraphLeave, nullptr
)

DEF_SMALL_BUTTON(sbHelpParagraphPrev, PageHelp::self,
    "Предыдущий раздел", "Previous section",
    "Выбрать предыдущий раздел справки",
    "To choose the previous section of the reference",
    nullptr, HelpContent_PrevParagraph, DrawSB_Help_ParagraphPrev, nullptr
)

DEF_SMALL_BUTTON(sbHelpParagraphNext, PageHelp::self,
    "", "",
    "",
    "",
    nullptr, HelpContent_NextParagraph, DrawSB_Help_ParagraphNext, nullptr
)

static void PressSB_Help_Exit()
{
    Display::RemoveAddDrawFunction();
}

DEF_SMALL_BUTTON(sbExitHelp, PageHelp::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, PressSB_Help_Exit, DrawSB_Exit, nullptr
)

static const arrayItems itemsHelp =
{
};

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
    nullptr, nullptr,  HelpContent_Draw, OnHelpRegSet
);


const Page *PageHelp::self = &pageHelp;
