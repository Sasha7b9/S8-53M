#pragma once


struct PinMode { enum E
{
    Input,
    Output,
    _SPI2 //-V2573
};};

struct PinPort { enum E
{
    A,
    B
};};

struct PinPin { enum E
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
};};


struct Pin
{
    Pin(PinPort::E port, PinPin::E pin);
    void Init(PinMode::E mode);
    void Write(uint state);
    void Set();
    void Reset();
    uint Read();

private:
    void *port; // GPIO_TypeDef *
    uint16 pin;
};

struct PinSL0 : public Pin { PinSL0(); };
struct PinSL1 : public Pin { PinSL1(); };
struct PinSL2 : public Pin { PinSL2(); };
struct PinSL3 : public Pin { PinSL3(); };
struct PinSL4 : public Pin { PinSL4(); };
struct PinSL5 : public Pin { PinSL5(); };

struct PinRL0 : public Pin { PinRL0(); };
struct PinRL1 : public Pin { PinRL1(); };
struct PinRL2 : public Pin { PinRL2(); };
struct PinRL3 : public Pin { PinRL3(); };
struct PinRL4 : public Pin { PinRL4(); };
struct PinRL5 : public Pin { PinRL5(); };
struct PinRL6 : public Pin { PinRL6(); };
struct PinRL7 : public Pin { PinRL7(); };

struct PinRC0_ChannelA : public Pin { PinRC0_ChannelA(); };
struct PinRC1 : public Pin { PinRC1(); };
struct PinRC2 : public Pin { PinRC2(); };
struct PinRC3 : public Pin { PinRC3(); };
struct PinON : public Pin { PinON(); };
