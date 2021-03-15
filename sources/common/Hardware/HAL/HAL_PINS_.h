#pragma once


namespace HAL_PINS
{
    void Init();

    namespace DAC2_ { void Init(); }

    namespace FMC_  { void Init(); }

    namespace SPI1_ { void Init(); }

    namespace SPI5_ { void Init(); }

    namespace LTDC_ { void Init(); }
};


struct PinMode { enum E
{
    _Input,
    _Output,
    _ADC3_OUT,
    _ADC3_IT,
    _DAC1,
    _DAC2,              // Динамик
    _ETH,
    _OTG_HS_FS,
    _OTG_FS,
    _SPI1,
    _SPI5,
    _FMC,
    _LTDC
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
    _I, //-V2573
    Count
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
    Pin(PinMode::E _mode, PinPort::E _port, PinPin::E _pin) : port(_port), pin(_pin), mode(_mode) { };
    void Init();
    void Write(int state);
    void Set();
    void Reset();
    uint Read();

    static Pin G2;
    static Pin G3;
    static Pin G5;
    static Pin LED;
    static Pin DisplayReady;
    static Pin Temp;

private:
    PinPort::E port;
    PinPin::E  pin;
    PinMode::E mode;
};
