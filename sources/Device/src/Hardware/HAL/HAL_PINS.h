#pragma once


struct PinMode
{
    enum E
    {
        _Input,
        _Output,
        _ADC3_OUT,
        _ADC3_IT,
        _DAC,
        _ETH,
        _OTG_HS_FS,
        _OTG_FS,
        _SP1
    };
};

struct PinPort
{
    enum E
    {
        _A,
        _B,
        _C,
        _D,
        _E,
        _F,
        _G,
        _H,
        _I
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
    void Init(PinMode::E mode, PinPort::E port, PinPin::E pin);
    void Write(uint state);
    void Set();
    void Reset();
    uint Read();

private:
    void *port;
    uint16 pin;
};
