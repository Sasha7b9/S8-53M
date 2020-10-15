#include "defines.h"
#include "Controls.h"


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
        "Esc",
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
