#include "defines.h"
#include "common/Utils/Math.h"
#include "Hardware/Sound.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsLogic.h"
#include "Utils/GlobalFunctions.h"

int8 gCurDigit = 0;


void Governor::StartChange(int delta)
{
    Sound::GovernorChangedValue();
    if (delta > 0 && ADDRESS_GOVERNOR == reinterpret_cast<uint>(this) && (IN_MOVE_INCREASE != 0))
    {
        *cell = NextValue();
    }
    else if (delta < 0 && ADDRESS_GOVERNOR == reinterpret_cast<uint>(this) && (IN_MOVE_DECREASE != 0))
    {
        *cell = PrevValue();
    }
    else
    {
        TIME_START_MS = gTimerMS;   
    }
    IN_MOVE_INCREASE = (delta > 0) ? 1U : 0U;
    IN_MOVE_DECREASE = (delta < 0) ? 1U : 0U;
}

void Governor::ChangeValue(int delta)
{
    int16 oldValue = *cell;
    LIMITATION(*cell,
                static_cast<int16>(oldValue + Math::Sign(delta) * Math::Pow10(gCurDigit)),
                static_cast<int16>(minValue),
                static_cast<int16>(maxValue));
    if (*cell != oldValue)
    {
        if (funcOfChanged)
        {
            funcOfChanged();
        }
        Sound::GovernorChangedValue();
    }
}

void IPaddress::ChangeValue(int delta)
{
    int numByte = 0;
    int numPos = 0;

    IPaddress::GetNumPosIPvalue(&numByte, &numPos);

    int oldValue = 0;

    if (numByte < 4)
    {
        uint8 *bytes = ip0;
        oldValue = bytes[numByte];
    }
    else
    {
        oldValue = *port;
    }

    int newValue = oldValue + Math::Sign(delta) * Math::Pow10(numPos);
    LIMITATION(newValue, newValue, 0, numByte == 4 ? 65535 : 255);

    if (oldValue != newValue)
    {
        if (numByte == 4)
        {
            *port = static_cast<uint16>(newValue);
        }
        else
        {
            ip0[numByte] = static_cast<uint8>(newValue);
        }
        Sound::GovernorChangedValue();
        Display::ShowWarningGood(Warning::NeedRebootDevice2);
        Display::ShowWarningGood(Warning::NeedRebootDevice1);
    }
}

void MACaddress::ChangeValue(int delta)
{
    uint8 *value = mac0 + gCurDigit;
    *value += delta > 0 ? 1 : -1;
    Sound::GovernorChangedValue();
    Display::ShowWarningGood(Warning::NeedRebootDevice2);
    Display::ShowWarningGood(Warning::NeedRebootDevice1);
}

void IPaddress::GetNumPosIPvalue(int *numIP, int *selPos)
{
    if (gCurDigit < 12)
    {
        *numIP = gCurDigit / 3;
        *selPos = 2 - (gCurDigit - (*numIP * 3));
    }
    else
    {
        *numIP = 4;
        *selPos = 4 - (gCurDigit - 12);
    }


}

float Governor::Step() const
{
    static const float speed = 0.05F;
    static const int numLines = 10;
    if (ADDRESS_GOVERNOR == reinterpret_cast<uint>(this) && (IN_MOVE_DECREASE != 0))
    {
        float delta = -speed * (gTimerMS - TIME_START_MS);
        if (delta == 0.0F) //-V2550 //-V550
        {
            return -0.001F;
        }
        if (delta < -numLines)
        {
            IN_MOVE_DECREASE = 0;
            IN_MOVE_INCREASE = 0;
            *cell = PrevValue();
            if (funcOfChanged)
            {
                funcOfChanged();
            }
            return 0.0F;
        }
        return delta;
    }
    if (ADDRESS_GOVERNOR == reinterpret_cast<uint>(this) && (IN_MOVE_INCREASE != 0))
    {
        float delta = speed * (gTimerMS - TIME_START_MS);
        if (delta == 0.0F) //-V550 //-V2550
        {
            return 0.001F;
        }
        if (delta > numLines)
        {
            IN_MOVE_DECREASE = 0;
            IN_MOVE_INCREASE = 0;
            *cell = NextValue();
            if(funcOfChanged)
            {
                funcOfChanged();
            }
            return 0.0F;
        }
        return delta;
    }
    return 0.0F;
}

int16 Governor::NextValue() const
{
    return ((*cell) < maxValue) ? (*cell) + 1 : static_cast<int16>(minValue);
}

int16 Governor::PrevValue() const
{
    return ((*cell) > minValue) ? (*cell) - 1 : maxValue;
}

void Governor::NextPosition()
{
    if (Menu::OpenedItem() == this)
    {
        CircleIncreaseInt8(&gCurDigit, 0, static_cast<int8>(NumDigits()) - 1);
    }
}

int Governor::NumDigits() const
{
    int min = Math::NumDigitsInNumber(Math::FabsInt(minValue));
    int max = Math::NumDigitsInNumber(Math::FabsInt(maxValue));
    if (min > max)
    {
        max = min;
    }
    return max;
}

void IPaddress::NextPosition() const
{
    CircleIncreaseInt8(&gCurDigit, 0, (port == 0) ? 11 : 16);
}

void Time::SetOpened()
{
    PackedTime time = HAL_RTC::GetPackedTime();
    *seconds = static_cast<int8>(time.seconds);
    *minutes = static_cast<int8>(time.minutes);
    *hours = static_cast<int8>(time.hours);
    *day = static_cast<int8>(time.day);
    *month = static_cast<int8>(time.month);
    *year = static_cast<int8>(time.year);
}

void Time::SetNewTime() const
{
    HAL_RTC::SetTimeAndData(*day, *month, *year, *hours, *minutes, *seconds);
}

void ItemTime_SelectNextPosition(Time *time)
{
    CircleIncreaseInt8(time->curField, 0, 7);
    Painter::ResetFlash();
}

void ItemTime_IncCurrentPosition(const Time *time)
{
    Sound::GovernorChangedValue();
    int8 position = *time->curField;
    if (position != iSET && position != iEXIT)
    {
        int8 *value[] = { 0, time->day, time->month, time->year, time->hours, time->minutes, time->seconds };
        static const int8 max[] = {0, 31, 12, 99, 23, 59, 59};
        static const int8 min[] = {0, 1, 1, 15, 0, 0, 0};
        *(value[position]) = (*(value[position]))++; //-V567
        if (*value[position] > max[position])
        {
            *value[position] = min[position];
        }
    }
}


void ItemTime_DecCurrentPosition(const Time *time)
{
    Sound::GovernorChangedValue();
    static const int8 max[] = {0, 31, 12, 99, 23, 59, 59};
    static const int8 min[] = {0, 1, 1, 15, 0, 0, 0};
    int8 position = *time->curField;
    if (position != iSET && position != iEXIT)
    {
        int8 *value[] = { 0, time->day, time->month, time->year, time->hours, time->minutes, time->seconds };
        (*(value[position]))--;
        if (*value[position] < min[position])
        {
            *value[position] = max[position];
        }
    }
}


void GovernorColor_ChangeValue(GovernorColor *governor, int delta)
{
    ColorType *ct = governor->colorType;
    if (ct->currentField == 0)
    {
        Color_BrightnessChange(ct, delta);
    }
    else
    {
        Color_ComponentChange(ct, delta);
    }
}


void Governor::ShortPress()
{
    if (!Menu::ItemIsActive(this))
    {
        return;
    }
    if (Menu::OpenedItem() == this)
    {
        NextPosition();
    }
    else
    {
        Menu::SetCurrentItem(this, Menu::CurrentItem() != this);
    }
}

