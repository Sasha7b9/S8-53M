#include "defines.h"
#include "common/Hardware/HAL/HAL_c.h"
#include <stm32f4xx_hal.h>

static Pin pinADC3_IT;
static Pin pinADC3_OUT;
static Pin pinDAC;

static Pin pinOTG_HS_DP;
static Pin pinOTG_HS_DM;
static Pin pinOTG_HS_VBUS;

static Pin pinOTG_FS_DP;
static Pin pinOTG_FS_DM;
static Pin pinOTG_FS_VBUS;

static Pin pinSPI1_SCK;
static Pin pinSPI1_MISO;
static Pin pinSPI1_MOSI;
static Pin pinSPI1_NSS;

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

Pin Pin::G1;
Pin Pin::G2;
Pin Pin::G3;
Pin Pin::G5;
Pin Pin::G7;
Pin Pin::LED;
Pin Pin::DisplayReady;


void HAL_PINS::Init()
{
    pinADC3_OUT   .Init(PinMode::_ADC3_OUT,  PinPort::_F, PinPin::_6);
    pinADC3_IT    .Init(PinMode::_ADC3_IT,   PinPort::_C, PinPin::_11);
    pinDAC        .Init(PinMode::_DAC,       PinPort::_A, PinPin::_4);

    pinOTG_HS_DP  .Init(PinMode::_OTG_HS_FS, PinPort::_B, PinPin::_15);
    pinOTG_HS_DM  .Init(PinMode::_OTG_HS_FS, PinPort::_B, PinPin::_14);
    pinOTG_HS_VBUS.Init(PinMode::_OTG_HS_FS, PinPort::_B, PinPin::_13);

    pinOTG_FS_DP  .Init(PinMode::_OTG_FS,    PinPort::_A, PinPin::_12);
    pinOTG_FS_DM  .Init(PinMode::_OTG_FS,    PinPort::_A, PinPin::_11);
    pinOTG_FS_VBUS.Init(PinMode::_OTG_FS,    PinPort::_A, PinPin::_9);

    pinSPI1_SCK   .Init(PinMode::_SPI1,      PinPort::_A, PinPin::_5);
    pinSPI1_MISO  .Init(PinMode::_SPI1,      PinPort::_A, PinPin::_6);
    pinSPI1_MOSI  .Init(PinMode::_SPI1,      PinPort::_B, PinPin::_5);
    pinSPI1_NSS   .Init(PinMode::_SPI1_NSS,  PinPort::_G, PinPin::_0);

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

    Pin::G1.Init(PinMode::_Output, PinPort::_G, PinPin::_1);
    Pin::G2.Init(PinMode::_Output, PinPort::_G, PinPin::_2);
    Pin::G3.Init(PinMode::_Output, PinPort::_G, PinPin::_3);
    Pin::G5.Init(PinMode::_Output, PinPort::_G, PinPin::_5);
    Pin::G7.Init(PinMode::_Output, PinPort::_G, PinPin::_7);
    Pin::LED.Init(PinMode::_Output, PinPort::_G, PinPin::_12);
    Pin::DisplayReady.Init(PinMode::_Input, PinPort::_G, PinPin::_11);
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
        isGPIO.Mode = GPIO_MODE_INPUT;
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
    else if (mode == PinMode::_OTG_HS_FS)
    {
        isGPIO.Mode = GPIO_MODE_AF_PP;
        isGPIO.Speed = GPIO_SPEED_FAST;
        isGPIO.Alternate = GPIO_AF12_OTG_HS_FS;
    }
    else if (mode == PinMode::_OTG_FS)
    {
        isGPIO.Mode = GPIO_MODE_AF_PP;
        isGPIO.Speed = GPIO_SPEED_HIGH;
        isGPIO.Alternate = GPIO_AF10_OTG_FS;
    }
    else if (mode == PinMode::_SPI1)
    {
        isGPIO.Mode = GPIO_MODE_AF_PP;
        isGPIO.Speed = GPIO_SPEED_FAST;
        isGPIO.Alternate = GPIO_AF5_SPI1;
    }
    else if (mode == PinMode::_SPI1_NSS) //-V2516
    {
        isGPIO.Mode = GPIO_MODE_IT_RISING;
    }

    HAL_GPIO_Init(reinterpret_cast<GPIO_TypeDef *>(port), &isGPIO);
}


void Pin::Write(int state)
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
