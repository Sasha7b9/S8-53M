#pragma once

#ifdef WIN32
#define __attribute__(x) //-V2573
#define __packed__ //-V2573
#else
#define nullptr 0 //-V1059
#endif


#define NUM_VER "1.3"


typedef signed      char        int8;
typedef signed      short   int int16;
typedef unsigned    char        uint8;
typedef unsigned    short   int uint16;
typedef unsigned    int         uint;
typedef unsigned long long int  uint64;
typedef unsigned    char        uchar;
typedef const char             *pCHAR;
typedef const uint8            *pUCHAR;
typedef const char       *const pString;

#ifndef __IO
#define __IO volatile //-V2573
#endif

typedef void    (*pFuncVV)();
typedef bool    (*pFuncBV)();
typedef void    (*pFuncVB)(bool);
typedef void    (*pFuncVI)(int);
typedef void    (*pFuncVII)(int, int);
typedef char*   (*pFuncCFBC)(float, bool, char*);
typedef void    (*pFuncpU8)(uint8*);
typedef void    (*pFuncpCU8)(pUCHAR );

#define _GET_BIT(value, bit) (((value) >> (bit)) & 0x01) //-V2573
#define _SET_BIT(value, bit) ((value) |= (1 << (bit))) //-V2573
#define _CLEAR_BIT(value, bit) ((value) &= (~(1 << (bit)))) //-V2573

// Объединение размером 32 бита
union BitSet32 //-V2514
{
    uint  word;
    uint8 byte[4]; //-V112
};

// Объединение размером 64 бита
union BitSet64 //-V2514
{
    long long unsigned int  dword;
    unsigned int            word[2];
} ;

struct StructRelAbs
{
    int16 rel;
    float abs;
};

#define _bitset(bits)                               \
  ((uint8)(                                         \
  (((uint8)((uint)bits / 01)        % 010) << 0) |  \
  (((uint8)((uint)bits / 010)       % 010) << 1) |  \
  (((uint8)((uint)bits / 0100)      % 010) << 2) |  \
  (((uint8)((uint)bits / 01000)     % 010) << 3) |  \
  (((uint8)((uint)bits / 010000)    % 010) << 4) |  \
  (((uint8)((uint)bits / 0100000)   % 010) << 5) |  \
  (((uint8)((uint)bits / 01000000)  % 010) << 6) |  \
  (((uint8)((uint)bits / 010000000) % 010) << 7)))

#define BINARY_U8( bits ) _bitset(0##bits)

#define DISABLE_RU  "Откл"
#define DISABLE_EN  "Disable"
#define ENABLE_RU   "Вкл"
#define ENABLE_EN   "Enable"

#define ERROR_VALUE_FLOAT   1.111e29F
#define ERROR_VALUE_INT16   SHRT_MAX
#define ERROR_VALUE_UINT8   255
#define ERROR_VALUE_INT     INT_MAX
#define ERROR_STRING_VALUE  "--.--"
#define M_PI                3.14159265358979323846F

#define EXIT_RU "Выход"
#define EXIT_EN "Exit"
#define EXIT_ILLUSTRATION_RU "Кнопка для выхода в предыдущее меню"
#define EXIT_ILLUSTRATION_EN "Button for return to previous menu"

void HardwareErrorHandler(const char *file, const char *function, int line);
