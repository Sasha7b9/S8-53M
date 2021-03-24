#pragma once
#include "common/Utils/Containers/String_.h"


/*
    ����� ����������� ������������ ��� ��������������� ���������/������������ ������ �� ����
*/

class Buffer
{
public:

    Buffer(uint size = 0U);

    ~Buffer();

    void Realloc(uint size);

    void Free();

    void Fill(uint8 value);

    inline uint Size() const { return size; }

    inline char *DataChar() { return reinterpret_cast<char *>(data); }

    inline uint8 *Data() { return data; }

    String Log();

    uint8 &operator[](uint i) const;

private:

    uint size;

    uint8 *data;

    void Malloc(uint size);
};
