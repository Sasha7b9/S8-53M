// 2021/02/25 8:24:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/LEDS.h"


LED_ChannelA led_ChannelA;
LED_ChannelB led_ChannelB;
LED_Trig     led_Trig;
LED_Set      led_Set;


void LED::Enable()
{
    (typeEnable == TypeEnable::Direct) ? pin.Set() : pin.Reset();
}


void LED::Disable()
{
    (typeEnable == TypeEnable::Direct) ? pin.Reset() : pin.Set();
}
