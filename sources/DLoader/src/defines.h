#pragma once

typedef unsigned char      uint8;
typedef unsigned int       uint;
typedef signed char        int8;
typedef unsigned short int uint16;
typedef signed short int   int16;
typedef unsigned char      uchar;
typedef const char *       pCHAR;
typedef const uint8 *      pUCHAR;
typedef const char *const  pString;

#ifndef __cplusplus
typedef unsigned char bool;
#define false   0
#define true    (!false)
#endif


struct Color { enum E
{
    BLACK,
    WHITE,
    GRID,
    DATA_A,
    DATA_B,
    MENU_FIELD,
    MENU_TITLE,
    MENU_TITLE_DARK,
    MENU_TITLE_BRIGHT,
    MENU_ITEM,
    MENU_ITEM_DARK,
    MENU_ITEM_BRIGHT,
    DATA_A_WHITE,
    DATA_B_WHITE,
    GRID_WHITE,
    EMPTY,
    Count,
    FLASH_10,
    FLASH_01,
    INVERSE
};};

typedef void    (*pFuncVV)(void);               // ????????? ?? ???????, ?????? ?? ??????????? ? ?????? ?? ????????????.
typedef void    (*pFuncVpV)(void*);             // ????????? ?? ???????, ??????????? ????????? ?? void ? ?????? ?? ????????????.
typedef bool    (*pFuncBV)(void);
typedef void    (*pFuncVB)(bool);
typedef void    (*pFuncVI)(int);
typedef void    (*pFuncVII)(int, int);
typedef void    (*pFuncVIIC)(int, int, Color);
typedef void    (*pFuncVI16)(int16);
typedef bool    (*pFuncBU8)(uint8);
typedef void    (*pFuncVI16pI16pI16)(int16, int16*, int16*);
typedef float   (*pFuncFU8)(uint8);
typedef char*   (*pFuncCFB)(float, bool);
typedef char*   (*pFuncCFBC)(float, bool, char*);
typedef void    (*pFuncpU8)(uint8*);
typedef void    (*pFuncVpVIIB)(void*, int, int, bool);

#define ERROR_VALUE_FLOAT   1.111e29F
#define ERROR_VALUE_INT16   SHRT_MAX
#define ERROR_VALUE_UINT8   255
#define ERROR_VALUE_INT     INT_MAX
#define ERROR_STRING_VALUE  "--.--"
#define M_PI                3.14159265358979323846F
