#pragma once
#include "Menu/Items/MenuItems.h"
#include "Menu/Pages/HelpContent.h"


struct TypePageHelp { enum E
{
    Content,       // �� ���� �������� ����������� ������ �� ������ ��������
    Description    // �� ���� �������� ��� ������, ������ ��������
};};


#define MAX_PAGES 3

/** @todo ���������� �� ��������� funcNotUsed */

class PageHelpItem;

struct DataPageHelp
{
    TypePageHelp::E type;
    const PageHelpItem *parent;
    const PageHelpItem *pages[MAX_PAGES];   // ������ �������� ������ ��������� ������� � ������ TypePageHelp::Content
};

class PageHelpItem : public Item
{
public:
    PageHelpItem(const DataItem *const data) : Item(data) {}
    const DataPageHelp *OwnData() const { return (const DataPageHelp *)(data->ad); }
    static const PageHelpItem empty;
};

DEF_PAGE_HELP(helpMenuCommon, PageHelpContent::PageMain::PageMenu::self, TypePageHelp::Description,
    "����� �������� ��������� ����", "General description of the principles of the menu"
    ,
    "������ �� ������ ���������� ����� ��� ���� ������� - ��������, ������������� ����� 0.5 ��� � �������, ������������� ����� 0.5 ���. "
    "���� ������������ ����� ����������� ��������� ���������. "
    "������� �������� ���� ����������� �������� ���� ������� �������� ������ ����. "
    "�� ���� ��������� ������� �������� ������� ���������������� ������ � ������� ������� ����� �� ������ �� ������ ����������:\n"
    "\"�������\" - �������\n"
    "\"����� 1\" - ����� 1\n"
    "\"����� 2\" - ����� 2\n"
    "\"�����.\" - �����\n"
    "\"����E����\" - ����\n"
    "\"�������\" - �������\n"
    "\"������\" - ������\n"
    "\"���������\" - �����\n"
    "\"������\" - ������\n"
    "\"������\" - ������\n"
    "� ������ �������� ������� �������� ������� ��������� ��������������� ���������"
    ,
    ""    
)

DEF_PAGE_HELP(helpMenuControls, PageHelpContent::PageMain::PageMenu::self, TypePageHelp::Description,
    "�������� ������� �����������", "Description of the controls"
    ,
    "",
    ""
)

DEF_PAGE_HELP(helpSCPI, PageHelpContent::PageMain::self, TypePageHelp::Description,
    "������ � SCPI", "Working with SCPI",
    "",
    ""
)

DEF_PAGE_HELP_2(helpMenu, PageHelpContent::PageMain::self, TypePageHelp::Content,
    "������ � ����", "Working with menus",
    "",
    "",
    helpMenuCommon,
    helpMenuControls
)

DEF_PAGE_HELP_2(pageHelpMain, &PageHelpItem::empty, TypePageHelp::Content,
    "������", "HELP",
    "",
    "",
    helpMenu,
    helpSCPI
)


const PageHelpItem *PageHelpContent::PageMain::self = &pageHelpMain;
const PageHelpItem *PageHelpContent::PageMain::PageMenu::self = &helpMenu;
