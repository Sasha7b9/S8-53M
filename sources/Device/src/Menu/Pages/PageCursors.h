#pragma once
#include "Settings/SettingsTypes.h"


void  Cursors_Update();    // �������� ��� ������� ��� ������� ���������, ����� �������� ������� ��������, ���� ��� ������ ����������� �������������.


class PageCursors
{
public:
    
    class PageSet
    {
    public:
        static void *pointer;
    };

    static void *GetPointer();

    static const Page *self;
};
