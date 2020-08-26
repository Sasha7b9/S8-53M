#include "defines.h"
#include "log.h"
#include "common/Command.h"
#include "Hardware/HAL/HAL.h"
#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <cstdlib>


#define SIZE_BUFFER_LOG 200

static void AddToConsole(const char *text);


void Log::Write(TypeTrace type, const char *format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    char *pointer = buffer;

    if (type == TypeTrace_Error)
    {
        buffer[0] = 0;
        std::strcat(buffer, "!!! ERROR !!! ");
        while (*pointer++) {};
        ++pointer;
    }
    std::va_list args;
    va_start(args, format); //-V2528
    vsprintf(pointer, format, args);
    va_end(args);
    AddToConsole(buffer);
}


void Log::Trace(TypeTrace type, const char *module, const char *func, int numLine, char *format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    char message[SIZE_BUFFER_LOG];
    std::va_list args;
    va_start(args, format); //-V2528
    vsprintf(buffer, format, args);
    va_end(args);
    const int SIZE = 20;
    char numBuffer[SIZE];
    std::snprintf(numBuffer, 19, ":%d", numLine);
    message[0] = 0;

    if (type == TypeTrace_Error)
    {
        std::strcat(message, "!!!ERROR!!! ");
    }
    else if (type == TypeTrace_Info) //-V547
    {
        std::strcat(message, "            ");
    }
    else
    {
        // ������ ����� ���
    }

    std::strcat(message, module);
    std::strcat(message, " ");
    std::strcat(message, func);
    std::strcat(message, numBuffer);
    AddToConsole(message);
    AddToConsole(buffer);
}


static void AddToConsole(const char *text)
{
    uint8 *buffer = static_cast<uint8 *>(std::malloc(std::strlen(text) + 3U));

    if (buffer)
    {
        buffer[0] = Command::AddToConsole;
        buffer[1] = static_cast<uint8>(std::strlen(text));
        std::strcpy(reinterpret_cast<char *>(buffer + 1), text);
        HAL_BUS::SendToDevice(buffer, std::strlen(text) + 2);

        std::free(buffer);
    }
}
