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


struct StateCalibration { enum E {
    None,
    NeedChannel1,       // ����������� � ���������� 1-�� ������
    NeedChannel2,       // ����������� � ���������� 2-�� ������
    ErrorChannel1,      // ������ ���������� 1-�� ������
    ErrorChannel2,      // ������ ���������� 2-�� ������
    Complete
};};


volatile static StateCalibration::E stateCalibration = StateCalibration::None;

static Float koeffCal0 = -1.0F;
static Float koeffCal1 = -1.0F;


void FPGA::Calibrator::PerformCalibration()
{
    stateCalibration = StateCalibration::None;

    Panel::DisableInput();

    Display::SetDrawMode(DrawMode::Timer, FPGA::Calibrator::FuncDraw);

    stateCalibration = StateCalibration::NeedChannel1;                              // ��������� ������ �����

    if (Panel::WaitPressingKey() != Key::Start)
    {
        return ExitCalibration();
    }

    if (!CalibrationChannel(ChA))
    {
        stateCalibration = StateCalibration::ErrorChannel1;

        Panel::WaitPressingKey();

        return ExitCalibration();
    }

    stateCalibration = StateCalibration::NeedChannel2;                              // ��������� ������ �����

    if (Panel::WaitPressingKey() != Key::Start)
    {
        return ExitCalibration();
    }

    if (!CalibrationChannel(ChB))
    {
        stateCalibration = StateCalibration::ErrorChannel2;

        Panel::WaitPressingKey();

        return ExitCalibration();
    }

    stateCalibration = StateCalibration::Complete;                                  // ������� ���������� � ����������

    Panel::WaitPressingKey();

    return ExitCalibration();
}


bool FPGA::Calibrator::CalibrationChannel(const Channel & /*ch*/)
{
    return false;
}


void FPGA::Calibrator::ExitCalibration()
{
    Display::SetDrawMode(DrawMode::Default);

    Panel::EnableInput();
}


void FPGA::Calibrator::Balancer::Perform(const Channel &ch)
{
//    *�� -������������� �����������

    static const pchar messages[Channel::Count][Language::Count] =
    {
        {"���������� ����� 1", "Balancing the channel 1"},
        {"���������� ����� 2", "Balancing the channel 2"}
    };

    Panel::DisableInput();
    Display::Message::Show(messages[LANG][ch]);     // ������� ��������� � ������������.

    Settings storedSettings = set;                  // ��������� ���������

    CalibrateAddRShift(ch);                         // ���������� ������������ ������:

    set = storedSettings;                           // ������������ ���������
    FPGA::LoadSettings();

    Display::Message::Hide();                       // ������ ��������� � ������������
    Panel::EnableInput();
}


void FPGA::Calibrator::Balancer::CalibrateAddRShift(const Channel &ch)
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

    Buffer<int16> buffer(Range::Count);

    for (int range = 2; range < Range::Count; range++)
    {
        setNRST.chan[ch].rshift[range][ModeCouple::DC] = 0;
        setNRST.chan[ch].rshift[range][ModeCouple::AC] = 0;

        Range::Set(ch, (Range::E)range);

        float ave = ReadPoints1024(ch);

        int16 addShift = CalculateAddRShift(ave);

        buffer[range] = addShift;

        setNRST.chan[ch].rshift[range][ModeCouple::DC] = addShift;
        setNRST.chan[ch].rshift[range][ModeCouple::AC] = addShift;
    }

    LOG_WRITE(buffer.ToString().c_str());
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

        Text("���������� �� ����� ������ 1 ����� ����������� � ������� ������ ����/����. ������� ����� ������ ������"
             " ��� ������ �� ������ ����������").DrawInRect(50, y + 25, Display::WIDTH - 100, 100);

        break;

    case StateCalibration::NeedChannel2:

        Text("���������� �� ����� ������ 1 ����� ����������� � ������� ������ ����/����. ������� ����� ������ ������"
            " ��� ������ �� ������ ����������").DrawInRect(50, y + 25, Display::WIDTH - 100, 100);

        break;

    case StateCalibration::ErrorChannel1:

        break;


    case StateCalibration::ErrorChannel2:

        break;

    case StateCalibration::Complete:
        break;
    }

    Display::EndFrame();
}
