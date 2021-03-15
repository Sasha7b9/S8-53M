#include "defines.h"
#include "common/Display/Painter_.h"
#include "common/Display/Primitives_.h"
#include "common/Display/Text_.h"
#include "common/Hardware/Timer_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Display/Display.h"
#include "Display/DisplayPrimitives.h"
#include "FPGA/FPGA.h"
#include "Menu/Pages/Definition.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"
#include <cstdio>
#include <limits>


using namespace Primitives;


static int16    CalculateAdditionRShift(Channel::E chan, Range::E range);	// �������� ���������� �������� ������ �� ����������.
static float    CalculateKoeffCalibration(Channel::E chan);			// �������� ����������� ���������� ������ �� ����������.
static void     AlignmentADC();
static void     FuncAttScreen();								// ������� ���������� ������ � ������ ����������.
static float    CalculateDeltaADC(Channel::E chan, float *avgADC1, float *avgADC2, float *delta);
static void     DrawParametersChannel(Channel::E chan, int x, int y, bool inProgress);

static float deltaADC[2] = {0.0F, 0.0F};
static float deltaADCPercents[2] = {0.0F, 0.0F};
static float avrADC1[2] = {0.0F, 0.0F};
static float avrADC2[2] = {0.0F, 0.0F};

static float deltaADCold[2] = {0.0F, 0.0F};
static float deltaADCPercentsOld[2] = {0.0F, 0.0F};
static float avrADC1old[2] = {0.0F, 0.0F};
static float avrADC2old[2] = {0.0F, 0.0F};

static int8 shiftADC0 = 0;
static int8 shiftADC1 = 0;

static float koeffCal0 = -1.0F;
static float koeffCal1 = -1.0F;

static ProgressBar bar0;                            // ��������-��� ��� ���������� ������� ������.
static ProgressBar bar1;                            // ��������-��� ��� ���������� ������� ������.

static uint startTimeChan0 = 0;                     // ����� ������ ���������� ������� ������.
static uint startTimeChan1 = 0;                     // ����� ������ ���������� ������� ������.

static float koeffCalibrationOld[2];


static void OnTimerDraw(void)
{
    Display::Update();
}


void FPGA::ProcedureCalibration(void)
{
    bool chanAenable = SET_ENABLED_A;
    bool chanBenable = SET_ENABLED_B;

    SET_ENABLED_A = true;
    SET_ENABLED_B = true;

    Display::SetDrawMode(DrawMode::Hand, FuncAttScreen);
    Timer::Enable(TypeTimer::TimerDrawHandFunction, 100, OnTimerDraw);

    koeffCalibrationOld[Channel::A] = STRETCH_ADC_A;
    koeffCalibrationOld[Channel::B] = STRETCH_ADC_B;

    bar0.fullTime = 0;
    bar0.passedTime = 0;
    bar1.fullTime = 0;
    bar1.passedTime = 0;

    FPGA::SaveState();                               // ��������� ������� ���������.
    Panel::Disable();                                // ��������� ������ ����������.

    while(1)
    {
        state.stateCalibration = StateCalibration::ADCinProgress;                  // ��������� ��������� ������������ ���.

        TBase::Set(TBase::_500us);
        FPGA::SetTShift(0);
        STRETCH_ADC_A = 1.0F;
        STRETCH_ADC_B = 1.0F;
        FPGA::LoadKoeffCalibration(Channel::A);
        FPGA::LoadKoeffCalibration(Channel::B);
        Range::Set(Channel::A, Range::_500mV);
        Range::Set(Channel::B, Range::_500mV);
        FPGA::SetRShift(Channel::A, RShiftZero);
        FPGA::SetRShift(Channel::B, RShiftZero);
        FPGA::SetModeCouple(Channel::A, ModeCouple::GND);
        FPGA::SetModeCouple(Channel::B, ModeCouple::GND);
//        HAL_FMC::Write(WR_ADD_RSHIFT_DAC1, 0);
//        HAL_FMC::Write(WR_ADD_RSHIFT_DAC2, 0);

        deltaADCPercentsOld[0] = CalculateDeltaADC(Channel::A, &avrADC1old[Channel::A], &avrADC2old[Channel::A], &deltaADCold[Channel::A]);
        deltaADCPercentsOld[1] = CalculateDeltaADC(Channel::B, &avrADC1old[Channel::B], &avrADC2old[Channel::B], &deltaADCold[Channel::B]);

        AlignmentADC();

        deltaADCPercents[Channel::A] = CalculateDeltaADC(Channel::A, &avrADC1[Channel::A], &avrADC2[Channel::A], &deltaADC[Channel::A]);
        deltaADCPercents[Channel::B] = CalculateDeltaADC(Channel::B, &avrADC1[Channel::B], &avrADC2[Channel::B], &deltaADC[Channel::B]);

        state.stateCalibration = StateCalibration::RShift0start;                 

        koeffCal0 = ERROR_VALUE_FLOAT;
        koeffCal1 = ERROR_VALUE_FLOAT;

        if(Panel::WaitPressingButton() == Key::Start)             // ������� ������������� ��� ������ ��������� ���������� ������� ������.
        {
			state.stateCalibration = StateCalibration::RShift0inProgress;

			koeffCal0 = CalculateKoeffCalibration(Channel::A);
			if(koeffCal0 == ERROR_VALUE_FLOAT) //-V2550 //-V550
            {
				state.stateCalibration = StateCalibration::ErrorCalibration0;
				Panel::WaitPressingButton();
                DEBUG_STRETCH_ADC_TYPE = StretchADCtype::Hand;
                PageDebug::LoadStretchADC(Channel::A);
            }
            else
            {
                STRETCH_ADC_A = koeffCal0;
                FPGA::LoadKoeffCalibration(Channel::A);
            }
			
            for (int range = 0; range < Range::Count; range++)
            {
                for (int mode = 0; mode < 2; mode++)
                {
                    if (!(mode == 0 && (range == Range::_2mV || range == Range::_5mV || range == Range::_10mV)))
                    {
                        FPGA::SetModeCouple(Channel::A, (ModeCouple::E)mode);
                        RSHIFT_ADD(Channel::A, range, mode) = 0;
                        RSHIFT_ADD(Channel::A, range, mode) = CalculateAdditionRShift(Channel::A, (Range::E)range);
                    }
                }
            }
		}

        state.stateCalibration = StateCalibration::RShift1start;

        HAL_TIM2::Delay(500);

		if(Panel::WaitPressingButton() == Key::Start)                 // ������� ������������� ��� ������ ��������� ���������� ������� ������.
        {
			state.stateCalibration = StateCalibration::RShift1inProgress;

            koeffCal1 = CalculateKoeffCalibration(Channel::B);
			if(koeffCal1 == ERROR_VALUE_FLOAT) //-V2550 //-V550
            {
				state.stateCalibration = StateCalibration::ErrorCalibration1;
				Panel::WaitPressingButton();
                DEBUG_STRETCH_ADC_TYPE = StretchADCtype::Hand;
                PageDebug::LoadStretchADC(Channel::B);
			}
            else
            {
                STRETCH_ADC_B = koeffCal1;
                FPGA::LoadKoeffCalibration(Channel::B);
            }

            for (int range = 0; range < Range::Count; range++)
            {
                for (int mode = 0; mode < 2; mode++)
                {
                    if (!(mode == 0 && (range == Range::_2mV || range == Range::_5mV || range == Range::_10mV)))
                    {
                        FPGA::SetModeCouple(Channel::B, (ModeCouple::E)mode);
                        RSHIFT_ADD(Channel::B, range, mode) = 0;
                        RSHIFT_ADD(Channel::B, range, mode) = CalculateAdditionRShift(Channel::B, (Range::E)range);
                    }
                }
            }
		}

        break;
    }

    FPGA::RestoreState();

    SET_BALANCE_ADC_A = shiftADC0;
    SET_BALANCE_ADC_B = shiftADC1;
//    HAL_FMC::Write(WR_ADD_RSHIFT_DAC1, (uint8)SET_BALANCE_ADC_A);
//    HAL_FMC::Write(WR_ADD_RSHIFT_DAC2, (uint8)SET_BALANCE_ADC_B);

    FPGA::SetRShift(Channel::A, SET_RSHIFT_A);
    FPGA::SetRShift(Channel::B, SET_RSHIFT_B);

    STRETCH_ADC_A = (koeffCal0 == ERROR_VALUE_FLOAT) ? koeffCalibrationOld[0] : koeffCal0; //-V2550 //-V550

    FPGA::LoadKoeffCalibration(Channel::A);

    STRETCH_ADC_B = (koeffCal1 == ERROR_VALUE_FLOAT) ? koeffCalibrationOld[1] : koeffCal1; //-V2550 //-V550
    FPGA::LoadKoeffCalibration(Channel::B);

    state.stateCalibration = StateCalibration::None;
    Panel::WaitPressingButton();
    Panel::Enable();
    Timer::Disable(TypeTimer::TimerDrawHandFunction);
    Display::SetDrawMode(DrawMode::Auto, 0);
    state.stateCalibration = StateCalibration::None;

    SET_ENABLED_A = chanAenable;
    SET_ENABLED_B = chanBenable;

    FPGA::OnPressStartStop();
}


void FuncAttScreen(void)
{
    Display::BeginFrame(Color::BLACK);

    static bool first = true;
    static uint startTime = 0;
    if(first)
    {
        first = false;
        startTime = TIME_MS;
    }
    int16 y = 10;
    Display::Clear();
    
    Color::FILL.SetAsCurrent();
    
#define dX 20
#define dY -15
    
    switch(FPGA::state.stateCalibration)
    {
        case StateCalibration::None:
        {
                Text("���������� ���������. ������� ����� ������, ����� ����� �� ������ ����������.").DrawInRect(40 + dX, y + 25 + dY, Display::WIDTH - 100, 200);

                Text("�������� ���� 1� :").Draw(10 + dX, 55 + dY);
                Text("�������� ���� 2� :").Draw(10 + dX, 80 + dY);

                for (int i = 0; i < Range::Count; i++)
                {
                    Text("%d", RSHIFT_ADD(Channel::A, i, 0)).Draw(95 + i * 16 + dX, 55 + dY, Color::FILL);
                    Text("%d", RSHIFT_ADD(Channel::A, i, 1)).Draw(95 + i * 16 + dX, 65 + dY, Color::FILL);
                    Text("%d", RSHIFT_ADD(Channel::B, i, 0)).Draw(95 + i * 16 + dX, 80 + dY, Color::FILL);
                    Text("%d", RSHIFT_ADD(Channel::B, i, 1)).Draw(95 + i * 16 + dX, 90 + dY, Color::FILL);
                }
                
                Text("����������� ���������� 1� : %f, %d", STRETCH_ADC_A, (int)(STRETCH_ADC_A * 0x80)).Draw(10 + dX, 110 + dY, Color::FILL);
                Text("������������ ���������� 2� : %f, %d", STRETCH_ADC_B, (int)(STRETCH_ADC_B * 0x80)).Draw(10 + dX, 130 + dY, Color::FILL);

                DrawParametersChannel(Channel::A, 10 + dX, 150 + dY, false);
                DrawParametersChannel(Channel::B, 10 + dX, 200 + dY, false);
        }
            break;

        case StateCalibration::ADCinProgress:
            DrawParametersChannel(Channel::A, 5, 25, true);
            DrawParametersChannel(Channel::B, 5, 75, true);
            break;

        case StateCalibration::RShift0start:
            Text("���������� �� ����� ������ 1 ����� ����������� � ������� ������ ����/����. ���� �� �� ������ ����������� ������ ������, ������� ����� ������ ������.").
                DrawInRect(50, y + 25, Display::WIDTH - 100, 200);
            break;

        case StateCalibration::RShift0inProgress:
            break;

        case StateCalibration::RShift1start:
            Text("���������� �� ����� ������ 2 ����� ����������� � ������� ������ ����/����. ���� �� �� ������ ����������� ������ �����, ������� ����� ������ ������.").
                DrawInRect(50, y + 25, Display::WIDTH - 100, 200);
            break;

        case StateCalibration::RShift1inProgress:
            break;

        case StateCalibration::ErrorCalibration0:
            Text("�������� !!! ����� 1 �� �����������.").DrawInRect(50, y + 25, Display::WIDTH - 100, 200);
            break;

        case StateCalibration::ErrorCalibration1:
            Text("�������� !!! ����� 2 �� �����������.").DrawInRect(50, y + 25, Display::WIDTH - 100, 200);
            break;
    }

    /*
    if(stateFPGA.stateCalibration == kNone || stateFPGA.stateCalibration == kRShift0start || stateFPGA.stateCalibration == kRShift1start) {
        x = 230;
        y = 187;
        int delta = 32;
        width = 80;
        height = 25;
        DrawRectangle(x, y, width, height, Color::BLACK);
        DrawStringInCenterRect(x, y, width, height, "����������", Color::BLACK, false);
        DrawRectangle(x, y - delta, width, height, Color::BLACK);
        DrawStringInCenterRect(x, y - delta, width, height, "��������", Color::BLACK, false);
    }
    */
    char buffer[100];
    std::sprintf(buffer, "%.1f", (TIME_MS - startTime) / 1000.0F);
    Text(buffer).Draw(0, 0, Color::BLACK);

    Painter::SendFrame();
}


void DrawParametersChannel(Channel::E chan, int eX, int eY, bool inProgress)
{
    Color::FILL.SetAsCurrent();
    if(inProgress)
    {
        Text(chan == 0 ? "����� 1" : "����� 2").Draw(eX, eY + 4);
        ProgressBar *bar = (chan == Channel::A) ? &bar0 : &bar1;
        bar->width = 240;
        bar->height = 15;
        bar->y = eY;
        bar->x = 60;
        bar->Draw();
    }

    if(Settings::DebugModeEnable())
    {
        int x = inProgress ? 5 : eX;
        int y = eY + (inProgress ? 110 : 0);
        Text("���������� �� ����:").Draw(x, y);
        char buffer[100] = {0};
        std::sprintf(buffer, "���1 = %.2f/%.2f, ���2 = %.2f/%.2f, d = %.2f/%.2f", avrADC1old[chan] - AVE_VALUE, avrADC1[chan] - AVE_VALUE,
                                                                             avrADC2old[chan] - AVE_VALUE, avrADC2[chan] - AVE_VALUE,
                                                                             deltaADCold[chan], deltaADC[chan]);
        y += 10;
        Text(buffer).Draw(x, y);
        buffer[0] = 0;
        std::sprintf(buffer, "����������� Channel::A�� = %.2f/%.2f %%", deltaADCPercentsOld[chan], deltaADCPercents[chan]);
        Text(buffer).Draw(x, y + 11);
        buffer[0] = 0;
        std::sprintf(buffer, "�������� %d", SET_BALANCE_ADC(chan));
        Text(buffer).Draw(x, y + 19);
    }
}


float CalculateDeltaADC(Channel::E chan, float *avgADC1, float *avgADC2, float *delta)
{
    uint *startTime = (chan == Channel::A) ? &startTimeChan0 : &startTimeChan1;
    *startTime = TIME_MS;
    
    ProgressBar *bar = (chan == Channel::A) ? &bar0 : &bar1;
    bar->passedTime = 0;
    bar->fullTime = 0;

    FPGA::SetTrigSource((TrigSource::E)chan);
    FPGA::SetTrigLev((TrigSource::E)chan, TrigLevZero);

    uint16 *address1 = chan == Channel::A ? RD_ADC_A : RD_ADC_B;
    uint16 *address2 = chan == Channel::A ? RD_ADC_A : RD_ADC_B;

    static const int numCicles = 10;
    for(int cicle = 0; cicle < numCicles; cicle++)
    {
        HAL_FMC::Write(WR_START, 1);
        while(_GET_BIT(HAL_FMC::Read(RD_FL), 2) == 0) {};
        FPGA::SwitchingTrig();
        while(_GET_BIT(HAL_FMC::Read(RD_FL), 0) == 0) {};
        HAL_FMC::Write(WR_STOP, 1);

        for(int i = 0; i < FPGA_MAX_POINTS; i++)
        {
            if(chan == Channel::A)
            {
                *avgADC1 += HAL_FMC::Read(address1);
                *avgADC2 += HAL_FMC::Read(address2);
                HAL_FMC::Read(RD_ADC_B);
                HAL_FMC::Read(RD_ADC_B);
            }
            else
            {
                HAL_FMC::Read(RD_ADC_A);
                HAL_FMC::Read(RD_ADC_A);
                *avgADC1 += HAL_FMC::Read(address1);
                *avgADC2 += HAL_FMC::Read(address2);
            }
        }
        
        bar->passedTime = static_cast<float>(TIME_MS - *startTime);
        bar->fullTime = bar->passedTime * (float)numCicles / (cicle + 1);
    }

    *avgADC1 /= (FPGA_MAX_POINTS * numCicles);
    *avgADC2 /= (FPGA_MAX_POINTS * numCicles);

    *delta = *avgADC1 - *avgADC2;

    return ((*avgADC1) - (*avgADC2)) / 255.0F * 100;
}


void AlignmentADC(void)
{
    shiftADC0 = static_cast<int8>((deltaADCold[0] > 0.0F) ? (deltaADCold[0] + 0.5F) : (deltaADCold[0] - 0.5F));
    SET_BALANCE_ADC_A = shiftADC0;
    shiftADC1 = static_cast<int8>((deltaADCold[1] > 0.0F) ? (deltaADCold[1] + 0.5F) : (deltaADCold[1] - 0.5F));
    SET_BALANCE_ADC_B = shiftADC1;
//    HAL_FMC::Write(WR_ADD_RSHIFT_DAC1, (uint8)SET_BALANCE_ADC_A);
//    HAL_FMC::Write(WR_ADD_RSHIFT_DAC2, (uint8)SET_BALANCE_ADC_B);
}


int16 CalculateAdditionRShift(Channel::E chan, Range::E range)
{
    Range::Set(chan, range);
    FPGA::SetRShift(chan, RShiftZero);
    TBase::Set(TBase::_200us);
    FPGA::SetTrigSource(chan == Channel::A ? TrigSource::A : TrigSource::B);
    FPGA::SetTrigPolarity(TrigPolarity::Front);
    FPGA::SetTrigLev((TrigSource::E)chan, TrigLevZero);

    FPGA::WriteToHardware(WR_UPR, BIN_U8(00000000), false);   // ������������� ����� ����������� � ����

    int numMeasures = 8;
    int sum = 0;
    int numPoints = 0;

    uint time = TIME_MS;

    while(TIME_MS - time < 50) {};

    for(int i = 0; i < numMeasures; i++)
    {
        uint startTime = TIME_MS;
        const uint timeWait = 100;

        HAL_FMC::Write(WR_START, 1);
        while(_GET_BIT(HAL_FMC::Read(RD_FL), 2) == 0 && (TIME_MS - startTime < timeWait)) {};
        if(TIME_MS - startTime > timeWait)                 // ���� ������ ������� ����� ������� - 
        {
            return ERROR_VALUE_INT16;                       // ����� � �������.
        }

        FPGA::SwitchingTrig();

        startTime = TIME_MS;

        while(_GET_BIT(HAL_FMC::Read(RD_FL), 0) == 0 && (TIME_MS - startTime < timeWait)) {};
        if(TIME_MS - startTime > timeWait)                 // ���� ������ ������� ����� ������� - 
        {
            return ERROR_VALUE_INT16;                       // ����� � �������.
        }

        HAL_FMC::Write(WR_STOP, 1);

        uint16 *addressRead1 = chan == Channel::A ? RD_ADC_A : RD_ADC_B;
        uint16 *addressRead2 = chan == Channel::A ? RD_ADC_A : RD_ADC_B;

        for(int j = 0; j < FPGA_MAX_POINTS; j += 2)
        {
            sum += HAL_FMC::Read(addressRead1);
            sum += HAL_FMC::Read(addressRead2);
            numPoints += 2;
        }
    }

    float aveValue = (float)sum / numPoints;
    int16 retValue = static_cast<int16>(-(aveValue - AVE_VALUE) * 2);

    if(retValue < - 100 || retValue > 100)
    {
        return ERROR_VALUE_INT16;
    }
    return retValue;
}


float CalculateKoeffCalibration(Channel::E chan)
{
    FPGA::WriteToHardware(WR_UPR, BIN_U8(00000100), false);

    FPGA::SetRShift(chan, RShiftZero - 40 * 4);
    FPGA::SetModeCouple(chan, ModeCouple::DC);
    FPGA::SetTrigSource((TrigSource::E)chan);
    FPGA::SetTrigLev((TrigSource::E)chan, TrigLevZero + 40 * 4);
    
    int numMeasures = 16;
    int sumMIN = 0;
    int numMIN = 0;
    int sumMAX = 0;
    int numMAX = 0;

    for(int i = 0; i < numMeasures; i++)
    {
        uint startTime = TIME_MS;
        const uint timeWait = 1000;

        while(TIME_MS - startTime < 20) {}
        startTime = TIME_MS;

        HAL_FMC::Write(WR_START, 1);
        while(_GET_BIT(HAL_FMC::Read(RD_FL), 2) == 0 && (TIME_MS - startTime > timeWait)) {};
        if(TIME_MS - startTime > timeWait)
        {
            return ERROR_VALUE_FLOAT;
        }

        FPGA::SwitchingTrig();
        startTime = TIME_MS;

        while(_GET_BIT(HAL_FMC::Read(RD_FL), 0) == 0 && (TIME_MS - startTime > timeWait)) {};
        if(TIME_MS - startTime > timeWait)
        {
            return ERROR_VALUE_FLOAT;
        }

        HAL_FMC::Write(WR_STOP, 1);

        uint16 *addressRead1 = chan == Channel::A ? RD_ADC_A : RD_ADC_B;
        uint16 *addressRead2 = chan == Channel::A ? RD_ADC_A : RD_ADC_B;

        for(int j = 0; j < FPGA_MAX_POINTS; j += 2)
        {
            uint8 val0 = (uint8)HAL_FMC::Read(addressRead1);
            if(val0 > AVE_VALUE + 60)
            {
                numMAX++;
                sumMAX += val0;
            }
            else if(val0 < AVE_VALUE - 60)
            {
                numMIN++;
                sumMIN += val0;
            }

            uint8 val1 = (uint8)HAL_FMC::Read(addressRead2);
            if(val1 > AVE_VALUE + 60)
            {
                numMAX++;
                sumMAX += val1;
            }
            else if(val1 < AVE_VALUE - 60)
            {
                numMIN++;
                sumMIN += val1;
            }
        }
    }

    float aveMin = (float)sumMIN / (float)numMIN;
    float aveMax = (float)sumMAX / (float)numMAX;

    float retValue = 160.0F / (aveMax - aveMin);

    if(retValue < 0.5F || retValue > 1.5F)
    {
        return ERROR_VALUE_FLOAT;
    }
    return retValue * 1.004F;
}
