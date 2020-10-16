#pragma once


struct Key { enum E
{
        None,               // 
        F1,                 // 1
        F2,                 // 2
        F3,                 // 3
        F4,                 // 4
        F5,                 // 5
        ChannelA,           // ����� 1
        ChannelB,           // ����� 2
        Time,               // ����
        Synchronization,    // �����
        Cursors,            // �������
        Measures,           // �����
        Display,            // �������
        Help,               // ������
        Start,              // ����/����
        Memory,             // ������
        Service,            // ������
        Menu,               // ����
        RangeA,             // �����/��� 1
        RangeB,             // �����/��� 2
        RShiftA,            // 
        RShiftB,            // 
        TBase,              // �����/���
        TShift,             // 
        TrigLev,            // �������
        Setting,            // ���������
        Count
    };

    Key(E v) : value(v) { }

    pString Name() const;

    E value;

    static Key::E FromCode(uint16 code);
    static uint16 ToCode(Key::E key);
};

Key::E& operator++(Key::E &right);


struct Action { enum E
{
        Down,         // ������� ������
        Up,           // ���������� ������
        Long,         // "�������" �������
        RotateLeft,   // �������� ����� �����
        RotateRight,  // ������� ����� ������
        Count
    };

    static Action::E FromCode(uint16 code);
    static uint16 ToCode(Action::E action);
};


struct KeyboardEvent
{
    Key::E key;
    Action::E action;

    bool Is(Key::E c) const           { return (c == key); };
    bool Is(Key::E c, Action::E a) const { return (key == c) && (action == a); };
       
    bool IsUp() const   { return (action == Action::Up); }
    bool IsDown() const { return (action == Action::Down); }
    bool IsLong() const { return (action == Action::Long); }
    // ���������� true � ������ ���������� ������ ��� "��������" �������
    bool IsRelease() const;

    // true, ���� �������������� �������
    bool IsFunctional() const;
    
    bool operator==(const KeyboardEvent &rhl) const
    {
        return (rhl.key == key) && (rhl.action == action);
    }
};
