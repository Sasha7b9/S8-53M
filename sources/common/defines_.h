// 2021/03/22 14:37:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


typedef signed      char        int8;
typedef signed      short   int int16;
typedef unsigned    char        uint8;
typedef unsigned    short   int uint16;
typedef unsigned    int         uint;
typedef unsigned long long int  uint64;
typedef unsigned    char        uchar;
typedef const char             *pchar;
typedef const uint8            *puchar;
typedef const char *const       pstring;


typedef void  (*pFuncVV)();
typedef void  (*pFuncVII)(int, int);
typedef bool  (*pFuncBV)();
typedef void  (*pFuncVI)(int);
typedef void  (*pFuncVB)(bool);
typedef void  (*pFuncpCU8)(puchar);
typedef char* (*pFuncCFBC)(float, bool, char*);


inline void EmptyFuncVV() {};


#define _GET_BIT(value, bit) (((value) >> (bit)) & 0x01)
#define _SET_BIT(value, bit) ((value) |= (1 << (bit)))
#define _CLEAR_BIT(value, bit) ((value) &= (~(1 << (bit))))


struct Set2Int
{
    int value1;
    int value2;
};
