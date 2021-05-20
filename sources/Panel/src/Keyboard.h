// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Panel/Controls_.h"


struct Keyboard
{
public:
    
    static void Init();

    static void Update();

    struct Buffer
    {
        // ���������� true, ���� ����� ����
        static bool IsEmpty();

        static int NumEvents();

        // ���������� ��������� ����� ����������, ���� ������� �������
        static KeyboardEvent GetNextEvent();
    };
};
