#pragma once


#define MALLOC_EXTRAMEM(NameStruct, name)   RAM::extra = std::malloc(sizeof(NameStruct));    \
                                            NameStruct *name = (NameStruct*)RAM::extra
#define ACCESS_EXTRAMEM(NameStruct, name)   NameStruct *name = (NameStruct*)RAM::extra //-V2546 //-V1003
#define FREE_EXTRAMEM()                     std::free(RAM::extra)


struct RAM
{
    static void *extra; // Это специальный указатель. Используется для выделения памяти переменным, которые не нужны всё время выполения программы,
                        // но нужны болеее чем в одной функции. Перед использованием с помощью вызова malloc() выделяется необходимое количество
                        // памяти, которая затем освобождается вызвом free()

};
