#pragma once


#define MP_Y                    ((Grid::TOP) + 1)
#define MP_X                    (Grid::Right() - Item::TITLE_WIDTH - 1)
#define MI_WIDTH                (Item::TITLE_WIDTH + 1)
#define MOI_WIDTH               Item::TITLE_WIDTH
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
