#include "defines.h"
#include "common/Log_.h"
#include "Display/Display.h"
#include "Settings/Settings.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>

#ifdef DEVICE

static bool loggerUSB = false;


#define SIZE_BUFFER_LOG 200


void Log_Write(pchar format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(buffer, format, args);
    va_end(args);
    Display::AddStringToIndicating(buffer);
    if(loggerUSB)
    {
        //VCP::SendFormatStringAsynch(buffer);
    }
}


void Log_Error(pchar module, const char *func, int num_line, char *format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(buffer, format, args);
    va_end(args);

    String message("!!!ERROR!!! %s %s:%", module, func, num_line);

    Display::AddStringToIndicating(message.c_str());

    Display::AddStringToIndicating(buffer);

    if(loggerUSB)
    {
        //VCP::SendFormatStringAsynch(message);
        //VCP::SendFormatStringAsynch(buffer);
    }
}


void Log_DisconnectLoggerUSB()
{
    //static uint8 data = 20;
    //Log_Write("посылаю %d", data);
    //VCP_SendData(&data, 1);
}


void Log_EnableLoggerUSB(bool enable)
{
    loggerUSB = enable;
}
#else

void Log_Error(pchar, pchar, int, char *, ...)
{
}


void Log_Write(pchar, ...)
{
}

#endif
