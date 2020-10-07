#pragma once
#include "Keyboard/Controls.h"


struct Keyboard
{
public:
    
    static void Init();

    static void Update();

    struct Buffer
    {
        // ���������� true, ���� ����� ����
        static bool IsEmpty();

        // ���������� ��������� ����� ����������, ���� ������� �������
        static Control GetNextControl();
    private:
        friend struct Keyboard;
        friend struct GovernorStruct;
        friend struct KeyStruct;
        static void AppendEvent(Key::E key, Action::E action);
    };
};
