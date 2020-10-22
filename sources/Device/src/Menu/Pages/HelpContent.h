#pragma once


class PageHelpItem;
class Item;


struct PageHelpContent
{
    static const char *stringForHint;   // ������ ���������, ������� ���� �������� � ������ ����������� ������ ���������.
    static Item *itemHint;              // Item, ��������� ��� �������� ����� �������� � ������ ����������� ������ ���������.

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
