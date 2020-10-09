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
        *cell = Governor_NextValue(this);
    }
    else if (delta < 0 && ADDRESS_GOVERNOR == reinterpret_cast<uint>(this) && (IN_MOVE_DECREASE != 0))
    {
        *cell = Governor_PrevValue(this);
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

void IPaddress_ChangeValue(IPaddress *ip, int delta)
{
    int numByte = 0;
    int numPos = 0;

    IPaddress_GetNumPosIPvalue(&numByte, &numPos);

    int oldValue = 0;

    if (numByte < 4)
    {
        uint8 *bytes = ip->ip0;
        oldValue = bytes[numByte];
    }
    else
    {
        oldValue = *ip->port;
    }

    int newValue = oldValue + Math::Sign(delta) * Math::Pow10(numPos);
    LIMITATION(newValue, newValue, 0, numByte == 4 ? 65535 : 255);

    if (oldValue != newValue)
    {
        if (numByte == 4)
        {
            *ip->port = static_cast<uint16>(newValue);
        }
        else
        {
            ip->ip0[numByte] = static_cast<uint8>(newValue);
        }
        Sound::GovernorChangedValue();
        Display::ShowWarningGood(Warning::NeedRebootDevice2);
        Display::ShowWarningGood(Warning::NeedRebootDevice1);
    }
}

void MACaddress_ChangeValue(MACaddress *mac, int delta)
{
    uint8 *value = mac->mac0 + gCurDigit;
    *value += delta > 0 ? 1 : -1;
    Sound::GovernorChangedValue();
    Display::ShowWarningGood(Warning::NeedRebootDevice2);
    Display::ShowWarningGood(Warning::NeedRebootDevice1);
}

void IPaddress_GetNumPosIPvalue(int *numIP, int *selPos)
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
            *cell = Governor_PrevValue(this);
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
            *cell = Governor_NextValue(this);
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

int16 Governor_NextValue(const Governor *governor)
{
    return ((*governor->cell) < governor->maxValue) ? (*governor->cell) + 1 : static_cast<int16>(governor->minValue);
}

int16 Governor_PrevValue(const Governor *governor)
{
    return ((*governor->cell) > governor->minValue) ? (*governor->cell) - 1 : governor->maxValue;
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

void IPaddress_NextPosition(const IPaddress *ipEthernet_IP)
{
    CircleIncreaseInt8(&gCurDigit, 0, ipEthernet_IP->port == 0 ? 11 : 16);
}

void ItemTime_SetOpened(Time *item)
{
    PackedTime time = HAL_RTC::GetPackedTime();
    *(item->seconds) = static_cast<int8>(time.seconds);
    *(item->minutes) = static_cast<int8>(time.minutes);
    *(item->hours) = static_cast<int8>(time.hours);
    *(item->day) = static_cast<int8>(time.day);
    *(item->month) = static_cast<int8>(time.month);
    *(item->year) = static_cast<int8>(time.year);
}

void ItemTime_SetNewTime(const Time *time)
{
    HAL_RTC::SetTimeAndData(*time->day, *time->month, *time->year, *time->hours, *time->minutes, *time->seconds);
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

