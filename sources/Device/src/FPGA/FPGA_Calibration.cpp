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


// Измерить добавочное смещение канала по напряжению.
static int16 CalculateAdditionRShift(const Channel &ch, Range::E range);

// Измерить коэффициент калибровки канала по напряжению
static float CalculateKoeffCalibration(const Channel &ch);

static void  AlignmentADC();

// Функция обновления экрана в режиме калибровки
static void  FuncAttScreen();

static float CalculateDeltaADC(const Channel &ch, float *avgADC1, float *avgADC2, float *delta);

static void  DrawParametersChannel(const Channel &ch, int x, int y, bool inProgress);

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

static ProgressBar bar0;                            // Прогресс-бар для калибровки первого канала.
static ProgressBar bar1;                            // Прогресс-бар для калибровки второго канала.

static uint startTimeChan0 = 0;                     // Время начала калибровки первого канала.
static uint startTimeChan1 = 0;                     // Время начала калибровки второго канала.

static float koeffCalibrationOld[2];


static void OnTimerDraw()
{
    Display::Update();
}


void FPGA::Calibrator::ProcedureCalibration()
{
    bool chanAenable = ChA.IsEnabled();
    bool chanBenable = ChB.IsEnabled();

    set.chan[ChA].enable = true;
    set.chan[ChB].enable = true;

    Display::SetDrawMode(DrawMode::Hand, FuncAttScreen);
    Timer::Enable(TypeTimer::TimerDrawHandFunction, 100, OnTimerDraw);

    koeffCalibrationOld[ChA] = SettingsChannel::StretchADC(ChA);
    koeffCalibrationOld[ChB] = SettingsChannel::StretchADC(ChB);

    bar0.fullTime = 0;
    bar0.passedTime = 0;
    bar1.fullTime = 0;
    bar1.passedTime = 0;

    FPGA::state.Save();                               // Сохраняем текущее состояние.
    Panel::Disable();                                // Отлкючаем панель управления.

    while(1)
    {
        state.state_calibration = StateCalibration::ADCinProgress;              // Запускаем процедуру балансировки АЦП.

        TBase::Set(TBase::_500us);
        TShift::Set(0);
        set.chan[ChA].stretchADC = 1.0F;
        set.chan[ChB].stretchADC = 1.0F;
        Calibrator::LoadKoeff(ChA);
        Calibrator::LoadKoeff(ChB);
        Range::Set(ChA, Range::_500mV);
        Range::Set(ChB, Range::_500mV);
        RShift::Set(ChA, RShiftZero);
        RShift::Set(ChB, RShiftZero);
        ModeCouple::Set(ChA, ModeCouple::GND);
        ModeCouple::Set(ChB, ModeCouple::GND);
//        HAL_FMC::Write(WR_ADD_RSHIFT_DAC1, 0);
//        HAL_FMC::Write(WR_ADD_RSHIFT_DAC2, 0);

        deltaADCPercentsOld[0] = CalculateDeltaADC(ChA, &avrADC1old[ChA], &avrADC2old[ChA], &deltaADCold[ChA]);
        deltaADCPercentsOld[1] = CalculateDeltaADC(ChB, &avrADC1old[ChB], &avrADC2old[ChB], &deltaADCold[ChB]);

        AlignmentADC();

        deltaADCPercents[ChA] = CalculateDeltaADC(ChA, &avrADC1[ChA], &avrADC2[ChA], &deltaADC[ChA]);
        deltaADCPercents[ChB] = CalculateDeltaADC(ChB, &avrADC1[ChB], &avrADC2[ChB], &deltaADC[ChB]);

        state.state_calibration = StateCalibration::RShift0start;                 

        koeffCal0 = ERROR_VALUE_FLOAT;
        koeffCal1 = ERROR_VALUE_FLOAT;

                                                 // Ожидаем подтверждения или отмены процедуры калибровки первого канала
        if(Panel::WaitPressingButton() == Key::Start)
        {
            state.state_calibration = StateCalibration::RShift0inProgress;

            koeffCal0 = CalculateKoeffCalibration(ChA);
            if (koeffCal0 == ERROR_VALUE_FLOAT)
            {
                state.state_calibration = StateCalibration::ErrorCalibration0;
                Panel::WaitPressingButton();
                set.debug.stretch_ADC_type = StretchADCtype::Hand;
                PageDebug::LoadStretchADC(ChA);
            }
            else
            {
                set.chan[ChA].stretchADC = koeffCal0;
                Calibrator::LoadKoeff(ChA);
            }

            for (int range = 0; range < Range::Count; range++)
            {
                for (int mode = 0; mode < 2; mode++)
                {
                    if (!(mode == 0 && (range == Range::_2mV || range == Range::_5mV || range == Range::_10mV)))
                    {
                        ModeCouple::Set(ChA, (ModeCouple::E)mode);
                        RSHIFT_ADD(ChA, range, mode) = 0;
                        RSHIFT_ADD(ChA, range, mode) = CalculateAdditionRShift(ChA, (Range::E)range);
                    }
                }
            }
        }

        state.state_calibration = StateCalibration::RShift1start;

        HAL_TIM2::Delay(500);

                                                 // Ожидаем подтверждения или отмены процедуры калибровки второго канала
        if (Panel::WaitPressingButton() == Key::Start)
        {
            state.state_calibration = StateCalibration::RShift1inProgress;

            koeffCal1 = CalculateKoeffCalibration(ChB);
            if (koeffCal1 == ERROR_VALUE_FLOAT)
            {
                state.state_calibration = StateCalibration::ErrorCalibration1;
                Panel::WaitPressingButton();
                set.debug.stretch_ADC_type = StretchADCtype::Hand;
                PageDebug::LoadStretchADC(ChB);
            }
            else
            {
                set.chan[ChB].stretchADC = koeffCal1;
                Calibrator::LoadKoeff(ChB);
            }

            for (int range = 0; range < Range::Count; range++)
            {
                for (int mode = 0; mode < 2; mode++)
                {
                    if (!(mode == 0 && (range == Range::_2mV || range == Range::_5mV || range == Range::_10mV)))
                    {
                        ModeCouple::Set(ChB, (ModeCouple::E)mode);
                        RSHIFT_ADD(ChB, range, mode) = 0;
                        RSHIFT_ADD(ChB, range, mode) = CalculateAdditionRShift(ChB, (Range::E)range);
                    }
                }
            }
        }

        break;
    }

    FPGA::state.Restore();

    set.chan[ChA].balanceShiftADC = shiftADC0;
    set.chan[ChB].balanceShiftADC = shiftADC1;

    //    HAL_FMC::Write(WR_ADD_RSHIFT_DAC1, (uint8)SET_BALANCE_ADC_A);
//    HAL_FMC::Write(WR_ADD_RSHIFT_DAC2, (uint8)SET_BALANCE_ADC_B);

    RShift::Set(ChA, RShift::Get(ChA));
    RShift::Set(ChB, RShift::Get(ChB));

    set.chan[ChA].stretchADC = (koeffCal0 == ERROR_VALUE_FLOAT) ? koeffCalibrationOld[0] : koeffCal0;

    Calibrator::LoadKoeff(ChA);

    set.chan[ChB].stretchADC = (koeffCal1 == ERROR_VALUE_FLOAT) ? koeffCalibrationOld[1] : koeffCal1;
    Calibrator::LoadKoeff(ChB);

    state.state_calibration = StateCalibration::None;
    Panel::WaitPressingButton();
    Panel::Enable();
    Timer::Disable(TypeTimer::TimerDrawHandFunction);
    Display::SetDrawMode(DrawMode::Auto, 0);
    state.state_calibration = StateCalibration::None;

    set.chan[ChA].enable = chanAenable;
    set.chan[ChB].enable = chanBenable;

    FPGA::OnPressStartStop();
}


void FuncAttScreen()
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
    
    switch(FPGA::state.state_calibration)
    {
        case StateCalibration::None:
        {
                Text("Калибровка завершена. Нажмите любую кнопку, чтобы выйти из режима калибровки.").
                    DrawInRect(40 + dX, y + 25 + dY, Display::WIDTH - 100, 200);

                Text("Поправка нуля 1к :").Draw(10 + dX, 55 + dY);
                Text("Поправка нуля 2к :").Draw(10 + dX, 80 + dY);

                for (int i = 0; i < Range::Count; i++)
                {
                    Text(String("%d", RSHIFT_ADD(ChA, i, 0))).Draw(95 + i * 16 + dX, 55 + dY, Color::FILL);
                    Text(String("%d", RSHIFT_ADD(ChA, i, 1))).Draw(95 + i * 16 + dX, 65 + dY, Color::FILL);
                    Text(String("%d", RSHIFT_ADD(ChB, i, 0))).Draw(95 + i * 16 + dX, 80 + dY, Color::FILL);
                    Text(String("%d", RSHIFT_ADD(ChB, i, 1))).Draw(95 + i * 16 + dX, 90 + dY, Color::FILL);
                }
                
                Text(String("Коэффициент калибровки 1к : %f, %d", SettingsChannel::StretchADC(ChA),
                    (int)(SettingsChannel::StretchADC(ChA) * 0x80))).Draw(10 + dX, 110 + dY, Color::FILL);

                Text(String("Коэфффициент калибровки 2к : %f, %d", SettingsChannel::StretchADC(ChB),
                    (int)(SettingsChannel::StretchADC(ChA) * 0x80))).Draw(10 + dX, 130 + dY, Color::FILL);

                DrawParametersChannel(ChA, 10 + dX, 150 + dY, false);
                DrawParametersChannel(ChB, 10 + dX, 200 + dY, false);
        }
            break;

        case StateCalibration::ADCinProgress:
            DrawParametersChannel(ChA, 5, 25, true);
            DrawParametersChannel(ChB, 5, 75, true);
            break;

        case StateCalibration::RShift0start:
            Text("Подключите ко входу канала 1 выход калибратора и нажмите кнопку ПУСК/СТОП. Если вы не хотите калибровать первый канала, нажмите любую другую кнопку.").
                DrawInRect(50, y + 25, Display::WIDTH - 100, 200);
            break;

        case StateCalibration::RShift0inProgress:
            break;

        case StateCalibration::RShift1start:
            Text("Подключите ко входу канала 2 выход калибратора и нажмите кнопку ПУСК/СТОП. Если вы не хотите калибровать второй канал, нажмите любую другую кнопку.").
                DrawInRect(50, y + 25, Display::WIDTH - 100, 200);
            break;

        case StateCalibration::RShift1inProgress:
            break;

        case StateCalibration::ErrorCalibration0:
            Text("Внимание !!! Канал 1 не скалиброван.").DrawInRect(50, y + 25, Display::WIDTH - 100, 200);
            break;

        case StateCalibration::ErrorCalibration1:
            Text("Внимание !!! Канал 2 не скалиброван.").DrawInRect(50, y + 25, Display::WIDTH - 100, 200);
            break;
    }

    /*
    if(stateFPGA.state_calibration == kNone || stateFPGA.state_calibration == kRShift0start || stateFPGA.state_calibration == kRShift1start) {
        x = 230;
        y = 187;
        int delta = 32;
        width = 80;
        height = 25;
        DrawRectangle(x, y, width, height, Color::BLACK);
        DrawStringInCenterRect(x, y, width, height, "ПРОДОЛЖИТЬ", Color::BLACK, false);
        DrawRectangle(x, y - delta, width, height, Color::BLACK);
        DrawStringInCenterRect(x, y - delta, width, height, "ОТМЕНИТЬ", Color::BLACK, false);
    }
    */
    char buffer[100];
    std::sprintf(buffer, "%.1f", (TIME_MS - startTime) / 1000.0F);
    Text(buffer).Draw(0, 0, Color::BLACK);

    Painter::SendFrame();
}


void DrawParametersChannel(const Channel &ch, int eX, int eY, bool inProgress)
{
    Color::FILL.SetAsCurrent();
    if(inProgress)
    {
        Text(ch == 0 ? "КАНАЛ 1" : "КАНАЛ 2").Draw(eX, eY + 4);
        ProgressBar *bar = (ch == ChA) ? &bar0 : &bar1;
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
        Text("Отклонение от нуля:").Draw(x, y);
        char buffer[100] = {0};
        std::sprintf(buffer, "АЦП1 = %.2f/%.2f, АЦП2 = %.2f/%.2f, d = %.2f/%.2f", avrADC1old[ch] - AVE_VALUE, avrADC1[ch] - AVE_VALUE,
                                                                             avrADC2old[ch] - AVE_VALUE, avrADC2[ch] - AVE_VALUE,
                                                                             deltaADCold[ch], deltaADC[ch]);
        y += 10;
        Text(buffer).Draw(x, y);
        buffer[0] = 0;
        std::sprintf(buffer, "Расхождение Channel::A_ЦП = %.2f/%.2f %%", deltaADCPercentsOld[ch], deltaADCPercents[ch]);
        Text(buffer).Draw(x, y + 11);
        buffer[0] = 0;
        std::sprintf(buffer, "Записано %d", SettingsChannel::BalanceShiftADC(ch));
        Text(buffer).Draw(x, y + 19);
    }
}


float CalculateDeltaADC(const Channel &ch, float *avgADC1, float *avgADC2, float *delta)
{
    uint *startTime = (ch == ChA) ? &startTimeChan0 : &startTimeChan1;
    *startTime = TIME_MS;
    
    ProgressBar *bar = (ch == ChA) ? &bar0 : &bar1;
    bar->passedTime = 0;
    bar->fullTime = 0;

    TrigSource::Set((TrigSource::E)ch.value);
    TrigLev::Set((TrigSource::E)ch.value, TrigLevZero);

    uint16 *address1 = (ch == ChA) ? RD_ADC_A : RD_ADC_B;
    uint16 *address2 = (ch == ChA) ? RD_ADC_A : RD_ADC_B;

    static const int numCicles = 10;
    for(int cicle = 0; cicle < numCicles; cicle++)
    {
        HAL_FMC::Write(WR_START, 1);
        while(_GET_BIT(HAL_FMC::Read(RD_FL), 2) == 0) {};

        TrigPolarity::Switch();

        while(_GET_BIT(HAL_FMC::Read(RD_FL), 0) == 0) {};
        HAL_FMC::Write(WR_STOP, 1);

        for(int i = 0; i < FPGA_MAX_POINTS; i++)
        {
            if(ch == ChA)
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


void AlignmentADC()
{
    shiftADC0 = static_cast<int8>((deltaADCold[0] > 0.0F) ? (deltaADCold[0] + 0.5F) : (deltaADCold[0] - 0.5F));
    set.chan[ChA].balanceShiftADC = shiftADC0;
    shiftADC1 = static_cast<int8>((deltaADCold[1] > 0.0F) ? (deltaADCold[1] + 0.5F) : (deltaADCold[1] - 0.5F));
    set.chan[ChB].balanceShiftADC = shiftADC1;
//    HAL_FMC::Write(WR_ADD_RSHIFT_DAC1, (uint8)SET_BALANCE_ADC_A);
//    HAL_FMC::Write(WR_ADD_RSHIFT_DAC2, (uint8)SET_BALANCE_ADC_B);
}


int16 CalculateAdditionRShift(const Channel &ch, Range::E range)
{
    Range::Set(ch, range);
    RShift::Set(ch, RShiftZero);
    TBase::Set(TBase::_200us);
    TrigSource::Set(ch == ChA ? TrigSource::A : TrigSource::B);
    TrigPolarity::Set(TrigPolarity::Front);
    TrigLev::Set((TrigSource::E)ch.value, TrigLevZero);

    FPGA::BUS::WriteWithoutStart(WR_UPR, BIN_U8(00000000));   // Устанавливаем выход калибратора в ноль

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
        if(TIME_MS - startTime > timeWait)                 // Если прошло слишком много времени - 
        {
            return ERROR_VALUE_INT16;                       // выход с ошибкой.
        }

        TrigPolarity::Switch();

        startTime = TIME_MS;

        while(_GET_BIT(HAL_FMC::Read(RD_FL), 0) == 0 && (TIME_MS - startTime < timeWait)) {};
        if(TIME_MS - startTime > timeWait)                 // Если прошло слишком много времени - 
        {
            return ERROR_VALUE_INT16;                       // выход с ошибкой.
        }

        HAL_FMC::Write(WR_STOP, 1);

        uint16 *addressRead1 = (ch == ChA) ? RD_ADC_A : RD_ADC_B;
        uint16 *addressRead2 = (ch == ChA) ? RD_ADC_A : RD_ADC_B;

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


float CalculateKoeffCalibration(const Channel &ch)
{
    FPGA::BUS::WriteWithoutStart(WR_UPR, BIN_U8(00000100));

    RShift::Set(ch, RShiftZero - 40 * 4);
    ModeCouple::Set(ch, ModeCouple::DC);
    TrigSource::Set((TrigSource::E)ch.value);
    TrigLev::Set((TrigSource::E)ch.value, TrigLevZero + 40 * 4);
    
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

        TrigPolarity::Switch();
        startTime = TIME_MS;

        while(_GET_BIT(HAL_FMC::Read(RD_FL), 0) == 0 && (TIME_MS - startTime > timeWait)) {};
        if(TIME_MS - startTime > timeWait)
        {
            return ERROR_VALUE_FLOAT;
        }

        HAL_FMC::Write(WR_STOP, 1);

        uint16 *addressRead1 = (ch == ChA) ? RD_ADC_A : RD_ADC_B;
        uint16 *addressRead2 = (ch == ChA) ? RD_ADC_A : RD_ADC_B;

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
