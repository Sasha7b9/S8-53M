// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Painter_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Timer_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Containers/Buffer_.h"
#include "common/Utils/Containers/Values_.h"
#include "Display/Display.h"
#include "Display/Painter/DisplayPrimitives.h"
#include "FPGA/FPGA.h"
#include "FPGA/ReaderFPGA.h"
#include "Menu/Pages/Definition.h"
#include "Panel/Panel.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>


static Float koeffCal0 = -1.0F;
static Float koeffCal1 = -1.0F;


void FPGA::Calibrator::PerformCalibration()
{
   
}


void FPGA::Calibrator::Balancer::Perform(const Channel &ch)
{
//    *КК -калибровочный коээфициент

    static const pchar messages[Channel::Count][Language::Count] =
    {
        {"Балансирую канал 1", "Balancing the channel 1"},
        {"Балансирую канал 2", "Balancing the channel 2"}
    };

    Panel::DisableInput();
    Display::Message::Show(messages[LANG][ch]);     // Вывести сообщение о балансировке.

    Settings storedSettings = set;                  // Сохранить настройки

    CalibrateAddRShift(ch);                         // Произвести балансировку канала:

    set = storedSettings;                           // Восстановить настройки
    FPGA::LoadSettings();

    Display::Message::Hide();                       // Убрать сообщение о балансировке
    Panel::EnableInput();
}


void FPGA::Calibrator::Balancer::CalibrateAddRShift(const Channel &ch)
{
    Stop();

    RShift::Set(ch, RShift::ZERO);                                  // установить общие настройки
    ModeCouple::Set(ch, ModeCouple::GND);
    TBase::Set(TBase::_200us);
    TShift::Set(0);
    TrigSource::Set(ch.ToTrigSource());
    TrigPolarity::Set(TrigPolarity::Front);
    TrigLev::Set(ch.ToTrigSource(), TrigLev::ZERO);
    PeackDetMode::Set(PeackDetMode::Disable);

    CalibratorMode::Set(CalibratorMode::GND);

    for (int range = 2; range < Range::Count; range++)
    {
        setNRST.chan[ch].rshift[range][ModeCouple::DC] = 0;
        setNRST.chan[ch].rshift[range][ModeCouple::AC] = 0;

        Range::Set(ch, (Range::E)range);

        float ave = ReadPoints1024(ch);

        int16 addShift = CalculateAddRShift(ave);

        setNRST.chan[ch].rshift[range][ModeCouple::DC] = addShift;
        setNRST.chan[ch].rshift[range][ModeCouple::AC] = addShift;
    }
}


float FPGA::Calibrator::ReadPoints1024(const Channel &ch)
{
    Start();

    bool readed = false;

    uint8 buffer[1024];

    std::memset(buffer, 255, 1024);

    while (!readed)
    {
        flag.Read();

        if (flag.IsPredLaunchReady())
        {
            if (flag.IsTrigReady())
            {
                if (flag.IsDataReady())
                {
                    uint16 addr_read = ReaderFPGA::CalculateAddressRead();

                    FPGA::BUS::Write(WR_PRED, addr_read, false);
                    FPGA::BUS::Write(WR_ADDR_READ, 0xffff, false);

                    ReaderFPGA::ADC::ReadPoints(ch, buffer, &buffer[0] + 1024);

                    readed = true;
                }
            }
        }
    }

    return Buffer<uint8>::Sum(buffer, 1024) / 1024;
}


int16 FPGA::Calibrator::Balancer::CalculateAddRShift(float ave)
{
    return (int16)((Value::AVE - ave) * 2);
}
