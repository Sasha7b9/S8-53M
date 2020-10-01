#pragma once


//#include "defines.h"


#define SCREEN_WIDTH            320
#define SCREEN_WIDTH_IN_BYTES   (320 / 8)
#define SCREEN_HEIGHT           240
#define SCREEN_HEIGHT_IN_BYTES  (240 / 8)
#define SIZE_BUFFER_FOR_SCREEN  ((SCREEN_WIDTH) * (SCREEN_HEIGHT) / 8)

#define GRID_DELTA              20
#define GRID_TOP                19
#define GRID_CELLS_IN_HEIGHT    10
#define GRID_HEIGHT             ((GRID_CELLS_IN_HEIGHT) * (GRID_DELTA))
#define GRID_BOTTOM             ((GRID_TOP) + (GRID_HEIGHT))
#define GRID_WIDTH              (280)

#define GRID_RIGHT              ((Grid::Left()) + (Grid::Width()))

#define CUR_WIDTH               6
#define CUR_HEIGHT              8

#define MP_Y                    ((GRID_TOP) + 1)
#define MP_TITLE_WIDTH          79
#define MP_X                    ((GRID_RIGHT) - (MP_TITLE_WIDTH) - 1)
#define MP_TITLE_HEIGHT         34
#define MI_HEIGHT               33
#define MI_WIDTH                ((MP_TITLE_WIDTH) + 1)
#define MI_HEIGHT_VALUE         13
#define MI_WIDTH_VALUE          ((MI_WIDTH) - 4)
#define MOSI_HEIGHT             14
#define MOI_HEIGHT_TITLE        19
#define MOI_WIDTH               MP_TITLE_WIDTH
#define MOI_WIDTH_D_IP          34      /* Увеличение ширины открытого ip-адреса в случае наличия порта */
#define WIDTH_SB                19
#define LEFT_SB                 299 

struct DrawMode
{
    enum E
    {
        Auto,
        Hand
    };
};

struct SourceBuffer
{
    enum E
    {
        Signals,
        Send,
        Intermediate
    };
};

struct TypeFont
{
    enum E
    {
        _5,
        _8,
        _UGO,
        _UGO2,
        _Count,
        _None
    };
};
