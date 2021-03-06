// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


template<typename T>
class Queue
{
public:
    Queue();

    ~Queue();

    void Push(T elem);

    T Front();

    T Back();

    void Clear();

    uint Size() const;

    bool IsEmpty() const;

    T &operator[](int n);

    T *Data();

private:

    void Destroy();
    
    T *pointer;         // ��������� �� ������ ���������
    int iFront;         // ������ ������� ��������
    int iBack;          // ������ �������� �� ���������
};
