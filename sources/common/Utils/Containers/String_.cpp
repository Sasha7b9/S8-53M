#include "defines.h"
#include "common/Display/Primitives_.h"
#include "common/Display/Text_.h"
#include "common/Utils/StringUtils_.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>


pchar const String::_ERROR = "---.---";


String::String() : buffer(nullptr)
{
    Set("");
}


String::String(const String &rhs) : buffer(nullptr)
{
    Set("");

    if (Allocate(std::strlen(rhs.c_str()) + 1))
    {
        std::strcpy(buffer, rhs.c_str());
    }
}


String::String(char symbol) : buffer(nullptr)
{
    Set("");

    if (Allocate(2))
    {
        buffer[0] = symbol;
        buffer[1] = 0;
    }
}


String::String(pchar format, ...) : buffer(nullptr)
{
    Set("");

    if (format == nullptr)
    {
        return;
    }

    static const int SIZE = 127;

    char buf[SIZE + 1];

    std::va_list args;
    va_start(args, format);
    int numSymbols = std::vsprintf(buf, format, args);
    va_end(args);

    if (numSymbols < 0 || numSymbols > SIZE)
    {
#define ERROR_STRING "Буфер слишком мал"

        Allocate(std::strlen(ERROR_STRING) + 1);

        std::strcpy(buffer, ERROR_STRING);
    }
    else if (Allocate(std::strlen(buf) + 1))
    {
        std::strcpy(buffer, buf);
    }
}


void String::Set(pchar format, ...)
{
    Free();

    if(format)
    {
        static const int SIZE = 100;
        char buf[SIZE + 1];

        std::va_list args;
        va_start(args, format);
        int numSymbols = std::vsprintf(buf, format, args);
        va_end(args);

        if(numSymbols < 0 || numSymbols > SIZE)
        {
            std::strcpy(buffer, "Буфер слишком мал");
        }
        else if(Allocate(std::strlen(buf) + 1))
        {
            std::strcpy(buffer, buf);
        }
    }
}


void String::Append(pchar str)
{
    if (!str || *str == '\0')
    {
        return;
    }

    String old(*this);

    Free();

    Allocate(old.Size() + std::strlen(str) + 1);

    std::strcpy(buffer, old.c_str());
    std::strcat(buffer, str);
}


void String::Append(pchar str, uint numSymbols)
{
    if (!str || *str == '\0')
    {
        return;
    }

    String old(*this);

    Free();

    uint size = numSymbols + old.Size() + 1;

    Allocate(size);

    std::strcpy(buffer, old.c_str());
    std::memcpy(buffer + old.Size(), str, (uint)(numSymbols));
    buffer[size - 1] = '\0';
}


void String::Append(char symbol)
{
    char b[2] = { symbol, '\0' };
    Append(b);
}


String::~String()
{
    std::free(buffer);
}


void String::Free()
{
    if(buffer)
    {
        std::free(buffer);
        buffer = nullptr;
        Set("");
    }
}


char *String::c_str() const
{
    return buffer;
}


bool String::Allocate(uint size)
{
    std::free(buffer);
    buffer = (char *)(std::malloc((uint)(size)));
    if (buffer)
    {
        return true;
    }

    return false;
}


int String::Draw(int x, int y, const Color &color) const
{
    color.SetAsCurrent();

    return Draw(x, y);
}


int String::Draw(int x, int y) const
{
    return Text(c_str()).Draw(x, y);
}


uint String::Size() const
{
    if (buffer == nullptr)
    {
        return 0;
    }

    return std::strlen(buffer);
}


char &String::operator[](uint i) const
{
    static char result = 0;

    if (buffer == nullptr || Size() < i)
    {
        return result;
    }

    return buffer[i];
}
