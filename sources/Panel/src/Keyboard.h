#pragma once
#include "common/Panel/Controls_.h"


struct Keyboard
{
public:
    
    static void Init();

    static void Update();

    struct Buffer
    {
        // Возвращает true, если буфер пуст
        static bool IsEmpty();

        static int NumEvents();

        // Возвращает следующий орган управления, если таковой имеется
        static KeyboardEvent GetNextEvent();
    };
};
