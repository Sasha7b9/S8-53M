#include "defines.h"
#include "common/Log.h"
#include "Display/Display.h"
#include "Settings/Settings.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>

#ifdef DEVICE

static bool loggerUSB = false;


#define SIZE_BUFFER_LOG 200


void Log_Write(char *format, ...)
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


void Log_Error(const char *module, const char *func, int numLine, char *format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(buffer, format, args);
    va_end(args);
    char numBuffer[20];
    std::sprintf(numBuffer, ":%d", numLine);
    char message[SIZE_BUFFER_LOG];
    message[0] = 0;
    std::strcat(message, "!!!ERROR!!! ");
    std::strcat(message, module);
    std::strcat(message, " ");
    std::strcat(message, func);
    std::strcat(message, numBuffer);
    Display::AddStringToIndicating(message);
    Display::AddStringToIndicating(buffer);
    if(loggerUSB)
    {
        //VCP::SendFormatStringAsynch(message);
        //VCP::SendFormatStringAsynch(buffer);
    }
}


void Log_DisconnectLoggerUSB(void)
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

void Log_Error(const char *, const char *, int, char *, ...)
{
}


void Log_Write(char *, ...)
{
}

#endif
