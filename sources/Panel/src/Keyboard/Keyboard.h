#pragma once
#include "Keyboard/Controls.h"


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
        static Control GetNextControl();
    private:
        friend struct Keyboard;
        friend struct GovernorStruct;
        friend struct KeyStruct;
        static void AppendEvent(Key::E key, Action::E action);
    };
};
