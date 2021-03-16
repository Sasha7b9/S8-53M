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

    int Size() const;

    bool IsEmpty() const;

    T &operator[](int n);

    T *Data();

private:

    void Destroy();
    
    T *pointer;         // Указатель на массив элементов
    int iFront;         // Индекс первого элемента
    int iBack;          // Индекс элемента за последним
};
