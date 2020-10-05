#pragma once


// Делитель.
struct Divider { enum E
    {
        _1,
        _10
    };
    static int ToAbs(Divider::E divider);
};
