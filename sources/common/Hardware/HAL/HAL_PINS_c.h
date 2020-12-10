#pragma once


struct PinMode { enum E
{
    _Input,     //-V2573
    _Output,    //-V2573
    _ADC3_OUT,  //-V2573
    _ADC3_IT,   //-V2573
    _DAC,       //-V2573
    _ETH,       //-V2573
    _OTG_HS_FS, //-V2573
    _OTG_FS,    //-V2573
    _SPI1,      //-V2573
    _SPI1_NSS,  //-V2573
    _FMC        //-V2573
};};

struct PinPort { enum E
{
    _A, //-V2573
    _B, //-V2573
    _C, //-V2573
    _D, //-V2573
    _E, //-V2573
    _F, //-V2573
    _G, //-V2573
    _H, //-V2573
    _I //-V2573
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
}; };


struct Pin
{
    void Init(PinMode::E mode, PinPort::E port, PinPin::E pin);
    void Write(int state);
    void Set();
    void Reset();
    uint Read();

    static Pin G2;
    static Pin G3;
    static Pin G5;
    static Pin G7;
    static Pin LED;
    static Pin DisplayReady;

private:
    void *port;
    uint16 pin;
};


struct PinFMC : public Pin
{
    PinFMC(int port, int pin);
};


struct PinETH : public Pin
{
    PinETH(int port, int pin);
};
