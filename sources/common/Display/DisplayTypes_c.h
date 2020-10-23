#pragma once


#define MP_Y                    ((Grid::TOP) + 1)
#define MP_X                    (Grid::Right() - Item::TITLE_WIDTH - 1)
#define MI_WIDTH                (Item::TITLE_WIDTH + 1)
#define MI_WIDTH_VALUE          ((MI_WIDTH) - 4)
#define MOI_HEIGHT_TITLE        19
#define MOI_WIDTH               Item::TITLE_WIDTH
#define MOI_WIDTH_D_IP          34      /* Увеличение ширины открытого ip-адреса в случае наличия порта */
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
