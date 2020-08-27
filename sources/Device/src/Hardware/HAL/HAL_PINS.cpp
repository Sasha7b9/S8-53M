#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PINS.h"


struct PinADC3_OUT : public Pin { PinADC3_OUT(); };
struct PinADC3_IT : public Pin  { PinADC3_IT();  };
struct PinDAC : public Pin      { PinDAC();      };

static PinADC3_IT  pinADC3_IT;
static PinADC3_OUT pinADC3_OUT;
static PinDAC      pinDAC;


PinADC3_OUT::PinADC3_OUT() : Pin(PinPort::_F, PinPin::_6)  { }
PinADC3_IT::PinADC3_IT()   : Pin(PinPort::_C, PinPin::_11) { }
PinDAC::PinDAC()           : Pin(PinPort::_A, PinPin::_4)  { }


void HAL_PINS::Init()
{
    pinADC3_OUT.Init(PinMode::_ADC3_OUT);
    pinADC3_IT.Init(PinMode::_ADC3_IT);
    pinDAC.Init(PinMode::_DAC);
}


void Pin::Init(PinMode::E mode)
{
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
