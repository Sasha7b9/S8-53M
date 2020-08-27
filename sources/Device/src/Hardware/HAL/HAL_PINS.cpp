#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PINS.h"


struct PinADC3_OUT : public Pin { PinADC3_OUT(); };
struct PinADC3_IT : public Pin  { PinADC3_IT();  };

static PinADC3_IT pinADC3_IT;
static PinADC3_OUT pinADC3_OUT;


PinADC3_OUT::PinADC3_OUT() : Pin(PinPort::F, PinPin::_6) { }
PinADC3_IT::PinADC3_IT() : Pin(PinPort::C, PinPin::_11) { }


void HAL_PINS::Init()
{
    pinADC3_OUT.Init(PinMode::ADC3_OUT);
    pinADC3_IT.Init(PinMode::ADC3_IT);
}


void Pin::Init(PinMode::E mode)
{
    GPIO_InitTypeDef isGPIO = { pin };

    if (mode == PinMode::Input)
    {

    }
    else if (mode == PinMode::Output)
    {
    }
    else if (mode == PinMode::ADC3_OUT)
    {
        isGPIO.Mode = GPIO_MODE_ANALOG;
        isGPIO.Pull = GPIO_NOPULL;
    }
    else if (mode == PinMode::ADC3_IT)
    {
        isGPIO.Mode = GPIO_MODE_IT_RISING;
        isGPIO.Pull = GPIO_NOPULL;
    }

    HAL_GPIO_Init(reinterpret_cast<GPIO_TypeDef *>(port), &isGPIO);
}
