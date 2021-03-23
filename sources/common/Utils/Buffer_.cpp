#include "defines.h"
#include "common/Log_.h"
#include "common/Utils/Buffer_.h"
#include <cstdlib>
#include <cstring>



Buffer::Buffer(uint s) : data(nullptr)
{
    Malloc(s);
}


Buffer::~Buffer()
{
    Free();
}


void Buffer::Realloc(uint _size)
{
    Free();
    Malloc(_size);
}


void Buffer::Fill(uint8 value)
{
    if (size)
    {
        std::memset(data, value, (uint)(size));
    }
}


void Buffer::Free()
{
    std::free(data);
    data = nullptr;
    size = 0U;
}


void Buffer::Malloc(uint s)
{
    if (s > 0)
    {
        data = (uint8 *)(std::malloc((uint)(s)));
        size = (data) ? s : 0;

        if(!data)
        {
            LOG_ERROR("Нет памяти");
        }
    }
    else
    {
        data = nullptr;
        size = 0U;
    }
}


void Buffer::Log()
{
    LogU8(data);
}


void Buffer::LogU8(const uint8 *buffer)
{
    LOG_WRITE("%d %d %d %d %d %d %d %d %d %d", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9]); //-V2563
}


uint8 &Buffer::operator[](uint i) const
{
    if (i < Size())
    {
        return data[i];
    }

    static uint8 empty = 0;

    return empty;
}
