#pragma once


#define MOI_WIDTH_D_IP          34      /* ���������� ������ ��������� ip-������ � ������ ������� ����� */

struct DrawMode { enum E
{
    Auto,
    Hand
};};

struct SourceBuffer { enum E
{
    Signals,
    Send,
    Intermediate
};};
