#pragma once


#define MP_Y                    ((Grid::TOP) + 1)
#define MP_TITLE_WIDTH          79
#define MP_X                    (Grid::Right() - (MP_TITLE_WIDTH) - 1)
#define MP_TITLE_HEIGHT         34
#define MI_HEIGHT               33
#define MI_WIDTH                ((MP_TITLE_WIDTH) + 1)
#define MI_HEIGHT_VALUE         13
#define MI_WIDTH_VALUE          ((MI_WIDTH) - 4)
#define MOSI_HEIGHT             14
#define MOI_HEIGHT_TITLE        19
#define MOI_WIDTH               MP_TITLE_WIDTH
#define MOI_WIDTH_D_IP          34      /* ���������� ������ ��������� ip-������ � ������ ������� ����� */
#define WIDTH_SB                19
#define LEFT_SB                 299 

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