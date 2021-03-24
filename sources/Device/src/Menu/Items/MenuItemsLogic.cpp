#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"


int8 Item::gCurDigit = 0;


void Governor::StartChange(int delta)
{
    Sound::GovernorChangedValue();
    if (delta > 0 && (address == this) && inMoveIncrease)
    {
        *OwnData()->cell = NextValue();
    }
    else if (delta < 0 && (address == this) && inMoveDecrease)
    {
        *OwnData()->cell = PrevValue();
    }
    else
    {
        timeStartMS = TIME_MS;
    }
    inMoveIncrease = (delta > 0);
    inMoveDecrease = (delta < 0);
}

void Governor::ChangeValue(int delta)
{
    int16 oldValue = *OwnData()->cell;
    LIMITATION(*OwnData()->cell,
                (int16)(oldValue + Math::Sign(delta) * Math::Pow10(gCurDigit)),
                (int16)(OwnData()->minValue),
                (int16)(OwnData()->maxValue));
    if (*OwnData()->cell != oldValue)
    {
        if (OwnData()->funcOfChanged)
        {
            OwnData()->funcOfChanged();
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
        uint8 *bytes = OwnData()->ip0;
        oldValue = bytes[numByte];
    }
    else
    {
        oldValue = *OwnData()->port;
    }

    int newValue = oldValue + Math::Sign(delta) * Math::Pow10(numPos);
    LIMITATION(newValue, newValue, 0, numByte == 4 ? 65535 : 255);

    if (oldValue != newValue)
    {
        if (numByte == 4)
        {
            *OwnData()->port = (uint16)(newValue);
        }
        else
        {
            OwnData()->ip0[numByte] = (uint8)(newValue);
        }
        Sound::GovernorChangedValue();
        Display::ShowWarningGood(Warning::NeedRebootDevice2);
        Display::ShowWarningGood(Warning::NeedRebootDevice1);
    }
}

void MACaddress::ChangeValue(int delta)
{
    uint8 *value = OwnData()->mac0 + gCurDigit;
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
    if ((address == this) && inMoveDecrease)
    {
        float delta = -speed * (TIME_MS - timeStartMS);
        if (delta == 0.0F)
        {
            return -0.001F;
        }
        if (delta < -numLines)
        {
            inMoveDecrease = false;
            inMoveIncrease = false;
            *OwnData()->cell = PrevValue();
            if (OwnData()->funcOfChanged)
            {
                OwnData()->funcOfChanged();
            }
            return 0.0F;
        }
        return delta;
    }
    if ((address == this) && inMoveIncrease)
    {
        float delta = speed * (TIME_MS - timeStartMS);
        if (delta == 0.0F)
        {
            return 0.001F;
        }
        if (delta > numLines)
        {
            inMoveDecrease = false;
            inMoveIncrease = false;
            *OwnData()->cell = NextValue();
            if(OwnData()->funcOfChanged)
            {
                OwnData()->funcOfChanged();
            }
            return 0.0F;
        }
        return delta;
    }
    return 0.0F;
}

int16 Governor::NextValue() const
{
    return ((*OwnData()->cell) < OwnData()->maxValue) ? (*OwnData()->cell) + 1 : (int16)(OwnData()->minValue);
}

int16 Governor::PrevValue() const
{
    return ((*OwnData()->cell) > OwnData()->minValue) ? (*OwnData()->cell) - 1 : OwnData()->maxValue;
}

void Governor::NextPosition()
{
    if (IsOpened())
    {
        Math::CircleIncrease<int8>(&gCurDigit, 0, (int8)(NumDigits()) - 1);
    }
}

int Governor::NumDigits() const
{
    int min = Math::NumDigitsInNumber(Math::FabsInt(OwnData()->minValue));
    int max = Math::NumDigitsInNumber(Math::FabsInt(OwnData()->maxValue));
    if (min > max)
    {
        max = min;
    }
    return max;
}

void IPaddress::NextPosition() const
{
    Math::CircleIncrease<int8>(&gCurDigit, 0, (OwnData()->port == 0) ? 11 : 16);
}

void TimeItem::SetOpened()
{
    PackedTime time = HAL_RTC::GetPackedTime();
    *OwnData()->seconds = (int8)(time.seconds);
    *OwnData()->minutes = (int8)(time.minutes);
    *OwnData()->hours = (int8)(time.hours);
    *OwnData()->day = (int8)(time.day);
    *OwnData()->month = (int8)(time.month);
    *OwnData()->year = (int8)(time.year);
}

void TimeItem::SetNewTime() const
{
    const DataTime *own = OwnData();

    HAL_RTC::SetTimeAndData(*own->day, *own->month, *own->year, *own->hours, *own->minutes, *own->seconds);
}

void TimeItem::SelectNextPosition()
{
    Math::CircleIncrease<int8>(OwnData()->curField, 0, 7);
    Color::ResetFlash();
}

void TimeItem::IncCurrentPosition() const
{
    const DataTime *own = OwnData();

    Sound::GovernorChangedValue();
    int8 position = *own->curField;
    if (position != iSET && position != iEXIT)
    {
        int8 *value[] = { 0, own->day, own->month, own->year, own->hours, own->minutes, own->seconds };
        static const int8 max[] = {0, 31, 12, 99, 23, 59, 59};
        static const int8 min[] = {0, 1, 1, 15, 0, 0, 0};
        *(value[position]) = (*(value[position]))++; //-V567
        if (*value[position] > max[position])
        {
            *value[position] = min[position];
        }
    }
}


void TimeItem::DecCurrentPosition() const
{
    const DataTime *own = OwnData();

    Sound::GovernorChangedValue();
    static const int8 max[] = {0, 31, 12, 99, 23, 59, 59};
    static const int8 min[] = {0, 1, 1, 15, 0, 0, 0};
    int8 position = *own->curField;
    if (position != iSET && position != iEXIT)
    {
        int8 *value[] = { 0, own->day, own->month, own->year, own->hours, own->minutes, own->seconds };
        (*(value[position]))--;
        if (*value[position] < min[position])
        {
            *value[position] = max[position];
        }
    }
}


void GovernorColor::ChangeValue(int delta)
{
    ColorType *ct = OwnData()->colorType;
    if (ct->currentField == 0)
    {
        ct->BrightnessChange(delta);
    }
    else
    {
        ct->ComponentChange(delta);
    }
}


void Governor::ShortPress()
{
    if (!IsActive())
    {
        return;
    }
    if (IsOpened())
    {
        NextPosition();
    }
    else
    {
        this->SetCurrent(!Menu::IsCurrentItem(this));
    }
}


char Item::GetSymbolForGovernor() const
{
    static const char chars[] =
    {
        Symbol::S8::GOVERNOR_SHIFT_0,
        Symbol::S8::GOVERNOR_SHIFT_1,
        Symbol::S8::GOVERNOR_SHIFT_2,
        Symbol::S8::GOVERNOR_SHIFT_3
    };

    if (IsGovernor())
    {
        return chars[*ReinterpretToGovernor()->OwnData()->cell];
    }
    else if (IsChoiceReg() || (IsOpened() && IsChoice()))
    {
        return chars[Item::positionGovernor];
    }
    else if (IsTime())
    {
        const DataTime *own = ReinterpretToTime()->OwnData();
        if (IsOpened() && (*own->curField != iEXIT) && (*own->curField != iSET))
        {
            int8 values[7] =
            {
                0,
                *own->day,
                *own->month,
                *own->year,
                *own->hours,
                *own->minutes,
                *own->seconds
            };
            return chars[values[*own->curField]];
        }
    }
    else if (IsPage())
    {
        return chars[ReinterpretToPage()->GetCurrentSubPage()];
    }

    return Symbol::S8::GOVERNOR_SHIFT_0;
}


const Item *Item::Opened()
{
    return PageMain::self->RetLastOpened();
}
