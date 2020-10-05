#pragma once


struct TypePageHelp { enum E
{
    Content,       // �� ���� �������� ����������� ������ �� ������ ��������
    Description    // �� ���� �������� ��� ������, ������ ��������
};};


#define MAX_PAGES 3

/** @todo ���������� �� ��������� funcNotUsed */

struct PageHelp
{
    TypePageHelp::E type;
    void*           parent;             // ����� ������������ ��������
    pFuncBV         funcNotUsed;        // ��������� ��� ������������� � ����� Page - ����� ��� ���������.
    char*           titleHint[4];       // �������� �������� �� ������� � ���������� ������, � ����� ���������� �� ������� � ���������� (��� ������ TypePageHelp::Description)
    void*           pages[MAX_PAGES];   // ������ �������� ������ ��������� ������� � ������ TypePageHelp::Content
};

extern const PageHelp helpMenu;

static const PageHelp helpMenuCommon =
{
    TypePageHelp::Description, (void*)&helpMenu, 0,
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
    TypePageHelp::Description, (void*)&helpMenu, 0,
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
    TypePageHelp::Description, (void*)&helpMain, 0,
    {
        "������ � SCPI", "Working with SCPI",
        "",
        ""
    }
};

static const PageHelp helpMenu =
{
    TypePageHelp::Content, (void*)&helpMain, 0,
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
    TypePageHelp::Content,  0, 0,
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
