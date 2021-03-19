#pragma once


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

    inline uint8 *DataConst() const { return data; }

    void Log();

    static void LogU8(const uint8 *buffer);

private:

    uint size;

    uint8 *data;

    void Malloc(uint size);
};
