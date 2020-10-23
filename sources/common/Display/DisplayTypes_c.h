#pragma once


#define MOI_WIDTH_D_IP          34      /* Увеличение ширины открытого ip-адреса в случае наличия порта */

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
