#pragma once

typedef unsigned char      uint8;
typedef unsigned int       uint;
typedef signed char        int8;
typedef unsigned short int uint16;
typedef signed short int   int16;
typedef unsigned char      uchar;
typedef const char        *pchar;
typedef const uint8       *puchar; 
typedef const char *const  pString;

#ifndef __cplusplus
typedef unsigned char bool;
#define false   0
#define true    (!false)
#endif


#ifndef WIN32
#define nullptr NULL //-V1059
#endif


typedef void    (*pFuncVV)(void);               // ????????? ?? ???????, ?????? ?? ??????????? ? ?????? ?? ????????????.
typedef void    (*pFuncVpV)(void*);             // ????????? ?? ???????, ??????????? ????????? ?? void ? ?????? ?? ????????????.
typedef bool    (*pFuncBV)(void);
typedef void    (*pFuncVB)(bool);
typedef void    (*pFuncVI)(int);
typedef void    (*pFuncVII)(int, int);
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


#define _GET_BIT(value, bit) (((value) >> (bit)) & 0x01) //-V2573
#define _SET_BIT(value, bit) ((value) |= (1 << (bit))) //-V2573
#define _CLEAR_BIT(value, bit) ((value) &= (~(1 << (bit)))) //-V2573

#define LANG_RU true
