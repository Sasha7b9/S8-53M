#include "defines.h"
#include "common/Keyboard/Controls_c.h"


bool KeyboardEvent::IsFunctional() const
{
    return (key >= Key::F1) && (key <= Key::F4);
}


bool KeyboardEvent::IsRelease() const
{
    return (action == Action::Up) || (action == Action::Long);
}


pString Key::Name() const
{
    static pString names[] =
    {
        "None",
        "F1",
        "F2",
        "F3",
        "F4",
        "F5",
        "����� 1",
        "����� 2",
        "����",
        "�����",
        "�������",
        "�����",
        "�������",
        "������",
        "����/����",
        "������",
        "������",
        "����",
        "Left",
        "Right",
        "On1",
        "On2",
        "RegLeft",
        "RegRight",
        "RegButton"
    };

    return names[value];
}


Key::E &operator++(Key::E &right)
{
    right = (Key::E)((int)right + 1);
    return right;
}


Key::E Key::FromCode(uint16 code)
{
    return static_cast<Key::E>(code & 0x1F);
}


uint16 Key::ToCode(Key::E key)
{
    return static_cast<uint16>(key);
}


Action::E Action::FromCode(uint16 code)
{
    return static_cast<Action::E>((code >> 5) & 0x7);
}


uint16 Action::ToCode(Action::E action)
{
    return static_cast<uint16>(action << 5);
}
