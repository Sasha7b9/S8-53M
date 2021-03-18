#pragma once


/*
    ����� ����������� ������������ ��� ��������������� ���������/������������ ������ �� ����
*/

class Buffer
{
public:
    Buffer(int size = 0);
    ~Buffer();
    void Realloc(int size);
    void Fill(uint8 value);
    inline int Size() const { return size; }
    inline char *DataChar() { return reinterpret_cast<char *>(data); }
    inline uint8 *Data() { return data; }
    void Log();
    static void LogU8(const uint8 *buffer);
private:
    int size;
    uint8 *data;
    void Free();
    void Malloc(int size);
};
