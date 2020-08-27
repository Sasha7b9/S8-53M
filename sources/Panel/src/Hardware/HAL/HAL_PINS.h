#pragma once


struct PinMode
{
    enum E
    {
        Input,
        Output
    };
};


struct Pin
{
    void Init(PinMode::E mode);
    void Write(uint state);
    uint Read();
};

struct PinSL0 : public Pin { PinSL0(); };
struct PinSL1 : public Pin { PinSL1(); };
