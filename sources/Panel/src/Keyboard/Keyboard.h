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
        // ���������� true, ���� ����� ����
        static bool IsEmpty();

        // ���������� ��������� ����� ����������, ���� ������� �������
        static Control GetNextControl();
    private:
        static void AppendEvent(Key::E key, Action::E action);
    };

private:
    static void ProcessKey(int sl, int rl);
    static void ProcessGovernor(int i);
};
