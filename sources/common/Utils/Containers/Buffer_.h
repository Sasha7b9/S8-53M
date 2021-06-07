// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Containers/String_.h"


/*
     ласс единственно предназначен дл€ автоматического выделени€/освобождени€ пам€ти из кучи
*/

template<class T>
class Buffer
{
public:

    Buffer(uint size = 0U);

    ~Buffer();

    void Realloc(uint size);

    void Free();

    void Fill(T value);

    inline uint Size() const { return size; }

    inline char *DataChar() { return (char *)data; }

    inline uint8 *DataU8() { return (uint8 *)data; }

    inline T *Data() { return data; }

    T &operator[](uint i) const;
    T &operator[](int i) const;

    static float Sum(T *data, uint number);

private:

    uint size;

    T *data;

    void Malloc(uint size);
};


class BufferU8 : public Buffer<uint8>
{
public:
    BufferU8(uint size = 0U) : Buffer<uint8>(size) {}

    String Log();
};
