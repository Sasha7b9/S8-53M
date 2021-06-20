// 2021/03/22 14:37:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#ifdef __ARMCC_VERSION
#pragma anon_unions
#endif


typedef signed      char        int8;
typedef unsigned char uint8_t;
typedef signed      short   int int16;
typedef unsigned    char        uint8;
typedef unsigned    short   int uint16;
typedef unsigned    int         uint;
typedef unsigned int uint32_t;
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


union BitSet16
{
    uint16 half_word;
    struct
    {
        uint8 byte0;
        uint8 byte1;
    };
};

// Объединение размером 32 бита
union BitSet32
{
    uint   word;
    uint16 half_word[2];
    uint8  byte[4];
};

// Объединение размером 64 бита
union BitSet64
{
    uint64  dword;
    uint    word[2];
    struct
    {
        int first;
        int second;
    };
    struct
    {
        uint u_first;
        uint u_second;
    };
};

