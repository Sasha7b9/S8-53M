#pragma once


class PageHelpItem;


struct PageHelpContent
{
    static const char *stringForHint;   // ������ ���������, ������� ���� �������� � ������ ����������� ������ ���������.
    static Item *itemHint;              // Item, ��������� ��� �������� ����� �������� � ������ ����������� ������ ���������.

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
