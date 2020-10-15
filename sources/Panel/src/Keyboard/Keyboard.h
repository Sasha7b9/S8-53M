#pragma once
#include "common/Keyboard/Controls_c.h"


struct Keyboard
{
public:
    
    static void Init();

    static void Update();

    struct Buffer
    {
        // Возвращает true, если буфер пуст
        static bool IsEmpty();

        // Возвращает следующий орган управления, если таковой имеется
        static KeyboardEvent GetNextEvent();
    };
};
