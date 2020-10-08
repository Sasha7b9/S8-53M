#pragma once
#include "common/Keyboard/Controls.h"


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
        static KeyboardEvent GetNextEvent();
    };
};
