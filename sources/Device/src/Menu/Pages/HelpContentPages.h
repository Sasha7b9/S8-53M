#pragma once
#include "Menu/MenuItems.h"


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
    const DataPageHelp *OwnData() const { return static_cast<const DataPageHelp *>(data->ad); }
    static const PageHelpItem empty;
};

extern const PageHelpItem helpMenu;


DEF_PAGE_HELP(helpMenuCommon, helpMenu, TypePageHelp::Description, //-V1043
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

DEF_PAGE_HELP(helpMenuControls, helpMenu, TypePageHelp::Description, //-V1043
    "�������� ������� �����������", "Description of the controls"
    ,
    "",
    ""
)

extern const PageHelpItem helpMain;

DEF_PAGE_HELP(helpSCPI, helpMain, TypePageHelp::Description, //-V1043
    "������ � SCPI", "Working with SCPI",
    "",
    ""
)

DEF_PAGE_HELP_2(helpMenu, helpMain, TypePageHelp::Content, //-V1043
    "������ � ����", "Working with menus",
    "",
    "",
    helpMenuCommon,
    helpMenuControls
)

DEF_PAGE_HELP_2(helpMain, PageHelpItem::empty, TypePageHelp::Content, //-V1043
    "������", "HELP",
    "",
    "",
    helpMenu,
    helpSCPI
)
