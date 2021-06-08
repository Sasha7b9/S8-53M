// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Painter_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Timer_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
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


struct StateCalibration { enum E {
    None,
    NeedChannel1,       // ����������� � ���������� 1-�� ������
    NeedChannel2,       // ����������� � ���������� 2-�� ������
    ProcessChannel1,    // ��� ���������� ������� ������
    ProcessChannel2,    // ��� ���������� ������� ������
    ErrorChannel1,      // ������ ���������� 1-�� ������
    ErrorChannel2,      // ������ ���������� 2-�� ������
    Complete
};};


uint FPGA::Calibrator::timeStart = 0;

static StateCalibration::E stateCalibration = StateCalibration::None;

static bool errorCalibration = false;                   // ���� true - ��������� ������ �� ����� ����������


void FPGA::Calibrator::PerformCalibration()
{
    timeStart = TIME_MS;

    Settings storedSettings = set;                          // ��������� ���������

    errorCalibration = false;                               // ���� true - �� ����� ���������� ��������� ������

    stateCalibration = StateCalibration::None;

    Panel::DisableInput();

    Display::SetDrawMode(DrawMode::Timer, FPGA::Calibrator::FuncDraw);

    stateCalibration = StateCalibration::NeedChannel1;                              // ��������� ������ �����

    if (Panel::WaitPressingKey() != Key::Start)         { goto ExitCalibration; }

    stateCalibration = StateCalibration::ProcessChannel1;

    if (!CalibrationChannel(ChA))                       { errorCalibration = true; }

    stateCalibration = StateCalibration::NeedChannel2;                              // ��������� ������ �����

    if (Panel::WaitPressingKey() != Key::Start)         { goto ExitCalibration; }

    stateCalibration = StateCalibration::ProcessChannel2;

    if (!CalibrationChannel(ChB))                       { errorCalibration = true; }

    stateCalibration = StateCalibration::Complete;                                  // ������� ���������� � ����������

    Panel::WaitPressingKey();

ExitCalibration:

    set = storedSettings;                                                           // ������������ ���������
    FPGA::LoadSettings();

    Display::SetDrawMode(DrawMode::Default);

    Panel::EnableInput();
}


bool FPGA::Calibrator::CalibrationChannel(const Channel &ch)
{
    /*
    * 1. �������� ������������ ���� ����������.
    * 2. �������� �������� ����������.
    */

    bool result = true;

//    if (!Balancer::Perform(ch, false))
//    {
//        result = false;
//    }

    if (!Stretcher::Perform(ch))
    {
        result = false;
    }

    return result;
}


void FPGA::Calibrator::Balancer::PerformIndividual(const Channel &ch)
{
//    *�� -������������� �����������

    static const pchar messages[Channel::Count][Language::Count] =
    {
        {"���������� ����� 1", "Balancing the channel 1"},
        {"���������� ����� 2", "Balancing the channel 2"}
    };

    Panel::DisableInput();

    Settings storedSettings;

    storedSettings = set;
    Display::Message::Show(messages[LANG][ch]);     // ������� ��������� � ������������.

    CalibrateAddRShift(ch);                         // ���������� ������������ ������:

    set = storedSettings;
    FPGA::LoadSettings();
    Display::Message::Hide();                       // ������ ��������� � ������������

    Panel::EnableInput();
}


bool FPGA::Calibrator::Stretcher::Perform(const Channel & /*ch*/)
{
    return true;
}


bool FPGA::Calibrator::Balancer::CalibrateAddRShift(const Channel &ch)
{
    Stop();

    RShift::Set(ch, RShift::ZERO);                                  // ���������� ����� ���������
    ModeCouple::Set(ch, ModeCouple::GND);
    TBase::Set(TBase::_200us);
    TShift::Set(0);
    TrigSource::Set(ch.ToTrigSource());
    TrigPolarity::Set(TrigPolarity::Front);
    TrigLev::Set(ch.ToTrigSource(), TrigLev::ZERO);
    PeackDetMode::Set(PeackDetMode::Disable);

    CalibratorMode::Set(CalibratorMode::GND);

    bool result = true;

    for (int range = 2; range < Range::Count; range++)
    {
        setNRST.chan[ch].rshift[range][ModeCouple::DC] = 0;
        setNRST.chan[ch].rshift[range][ModeCouple::AC] = 0;

        Range::Set(ch, (Range::E)range);

        float ave = ReadPoints1024(ch);

        int16 addShift = CalculateAddRShift(ave);

        if (Math::Abs(addShift) < 40)
        {
            setNRST.chan[ch].rshift[range][ModeCouple::DC] = addShift;
            setNRST.chan[ch].rshift[range][ModeCouple::AC] = addShift;
        }
        else
        {
            result = false;
        }
    }

    return result;
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

        Text(LANG_RU ? "���������� �� ����� ������ %d ����� ����������� � ������� ������ ����/����."
            " ������� ����� ������ ������ ��� ������ �� ������ ����������" :
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
                time = (100 - 50);
            }

            Text(LANG_RU ? "����������� ���������� ������ %d" : "Channel %d is being calibrated",
                stateCalibration == StateCalibration::ProcessChannel1 ? 1 : 2).
                DrawInRect(50, y + 25, Display::WIDTH - 100, 100);
        }

        break;

    case StateCalibration::ErrorChannel1:

        break;


    case StateCalibration::ErrorChannel2:

        break;

    case StateCalibration::Complete:

        if (errorCalibration)
        {

        }

        break;
    }

    Display::EndFrame();
}
