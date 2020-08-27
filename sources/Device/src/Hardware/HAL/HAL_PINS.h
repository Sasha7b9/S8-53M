#pragma once


struct PinMode
{
    enum E
    {
        Input,
        Output,
        ADC3_OUT,
        ADC3_IT
    };
};

struct PinPort
{
    enum P
    {
        A,
        B,
        C,
        D,
        E,
        F
    };
};

struct PinPin
{
    enum E
    {
        _0,
        _1,
        _2,
        _3,
        _4,
        _5,
        _6,
        _7,
        _8,
        _9,
        _10,
        _11,
        _12,
        _13,
        _14,
        _15
    };
};


struct Pin
{
    Pin(PinPort::P port, PinPin::E pin);
    void Init(PinMode::E mode);
    void Write(uint state);
    void Set();
    void Reset();
    uint Read();

private:
    void *port;
    uint16 pin;
};
