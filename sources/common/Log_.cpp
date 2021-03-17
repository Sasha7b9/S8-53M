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


void Log_Write(pchar format, ...) //-V2560
{
    char buffer[SIZE_BUFFER_LOG];
    std::va_list args;
    va_start(args, format); //-V2563 //-V2567 //-V2571
    std::vsprintf(buffer, format, args);
    va_end(args);
    Display::AddStringToIndicating(buffer);
    if(loggerUSB)
    {
        //VCP::SendFormatStringAsynch(buffer);
    }
}


void Log_Error(pchar module, const char *func, int numLine, char *format, ...) //-V2560
{
    char buffer[SIZE_BUFFER_LOG];
    std::va_list args;
    va_start(args, format); //-V2563 //-V2567 //-V2571
    std::vsprintf(buffer, format, args);
    va_end(args);
    char numBuffer[20];
    std::sprintf(numBuffer, ":%d", numLine);
    char message[SIZE_BUFFER_LOG];
    message[0] = 0;
    std::strcat(message, "!!!ERROR!!! "); //-V2513
    std::strcat(message, module); //-V2513
    std::strcat(message, " "); //-V2513
    std::strcat(message, func); //-V2513
    std::strcat(message, numBuffer); //-V2513
    Display::AddStringToIndicating(message);
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
