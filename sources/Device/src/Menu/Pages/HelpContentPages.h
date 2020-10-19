#pragma once
#include "Menu/MenuItems.h"


struct TypePageHelp { enum E
{
    Content,       // �� ���� �������� ����������� ������ �� ������ ��������
    Description    // �� ���� �������� ��� ������, ������ ��������
};};


#define MAX_PAGES 3

/** @todo ���������� �� ��������� funcNotUsed */

class PageHelp;

struct DataPageHelp
{
    TypePageHelp::E type;
    const PageHelp *parent;
    const PageHelp *pages[MAX_PAGES];   // ������ �������� ������ ��������� ������� � ������ TypePageHelp::Content
};

class PageHelp : public Item
{
public:
    PageHelp(const DataItem *const data) : Item(data) {}
    const DataPageHelp *OwnData() const { return static_cast<const DataPageHelp *>(data->ad); }
    static const PageHelp empty;
};

extern const PageHelp helpMenu;


DEF_PAGE_HELP(helpMenuCommon, helpMenu, TypePageHelp::Description,
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

DEF_PAGE_HELP(helpMenuControls, helpMenu, TypePageHelp::Description,
    "�������� ������� �����������", "Description of the controls"
    ,
    "",
    ""
)

extern const PageHelp helpMain;

DEF_PAGE_HELP(helpSCPI, helpMain, TypePageHelp::Description,
    "������ � SCPI", "Working with SCPI",
    "",
    ""
)

DEF_PAGE_HELP_2(helpMenu, helpMain, TypePageHelp::Content,
    "������ � ����", "Working with menus",
    "",
    "",
    helpMenuCommon,
    helpMenuControls
)

DEF_PAGE_HELP_2(helpMain, PageHelp::empty, TypePageHelp::Content,
    "������", "HELP",
    "",
    "",
    helpMenu,
    helpSCPI
)
