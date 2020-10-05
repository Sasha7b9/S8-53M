#pragma once


enum TypePageHelp
{
    TypePage::EContent,       // �� ���� �������� ����������� ������ �� ������ ��������
    TypePage::EDescription    // �� ���� �������� ��� ������, ������ ��������
};


#define MAX_PAGES 3

/** @todo ���������� �� ��������� funcNotUsed */

struct PageHelp
{
    TypePageHelp    type;
    void*           parent;             // ����� ������������ ��������
    pFuncBV         funcNotUsed;        // ��������� ��� ������������� � ����� Page - ����� ��� ���������.
    char*           titleHint[4];       // �������� �������� �� ������� � ���������� ������, � ����� ���������� �� ������� � ���������� (��� ������ TypePage::EDescription)
    void*           pages[MAX_PAGES];   // ������ �������� ������ ��������� ������� � ������ TypePage::EContent
};

extern const PageHelp helpMenu;

static const PageHelp helpMenuCommon =
{
    TypePage::EDescription, (void*)&helpMenu, 0,
    {
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
    }
};

static const PageHelp helpMenuControls =
{
    TypePage::EDescription, (void*)&helpMenu, 0,
    {
        "�������� ������� �����������", "Description of the controls"
        ,
        "",
        ""
    }
};

extern const PageHelp helpMain;

static const PageHelp helpSCPI =
{
    TypePage::EDescription, (void*)&helpMain, 0,
    {
        "������ � SCPI", "Working with SCPI",
        "",
        ""
    }
};

static const PageHelp helpMenu =
{
    TypePage::EContent, (void*)&helpMain, 0,
    {
        "������ � ����", "Working with menus",
        "",
        ""
    },
    {
        (void*)&helpMenuCommon,
        (void*)&helpMenuControls
    }
};

static const PageHelp helpMain =
{
    TypePage::EContent,  0, 0,
    {
        "������", "HELP",
        "",
        ""
    },
    {
        (void*)&helpMenu,
        (void*)&helpSCPI
    }
};
