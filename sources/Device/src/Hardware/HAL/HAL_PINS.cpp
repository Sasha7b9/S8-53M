#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PINS.h"


struct PinADC3_OUT : public Pin { PinADC3_OUT(); };
struct PinADC3_IT : public Pin  { PinADC3_IT();  };
struct PinDAC : public Pin      { PinDAC();      };

static PinADC3_IT  pinADC3_IT;
static PinADC3_OUT pinADC3_OUT;
static PinDAC      pinDAC;


void HAL_PINS::Init()
{
    pinADC3_OUT.Init(PinMode::_ADC3_OUT, PinPort::_F, PinPin::_6);
    pinADC3_IT .Init(PinMode::_ADC3_IT,  PinPort::_C, PinPin::_11);
    pinDAC     .Init(PinMode::_DAC,      PinPort::_A, PinPin::_4);
}


void Pin::Init(PinMode::E mode, PinPort::E _port, PinPin::E _pin)
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

    GPIO_InitTypeDef isGPIO = { pin };
    isGPIO.Pull = GPIO_PULLUP;

    if (mode == PinMode::_Input)
    {

    }
    else if (mode == PinMode::_Output)
    {
    }
    else if (mode == PinMode::_ADC3_OUT)
    {
        isGPIO.Mode = GPIO_MODE_ANALOG;
    }
    else if (mode == PinMode::_ADC3_IT)
    {
        isGPIO.Mode = GPIO_MODE_IT_RISING;
    }
    else if (mode == PinMode::_DAC)
    {
        isGPIO.Mode = GPIO_MODE_ANALOG;
    }

    HAL_GPIO_Init(reinterpret_cast<GPIO_TypeDef *>(port), &isGPIO);
}
