#pragma once
#include "common/Utils/Containers/String_.h"


/*
    ����� ����������� ������������ ��� ��������������� ���������/������������ ������ �� ����
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

    T &operator[](uint i) const;

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
