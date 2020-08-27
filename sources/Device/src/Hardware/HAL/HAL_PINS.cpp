#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PINS.h"


static Pin pinADC3_IT;
static Pin pinADC3_OUT;
static Pin pinDAC;
static Pin pinETH_CRS;
static Pin pinETH_MDIO;
static Pin pinETH_COL;
static Pin pinETH_RX_DV;
static Pin pinETH_RXD2;
static Pin pinETH_RXD3;
static Pin pinETH_TXD3;
static Pin pinETH_RX_ER;
static Pin pinETH_TX_EN;
static Pin pinETH_MDC;
static Pin pinETH_TXD2;
static Pin pinETH_TX_CLK;
static Pin pinETH_RX_CLK;
static Pin pinETH_RXD0;
static Pin pinETH_RXD1;
static Pin pinETH_TXD0;
static Pin pinETH_TXD1;

Pin pinG1;
Pin pinG2;
Pin pinG3;
Pin pinG5;
Pin pinG7;


void HAL_PINS::Init()
{
    pinADC3_OUT  .Init(PinMode::_ADC3_OUT, PinPort::_F, PinPin::_6);
    pinADC3_IT   .Init(PinMode::_ADC3_IT,  PinPort::_C, PinPin::_11);
    pinDAC       .Init(PinMode::_DAC,      PinPort::_A, PinPin::_4);

    pinETH_RX_ER .Init(PinMode::_ETH, PinPort::_I, PinPin::_10);
    pinETH_MDC   .Init(PinMode::_ETH, PinPort::_C, PinPin::_1);
    pinETH_TXD2  .Init(PinMode::_ETH, PinPort::_C, PinPin::_2);
    pinETH_TX_CLK.Init(PinMode::_ETH, PinPort::_C, PinPin::_3);
    pinETH_RX_CLK.Init(PinMode::_ETH, PinPort::_A, PinPin::_1);
    pinETH_MDIO  .Init(PinMode::_ETH, PinPort::_A, PinPin::_2);
    pinETH_CRS   .Init(PinMode::_ETH, PinPort::_H, PinPin::_2);
    pinETH_COL   .Init(PinMode::_ETH, PinPort::_H, PinPin::_3);
    pinETH_RX_DV .Init(PinMode::_ETH, PinPort::_A, PinPin::_7);
    pinETH_RXD0  .Init(PinMode::_ETH, PinPort::_C, PinPin::_4);
    pinETH_RXD1  .Init(PinMode::_ETH, PinPort::_C, PinPin::_5);
    pinETH_RXD3  .Init(PinMode::_ETH, PinPort::_B, PinPin::_1);
    pinETH_TX_EN .Init(PinMode::_ETH, PinPort::_B, PinPin::_11);
    pinETH_RXD2  .Init(PinMode::_ETH, PinPort::_H, PinPin::_6);
    pinETH_TXD0  .Init(PinMode::_ETH, PinPort::_B, PinPin::_12);
    pinETH_TXD1  .Init(PinMode::_ETH, PinPort::_G, PinPin::_14);
    pinETH_TXD3  .Init(PinMode::_ETH, PinPort::_B, PinPin::_8);

    pinG1.Init(PinMode::_Output, PinPort::_G, PinPin::_1);
    pinG2.Init(PinMode::_Output, PinPort::_G, PinPin::_2);
    pinG3.Init(PinMode::_Output, PinPort::_G, PinPin::_3);
    pinG5.Init(PinMode::_Output, PinPort::_G, PinPin::_5);
    pinG7.Init(PinMode::_Output, PinPort::_G, PinPin::_7);
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
        isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
        isGPIO.Speed = GPIO_SPEED_HIGH;
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
    else if (mode == PinMode::_ETH)
    {
        isGPIO.Mode = GPIO_MODE_AF_PP;
        isGPIO.Speed = GPIO_SPEED_HIGH;
        isGPIO.Alternate = GPIO_AF11_ETH;
    }

    HAL_GPIO_Init(reinterpret_cast<GPIO_TypeDef *>(port), &isGPIO);
}


void Pin::Write(uint state)
{
    HAL_GPIO_WritePin(reinterpret_cast<GPIO_TypeDef *>(port), pin, static_cast<GPIO_PinState>(state));
}


void Pin::Set()
{
    HAL_GPIO_WritePin(reinterpret_cast<GPIO_TypeDef *>(port), pin, GPIO_PIN_SET);
}


void Pin::Reset()
{
    HAL_GPIO_WritePin(reinterpret_cast<GPIO_TypeDef *>(port), pin, GPIO_PIN_RESET);
}


uint Pin::Read()
{
    return HAL_GPIO_ReadPin(reinterpret_cast<GPIO_TypeDef *>(port), pin);
}
