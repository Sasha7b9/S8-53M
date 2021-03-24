#include "defines.h"
#include "common/Log_.h"
#include "common/Utils/Containers/Buffer_.h"
#include "common/Utils/Containers/Values_.h"
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


String Buffer::Log()
{
    String result;

    for (uint i = 0; i < Size(); i++)
    {
        result.Append(Int(data[i]).ToString(false, 1).c_str());

        if (i != size - 1)
        {
            result.Append(' ');
        }
    }

    return result;
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
