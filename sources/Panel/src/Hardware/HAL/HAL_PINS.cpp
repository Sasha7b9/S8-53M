#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32l0xx_hal.h>

struct PinSPI2_SCK : public Pin  { PinSPI2_SCK();  };
struct PinSPI2_NSS : public Pin  { PinSPI2_NSS();  };
struct PinSPI2_MISO : public Pin { PinSPI2_MISO(); };
struct PinSPI2_MOSI : public Pin { PinSPI2_MOSI(); };

// К этим пинам нет доступа из проиграммы
static PinSPI2_SCK  pinSPI2_SCK;
static PinSPI2_NSS  pinSPI2_NSS;
static PinSPI2_MISO pinSPI2_MISO;
static PinSPI2_MOSI pinSPI2_MOSI;


// Этими пинами можно управлять во время исполнения программы
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
PinSL3::PinSL3() : Pin(PinPort::A, PinPin::_5) { }
PinSL4::PinSL4() : Pin(PinPort::A, PinPin::_3) { }
PinSL5::PinSL5() : Pin(PinPort::A, PinPin::_4) { }

PinRL0::PinRL0() : Pin(PinPort::B, PinPin::_0) { }
PinRL1::PinRL1() : Pin(PinPort::B, PinPin::_1) { }
PinRL2::PinRL2() : Pin(PinPort::B, PinPin::_2) { }
PinRL3::PinRL3() : Pin(PinPort::B, PinPin::_3) { }
PinRL4::PinRL4() : Pin(PinPort::B, PinPin::_4) { }
PinRL5::PinRL5() : Pin(PinPort::B, PinPin::_5) { }
PinRL6::PinRL6() : Pin(PinPort::B, PinPin::_6) { }
PinRL7::PinRL7() : Pin(PinPort::B, PinPin::_7) { }

PinRC0::PinRC0() : Pin(PinPort::A, PinPin::_10) { }
PinRC1::PinRC1() : Pin(PinPort::A, PinPin::_11) { }
PinRC2::PinRC2() : Pin(PinPort::A, PinPin::_9) { }
PinRC3::PinRC3() : Pin(PinPort::A, PinPin::_12) { }

PinON::PinON() : Pin(PinPort::A, PinPin::_8) { }

PinSPI2_SCK::PinSPI2_SCK() : Pin(PinPort::B, PinPin::_10) { }
PinSPI2_NSS::PinSPI2_NSS() : Pin(PinPort::B, PinPin::_12) { }
PinSPI2_MISO::PinSPI2_MISO() : Pin(PinPort::B, PinPin::_14) { }
PinSPI2_MOSI::PinSPI2_MOSI() : Pin(PinPort::B, PinPin::_15) { }


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

    pinSPI2_SCK.Init(PinMode::_SPI2);
    pinSPI2_NSS.Init(PinMode::_SPI2);
    pinSPI2_MISO.Init(PinMode::_SPI2);
    pinSPI2_MOSI.Init(PinMode::_SPI2);
}


Pin::Pin(PinPort::E _port, PinPin::E _pin)
{
    static const GPIO_TypeDef *ports[2] = { GPIOA, GPIOB }; //-V2571

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

    port = const_cast<GPIO_TypeDef *>(ports[_port]); //-V2567
    pin = pins[_pin];
}

void Pin::Init(PinMode::E mode)
{
    static GPIO_InitTypeDef isGPIO =
    {
        GPIO_PIN_0,
        GPIO_MODE_INPUT,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_VERY_HIGH
    };

    isGPIO.Pin = pin;

    if (mode == PinMode::Input)
    {
        isGPIO.Mode = GPIO_MODE_INPUT;
    }
    else if (mode == PinMode::Output)
    {
        isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
    }
    else if (mode == PinMode::_SPI2) //-V2516
    {
        isGPIO.Mode = GPIO_MODE_AF_PP;
        isGPIO.Alternate = GPIO_AF0_SPI2;
    }

    HAL_GPIO_Init(reinterpret_cast<GPIO_TypeDef *>(port), &isGPIO); //-V2571
}


void Pin::Write(uint state)
{
    HAL_GPIO_WritePin(reinterpret_cast<GPIO_TypeDef *>(port), pin, static_cast<GPIO_PinState>(state)); //-V2571
}


void Pin::Set()
{
    HAL_GPIO_WritePin(reinterpret_cast<GPIO_TypeDef *>(port), pin, GPIO_PIN_SET); //-V2571
}


void Pin::Reset()
{
    HAL_GPIO_WritePin(reinterpret_cast<GPIO_TypeDef *>(port), pin, GPIO_PIN_RESET); //-V2571
}


uint Pin::Read()
{
    return HAL_GPIO_ReadPin(reinterpret_cast<GPIO_TypeDef *>(port), pin); //-V2571
}
