#include "defines.h"
#include "common/Hardware/HAL/HAL_c.h"
#include <stm32f4xx_hal.h>


enum Port
{
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I
};


static const GPIO_TypeDef *ports[PinPort::Count] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH, GPIOI }; //-V2571

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



struct PinFMC : public Pin
{
    PinFMC(int port, int pin) : Pin(PinMode::_FMC, static_cast<PinPort::E>(port), static_cast<PinPin::E>(pin)) { }
};


struct PinETH : public Pin
{
    PinETH(int port, int pin) : Pin(PinMode::_ETH, static_cast<PinPort::E>(port), static_cast<PinPin::E>(pin)) { }
};


struct PinSPI1 : public Pin
{
    PinSPI1(int port, int pin) : Pin(PinMode::_SPI1, static_cast<PinPort::E>(port), static_cast<PinPin::E>(pin)) { }
};


struct PinOTG_HS : public Pin
{
    PinOTG_HS(int port, int pin) : Pin(PinMode::_OTG_HS_FS, static_cast<PinPort::E>(port), static_cast<PinPin::E>(pin)) { }
};


struct PinOTG_FS : public Pin
{
    PinOTG_FS(int port, int pin) : Pin(PinMode::_OTG_FS, static_cast<PinPort::E>(port), static_cast<PinPin::E>(pin)) { }
};


static Pin pinADC3_IT(PinMode::_ADC3_IT, PinPort::_C, PinPin::_11);
static Pin pinADC3_OUT(PinMode::_ADC3_OUT, PinPort::_F, PinPin::_6);
static Pin pinDAC(PinMode::_DAC, PinPort::_A, PinPin::_4);

static PinOTG_HS pinOTG_HS_DP(B, 15);
static PinOTG_HS pinOTG_HS_DM(B, 14);
static PinOTG_HS pinOTG_HS_VBUS(B, 13);

static PinOTG_FS pinOTG_FS_DP(A, 12);
static PinOTG_FS pinOTG_FS_DM(A, 11);
static PinOTG_FS pinOTG_FS_VBUS(A, 9);

static PinSPI1 pinSPI1_SCK(A, 5);
static PinSPI1 pinSPI1_MISO(A, 6);
static PinSPI1 pinSPI1_MOSI(B, 5);
static PinSPI1 pinSPI1_NSS(G, 0);

static PinETH pinCRS(H, 2);
static PinETH pinMDIO(A, 2);
static PinETH pinCOL(H, 3);
static PinETH pinRX_DV(A, 7);
static PinETH pinRXD2(H, 6);
static PinETH pinRXD3(B, 1);
static PinETH pinTXD3(B, 8);
static PinETH pinRX_ER(I, 10);
static PinETH pinTX_EN(B, 11);
static PinETH pinMDC(C, 1);
static PinETH pinTXD2(C, 2);
static PinETH pinTX_CLK(C, 3);
static PinETH pinRX_CLK(A, 1);
static PinETH pinRXD0(C, 4);
static PinETH pinRXD1(C, 5);
static PinETH pinTXD0(B, 12);
static PinETH pinTXD1(G, 14);

static PinFMC pinNOE(D, 4);
static PinFMC pinNWE(D, 5);
static PinFMC pinNE1(D, 7);
static PinFMC pinNE2(G, 9);
static PinFMC pinD0(D, 14);
static PinFMC pinD1(D, 15);
static PinFMC pinD2(D, 0);
static PinFMC pinD3(D, 1);
static PinFMC pinD4(E, 7);
static PinFMC pinD5(E, 8);
static PinFMC pinD6(E, 9);
static PinFMC pinD7(E, 10);
static PinFMC pinD8(E, 11);
static PinFMC pinD9(E, 12);
static PinFMC pinD10(E, 13);
static PinFMC pinD11(E, 14);
static PinFMC pinD12(E, 15);
static PinFMC pinD13(D, 8);
static PinFMC pinD14(D, 9);
static PinFMC pinD15(D, 10);
static PinFMC pinA0(F, 0);
static PinFMC pinA1(F, 1);
static PinFMC pinA2(F, 2);
static PinFMC pinA3(F, 3);
static PinFMC pinA4(F, 4);
static PinFMC pinA5(F, 5);
static PinFMC pinA6(F, 12);
static PinFMC pinA7(F, 13);
static PinFMC pinA8(F, 14);
static PinFMC pinA9(F, 15);
static PinFMC pinA10(G, 0);
static PinFMC pinA11(G, 1);
static PinFMC pinA12(G, 2);
static PinFMC pinA13(G, 3);
static PinFMC pinA14(G, 4);
static PinFMC pinA15(G, 5);
static PinFMC pinA16(D, 11);
static PinFMC pinA17(D, 12);
static PinFMC pinA18(D, 13);
static PinFMC pinA19(D, 3);
static PinFMC pinA20(D, 4);


Pin Pin::G2(PinMode::_Output, PinPort::_G, PinPin::_2);
Pin Pin::G3(PinMode::_Output, PinPort::_G, PinPin::_3);
Pin Pin::G5(PinMode::_Output, PinPort::_G, PinPin::_5);
Pin Pin::G7(PinMode::_Output, PinPort::_G, PinPin::_7);
Pin Pin::LED(PinMode::_Output, PinPort::_G, PinPin::_12);
Pin Pin::DisplayReady(PinMode::_Input, PinPort::_G, PinPin::_11);


void HAL_PINS::Init()
{
    Pin::G2.Init();
    Pin::G3.Init();
    Pin::G5.Init();
    Pin::G7.Init();
    Pin::LED.Init();
    Pin::DisplayReady.Init();
}


void Pin::Init()
{
    GPIO_InitTypeDef isGPIO = { pins[pin] };

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
        if (pin == PinPin::_0)
        {
            isGPIO.Mode = GPIO_MODE_IT_RISING;
        }
        else
        {
            isGPIO.Mode = GPIO_MODE_AF_PP;
            isGPIO.Speed = GPIO_SPEED_FAST;
            isGPIO.Alternate = GPIO_AF5_SPI1;
        }
    }
    else if (mode == PinMode::_FMC) //-V2516
    {
        isGPIO.Mode = GPIO_MODE_AF_PP;
        isGPIO.Speed = GPIO_SPEED_FREQ_HIGH;
        isGPIO.Alternate = GPIO_AF12_FMC;
    }

    HAL_GPIO_Init(reinterpret_cast<GPIO_TypeDef *>(const_cast<GPIO_TypeDef *>(ports[port])), &isGPIO); //-V2571
}


void Pin::Write(int state)
{
    HAL_GPIO_WritePin(reinterpret_cast<GPIO_TypeDef *>(port), pins[pin], static_cast<GPIO_PinState>(state)); //-V2571
}


void Pin::Set()
{
    HAL_GPIO_WritePin(reinterpret_cast<GPIO_TypeDef *>(port), pins[pin], GPIO_PIN_SET); //-V2571
}


void Pin::Reset()
{
    HAL_GPIO_WritePin(reinterpret_cast<GPIO_TypeDef *>(port), pins[pin], GPIO_PIN_RESET); //-V2571
}


uint Pin::Read()
{
    return HAL_GPIO_ReadPin(reinterpret_cast<GPIO_TypeDef *>(port), pins[pin]); //-V2571
}
