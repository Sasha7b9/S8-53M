// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once   
#include "defines.h"


#ifdef WIN32
#define __MODULE__ ""
#endif

#ifdef __linux__
#define __MODULE__ ""
#endif


#define LOG_WRITE(...)   Log_Write(__VA_ARGS__)
#define TLOG_WRITE(...)  Log_TraceWrite(__FUNCTION__, __LINE__, __VA_ARGS__);
#define LOG_ERROR(...)   Log_Error(__MODULE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_FUNC_ENTER() Log_Write("%s:%d enter", __FUNCTION__, __LINE__);
#define LOG_FUNC_LEAVE() Log_Write("%s:%d leave", __FUNCTION__, __LINE__);
#define LOG_TRACE()      Log_Write("%s : %d", __MODULE__, __LINE__);

void Log_Error(pchar module, pchar func, int numLine, char *format, ...);
#ifdef __cplusplus
extern "C" {
#endif
    void Log_Write(pchar format, ...);
#ifdef __cplusplus
}
#endif
void Log_TraceWrite(pchar func, int numLine, char *format, ...);
void Log_DisconnectLoggerUSB();
void Log_EnableLoggerUSB(bool enable);
