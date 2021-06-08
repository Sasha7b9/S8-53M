// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Painter_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Timer_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "common/Utils/Containers/Buffer_.h"
#include "common/Utils/Containers/Values_.h"
#include "Display/Console.h"
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


struct StateCalibration { enum E {
    None,
    NeedChannel1,       // Приглашение к калибровке 1-го канала
    NeedChannel2,       // Приглашение к калибровке 2-го канала
    ProcessChannel1,    // Идёт калибровка первого канала
    ProcessChannel2,    // Идёт калибровка второго канала
    CompleteNormal,     // Нормальное завершение калибровки
    CompleteFail        // Калибровка не прошла
};};


uint FPGA::Calibrator::timeStart = 0;

static StateCalibration::E stateCalibration = StateCalibration::None;


void FPGA::Calibrator::PerformCalibration()
{
    Panel::DisableInput();

    timeStart = TIME_MS;

    bool channelAisOK = false;
    bool channelBisOK = false;

    Settings storedSettings = set;                          // Сохранить настройки

    stateCalibration = StateCalibration::None;

    Display::SetDrawMode(DrawMode::Timer, FPGA::Calibrator::FuncDraw);

    stateCalibration = StateCalibration::NeedChannel1;                              // Калибруем первый канал

    if (Panel::WaitPressingKey() != Key::Start)         { goto ExitCalibration; }

    stateCalibration = StateCalibration::ProcessChannel1;

    channelAisOK = CalibrationChannel(ChA);

    stateCalibration = StateCalibration::NeedChannel2;                              // Калибруем второй канал

    if (Panel::WaitPressingKey() != Key::Start)         { goto ExitCalibration; }

    stateCalibration = StateCalibration::ProcessChannel2;

    channelBisOK = CalibrationChannel(ChB);

    stateCalibration = (channelAisOK && channelBisOK) ? StateCalibration::CompleteNormal : StateCalibration::CompleteFail;

    Panel::WaitPressingKey();

ExitCalibration:

    set = storedSettings;                                                           // Восстановить настройки
    FPGA::Init();

    Display::SetDrawMode(DrawMode::Default);

    Panel::EnableInput();
}


bool FPGA::Calibrator::CalibrationChannel(const Channel &ch)
{
    /*
    * 1. Провести балансировку всех диапазонов.
    * 2. Провести растяжку диапазонов.
    */

    bool balance = Balancer::PerformNormal(ch);

    bool stretch = Stretcher::Perform(ch);

    return (balance && stretch);
}


void FPGA::Calibrator::Balancer::PerformOnGround(const Channel &ch)
{
//    *КК -калибровочный коээфициент

    static const pchar messages[Channel::Count][Language::Count] =
    {
        {"Балансирую канал 1", "Balancing the channel 1"},
        {"Балансирую канал 2", "Balancing the channel 2"}
    };

    Panel::DisableInput();

    Settings storedSettings;

    storedSettings = set;
    Display::Message::Show(messages[LANG][ch]);                         // Вывести сообщение о балансировке.

    CalibrateAddRShiftGND(ch);                                          // Произвести балансировку канала

    set = storedSettings;
    FPGA::LoadSettings();
    Display::Message::Hide();                                           // Убрать сообщение о балансировке

    Panel::EnableInput();
}


bool FPGA::Calibrator::Balancer::PerformNormal(const Channel &ch)
{
    return CalibrateAddRShiftNormal(ch);
}


bool FPGA::Calibrator::Stretcher::Perform(const Channel & /*ch*/)
{
    return true;
}


void FPGA::Calibrator::Balancer::CalibrateAddRShiftGND(const Channel &ch)
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


bool FPGA::Calibrator::Balancer::CalibrateAddRShiftNormal(const Channel &ch)
{
    Stop();

    RShift::Set(ch, RShift::ZERO);                                  // установить общие настройки
    TBase::Set(TBase::_200us);
    TShift::Set(0);
    TrigSource::Set(ch.ToTrigSource());
    TrigPolarity::Set(TrigPolarity::Front);
    TrigLev::Set(ch.ToTrigSource(), TrigLev::ZERO);
    PeackDetMode::Set(PeackDetMode::Disable);

    CalibratorMode::Set(CalibratorMode::GND);

    bool result = true;

    for (int range = 0; range < Range::Count; range++)
    {
        for (int mode = 0; mode < 2; mode++)
        {
            setNRST.chan[ch].rshift[range][mode] = 0;
            Range::Set(ch, (Range::E)range);

            float ave = ReadPoints1024(ch);

            int16 addShift = CalculateAddRShift(ave);

            if (Math::Abs(addShift) < 40)
            {
                setNRST.chan[ch].rshift[range][mode] = addShift;
            }
            else
            {
                result = false;
            }
        }
    }
    
    return result;
}


float FPGA::Calibrator::ReadPoints1024(const Channel &ch)
{
    Start();

    uint8 buffer[1024];

    std::memset(buffer, 255, 1024);

    HAL_TIM2::Delay(8);

    uint16 fl = HAL_FMC::Read(RD_FL);

    while (_GET_BIT(fl, FL_PRED_READY) == 0) { fl = HAL_FMC::Read(RD_FL); }

    TrigPolarity::Switch();

    while (_GET_BIT(fl, FL_TRIG_READY) == 0) { fl = HAL_FMC::Read(RD_FL); }

    HAL_TIM2::Delay(8);

    while (_GET_BIT(fl, FL_DATA_READY) == 0) { fl = HAL_FMC::Read(RD_FL); }

    uint16 addr_read = ReaderFPGA::CalculateAddressRead();

    FPGA::BUS::Write(WR_PRED, addr_read, false);
    FPGA::BUS::Write(WR_ADDR_READ, 0xffff, false);

    ReaderFPGA::ADC::ReadPoints(ch, buffer, &buffer[0] + 1024);

    return Buffer<uint8>::Sum(buffer, 1024) / 1024;
}


int16 FPGA::Calibrator::Balancer::CalculateAddRShift(float ave)
{
    return (int16)((Value::AVE - ave) * 2);
}


void FPGA::Calibrator::FuncDraw()
{
    Display::BeginFrame(Color::BLACK);

    Color::WHITE.SetAsCurrent();

    int y = 10;

    switch (stateCalibration)
    {
    case StateCalibration::None:
        break;

    case StateCalibration::NeedChannel1:
    case StateCalibration::NeedChannel2:

        Text(LANG_RU ? "Подключите ко входу канала %d выход калибратора и нажмите кнопку ПУСК/СТОП."
            " Нажмите любую другую кнопку для выхода из режима калибровки" :
            "Connect the output of the calibrator to the input of channel %d and press the START/STOP button."
            " Press any other button to exit calibration mode",
            stateCalibration == StateCalibration::NeedChannel1 ? 1 : 2).
            DrawInRect(50, y + 25, Display::WIDTH - 100, 100);

        break;

    case StateCalibration::ProcessChannel1:
    case StateCalibration::ProcessChannel2:
        {
            uint time = ((TIME_MS - timeStart) / 50) % 100;

            if (time > 50)
            {
                time = (100 - time);
            }

            Text(LANG_RU ? "Выполняется калибровка канала %d" : "Channel %d is being calibrated",
                stateCalibration == StateCalibration::ProcessChannel1 ? 1 : 2).
                DrawInRect(50, y + 25, Display::WIDTH - 100, 100);

            char buf[100] = { 0 };

            for (uint i = 0; i < time; i++)
            {
                std::strcat(buf, ".");
            }

            Text(buf).DrawInCenterRect(50, y + 25, Display::WIDTH - 100, 100);
        }

        break;

    case StateCalibration::CompleteNormal:
    case StateCalibration::CompleteFail:

        if (stateCalibration == StateCalibration::CompleteNormal)
        {
            Text(LANG_RU ? "Калибровка завершена успешно" :
                "Calibration completed successfully").Draw(50, 50);
        }
        else
        {
            Text(LANG_RU ? "ОШИБКА !!! Колибровка завершена неуспешно" :
                "ERROR !!! Calibration completed unsuccessfully").Draw(50, 50);
        }

        Text(LANG_RU ? "Канал 1" : "Channel 1").Draw(10, 100);

        Buffer<int16> buffer(Range::Count);

        FillBuffer(buffer, &setNRST.chan[ChA].rshift[0][0]);

        Text(buffer.ToString().c_str()).Draw(50, 100);

        FillBuffer(buffer, &setNRST.chan[ChA].rshift[0][1]);

        Text(buffer.ToString().c_str()).Draw(50, 110);

        break;
    }

    Display::EndFrame();
}


void FPGA::Calibrator::FillBuffer(Buffer<int16> &buffer, int16 *first)
{
    for (int i = 0; i < Range::Count; i++)
    {
        buffer[i] = *first;
        first += 2;
    }
}
