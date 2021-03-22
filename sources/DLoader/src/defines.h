#pragma once
#include "common/defines_.h"


#ifndef __cplusplus
typedef unsigned char bool;
#define false   0
#define true    (!false)
#endif


#ifndef WIN32
#define nullptr NULL //-V1059
#endif


typedef void  (*pFuncVV)();
typedef void  (*pFuncVpV)(void*);
typedef bool  (*pFuncBV)();
typedef void  (*pFuncVB)(bool);
typedef void  (*pFuncVI)(int);
typedef void  (*pFuncVII)(int, int);
typedef void  (*pFuncVI16)(int16);
typedef bool  (*pFuncBU8)(uint8);
typedef void  (*pFuncVI16pI16pI16)(int16, int16*, int16*);
typedef float (*pFuncFU8)(uint8);
typedef char* (*pFuncCFB)(float, bool);
typedef char* (*pFuncCFBC)(float, bool, char*);
typedef void  (*pFuncpU8)(uint8*);
typedef void  (*pFuncVpVIIB)(void*, int, int, bool);

#define LANG_RU true
