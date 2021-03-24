#include "defines.h"
#include "common/Display/Colors_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Utils/StringUtils_.h"
#include "common/Utils/Containers/Buffer_.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>


pchar const String::_ERROR = "---.---";


String::String() : buffer(nullptr), capacity(0)
{
    Set("");
}


String::String(const String &rhs) : buffer(nullptr), capacity(0)
{
    if (Allocate(std::strlen(rhs.c_str()) + 1))
    {
        std::strcpy(buffer, rhs.c_str());
    }
}


String::String(char symbol) : buffer(nullptr), capacity(0)
{
    if (Allocate(2))
    {
        buffer[0] = symbol;
        buffer[1] = 0;
    }
}


String::String(pchar format, ...) : buffer(nullptr), capacity(0)
{
#define SIZE_TEMP_BUFFER 1024

    char temp_buffer[1024];

    std::va_list args;
    va_start(args, format);
    uint num_symbols = (uint)std::vsprintf(temp_buffer, format, args);

    if (capacity < num_symbols)
    {
        Allocate(num_symbols);
    }

    if (buffer)
    {
        std::strcpy(buffer, temp_buffer);
    }
}


void String::Set(pchar format, ...)
{
#define SIZE_TEMP_BUFFER 1024

    char temp_buffer[1024];

    std::va_list args;
    va_start(args, format);
    uint num_symbols = (uint)std::vsprintf(temp_buffer, format, args);

    if (capacity < num_symbols)
    {
        Allocate(num_symbols);
    }

    if (buffer)
    {
        std::strcpy(buffer, temp_buffer);
    }
}


void String::Append(pchar str)
{
    Append(str, std::strlen(str));
}


void String::Append(pchar str, uint num_symbols)
{
    if (!str || *str == '\0')
    {
        return;
    }

    uint need_size = Size() + num_symbols + 1;

    if (capacity < need_size)
    {
        String old(*this);
        Allocate(need_size);
        std::strcpy(buffer, old.c_str());
    }

    std::memcpy(buffer + std::strlen(buffer), str, num_symbols);
    buffer[need_size - 1] = '\0';
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
        capacity = 0;
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

    capacity = NeedMemory(size);

    buffer = (char *)(std::malloc(capacity));

    if (buffer)
    {
        return true;
    }

    capacity = 0;

    return false;
}


uint String::NeedMemory(uint size)
{
    if (size % SIZE_SEGMENT == 0)
    {
        return size;
    }
    else
    {
        return (size / SIZE_SEGMENT) * SIZE_SEGMENT + SIZE_SEGMENT;
    }
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


bool String::ToInt(int *out)
{
    char *str = buffer;

    int sign = str[0] == '-' ? -1 : 1;

    if (str[0] < '0' || str[0] > '9')
    {
        ++str;
    }

    int length = (int)(std::strlen(str));

    if (length == 0)
    {
        return false;
    }

    *out = 0;

    int pow = 1;

    for (int i = length - 1; i >= 0; i--)
    {
        int val = str[i] & (~(0x30));
        if (val < 0 || val > 9)
        {
            return false;
        }
        *out += val * pow;
        pow *= 10;
    }

    if (sign == -1)
    {
        *out *= -1;
    }

    return true;
}
