#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32l0xx_hal.h>


PinSL0 pinSL0;
PinSL1 pinSL1;
PinSL2 pinSL2;
PinSL3 pinSL3;
PinSL4 pinSL4;
PinSL5 pinSL5;

PinRL0 pinRL0;
PinRL1 pinRL1;
PinRL2 pinRL2;
PinRL3 pinRL3;
PinRL4 pinRL4;
PinRL5 pinRL5;
PinRL6 pinRL6;
PinRL7 pinRL7;

PinRC0 pinRC0;
PinRC1 pinRC1;
PinRC2 pinRC2;
PinRC3 pinRC3;

PinON pinON;

PinSL0::PinSL0() : Pin(PinPort::A, PinPin::_0) { }
PinSL1::PinSL1() : Pin(PinPort::A, PinPin::_1) { }
PinSL2::PinSL2() : Pin(PinPort::A, PinPin::_2) { }
PinSL3::PinSL3() : Pin(PinPort::A, PinPin::_3) { }
PinSL4::PinSL4() : Pin(PinPort::A, PinPin::_4) { }
PinSL5::PinSL5() : Pin(PinPort::A, PinPin::_5) { }

PinRL0::PinRL0() : Pin(PinPort::B, PinPin::_0) { }
PinRL1::PinRL1() : Pin(PinPort::B, PinPin::_1) { }
PinRL2::PinRL2() : Pin(PinPort::B, PinPin::_2) { }
PinRL3::PinRL3() : Pin(PinPort::B, PinPin::_3) { }
PinRL4::PinRL4() : Pin(PinPort::B, PinPin::_4) { }
PinRL5::PinRL5() : Pin(PinPort::B, PinPin::_5) { }
PinRL6::PinRL6() : Pin(PinPort::B, PinPin::_6) { }
PinRL7::PinRL7() : Pin(PinPort::B, PinPin::_7) { }

PinRC0::PinRC0() : Pin(PinPort::A, PinPin::_11) { }
PinRC1::PinRC1() : Pin(PinPort::A, PinPin::_10) { }
PinRC2::PinRC2() : Pin(PinPort::A, PinPin::_9) { }
PinRC3::PinRC3() : Pin(PinPort::A, PinPin::_12) { }

PinON::PinON() : Pin(PinPort::A, PinPin::_8) { }


void HAL_PINS::Init()
{
    pinRL0.Init(PinMode::Input);
    pinRL1.Init(PinMode::Input);
    pinRL2.Init(PinMode::Input);
    pinRL3.Init(PinMode::Input);
    pinRL4.Init(PinMode::Input);
    pinRL5.Init(PinMode::Input);
    pinRL6.Init(PinMode::Input);
    pinRL7.Init(PinMode::Input);
 
    pinSL0.Init(PinMode::Output);
    pinSL1.Init(PinMode::Output);
    pinSL2.Init(PinMode::Output);
    pinSL3.Init(PinMode::Output);
    pinSL4.Init(PinMode::Output);
    pinSL5.Init(PinMode::Output);

    pinRC0.Init(PinMode::Output);
    pinRC1.Init(PinMode::Output);
    pinRC2.Init(PinMode::Output);
    pinRC3.Init(PinMode::Output);

    pinON.Init(PinMode::Output);
}


Pin::Pin(PinPort::E _port, PinPin::E _pin)
{
    static const GPIO_TypeDef *ports[2] = { GPIOA, GPIOB };

    static const uint16 pins[16] =
    {
        GPIO_PIN_0,
        GPIO_PIN_1,
        GPIO_PIN_2,
        GPIO_PIN_3,
        GPIO_PIN_4,
        GPIO_PIN_5,
        GPIO_PIN_6,
        GPIO_PIN_7,
        GPIO_PIN_8,
        GPIO_PIN_9,
        GPIO_PIN_10,
        GPIO_PIN_11,
        GPIO_PIN_12,
        GPIO_PIN_13,
        GPIO_PIN_14,
        GPIO_PIN_15
    };

    port = const_cast<GPIO_TypeDef *>(ports[_port]);
    pin = pins[_pin];
}

void Pin::Init(PinMode::E mode)
{

}

void Pin::Write(uint state)
{

}

uint Pin::Read()
{
    return 0;
}
