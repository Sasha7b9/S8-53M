// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class PageHelpItem;
class Item;


struct PageHelpContent
{
    static pchar stringForHint;   // Строка подсказки, которую надо выводить в случае включённого режима подсказок.
    static Item *itemHint;              // Item, подсказку для которого нужно выводить в случае включённого режима подсказок.

    static void Draw();
    static void NextParagraph();
    static void PrevParagraph();
    static void EnterParagraph();
    static void LeaveParagraph();
    static bool LeaveParagraphIsActive();
    static bool EnterParagraphIsActive();

    static void SetItemForHint(Item *item);

    struct PageMain
    {
        static const PageHelpItem *self;

        struct PageMenu
        {
            static const PageHelpItem *self;
        };
    };
};
