#pragma once
#include "Keyboard/Controls.h"


struct Keyboard
{
public:
    
    static void Init();

    static void Update();

    // ���������� true, ���� ����� ����
    static bool BufferIsEmpty();
    
    // ���������� ��������� ����� ����������, ���� ������� �������
    static Control GetNextControl();
    
    static void AppendEvent(Key::E key, Action::E action);
};
