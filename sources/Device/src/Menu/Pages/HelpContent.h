#pragma once


class PageHelpItem;


struct PageHelpContent
{
    static const char *stringForHint;   // Строка подсказки, которую надо выводить в случае включённого режима подсказок.
    static Item *itemHint;              // Item, подсказку для которого нужно выводить в случае включённого режима подсказок.

    struct PageMain
    {
        static const PageHelpItem *self;
    };
};


void HelpContent_Draw();
void HelpContent_NextParagraph();
void HelpContent_PrevParagraph();
void HelpContent_EnterParagraph();
void HelpContent_LeaveParagraph();
bool HelpContent_LeaveParagraphIsActive();
bool HelpContent_EnterParagraphIsActive();
