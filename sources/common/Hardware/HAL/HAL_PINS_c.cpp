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


struct PinLTDC : public Pin
{
    PinLTDC(int port, int pin) : Pin(PinMode::_LTDC, static_cast<PinPort::E>(port), static_cast<PinPin::E>(pin)) { }
};


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

    Pin(PinMode::_ADC3_IT, PinPort::_C, PinPin::_11).Init();    // ADC3 IT
    Pin(PinMode::_ADC3_OUT, PinPort::_F, PinPin::_6).Init();    // ADC3 OUT
    Pin(PinMode::_DAC, PinPort::_A, PinPin::_4).Init();         // DAC

    PinOTG_HS(B, 15).Init();    // DP
    PinOTG_HS(B, 14).Init();    // DM
    PinOTG_HS(B, 13).Init();    // VBUS

    PinOTG_FS(A, 12).Init();    // DP
    PinOTG_FS(A, 11).Init();    // DM
    PinOTG_FS(A, 9).Init();     // VBUS

    PinSPI1(A, 5).Init();       // SCK
    PinSPI1(A, 6).Init();       // MISO
    PinSPI1(B, 5).Init();       // MOSI
    PinSPI1(G, 0).Init();       // NSS

    PinETH(H, 2).Init();        // CRS
    PinETH(A, 2).Init();        // MDIO
    PinETH(H, 3).Init();        // COL
    PinETH(A, 7).Init();        // RX_DV
    PinETH(H, 6).Init();        // RXD2
    PinETH(B, 1).Init();        // RXD3
    PinETH(B, 8).Init();        // TXD3
    PinETH(I, 10).Init();       // RX_ER
    PinETH(B, 11).Init();       // TX_EN
    PinETH(C, 1).Init();        // MDC
    PinETH(C, 2).Init();        // TXD2
    PinETH(C, 3).Init();        // TX_CLK
    PinETH(A, 1).Init();        // RX_CLK
    PinETH(C, 4).Init();        // RXD0
    PinETH(C, 5).Init();        // RXD1
    PinETH(B, 12).Init();       // TXD0
    PinETH(G, 14).Init();       // TXD1

    PinFMC(D, 4).Init();        // NOE
    PinFMC(D, 5).Init();        // NWE
    PinFMC(D, 7).Init();        // NE1
    PinFMC(G, 9).Init();        // NE2
    PinFMC(D, 14).Init();       // D0
    PinFMC(D, 15).Init();       // D1
    PinFMC(D, 0).Init();        // D2
    PinFMC(D, 1).Init();        // D3
    PinFMC(E, 7).Init();        // D4
    PinFMC(E, 8).Init();        // D5
    PinFMC(E, 9).Init();        // D6
    PinFMC(E, 10).Init();       // D7
    PinFMC(E, 11).Init();       // D8
    PinFMC(E, 12).Init();       // D9
    PinFMC(E, 13).Init();       // D10
    PinFMC(E, 14).Init();       // D11
    PinFMC(E, 15).Init();       // D12
    PinFMC(D, 8).Init();        // D13
    PinFMC(D, 9).Init();        // D14
    PinFMC(D, 10).Init();       // D15
    PinFMC(F, 0).Init();        // A0
    PinFMC(F, 1).Init();        // A1
    PinFMC(F, 2).Init();        // A2
    PinFMC(F, 3).Init();        // A3
    PinFMC(F, 4).Init();        // A4
    PinFMC(F, 5).Init();        // A5
    PinFMC(F, 12).Init();       // A6
    PinFMC(F, 13).Init();       // A7
    PinFMC(F, 14).Init();       // A8
    PinFMC(F, 15).Init();       // A9
    PinFMC(G, 0).Init();        // A10
    PinFMC(G, 1).Init();        // A11
    PinFMC(G, 2).Init();        // A12
    PinFMC(G, 3).Init();        // A13
    PinFMC(G, 4).Init();        // A14
    PinFMC(G, 5).Init();        // A15
    PinFMC(D, 11).Init();       // A16
    PinFMC(D, 12).Init();       // A17
    PinFMC(D, 13).Init();       // A18
    PinFMC(D, 3).Init();        // A19
    PinFMC(D, 4).Init();        // A20

    PinLTDC(A, 4).Init();       // VSYNC
    PinLTDC(C, 6).Init();       // HSYNC
    PinLTDC(E, 13).Init();      // DE
    PinLTDC(E, 14).Init();      // CLK
    PinLTDC(C, 10).Init();      // R2
    PinLTDC(B, 0).Init();       // R3
    PinLTDC(A, 11).Init();      // R4
    PinLTDC(A, 12).Init();      // R5
    PinLTDC(B, 1).Init();       // R6
    PinLTDC(E, 15).Init();      // R7
    PinLTDC(A, 6).Init();       // G2
    PinLTDC(E, 11).Init();      // G3
    PinLTDC(B, 10).Init();      // G4
    PinLTDC(B, 11).Init();      // G5
    PinLTDC(C, 7).Init();       // G6
    PinLTDC(D, 3).Init();       // G7
    PinLTDC(D, 6).Init();       // B2
    PinLTDC(D, 10).Init();      // B3
    PinLTDC(E, 12).Init();      // B4
    PinLTDC(A, 3).Init();       // B5
    PinLTDC(B, 8).Init();       // B6
    PinLTDC(B, 9).Init();       // B7

    Pin(PinMode::_Output, PinPort::_A, PinPin::_5).Set();       // Включаем подсветку дисплея
    Pin(PinMode::_Output, PinPort::_C, PinPin::_11).Reset();    // Выбор горизонтальной ориентации дисплея R/L
    Pin(PinMode::_Output, PinPort::_C, PinPin::_12).Set();      // Выбор вертикальной ориентации дисплея U/D
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
    else if (mode == PinMode::_FMC)
    {
        isGPIO.Mode = GPIO_MODE_AF_PP;
        isGPIO.Speed = GPIO_SPEED_FREQ_HIGH;
        isGPIO.Alternate = GPIO_AF12_FMC;
    }
    else if (mode == PinMode::_LTDC) //-V2516
    {
        isGPIO.Mode = GPIO_MODE_AF_PP;
        isGPIO.Speed = GPIO_SPEED_FREQ_LOW;
        isGPIO.Alternate = GPIO_AF9_LTDC;
    }

    HAL_GPIO_Init(reinterpret_cast<GPIO_TypeDef *>(const_cast<GPIO_TypeDef *>(ports[port])), &isGPIO); //-V2571 //-V2567
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
