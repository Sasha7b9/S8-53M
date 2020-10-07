#pragma once
#include "Keyboard/Controls.h"


struct Keyboard
{
public:
    
    static void Init();

    static void Update();

    struct Buffer
    {
    friend struct Keyboard;
        // Возвращает true, если буфер пуст
        static bool IsEmpty();

        // Возвращает следующий орган управления, если таковой имеется
        static Control GetNextControl();
    private:
        static void AppendEvent(Key::E key, Action::E action);
    };

private:
    static void ProcessKey(int sl, int rl);
    static void ProcessGovernor(int i);
};
