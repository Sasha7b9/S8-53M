#include "defines.h"
#include "Globals.h"
#include "Settings/SettingsTypes.h"
#include "Settings/Settings.h"
#include "Symbols.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "common/Display/Primitives.h"
#include "common/Display/Font/Font.h"
#include "Colors.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/Storage.h"
#include "Hardware/Hardware.h"
#include "Hardware/Sound.h"
#include "common/Hardware/HAL/HAL.h"
#include "VCP/VCP.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageCursors.h"
#include "Menu/Pages/PageMemory.h"
#include "Menu/Pages/PageHelp.h"
#include "Utils/Measures.h"
#include "Utils/Math.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/ProcessingSignal.h"
#include "Hardware/Timer.h"
#include "common/Log.h"
#include "PainterMem.h"
#include <cmath>
#include <limits>
#include <cstring>
#include <cstdio>
#include "Display/Grid.h"


using namespace Primitives;


#define NUM_P2P_POINTS (FPGA_MAX_POINTS)
static uint8 dataP2P_0[NUM_P2P_POINTS];
static uint8 dataP2P_1[NUM_P2P_POINTS];
static int   lastP2Pdata = 0;
static bool  dataP2PIsEmpty = true;

#define MAX_NUM_STRINGS         35
#define SIZE_BUFFER_FOR_STRINGS 2000
static char                     *strings[MAX_NUM_STRINGS] = {0};
static char                     bufferForStrings[SIZE_BUFFER_FOR_STRINGS] = {0};
static int                      lastStringForPause = -1;

#define NUM_WARNINGS            10
static const char               *warnings[NUM_WARNINGS] = {0};      // Здесь предупреждающие сообщения.
static uint                     timeWarnings[NUM_WARNINGS] = {0};   // Здесь время, когда предупреждающее сообщение поступило на экран.

static pFuncVV funcOnHand       = 0;
static pFuncVV funcAdditionDraw = 0;
static pFuncVV funcAfterDraw    = 0;


void Display::DrawStringNavigation() 
{
    char buffer[100];
    char *string = Menu::StringNavigation(buffer);
    if(string) 
    {
        int length = Font::GetLengthText(string);
        int height = 10;
        Rectangle(length + 2, height).Draw(Grid::Left(), Grid::TOP, COLOR_FILL);
        Region(length, height - 2).Fill(Grid::Left() + 1, Grid::TOP + 1, COLOR_BACK);
        Text(string).Draw(Grid::Left() + 2, Grid::TOP + 1, COLOR_FILL);
    }
}


void Display::RotateRShift(Channel::E chan)
{
    ResetP2Ppoints(true);
    LAST_AFFECTED_CHANNEL = chan;
    if(TIME_SHOW_LEVELS)
    {
        if (chan == Channel::A) { SHOW_LEVEL_RSHIFT_0 = 1; }
        else                    { SHOW_LEVEL_RSHIFT_1 = 1; }
        Timer::Enable((chan == Channel::A) ? TypeTimer::ShowLevelRShift0 : TypeTimer::ShowLevelRShift1, TIME_SHOW_LEVELS  * 1000, (chan == Channel::A) ? FuncOnTimerDisableShowLevelRShiftA :
                     FuncOnTimerDisableShowLevelRShiftB);
    };
    Display::Redraw();
};


void Display::FuncOnTimerDisableShowLevelRShiftA()
{
    DisableShowLevelRShiftA();
}


void Display::FuncOnTimerDisableShowLevelRShiftB()
{
    Display::DisableShowLevelRShiftB();
}


void Display::RotateTrigLev()
{
    if (TIME_SHOW_LEVELS)
    {
        SHOW_LEVEL_TRIGLEV = 1;
        Timer::Enable(TypeTimer::ShowLevelTrigLev, TIME_SHOW_LEVELS * 1000, FuncOnTimerDisableShowLevelTrigLev);
    }
    Display::Redraw();
}


void Display::FuncOnTimerDisableShowLevelTrigLev()
{
    Display::DisableShowLevelTrigLev();
}


void Display::Redraw(void)
{
    NEED_FINISH_REDRAW = 1;
}


bool Display::ChannelNeedForDraw(pUCHAR data, Channel::E chan, const DataSettings *ds)
{
    if (!data)
    {
        return false;
    }

    if (MODE_WORK_IS_DIRECT)
    {
        if (!sChannel_Enabled(chan))
        {
            return false;
        }
    }
    else if (ds != 0)
    {
        if ((chan == Channel::A && ds->enableCh0 == 0) || (chan == Channel::B && ds->enableCh1 == 0))
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}



void Display::DrawMarkersForMeasure(float scale, Channel::E chan)
{
    if (chan == Channel::Math)
    {
        return;
    }
    Painter::SetColor(ColorCursors(chan));
    for(int numMarker = 0; numMarker < 2; numMarker++)
    {
        int pos = Processing::GetMarkerHorizontal(chan, numMarker);
        if(pos != ERROR_VALUE_INT && pos > 0 && pos < 200)
        {
            Painter::DrawDashedHLine(Grid::FullBottom() - static_cast<int>(pos * scale), Grid::Left(), Grid::Right(), 3, 2, 0);
        }

        pos = Processing::GetMarkerVertical(chan, numMarker);
        if (pos != ERROR_VALUE_INT && pos > 0 && pos < Grid::Right())
        {
            Painter::DrawDashedVLine(Grid::Left() + static_cast<int>(pos * scale), Grid::TOP, Grid::FullBottom(), 3, 2, 0);
        }
       
    }
}



#define CONVERT_DATA_TO_DISPLAY(out, in)                \
    out = (uint8)(maxY - ((in) - MIN_VALUE) * scaleY);  \
    if(out < minY)          { out = (uint8)minY; }      \
    else if (out > maxY)    { out = (uint8)maxY; };



void Display::DrawSignalLined(pUCHAR data, const DataSettings *ds, int startPoint, int endPoint, int minY, int maxY, float scaleY, float scaleX, 
    bool calculateFiltr)
{
    if (endPoint < startPoint)
    {
        return;
    }
    uint8 dataCD[281];

	int gridLeft = Grid::Left();
	int gridRight = Grid::Right();
    
    int numPoints = sMemory_GetNumPoints(false);
    int numSmoothing = Smoothing::NumPoints();

    if (ds->peakDet == PeackDetMode::Disable)
    {
        for (int i = startPoint; i < endPoint; i++)
        {
            float x0 = gridLeft + (i - startPoint) * scaleX;
            if (x0 >= gridLeft && x0 <= gridRight)
            {
                int index = i - startPoint;
                int y = calculateFiltr ? Math_CalculateFiltr(data, i, numPoints, numSmoothing) : data[i];
                CONVERT_DATA_TO_DISPLAY(dataCD[index], y);
            }
        }
    }
    else
    {
        int shift = static_cast<int>(ds->length1channel);

        int yMinNext = -1;
        int yMaxNext = -1;
       
        for (int i = startPoint; i < endPoint; i++)
        {
            float x = gridLeft + (i - startPoint) * scaleX;
            if (x >= gridLeft && x <= gridRight)
            {
                int yMin = yMinNext;
                if (yMin == -1)
                {
                    CONVERT_DATA_TO_DISPLAY(yMin, data[i + shift]);
                }
                int yMax = yMaxNext;
                if (yMax == -1)
                {
                    CONVERT_DATA_TO_DISPLAY(yMax, data[i]);
                }

                CONVERT_DATA_TO_DISPLAY(yMaxNext, data[i + 1]);
                if (yMaxNext < yMin)
                {
                    yMin = yMaxNext + 1;
                }

                CONVERT_DATA_TO_DISPLAY(yMinNext, data[i + shift + 1]);
                if (yMinNext > yMax)
                {
                    yMax = yMinNext - 1;
                }

                HLine().Draw(static_cast<int>(x), yMin, yMax);
            }
        }
    }

    if(endPoint - startPoint < 281)
    {
        int _numPoints = 281 - (endPoint - startPoint);
        for(int i = 0; i < _numPoints; i++)
        {
            int index = endPoint - startPoint + i;
            CONVERT_DATA_TO_DISPLAY(dataCD[index], MIN_VALUE);
        }
    }

    if(ds->peakDet == PeackDetMode::Disable)
    {
        CONVERT_DATA_TO_DISPLAY(dataCD[280], data[endPoint]);
        Painter::DrawSignal(Grid::Left(), dataCD, true);
    }
}



void Display::DrawSignalPointed(pUCHAR data, const DataSettings *ds, int startPoint, int endPoint, int minY, int maxY, float scaleY, float scaleX)
{
    int numPoints = sMemory_GetNumPoints(false);
    int numSmoothing = Smoothing::NumPoints();
    
    uint8 dataCD[281];

    if (scaleX == 1.0F) //-V550 //-V2550
    {
        for (int i = startPoint; i < endPoint; i++)
        {
            int index = i - startPoint;
            CONVERT_DATA_TO_DISPLAY(dataCD[index], Math_CalculateFiltr(data, i, numPoints, numSmoothing));
        }
        Painter::DrawSignal(Grid::Left(), dataCD, false);

        if (ds->peakDet)
        {
            int size = endPoint - startPoint;
            startPoint += numPoints;
            endPoint = startPoint + size;;

            for (int i = startPoint; i < endPoint; i++)
            {
                int index = i - startPoint;
                CONVERT_DATA_TO_DISPLAY(dataCD[index], Math_CalculateFiltr(data, i, numPoints, numSmoothing));
            }
            Painter::DrawSignal(Grid::Left(), dataCD, false);
        }
    }
    else
    {
        for (int i = startPoint; i < endPoint; i++)
        {
            int index = i - startPoint;
            int dat = 0;
            CONVERT_DATA_TO_DISPLAY(dat, Math_CalculateFiltr(data, i, numPoints, numSmoothing));
            Point().Draw(Grid::Left() + static_cast<int>(index * scaleX), dat);
        }
    }
}



// Если data == 0, то данные брать из GetData
void Display::DrawDataChannel(uint8 *data, Channel::E chan, DataSettings *ds, int minY, int maxY)
{
    bool calculateFiltr = true;
    if (data == 0)
    {
        calculateFiltr = false;
        if (chan == Channel::A)
        {
            Processing::GetData(&data, 0, &ds);
        }
        else
        {
            Processing::GetData(0, &data, &ds);
        }
    }

    if (!ChannelNeedForDraw(data, chan, ds))
    {
        return;
    }

    float scaleY = static_cast<float>(maxY - minY) / (MAX_VALUE - MIN_VALUE);
    float scaleX = Grid::Width() / 280.0F;

    if(SHOW_MEASURES)
    {
        DrawMarkersForMeasure(scaleY, chan);
    }

    int firstPoint = 0;
    int lastPoint = 0;
    SettingsDisplay::PointsOnDisplay(&firstPoint, &lastPoint);
    if(data == dataP2P_0 || data == dataP2P_1)
    {
        if(SET_SELFRECORDER)
        {
            LOG_TRACE
        }
        else if(lastPoint > lastP2Pdata)
        {
            lastPoint = lastP2Pdata;
        }
    }

    Painter::SetColor(ColorChannel(chan));
    if(MODE_DRAW_IS_SIGNAL_LINES)
    {
        /*
        if (set.display.numAveraging > ENumAveraging::_1)
        {
            Painter::SetColor(ColorGrid());
            DrawSignalLined(DS_GetData(chan, 0), ds, firstPoint, lastPoint, minY, maxY, scaleY, scaleX, calculateFiltr);    // WARN
        }
        Painter::SetColor(ColorChannel(chan));
        */
        DrawSignalLined(data, ds, firstPoint, lastPoint, minY, maxY, scaleY, scaleX, calculateFiltr);
    }
    else
    {
        DrawSignalPointed(data, ds, firstPoint, lastPoint, minY, maxY, scaleY, scaleX);
    }
}


void Display::DrawMath()
{
    if (DISABLED_DRAW_MATH || Storage::GetData(Channel::A, 0) == 0 || Storage::GetData(Channel::B, 0) == 0)
    {
        return;
    }

    uint8 *dataRel0 = 0;
    uint8 *dataRel1 = 0;
    DataSettings *ds = 0;
    Processing::GetData(&dataRel0, &dataRel1, &ds);

    float dataAbs0[FPGA_MAX_POINTS];
    float dataAbs1[FPGA_MAX_POINTS];

    Math_PointsRelToVoltage(dataRel0, static_cast<int>(ds->length1channel), ds->range[Channel::A], static_cast<int16>(ds->rShiftCh0), dataAbs0);
    Math_PointsRelToVoltage(dataRel1, static_cast<int>(ds->length1channel), ds->range[Channel::B], static_cast<int16>(ds->rShiftCh1), dataAbs1);

    Math_CalculateMathFunction(dataAbs0, dataAbs1, static_cast<int>(ds->length1channel));
    
    uint8 points[FPGA_MAX_POINTS];
    Math_PointsVoltageToRel(dataAbs0, static_cast<int>(ds->length1channel), SET_RANGE_MATH, SET_RSHIFT_MATH, points);

    DrawDataChannel(points, Channel::Math, ds, Grid::MathTop(), Grid::MathBottom());

    static const int w = 71;
    static const int h = 10;
    int delta = (SHOW_STRING_NAVIGATION && MODE_DRAW_MATH_IS_TOGETHER) ? 10 : 0;
    Rectangle(w, h).Draw(Grid::Left(), Grid::MathTop() + delta, COLOR_FILL);
    Region(w - 2, h - 2).Fill(Grid::Left() + 1, Grid::MathTop() + 1 + delta, COLOR_BACK);
    Divider::E multiplier = MATH_MULTIPLIER;
    Text(sChannel_Range2String(SET_RANGE_MATH, multiplier)).Draw(Grid::Left() + 2, Grid::MathTop() + 1 + delta, COLOR_FILL);
    Text(":").Draw(Grid::Left() + 25, Grid::MathTop() + 1 + delta);
    char buffer[20];
    Text(sChannel_RShift2String(SET_RSHIFT_MATH, SET_RANGE_MATH, multiplier, buffer)).Draw(Grid::Left() + 27, Grid::MathTop() + 1 + delta);
}



void Display::DrawSpectrumChannel(const float *spectrum, Color::E color)
{
    Painter::SetColor(color);
	int gridLeft = Grid::Left();
	int gridBottom = Grid::MathBottom();
	int gridHeight = Grid::MathHeight();
    for (int i = 0; i < 256; i++) 
    {
        HLine().Draw(gridLeft + i, gridBottom, gridBottom - static_cast<int>(gridHeight * spectrum[i]));
    }
}



void Display::WriteParametersFFT(Channel::E chan, float freq0, float density0, float freq1, float density1)
{
    int x = Grid::Left() + 259;
    int y = Grid::ChannelBottom() + 5;
    int dY = 10;

    char buffer[20];
    Painter::SetColor(COLOR_FILL);
    Text(Freq2String(freq0, false, buffer)).Draw(x, y);
    y += dY;
    Text(Freq2String(freq1, false, buffer)).Draw(x, y);
    if (chan == Channel::A)
    {
        y += dY + 2;
    }
    else
    {
        y += dY * 3 + 4;
    }
    Painter::SetColor(ColorChannel(chan));
    Text(SCALE_FFT_IS_LOG ? Float2Db(density0, 4, buffer) : Float2String(density0, false, 7, buffer)).Draw(x, y);
    y += dY;
    Text(SCALE_FFT_IS_LOG ? Float2Db(density1, 4, buffer) : Float2String(density1, false, 7, buffer)).Draw(x, y);
}



void Display::DRAW_SPECTRUM(pUCHAR data, int numPoints, Channel::E channel)
{
    if (!sChannel_Enabled(channel))
    {
        return;
    }
    float dataR[FPGA_MAX_POINTS];
    float spectrum[FPGA_MAX_POINTS];

    float freq0 = 0.0F;
    float freq1 = 0.0F;
    float density0 = 0.0F;
    float density1 = 0.0F;
    int y0 = 0;
    int y1 = 0;

    Math_PointsRelToVoltage(data, numPoints, gDSet->range[channel], channel == Channel::A ? static_cast<int16>(gDSet->rShiftCh0) : static_cast<int16>(gDSet->rShiftCh1), dataR);
    Math_CalculateFFT(dataR, numPoints, spectrum, &freq0, &density0, &freq1, &density1, &y0, &y1);
    DrawSpectrumChannel(spectrum, ColorChannel(channel));
    if (!MenuIsShown() || MenuIsMinimize())
    {
        int s = 2;

        Color::E color = COLOR_FILL;
        WriteParametersFFT(channel, freq0, density0, freq1, density1);
        Rectangle(s * 2, s * 2).Draw(FFT_POS_CURSOR_0 + Grid::Left() - s, y0 - s, color);
        Rectangle(s * 2, s * 2).Draw(FFT_POS_CURSOR_1 + Grid::Left() - s, y1 - s);

        HLine().Draw(Grid::Left() + FFT_POS_CURSOR_0, Grid::MathBottom(), y0 + s);
        HLine().Draw(Grid::Left() + FFT_POS_CURSOR_1, Grid::MathBottom(), y1 + s);
    }
}


void Display::DrawSpectrum()
{
    if (!ENABLED_FFT)
    {
        return;
    }

    HLine().Draw(Grid::Right(), Grid::ChannelBottom() + 1, Grid::MathBottom() - 1, COLOR_BACK);

    if (MODE_WORK_IS_DIRECT)
    {
        int numPoints = sMemory_GetNumPoints(false);
        if (numPoints < 512)
        {
            numPoints = 256;
        }

        if (SOURCE_FFT_IS_A)
        {
            DRAW_SPECTRUM(gData0, numPoints, Channel::A);
        }
        else if (SOURCE_FFT_IS_B)
        {
            DRAW_SPECTRUM(gData1, numPoints, Channel::B);
        }
        else
        {
            if (LAST_AFFECTED_CHANNEL_IS_A)
            {
                DRAW_SPECTRUM(gData1, numPoints, Channel::B);
                DRAW_SPECTRUM(gData0, numPoints, Channel::A);
            }
            else
            {
                DRAW_SPECTRUM(gData0, numPoints, Channel::A);
                DRAW_SPECTRUM(gData1, numPoints, Channel::B);
            }
        }
    }

    HLine().Draw(Grid::ChannelBottom(), Grid::Left(), Grid::Right(), COLOR_FILL);
    HLine().Draw(Grid::MathBottom(), Grid::Left(), Grid::Right());
}



void Display::DrawBothChannels(uint8 *data0, uint8 *data1)
{
	if (LAST_AFFECTED_CHANNEL_IS_B)
    {
        DrawDataChannel(data0, Channel::A, gDSet, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(data1, Channel::B, gDSet, Grid::TOP, Grid::ChannelBottom());
    }
    else
    {
        DrawDataChannel(data1, Channel::B, gDSet, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(data0, Channel::A, gDSet, Grid::TOP, Grid::ChannelBottom());
    }
}



void Display::DrawDataMemInt()
{
    if(gDSmemInt != 0)
     {
        DrawDataChannel(gData0memInt, Channel::A, gDSmemInt, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(gData1memInt, Channel::B, gDSmemInt, Grid::TOP, Grid::ChannelBottom());
    }
}



void Display::DrawDataInModeWorkLatest()
{
    if (gDSmemLast != 0)
    {
        DrawDataChannel(gData0memLast, Channel::A, gDSmemLast, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(gData1memLast, Channel::B, gDSmemLast, Grid::TOP, Grid::ChannelBottom());
    }
}



void Display::DrawDataInModePoint2Point()
{
    uint8 *data0 = 0;
    uint8 *data1 = 0;
    DataSettings *ds = 0;
    Processing::GetData(&data0, &data1, &ds);

    if (LAST_AFFECTED_CHANNEL_IS_B)
    {
        if (SET_SELFRECORDER || (Storage::NumElementsWithCurrentSettings() == 0))
        {
            DrawDataChannel(dataP2P_0, Channel::A, ds, Grid::TOP, Grid::ChannelBottom());
            DrawDataChannel(dataP2P_1, Channel::B, ds, Grid::TOP, Grid::ChannelBottom());
        }
        else
        {
            DrawDataChannel(data0, Channel::A, ds, Grid::TOP, Grid::ChannelBottom());
            DrawDataChannel(data1, Channel::B, ds, Grid::TOP, Grid::ChannelBottom());
        }
    }
    else
    {
        if (SET_SELFRECORDER || (Storage::NumElementsWithCurrentSettings() == 0))
        {
            DrawDataChannel(dataP2P_1, Channel::B, ds, Grid::TOP, Grid::ChannelBottom());
            DrawDataChannel(dataP2P_0, Channel::A, ds, Grid::TOP, Grid::ChannelBottom());
        }
        else
        {
            DrawDataChannel(data1, Channel::B, ds, Grid::TOP, Grid::ChannelBottom());
            DrawDataChannel(data0, Channel::A, ds, Grid::TOP, Grid::ChannelBottom());
        }
    }
}



void Display::DrawDataInModeSelfRecorder()
{
    LOG_TRACE
}



void Display::DrawDataInModeNormal()
{
    static void* prevAddr = 0;

    uint8 *data0 = 0;
    uint8 *data1 = 0;
    DataSettings *ds = 0;
    Processing::GetData(&data0, &data1, &ds);

    int16 numSignals = (int16)Storage::NumElementsWithSameSettings();
    LIMITATION(numSignals, numSignals, 1, NUM_ACCUM);
    if (numSignals == 1 || ENUM_ACCUM_IS_INFINITY || MODE_ACCUM_IS_RESET || sTime_RandomizeModeEnabled())
    {
        DrawBothChannels(0, 0);
        if (prevAddr == 0 || prevAddr != ds->addrPrev)
        {
            NUM_DRAWING_SIGNALS++;
            prevAddr = ds->addrPrev;
        }
    }
    else
    {
        for (int i = 0; i < numSignals; i++)
        {
            DrawBothChannels(Storage::GetData(Channel::A, i), Storage::GetData(Channel::B, i));
        }
    }
}



void Display::DrawDataMinMax()
{
    ModeDrawSignal::E modeDrawSignalOld = MODE_DRAW_SIGNAL;
    MODE_DRAW_SIGNAL = ModeDrawSignal::Lines;
    if (LAST_AFFECTED_CHANNEL_IS_B)
    {
        DrawDataChannel(Storage::GetLimitation(Channel::A, 0), Channel::A, gDSet, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(Channel::A, 1), Channel::A, gDSet, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(Channel::B, 0), Channel::B, gDSet, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(Channel::B, 1), Channel::B, gDSet, Grid::TOP, Grid::ChannelBottom());
    }
    else
    {
        DrawDataChannel(Storage::GetLimitation(Channel::B, 0), Channel::B, gDSet, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(Channel::B, 1), Channel::B, gDSet, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(Channel::A, 0), Channel::A, gDSet, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(Channel::A, 1), Channel::A, gDSet, Grid::TOP, Grid::ChannelBottom());
    }
    MODE_DRAW_SIGNAL = modeDrawSignalOld;
}



void Display::DrawDataNormal()
{
    if (!dataP2PIsEmpty)
    {
        static const pFuncVV funcs[2] = {DrawDataInModePoint2Point, DrawDataInModeSelfRecorder};
        funcs[(int)SET_SELFRECORDER]();
    }
    else
    {
        DrawDataInModeNormal();
    }
}



void Display::DrawData()
{
    if (Storage::AllDatas())
    {

        if (MODE_WORK_IS_MEMINT)
        {
            if (!MODE_SHOW_MEMINT_IS_DIRECT)
            {
                DrawDataMemInt();
            }
            if (!MODE_SHOW_MEMINT_IS_SAVED)
            {
                DrawDataNormal();
            }
        }
        else if (MODE_WORK_IS_LATEST)
        {
            DrawDataInModeWorkLatest();
        }
        else
        {
            if (INT_SHOW_ALWAYS)
            {
                DrawDataMemInt();
            }
            DrawDataNormal();
        }

        if (NUM_MIN_MAX != 1)
        {
            DrawDataMinMax();
        }
    }

    Rectangle(Grid::Width(), Grid::FullHeight()).Draw(Grid::Left(), Grid::TOP, COLOR_FILL);
}


void Display::DrawTime(int x, int y)
{
    int dField = 10;
    int dSeparator = 2;

    PackedTime time = HAL_RTC::GetPackedTime();
    
    char buffer[20];
    
    Painter::SetColor(COLOR_FILL);
    
    if (MODE_WORK_IS_MEMINT || MODE_WORK_IS_LATEST)
    {
        DataSettings *ds = MODE_WORK_IS_MEMINT ? gDSmemInt : gDSmemLast;

        if (ds != 0)
        {
            y -= 9;
            time.day = ds->time.day;
            time.hours = ds->time.hours;
            time.minutes = ds->time.minutes;
            time.seconds = ds->time.seconds;
            time.month = ds->time.month;
            time.year = ds->time.year;
            Text(Int2String((int)time.day, false, 2, buffer)).Draw(x, y);
            Text(":").Draw(x + dField, y);
            Text(Int2String((int)time.month, false, 2, buffer)).Draw(x + dField + dSeparator, y);
            Text(":").Draw(x + 2 * dField + dSeparator, y);
            Text(Int2String((int)time.year + 2000, false, 4, buffer)).Draw(x + 2 * dField + 2 * dSeparator, y);
            y += 9;
        }
        else
        {
            return;
        }
    }
    
    
    Text(Int2String((int)time.hours, false, 2, buffer)).Draw(x, y);
    Text(":").Draw(x + dField, y);
    Text(Int2String((int)time.minutes, false, 2, buffer)).Draw(x + dField + dSeparator, y);
    Text(":").Draw(x + 2 * dField + dSeparator, y);
    Text(Int2String((int)time.seconds, false, 2, buffer)).Draw(x + 2 * dField + 2 * dSeparator, y);
}


void Display::DrawHiPart()
{
    WriteCursors();
    DrawHiRightPart();
}


// shiftForPeakDet - если рисуем информацию с пикового детектора - то через shiftForPeakDet точек расположена иниформация о максимумах.
void Display::DrawDataInRect(int x, int width, pUCHAR data, int numElems, Channel::E chan, int shiftForPeakDet)
{
    if(numElems == 0)
    {
        return;
    }

    width--;
    float elemsInColumn = (float)numElems / (float)width;
    uint8 min[300];
    uint8 max[300];


    if ((SET_TBASE >= TBase::_20ms) && (PEAKDET != PeackDetMode::Disable))
    {
        for (int col = 0; col < width; col++)
        {
            float firstElem = col * elemsInColumn;
            float lastElem = firstElem + elemsInColumn - 1;
            min[col] = data[(int)firstElem];
            max[col] = data[(int)firstElem + 1];
            for (int elem = static_cast<int>(firstElem) + 2; elem <= lastElem; elem += 2)
            {
                SET_MIN_IF_LESS(data[elem], min[col]);
                SET_MAX_IF_LARGER(data[elem + 1], max[col]);
            }
        }
    }
    else if (shiftForPeakDet == 0)
    {
        uint8 *iMin = &min[0];
        uint8 *iMax = &max[0];

        for (int col = 0; col < width; col++, iMin++, iMax++) //-V2528
        {
            int firstElem = static_cast<int>(col * elemsInColumn);
            int lastElem = firstElem + static_cast<int>(elemsInColumn) - 1;
            *iMin = data[firstElem];
            *iMax = data[firstElem];
            for (int elem = firstElem + 1; elem <= lastElem; elem++)
            {
                SET_MIN_IF_LESS(data[elem], *iMin);
                SET_MAX_IF_LARGER(data[elem], *iMax);
            }
        }
    }
    else        // А здесь будет, если пик. дет. включен
    {
        for (int col = 0; col < width; col++)
        {
            float firstElem = col * elemsInColumn;
            float lastElem = firstElem + elemsInColumn - 1;
            min[col] = data[(int)firstElem];
            max[col] = data[(int)firstElem + shiftForPeakDet];
            for (int elem = static_cast<int>(firstElem) + 1; elem <= lastElem; elem++)
            {
                SET_MIN_IF_LESS(data[elem], min[col]);
                SET_MAX_IF_LARGER(data[elem + shiftForPeakDet], max[col]);
            }
        }
    }

    int bottom = 16;
    int height = 14;
    float scale = (float)height / (float)(MAX_VALUE - MIN_VALUE);

#define ORDINATE(x) bottom - scale * LimitationInt((x) - MIN_VALUE, 0, 200)

#define NUM_POINTS (300 * 2)
    uint8 points[NUM_POINTS];

    points[0] = static_cast<uint8>(ORDINATE(max[0]));
    points[1] = static_cast<uint8>(ORDINATE(min[0]));



    for(int i = 1; i < width; i++)
    {
        int value0 = min[i] > max[i - 1] ? max[i - 1] : min[i];
        int value1 = max[i] < min[i - 1] ? min[i - 1] : max[i];
        points[i * 2] = static_cast<uint8>(ORDINATE(value1));
        points[i * 2 + 1] = static_cast<uint8>(ORDINATE(value0));
    }
	if(width < 256)
    {
		Painter::DrawVLineArray(x, width, points, ColorChannel(chan));
	}
    else
    {
		Painter::DrawVLineArray(x, 255, points, ColorChannel(chan));
		Painter::DrawVLineArray(x + 255, width - 255, points + 255 * 2, ColorChannel(chan));
	}
}



// shiftForPeakDet - если рисуем информацию с пикового детектора - то через shiftForPeakDet точек расположена иниформация о максимумах.
void Display::DrawChannelInWindowMemory(int timeWindowRectWidth, int xVert0, int xVert1, int startI, int endI, pUCHAR data, int rightX, Channel::E chan, int shiftForPeakDet)
{
    if(data == dataP2P_0 && data == dataP2P_1)
    {

    }
    else
    {
        DrawDataInRect(1,          xVert0 - 1,              &(data[0]),        startI,                             chan, shiftForPeakDet);
        DrawDataInRect(xVert0 + 2, timeWindowRectWidth - 2, &(data[startI]),   281,                                chan, shiftForPeakDet);
        DrawDataInRect(xVert1 + 2, rightX - xVert1 + 2,     &(data[endI + 1]), sMemory_GetNumPoints(false) - endI, chan, shiftForPeakDet);
    }
}



void Display::DrawMemoryWindow()
{
    uint8 *dat0 = gData0memInt;
    uint8 *dat1 = gData1memInt;
    DataSettings *ds = gDSmemInt;
    
    if(MODE_WORK_IS_DIRECT || MODE_WORK_IS_LATEST)
    {
        dat0 = gData0;
        dat1 = gData1;
        ds = gDSet;
    }

    if(ds == nullptr)
    {
        return;
    }
    
    int leftX = 3;
    int top = 1;
    int height = Grid::TOP - 3;
    int bottom = top + height;

    static const int rightXses[3] = {276, 285, 247};
    int rightX = rightXses[MODE_WORK];
    if (sCursors_NecessaryDrawCursors())
    {
        rightX = 68;
    }

    int timeWindowRectWidth = static_cast<int>((rightX - leftX) * (282.0F / sMemory_GetNumPoints(false)));
    float scaleX = (float)(rightX - leftX + 1) / sMemory_GetNumPoints(false);

    int16 shiftInMemory = SHIFT_IN_MEMORY;
    
    const int xVert0 = static_cast<int>(leftX + shiftInMemory * scaleX);
    const int xVert1 = leftX + static_cast<int>(shiftInMemory * scaleX) + timeWindowRectWidth;
    bool showFull = set.display.showFullMemoryWindow;
    Rectangle(xVert1 - xVert0, bottom - top - (showFull ? 0 : 2)).Draw(xVert0, top + (showFull ? 0 : 1), COLOR_FILL);

    if(!dataP2PIsEmpty)
    {
        dat0 = dataP2P_0;
        dat1 = dataP2P_1;
    }

    if (showFull)
    {
        if (gData0 || gData1 || (!dataP2PIsEmpty))
        {
            int startI = shiftInMemory;
            int endI = startI + 281;

            Channel::E chanFirst = LAST_AFFECTED_CHANNEL_IS_A ? Channel::B : Channel::A;
            Channel::E chanSecond = LAST_AFFECTED_CHANNEL_IS_A ? Channel::A : Channel::B;
            pUCHAR dataFirst = LAST_AFFECTED_CHANNEL_IS_A ? dat1 : dat0;
            pUCHAR dataSecond = LAST_AFFECTED_CHANNEL_IS_A ? dat0 : dat1;

            int shiftForPeakDet = ds->peakDet == PeackDetMode::Disable ? 0 : (int)ds->length1channel;

            if (ChannelNeedForDraw(dataFirst, chanFirst, ds))
            {
                DrawChannelInWindowMemory(timeWindowRectWidth, xVert0, xVert1, startI, endI, dataFirst, rightX, chanFirst, shiftForPeakDet);
            }
            if (ChannelNeedForDraw(dataSecond, chanSecond, ds))
            {
                DrawChannelInWindowMemory(timeWindowRectWidth, xVert0, xVert1, startI, endI, dataSecond, rightX, chanSecond, shiftForPeakDet);
            }
        }
    }
    else
    {
        HLine().Draw(leftX - 2, top, bottom, COLOR_FILL);
        HLine().Draw(rightX + 2, top, bottom);
        HLine().Draw((bottom + top) / 2 - 3, leftX, xVert0 - 2);
        HLine().Draw((bottom + top) / 2 + 3, leftX, xVert0 - 2);
        HLine().Draw((bottom + top) / 2 + 3, xVert1 + 2, rightX);
        HLine().Draw((bottom + top) / 2 - 3, xVert1 + 2, rightX);
    }

    int x[] = {leftX, (rightX - leftX) / 2 + leftX + 1, rightX};
    int x0 = x[SET_TPOS];

    // Маркер TPos
    Region(6, 6).Fill(x0 - 3, 9, COLOR_BACK);
    Painter::DrawCharC(x0 - 3, 9, SYMBOL_TPOS_1, COLOR_FILL);

    // Маркер tShift
    float scale = (float)(rightX - leftX + 1) / ((float)sMemory_GetNumPoints(false) - (sMemory_GetNumPoints(false) == 281 ? 1 : 0));
    float xShift = 1 + (sTime_TPosInPoints((PeackDetMode::E)gDSet->peakDet, (int)gDSet->length1channel, SET_TPOS) - sTime_TShiftInPoints((PeackDetMode::E)gDSet->peakDet)) * scale;
    
    if(xShift < leftX - 2)
    {
        xShift = static_cast<float>(leftX - 2);
    }

    Region(6, 6).Fill(static_cast<int>(xShift - 1), 3, COLOR_BACK);
    Region(4, 4).Fill(static_cast<int>(xShift), 4, COLOR_FILL);
    Painter::SetColor(COLOR_BACK);

    if(xShift == leftX - 2) //-V2550 //-V550
    {
        xShift = static_cast<float>(leftX - 2);
        Painter::DrawLine(static_cast<int>(xShift) + 3, 5, static_cast<int>(xShift) + 3, 7);
        Painter::DrawLine(static_cast<int>(xShift) + 1, 6, static_cast<int>(xShift) + 2, 6);
    }
    else if(xShift > rightX - 1)
    {
        xShift = static_cast<float>(rightX - 2);
        Painter::DrawLine(static_cast<int>(xShift) + 1, 5, static_cast<int>(xShift) + 1, 7);
        Painter::DrawLine(static_cast<int>(xShift) + 2, 6, static_cast<int>(xShift) + 3, 6);
    }
    else
    {
        Painter::DrawLine(static_cast<int>(xShift) + 1, 5, static_cast<int>(xShift) + 3, 5);
        Painter::DrawLine(static_cast<int>(xShift) + 2, 6, static_cast<int>(xShift) + 2, 7);
    }
}


void Display::WriteCursors()
{
    char buffer[20];
    int startX = 43;
    if(MODE_WORK_IS_DIRECT)
    {
        startX += 29;
    }

    if(sCursors_NecessaryDrawCursors())
    {
        int x = startX;
        int y1 = 0;
        int y2 = 9;

        HLine().Draw(x, 1, Grid::TOP - 2, COLOR_FILL);
        x += 3;
        Channel::E source = CURS_SOURCE;
        Color::E colorText = ColorChannel(source);
        if(!CURS_CNTRL_U_IS_DISABLE(source))
        {
            Text("1:").Draw(x, y1, colorText);
            Text("2:").Draw(x, y2);
            x += 7;
            Text(sCursors_GetCursVoltage(source, 0, buffer)).Draw(x, y1);
            Text(sCursors_GetCursVoltage(source, 1, buffer)).Draw(x, y2);
            x = startX + 49;
            float pos0 = Math_VoltageCursor(sCursors_GetCursPosU(source, 0), SET_RANGE(source), SET_RSHIFT(source));
            float pos1 = Math_VoltageCursor(sCursors_GetCursPosU(source, 1), SET_RANGE(source), SET_RSHIFT(source));
            float delta = std::fabsf(pos1 - pos0);
            Text(":dU=").Draw(x, y1);
            Text(Voltage2String(delta, false, buffer)).Draw(x + 17, y1);
            Text(":").Draw(x, y2);
            Text(sCursors_GetCursorPercentsU(source, buffer)).Draw(x + 10, y2);
        }

        x = startX + 101;
        HLine().Draw(x, 1, Grid::TOP - 2, COLOR_FILL);
        x += 3;
        if(!CURS_CNTRL_T_IS_DISABLE(source))
        {
            Painter::SetColor(colorText);
            Text("1:").Draw(x, y1);
            Text("2:").Draw(x, y2);
            x+=7;
            Text(sCursors_GetCursorTime(source, 0, buffer)).Draw(x, y1);
            Text(sCursors_GetCursorTime(source, 1, buffer)).Draw(x, y2);
            x = startX + 153;
            float pos0 = Math_TimeCursor(CURS_POS_T0(source), SET_TBASE);
            float pos1 = Math_TimeCursor(CURS_POS_T1(source), SET_TBASE);
            float delta = std::fabsf(pos1 - pos0);
            Text(":dT=").Draw(x, y1);
            char buf[20];
            Text(Time2String(delta, false, buf)).Draw(x + 17, y1);
            Text(":").Draw(x, y2);
            Text(sCursors_GetCursorPercentsT(source, buf)).Draw(x + 8, y2);

            if(CURSORS_SHOW_FREQ)
            {
                int width = 65;
                int x0 = Grid::Right() - width;
                Rectangle(width, 12).Draw(x0, Grid::TOP, COLOR_FILL);
                Region(width - 2, 10).Fill(x0 + 1, Grid::TOP + 1, COLOR_BACK);
                Text("1/dT=").Draw(x0 + 1, Grid::TOP + 2, colorText);
                char buff[20];
                Text(Freq2String(1.0F / delta, false, buff)).Draw(x0 + 25, Grid::TOP + 2);
            }
        }
    }
}


void Display::DrawHiRightPart()
    {
    // Синхроимпульс
    int y = 2;

    static const int xses[3] = {280, 271, 251};
    int x = xses[MODE_WORK];

    if (!MODE_WORK_IS_LATEST)
    {
        HLine().Draw(x, 1, Grid::TOP - 2, COLOR_FILL);

        x += 2;

        if (TRIG_ENABLE)
        {
            Region(Grid::TOP - 3, Grid::TOP - 7).Fill(x, 1 + y);
            Text(set.common.lang == Language::Russian ? "СИ" : "Tr").Draw(x + 3, 3 + y, COLOR_BACK);
        }
    }

    // Режим работы
    static const char *strings_[][2] =
    {
        {"ИЗМ",     "MEAS"},
        {"ПОСЛ",    "LAST"},
        {"ВНТР",    "INT"}
    };

    if(!MODE_WORK_IS_DIRECT)
    {
        x += 18;
        HLine().Draw(x, 1, Grid::TOP - 2, COLOR_FILL);
        x += 2;
        Text(set.common.lang == Language::Russian ? "режим" : "mode").Draw(set.common.lang == Language::Russian ? x : x + 3, -1);
        Painter::DrawStringInCenterRect(x + 1, 9, 25, 8, strings_[MODE_WORK][set.common.lang]);
    }
    else
    {
        x -= 9;
    }

    if (!MODE_WORK_IS_LATEST)
    {

        x += 27;
        HLine().Draw(x, 1, Grid::TOP - 2, COLOR_FILL);

        x += 2;
        y = 1;
        if (FPGA::CurrentStateWork() == StateWorkFPGA::Work)
        {
            Painter::Draw4SymbolsInRect(x, 1, SYMBOL_PLAY);
        }
        else if (FPGA::CurrentStateWork() == StateWorkFPGA::Stop)
        {
            Region(10, 10).Fill(x + 3, y + 3);
        }
        else if (FPGA::CurrentStateWork() == StateWorkFPGA::Wait)
        {
            int w = 4;
            int h = 14;
            int delta = 4;
            x = x + 2;
            Region(w, h).Fill(x, y + 1);
            Region(w, h).Fill(x + w + delta, y + 1);
        }
    }
}



void Display::DrawCursorsRShift()
{
    if (!DISABLED_DRAW_MATH)
    {
        DrawCursorRShift(Channel::Math);
    }
    if(LAST_AFFECTED_CHANNEL_IS_B)
    {
        DrawCursorRShift(Channel::A);
        DrawCursorRShift(Channel::B);
    }
    else
    {
        DrawCursorRShift(Channel::B);
        DrawCursorRShift(Channel::A);
    }
}



bool Display::NeedForClearScreen()
{
    int numAccum = NUM_ACCUM;

    if (sTime_RandomizeModeEnabled() || numAccum == 1 || MODE_ACCUM_IS_NORESET || SET_SELFRECORDER)
    {
        return true;
    }

    if (NEED_FINISH_REDRAW)
    {
        NEED_FINISH_REDRAW = 0;
        return true;
    }

    if (MODE_ACCUM_IS_RESET && (NUM_DRAWING_SIGNALS >= static_cast<uint>(numAccum))) //-V560
    {
        NUM_DRAWING_SIGNALS = 0;
        return true;
    }
    return false;
}


void Display::Update(bool endScene)
{
	uint timeStart = gTimerTics;
    if (funcOnHand != 0)
    {
        funcOnHand();
        return;
    }

    bool needClear = NeedForClearScreen();

    if (needClear)
    {
        Painter::BeginScene(COLOR_BACK);
        DrawMemoryWindow();
        DrawFullGrid();
    }

    DrawData();

    if (needClear)
    {
        DrawMath();
        DrawSpectrum();
        DrawCursors();
        DrawHiPart();
        DrawLowPart();
        DrawCursorsWindow();
        DrawCursorTrigLevel();
        DrawCursorsRShift();
        DrawMeasures();
        DrawStringNavigation();
        DrawCursorTShift();
    }
    
    Menu::Draw();

    if (needClear)
    {
        DrawWarnings();

        if (funcAdditionDraw)
        {
            funcAdditionDraw();
        }
    }

    DrawConsole();

    if (needClear)    
    {
        WriteValueTrigLevel();
    }

    DrawTimeForFrame(gTimerTics - timeStart);

    Painter::SetColor(COLOR_FILL);

    Painter::EndScene(endScene);

    if (NEED_SAVE_TO_DRIVE)
    {
        if (Painter::SaveScreenToFlashDrive())
        {
            Display::ShowWarningGood(Warning::FileIsSaved);
        }
        NEED_SAVE_TO_DRIVE = 0;
    }

    if (funcAfterDraw)
    {
        funcAfterDraw();
        funcAfterDraw = 0;
    }
}


void Display::WriteValueTrigLevel()
{
    if ((SHOW_LEVEL_TRIGLEV != 0) && MODE_WORK_IS_DIRECT)
    {
        float trigLev = RSHIFT_2_ABS(TRIG_LEVEL_SOURCE, SET_RANGE(TRIG_SOURCE));     // WARN Здесь для внешней синхронизации неправильно рассчитывается уровень.
        TrigSource::E trigSource = TRIG_SOURCE;
        if (TRIG_INPUT_IS_AC && trigSource <= TrigSource::B)
        {
            int16 rShift = SET_RSHIFT(trigSource);
            float rShiftAbs = RSHIFT_2_ABS(rShift, SET_RANGE(trigSource));
            trigLev += rShiftAbs;
        }
        char buffer[20];
        std::strcpy(buffer, LANG_RU ? "Ур синхр = " : "Trig lvl = ");
        char bufForVolt[20];
        std::strcat(buffer, Voltage2String(trigLev, true, bufForVolt));
        int width = 96;
        int x = (Grid::Width() - width) / 2 + Grid::Left();
        int y = Grid::BottomMessages() - 20;
        Rectangle(width, 10).Draw(x, y, COLOR_FILL);
        Region(width - 2, 8).Fill(x + 1, y + 1, COLOR_BACK);
        Text(buffer).Draw(x + 2, y + 1, COLOR_FILL);
    }
}


void Display::DrawGridSpectrum()
{
    if (SCALE_FFT_IS_LOG)
    {
        static const int nums[] = {4, 6, 8};
        static const char *strs[] = {"0", "-10", "-20", "-30", "-40", "-50", "-60", "-70"};
        int numParts = nums[FFT_MAX_DB];
        float scale = (float)Grid::MathHeight() / numParts;
        for (int i = 1; i < numParts; i++)
        {
            int y = static_cast<int>(Grid::MathTop() + i * scale);
            HLine().Draw(y, Grid::Left(), Grid::Left() + 256, ColorGrid());
            if (!MenuIsMinimize())
            {
                Painter::SetColor(COLOR_FILL);
                Text(strs[i]).Draw(3, y - 4);
            }
        }
        if (!MenuIsMinimize())
        {
            Painter::SetColor(COLOR_FILL);
            Text("дБ").Draw(5, Grid::MathTop() + 1);
        }
    }
    else if (SCALE_FFT_IS_LINEAR)
    {
        static const char *strs[] = {"1.0", "0.8", "0.6", "0.4", "0.2"};
        float scale = (float)Grid::MathHeight() / 5;
        for (int i = 1; i < 5; i++)
        {
            int y = static_cast<int>(Grid::MathTop() + i * scale);
            HLine().Draw(y, Grid::Left(), Grid::Left() + 256, ColorGrid());
            if (!MenuIsMinimize())
            {
                Text(strs[i]).Draw(5, y - 4, COLOR_FILL);
            }
        }
    }
    HLine().Draw(Grid::Left() + 256, Grid::MathTop(), Grid::MathBottom(), COLOR_FILL);
}



void Display::DrawFullGrid()
{
    if (SettingsDisplay::IsSeparate())
    {
        DrawGrid(Grid::Left(), Grid::TOP, Grid::Width(), Grid::FullHeight() / 2);
        if (ENABLED_FFT)
        {
            DrawGridSpectrum();
        }
        if (!DISABLED_DRAW_MATH)
        {
            DrawGrid(Grid::Left(), Grid::TOP + Grid::FullHeight() / 2, Grid::Width(), Grid::FullHeight() / 2);
        }
        HLine().Draw(Grid::TOP + Grid::FullHeight() / 2, Grid::Left(), Grid::Left() + Grid::Width(), COLOR_FILL);
    }
    else
    {
        DrawGrid(Grid::Left(), Grid::TOP, Grid::Width(), Grid::FullHeight());
    }
}



int Display::CalculateCountV()
{
    if (MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if (MEAS_NUM_IS_1_5)
        {
            return MEAS_SOURCE_IS_A_B ? 42 : 44;
        }
        if (MEAS_NUM_IS_2_5)
        {
            return MEAS_SOURCE_IS_A_B ? 69 : 39;
        }
        if (MEAS_NUM_IS_3_5)
        {
            return MEAS_SOURCE_IS_A_B ? 54 : 68;
        }
    }

    return 49;
}



int Display::CalculateCountH()
{
    if (MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if (MEAS_NUM_IS_6_1)
        {
            return 73;
        }
        if (MEAS_NUM_IS_6_2)
        {
            return 83;
        }
    }
    return 69;
}



void Display::DrawGridType1(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX, float deltaY, float stepX, float stepY)
{
    uint16 masX[17];
    masX[0] = (uint16)(left + 1);
    for (int i = 1; i < 7; i++)
    {
        masX[i] = static_cast<uint16>(left + deltaX * i);
    }
    for (int i = 7; i < 10; i++)
    {
        masX[i] = static_cast<uint16>(centerX - 8 + i);
    }
    for (int i = 10; i < 16; i++)
    {
        masX[i] = static_cast<uint16>(centerX + deltaX * (i - 9));
    }
    masX[16] = (uint16)(right - 1);

    MultiVPointLine(17, masX, static_cast<int>(stepY), CalculateCountV()).Draw(static_cast<int>(top + stepY), ColorGrid());

    uint8 mas[13];
    mas[0] = (uint8)(top + 1);
    for (int i = 1; i < 5; i++)
    {
        mas[i] = static_cast<uint8>(top + deltaY * i);
    }
    for (int i = 5; i < 8; i++)
    {
        mas[i] = static_cast<uint8>(centerY - 6 + i);
    }
    for (int i = 8; i < 12; i++)
    {
        mas[i] = static_cast<uint8>(centerY + deltaY * (i - 7));
    }
    mas[12] = (uint8)(bottom - 1);

    MultiHPointLine(13, mas, static_cast<int>(stepX), CalculateCountH()).Draw(static_cast<int>(left + stepX), ColorGrid());
}



void Display::DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY)
{ 
    uint16 masX[15];
    masX[0] = (uint16)(left + 1);
    for (int i = 1; i < 14; i++)
    {
        masX[i] = (uint16)(left + deltaX * i);
    }
    masX[14] = (uint16)(right - 1);
    MultiVPointLine(15, masX, stepY, CalculateCountV()).Draw(top + stepY, ColorGrid());

    uint8 mas[11];
    mas[0] = (uint8)(top + 1);
    for (int i = 1; i < 10; i++)
    {
        mas[i] = (uint8)(top + deltaY * i);
    }
    mas[10] = (uint8)(bottom - 1);
    MultiHPointLine(11, mas, stepX, CalculateCountH()).Draw(left + stepX, ColorGrid());
}



void Display::DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY, int stepX, int stepY)
{
    Painter::DrawHPointLine(centerY, left + stepX, right, static_cast<float>(stepX));
    uint8 masY[6] = {(uint8)(top + 1), (uint8)(top + 2), (uint8)(centerY - 1), (uint8)(centerY + 1), (uint8)(bottom - 2), (uint8)(bottom - 1)};
    MultiHPointLine(6, masY, deltaX, (right - top) / deltaX).Draw(left + deltaX, ColorGrid());
    Painter::DrawVPointLine(centerX, top + stepY, bottom, static_cast<float>(stepY), ColorGrid());
    uint16 masX[6] = {(uint16)(left + 1), (uint16)(left + 2), (uint16)(centerX - 1), (uint16)(centerX + 1), (uint16)(right - 2), (uint16)(right - 1)};
    MultiVPointLine(6, masX, deltaY, (bottom - top) / deltaY).Draw(top + deltaY, ColorGrid());
}



void Display::DrawGrid(int left, int top, int width, int height)
{
    int right = left + width;
    int bottom = top + height;

    Painter::SetColor(COLOR_FILL);

    if (top == Grid::TOP)
    {
        HLine().Draw(top, 1, left - 2);
        HLine().Draw(top, right + 2, Display::WIDTH - 2);

        if (!MenuIsMinimize() || !MenuIsShown())
        {
            VLine().Draw(1, top + 2, bottom - 2);
            VLine().Draw(318, top + 2, bottom - 2);
        }
    }

    float deltaX = Grid::DeltaX() *(float)width / width;
    float deltaY = Grid::DeltaY() * (float)height / height;
    float stepX = deltaX / 5;
    float stepY = deltaY / 5;
    
    int centerX = left + width / 2;
    int centerY = top + height / 2;

    Painter::SetColor(ColorGrid());
    if (TYPE_GRID_IS_1)
    {
        DrawGridType1(left, top, right, bottom, static_cast<float>(centerX), static_cast<float>(centerY), deltaX, deltaY, stepX, stepY);
    }
    else if (TYPE_GRID_IS_2)
    {
        DrawGridType2(left, top, right, bottom, static_cast<int>(deltaX), static_cast<int>(deltaY), static_cast<int>(stepX), static_cast<int>(stepY));
    }
    else if (TYPE_GRID_IS_3)
    {
        DrawGridType3(left, top, right, bottom, centerX, centerY, static_cast<int>(deltaX), static_cast<int>(deltaY), static_cast<int>(stepX), static_cast<int>(stepY));
    }
}



#define  DELTA 5


void Display::DrawScaleLine(int x, bool forTrigLev)
{
    if(ALT_MARKERS_HIDE)
    {
        return;
    }
    int width = 6;
    int topY = Grid::TOP + DELTA;
    int x2 = width + x + 2;
    int bottomY  = Grid::ChannelBottom() - DELTA;
    int centerY = (Grid::ChannelBottom() + Grid::TOP) / 2;
    int levels[] =
    {
        topY,
        bottomY,
        centerY,
        centerY - (bottomY - topY) / (forTrigLev ? 8 : 4),
        centerY + (bottomY - topY) / (forTrigLev ? 8 : 4)
    };
    for(int i = 0; i < 5; i++)
    {
        Painter::DrawLineC(x + 1, levels[i], x2 - 1, levels[i], COLOR_FILL);
    }
}


void Display::DrawCursorsWindow()
{
    if((!MenuIsMinimize() || !MenuIsShown()) && (DRAW_RSHIFT_MARKERS != 0))
    {
        DrawScaleLine(2, false);
    }
}


void Display::DrawCursorTrigLevel()
{
    TrigSource::E chan = TRIG_SOURCE;
    if (chan == TrigSource::Ext)
    {
        return;
    }
    int trigLev = TRIG_LEVEL(chan) + (SET_RSHIFT(chan) - RShiftZero);
    float scale = 1.0F / ((TrigLevMax - TrigLevMin) / 2.0F / Grid::ChannelHeight());
    int y0 = static_cast<int>((Grid::TOP + Grid::ChannelBottom()) / 2 + scale * (TrigLevZero - TrigLevMin));
    int y = static_cast<int>(y0 - scale * (trigLev - TrigLevMin));

    y = (y - Grid::ChannelCenterHeight()) + Grid::ChannelCenterHeight();

    int x = Grid::Right();
    Painter::SetColor(ColorTrig());
    if(y > Grid::ChannelBottom())
    {
        Painter::DrawChar(x + 3, Grid::ChannelBottom() - 11, SYMBOL_TRIG_LEV_LOWER);
        Point().Draw(x + 5, Grid::ChannelBottom() - 2);
        y = Grid::ChannelBottom() - 7;
        x--;
    }
    else if(y < Grid::TOP)
    {
        Painter::DrawChar(x + 3, Grid::TOP + 2, SYMBOL_TRIG_LEV_ABOVE);
        Point().Draw(x + 5, Grid::TOP + 2);
        y = Grid::TOP + 7;
        x--;
    }
    else
    {
        Painter::DrawChar(x + 1, y - 4, SYMBOL_TRIG_LEV_NORMAL);
    }
    Painter::SetFont(TypeFont::_5);

    const char simbols[3] = {'1', '2', 'В'};

    Painter::DrawCharC(x + 5, y - 9, simbols[TRIG_SOURCE], COLOR_BACK);
    Painter::SetFont(TypeFont::_8);

    if ((DRAW_RSHIFT_MARKERS != 0) && !MenuIsMinimize())
    {
        DrawScaleLine(Display::WIDTH - 11, true);
        int left = Grid::Right() + 9;
        int height = Grid::ChannelHeight() - 2 * DELTA;
        int shiftFullMin = RShiftMin + TrigLevMin;
        int shiftFullMax = RShiftMax + TrigLevMax;
        scale = (float)height / (shiftFullMax - shiftFullMin);
        int shiftFull = TRIG_LEVEL_SOURCE + (TRIG_SOURCE_IS_EXT ? 0 : SET_RSHIFT(chan));
        int yFull = static_cast<int>(Grid::TOP + DELTA + height - scale * (shiftFull - RShiftMin - TrigLevMin) - 4);
        Region(4, 6).Fill(left + 2, yFull + 1, ColorTrig());
        Painter::SetFont(TypeFont::_5);
        Painter::DrawCharC(left + 3, yFull - 5, simbols[TRIG_SOURCE], COLOR_BACK);
        Painter::SetFont(TypeFont::_8);
    }
}


void Display::DrawCursorRShift(Channel::E chan)
{
    float x = static_cast<float>(Grid::Right() - Grid::Width() - Measure::GetDeltaGridLeft());

    if (chan == Channel::Math)
    {
        int rShift = SET_RSHIFT_MATH;
        float scale = (float)Grid::MathHeight() / 960;
        float y = (Grid::MathTop() + Grid::MathBottom()) / 2.0F - scale * (rShift - RShiftZero);
        Painter::DrawCharC(static_cast<int>(x - 9), static_cast<int>(y - 4), SYMBOL_RSHIFT_NORMAL, COLOR_FILL);
        Painter::DrawCharC(static_cast<int>(x - 8), static_cast<int>(y - 5), 'm', COLOR_BACK);
        return;
    }
    if(!sChannel_Enabled(chan))
    {
        return;
    }

    int rShift = SET_RSHIFT(chan);
 
    float scale = Grid::ChannelHeight() / (STEP_RSHIFT * 200.0F);
    float y = Grid::ChannelCenterHeight() - scale * (rShift - RShiftZero);

    if(y > Grid::ChannelBottom())
    {
        Painter::DrawCharC(static_cast<int>(x - 7), Grid::ChannelBottom() - 11, SYMBOL_RSHIFT_LOWER, ColorChannel(chan));
        Point().Draw(static_cast<int>(x - 5), Grid::ChannelBottom() - 2);
        y = static_cast<float>(Grid::ChannelBottom() - 7);
        x++;
    }
    else if(y < Grid::TOP)
    {
        Painter::DrawCharC(static_cast<int>(x - 7), Grid::TOP + 2, SYMBOL_RSHIFT_ABOVE, ColorChannel(chan));
        Point().Draw(static_cast<int>(x - 5), Grid::TOP + 2);
        y = Grid::TOP + 7;
        x++;
    }
    else
    {
        Painter::DrawCharC(static_cast<int>(x - 8), static_cast<int>(y - 4), SYMBOL_RSHIFT_NORMAL, ColorChannel(chan));
        if(((chan == Channel::A) ? (SHOW_LEVEL_RSHIFT_0 == 1) : (SHOW_LEVEL_RSHIFT_1 == 1)) && MODE_WORK_IS_DIRECT) //-V2570
        {
            Painter::DrawDashedHLine(static_cast<int>(y), Grid::Left(), Grid::Right(), 7, 3, 0);
        }
    }

    Painter::SetFont(TypeFont::_5);

    if((!MenuIsMinimize() || !MenuIsShown()) && DRAW_RSHIFT_MARKERS_IS_TRUE)
    {
        float scaleFull = (float)Grid::ChannelHeight() / (RShiftMax - RShiftMin) * (sService_MathEnabled() ? 0.9F : 0.91F);
        float yFull = Grid::ChannelCenterHeight() - scaleFull * (rShift - RShiftZero);

        Region(4, 6).Fill(4, static_cast<int>(yFull - 3), ColorChannel(chan));
        Painter::DrawCharC(5, static_cast<int>(yFull - 9), chan == Channel::A ? '1' : '2', COLOR_BACK);
    }
    Painter::DrawCharC(static_cast<int>(x - 7), static_cast<int>(y - 9), chan == Channel::A ? '1' : '2', COLOR_BACK);
    Painter::SetFont(TypeFont::_8);
}


void Display::DrawCursorTShift()
{
    int firstPoint = 0;
    int lastPoint = 0;
    SettingsDisplay::PointsOnDisplay(&firstPoint, &lastPoint);

    if (gDSet == nullptr)
    {
        return;
    }

    // Рисуем TPos
    int shiftTPos = sTime_TPosInPoints((PeackDetMode::E)gDSet->peakDet, (int)gDSet->length1channel, SET_TPOS) - SHIFT_IN_MEMORY;
    float scale = static_cast<float>((lastPoint - firstPoint) / Grid::Width());
    int gridLeft = Grid::Left();
    int x = static_cast<int>(gridLeft + shiftTPos * scale - 3);
    if (IntInRange(x + 3, gridLeft, Grid::Right() + 1))
    {
        Painter::Draw2SymbolsC(x, Grid::TOP - 1, SYMBOL_TPOS_2, SYMBOL_TPOS_3, COLOR_BACK, COLOR_FILL);
    };

    // Рисуем tShift
    int shiftTShift = sTime_TPosInPoints((PeackDetMode::E)gDSet->peakDet, (int)gDSet->length1channel, SET_TPOS) - sTime_TShiftInPoints((PeackDetMode::E)gDSet->peakDet);
    if(IntInRange(shiftTShift, firstPoint, lastPoint))
    {
        x = gridLeft + shiftTShift - firstPoint - 3;
        Painter::Draw2SymbolsC(x, Grid::TOP - 1, SYMBOL_TSHIFT_NORM_1, SYMBOL_TSHIFT_NORM_2, COLOR_BACK, COLOR_FILL);
    }
    else if(shiftTShift < firstPoint)
    {
        Painter::Draw2SymbolsC(gridLeft + 1, Grid::TOP, SYMBOL_TSHIFT_LEFT_1, SYMBOL_TSHIFT_LEFT_2, COLOR_BACK, COLOR_FILL);
        Painter::DrawLineC(Grid::Left() + 9, Grid::TOP + 1, Grid::Left() + 9, Grid::TOP + 7, COLOR_BACK);
    }
    else if(shiftTShift > lastPoint)
    {
        Painter::Draw2SymbolsC(Grid::Right() - 8, Grid::TOP, SYMBOL_TSHIFT_RIGHT_1, SYMBOL_TSHIFT_RIGHT_2, COLOR_BACK, COLOR_FILL);
        Painter::DrawLineC(Grid::Right() - 9, Grid::TOP + 1, Grid::Right() - 9, Grid::TOP + 7, COLOR_BACK);
    }
}


void Display::DrawHorizontalCursor(int y, int xTearing)
{
    y += Grid::TOP;
    if(xTearing == -1)
    {
        Painter::DrawDashedHLine(y, Grid::Left() + 2, Grid::Right() - 1, 1, 1, 0);
    }
    else
    {
        Painter::DrawDashedHLine(y, Grid::Left() + 2, xTearing - 2, 1, 1, 0);
        Painter::DrawDashedHLine(y, xTearing + 2, Grid::Right() - 1, 1, 1, 0);
    }
    Rectangle(2, 2).Draw(Grid::Left() - 1, y - 1);
    Rectangle(2, 2).Draw(Grid::Right() - 1, y - 1);
}


void Display::DrawVerticalCursor(int x, int yTearing)
{
    x += Grid::Left();
    if(yTearing == -1)
    {
        Painter::DrawDashedVLine(x, Grid::TOP + 2, Grid::ChannelBottom() - 1, 1, 1, 0);
    }
    else
    {
        Painter::DrawDashedVLine(x, Grid::TOP + 2, yTearing - 2, 1, 1, 0);
        Painter::DrawDashedVLine(x, yTearing + 2, Grid::ChannelBottom() - 1, 1, 1, 0);
    }
    Rectangle(2, 2).Draw(x - 1, Grid::TOP - 1);
    Rectangle(2, 2).Draw(x - 1, Grid::ChannelBottom() - 1);
}


void Display::DrawCursors()
{
    Channel::E source = CURS_SOURCE;
    Painter::SetColor(ColorCursors(source));
    if (sCursors_NecessaryDrawCursors())
    {
        bool bothCursors = !CURS_CNTRL_T_IS_DISABLE(source) && !CURS_CNTRL_U_IS_DISABLE(source);  // Признак того, что включены и вертикальные и 
                                                                            // горизонтальные курсоры - надо нарисовать квадраты в местах пересечения
        int x0 = -1;
        int x1 = -1;
        int y0 = -1;
        int y1 = -1;

        if (bothCursors)
        {
            x0 = static_cast<int>(Grid::Left() + CURS_POS_T0(source));
            x1 = static_cast<int>(Grid::Left() + CURS_POS_T1(source));
            y0 = static_cast<int>(Grid::TOP + sCursors_GetCursPosU(source, 0));
            y1 = static_cast<int>(Grid::TOP + sCursors_GetCursPosU(source, 1));

            Rectangle(4, 4).Draw(x0 - 2, y0 - 2);
            Rectangle(4, 4).Draw(x1 - 2, y1 - 2);
        }

        CursCntrl::E cntrl = CURS_CNTRL_T(source);
        if (cntrl != CursCntrl::Disable)
        {
            DrawVerticalCursor(static_cast<int>(CURS_POS_T0(source)), y0);
            DrawVerticalCursor(static_cast<int>(CURS_POS_T1(source)), y1);
        }
        cntrl = CURsU_CNTRL;
        if (cntrl != CursCntrl::Disable)
        {
            DrawHorizontalCursor(static_cast<int>(sCursors_GetCursPosU(source, 0)), x0);
            DrawHorizontalCursor(static_cast<int>(sCursors_GetCursPosU(source, 1)), x1);
        }
    }
}


void Display::DrawMeasures()
{
    if(!SHOW_MEASURES)
    {
        TOP_MEASURES = Grid::Bottom();
        return;
    }

    Processing::CalculateMeasures();

    if(MEAS_FIELD_IS_HAND)
    {
        int x0 = MEAS_POS_CUR_T0 - SHIFT_IN_MEMORY + Grid::Left();
        int y0 = MEAS_POS_CUR_U0 + Grid::TOP;
        int x1 = MEAS_POS_CUR_T1 - SHIFT_IN_MEMORY + Grid::Left();
        int y1 = MEAS_POS_CUR_U1 + Grid::TOP;
        SortInt(&x0, &x1);
        SortInt(&y0, &y1);
        Rectangle(x1 - x0, y1 - y0).Draw(x0, y0, COLOR_FILL);
    }

    int x0 = Grid::Left() - Measure::GetDeltaGridLeft();
    int dX = Measure::GetDX();
    int dY = Measure::GetDY();
    int y0 = Measure::GetTopTable();

    int numRows = Measure::NumRows();
    int numCols = Measure::NumCols();

    for(int str = 0; str < numRows; str++)
    {
        for(int elem = 0; elem < numCols; elem++)
        {
            int x = x0 + dX * elem;
            int y = y0 + str * dY;
            bool active = Measure::IsActive(str, elem) && Menu::GetNameOpenedPage() == NamePage::SB_MeasTuneMeas;
            Color::E color = active ? COLOR_BACK : COLOR_FILL;
            Measure::E meas = Measure::Type(str, elem);
            if(meas != Measure::None)
            {
                Region(dX, dY).Fill(x, y, COLOR_BACK);
                Rectangle(dX, dY).Draw(x, y, COLOR_FILL);
                TOP_MEASURES = Math_MinFrom2Int(TOP_MEASURES, y);
            }
            if(active)
            {
                Region(dX - 4, dY - 4).Fill(x + 2, y + 2, COLOR_FILL);
            }
            if(meas != Measure::None)
            {
                char buffer[20];
                Text(Measure::Name(str, elem)).Draw(x + 4, y + 2, color);
                if(meas == MEAS_MARKED)
                {
                    Region(dX - 2, 9).Fill(x + 1, y + 1, active ? COLOR_BACK : COLOR_FILL);
                    Text(Measure::Name(str, elem)).Draw(x + 4, y + 2, active ? COLOR_FILL : COLOR_BACK);
                }
                if(MEAS_SOURCE_IS_A)
                {
                    Text(Processing::GetStringMeasure(meas, Channel::A, buffer)).Draw(x + 2, y + 11, ColorChannel(Channel::A));
                }
                else if(MEAS_SOURCE_IS_B)
                {
                    Text(Processing::GetStringMeasure(meas, Channel::B, buffer)).Draw(x + 2, y + 11, ColorChannel(Channel::B));
                }
                else
                {
                    Text(Processing::GetStringMeasure(meas, Channel::A, buffer)).Draw(x + 2, y + 11, ColorChannel(Channel::A));
                    Text(Processing::GetStringMeasure(meas, Channel::B, buffer)).Draw(x + 2, y + 20, ColorChannel(Channel::B));
                }
            }
        }
    }

    if(Menu::GetNameOpenedPage() == NamePage::SB_MeasTuneMeas)
    {
        Measure::DrawPageChoice();
    }
}



void Display::WriteTextVoltage(Channel::E chan, int x, int y)
{
    static const char *couple[] =
    {
        "\x92",
        "\x91",
        "\x90"
    };
    Color::E color = ColorChannel(chan);

    bool inverse = SET_INVERSE(chan);
    ModeCouple::E modeCouple = SET_COUPLE(chan);
    Divider::E multiplier = SET_DIVIDER(chan);
    Range::E range = SET_RANGE(chan);
    uint rShift = (uint)SET_RSHIFT(chan);
    bool enable = SET_ENABLED(chan);

    if (!MODE_WORK_IS_DIRECT)
    {
        DataSettings *ds = MODE_WORK_IS_DIRECT ? gDSet : gDSmemInt;
        if (ds != 0)
        {
            inverse = (chan == Channel::A) ? ds->inverseCh0 : ds->inverseCh1;
            modeCouple = (chan == Channel::A) ? ds->modeCouple0 : ds->modeCouple1;
            multiplier = (chan == Channel::A) ? ds->multiplier0 : ds->multiplier1;
            range = ds->range[chan];
            rShift = (chan == Channel::A) ? ds->rShiftCh0 : ds->rShiftCh1;
            enable = (chan == Channel::A) ? ds->enableCh0 : ds->enableCh1;
        }
    }

    if(enable)
    {
        Color::E colorDraw = inverse ? Color::WHITE : color;
        if(inverse)
        {
            const int widthField = 91;
            const int heightField = 8;
            Region(widthField, heightField).Fill(x, y, color);
        }

        char buffer[100] = {0};

        std::sprintf(buffer, "%s\xa5%s\xa5%s", (chan == Channel::A) ? ((set.common.lang == Language::Russian) ? "1к" : "1c") : ((set.common.lang == Language::Russian) ? "2к" : "2c"), couple[modeCouple],
            sChannel_Range2String(range, multiplier));

        Text(buffer).Draw(x + 1, y, colorDraw);

        char bufferTemp[20];
        std::sprintf(buffer, "\xa5%s", sChannel_RShift2String((int16)rShift, range, multiplier, bufferTemp));
        Text(buffer).Draw(x + 46, y);
    }
}



void Display::WriteStringAndNumber(pCHAR text, int x, int y, int number)
{
    char buffer[100];
    Text(text).Draw(x, y, COLOR_FILL);
    if(number == 0)
    {
        std::sprintf(buffer, "-");
    }
    else
    {
        std::sprintf(buffer, "%d", number);
    }
    Painter::DrawTextRelativelyRight(x + 41, y, buffer);
}



void Display::DrawLowPart()
{
    int y0 = Display::HEIGHT - 19;
    int y1 = Display::HEIGHT - 10;
    int x = -1;

    HLine().Draw(Grid::ChannelBottom(), 1, Grid::Left() - Measure::GetDeltaGridLeft() - 2, COLOR_FILL);
    HLine().Draw(Grid::FullBottom(), 1, Grid::Left() - Measure::GetDeltaGridLeft() - 2);

    WriteTextVoltage(Channel::A, x + 2, y0);

    WriteTextVoltage(Channel::B, x + 2, y1);

    VLine().Draw(x + 95, Grid::Bottom() + 2, Display::HEIGHT - 2, COLOR_FILL);

    x += 98;
    char buffer[100] = {0};

    TBase::E tBase = SET_TBASE;
    int16 tShift = TSHIFT;

    if (!MODE_WORK_IS_DIRECT)
    {
        DataSettings *ds = MODE_WORK_IS_LATEST ? gDSmemLast : gDSmemInt;
        if (ds != 0)
        {
            tBase = ds->tBase;
            tShift = ds->tShift;
        }
    }

    std::sprintf(buffer, "р\xa5%s", Tables_GetTBaseString(tBase));
    Text(buffer).Draw(x, y0);

    buffer[0] = 0;
    char bufForVal[20];
    std::sprintf(buffer, "\xa5%s", FPGA::GetTShiftString(tShift, bufForVal));
    Text(buffer).Draw(x + 35, y0);

    buffer[0] = 0;
    if (MODE_WORK_IS_DIRECT)
    {
        const char *source[3] = { "1", "2", "\x82" };
        std::sprintf(buffer, "с\xa5\x10%s", source[TRIG_SOURCE]);
    }

    Text(buffer).Draw(x, y1, ColorTrig());

    buffer[0] = 0;
    static const char *couple[] =
    {
        "\x92",
        "\x91",
        "\x92",
        "\x92"
    };
    static const char *polar[] =
    {
        "\xa7",
        "\xa6"
    };
    static const char *filtr[] =
    {
        "\xb5\xb6",
        "\xb5\xb6",
        "\xb3\xb4",
        "\xb1\xb2"
    };
    if (MODE_WORK_IS_DIRECT)
    {
        std::sprintf(buffer, "\xa5\x10%s\x10\xa5\x10%s\x10\xa5\x10", couple[TRIG_INPUT], polar[TRIG_POLARITY]);
        Text(buffer).Draw(x + 18, y1);
        Painter::DrawChar(x + 45, y1, filtr[TRIG_INPUT][0]);
        Painter::DrawChar(x + 53, y1, filtr[TRIG_INPUT][1]);
    }

    buffer[0] = '\0';
    const char mode[] =
    {
        '\xb7',
        '\xa0',
        '\xb0'
    };
    if (MODE_WORK_IS_DIRECT)
    {
        std::sprintf(buffer, "\xa5\x10%c", mode[START_MODE]);
        Text(buffer).Draw(x + 63, y1);
    }
    
    VLine().Draw(x + 79, Grid::Bottom() + 2, Display::HEIGHT - 2, COLOR_FILL);

    HLine().Draw(Grid::Bottom(), Grid::Right() + 2, Display::WIDTH - 2);
    HLine().Draw(Grid::ChannelBottom(), Grid::Right() + 2, Display::WIDTH - 2);

    x += 82;
    y0 = y0 - 3;
    y1 = y1 - 6;
    Painter::SetFont(TypeFont::_5);
    
    if (MODE_WORK_IS_DIRECT)
    {
        int y2 = y1 + 6;
        WriteStringAndNumber("накопл", x, y0, NUM_ACCUM);
        WriteStringAndNumber("усредн", x, y1, NUM_AVE);
        WriteStringAndNumber("мн\x93мкс", x, y2, NUM_MIN_MAX);
    }

    x += 42;
    VLine().Draw(x, Grid::Bottom() + 2, Display::HEIGHT - 2);

    Painter::SetFont(TypeFont::_8);

    if (MODE_WORK_IS_DIRECT)
    {
        char mesFreq[20] = "\x7c=";
        char buf[20];
        float freq = FPGA::GetFreq();
        if (freq == -1.0F) //-V550 //-V2550
        {
            std::strcat(mesFreq, "******");
        }
        else
        {
            std::strcat(mesFreq, Freq2String(freq, false, buf));
        }
        Text(mesFreq).Draw(x + 3, Grid::Bottom() + 2);
    }

    DrawTime(x + 3, Grid::Bottom() + 11);

    HLine().Draw(x + 55, Grid::Bottom() + 2, Display::HEIGHT - 2);

    Painter::SetFont(TypeFont::_UGO2);

    // Флешка
    if (FLASH_DRIVE_IS_CONNECTED)
    {
        Painter::Draw4SymbolsInRect(x + 57, Grid::Bottom() + 2, SYMBOL_FLASH_DRIVE);
    }

    // Ethernet
    if (((CLIENT_LAN_IS_CONNECTED != 0) || (CABLE_LAN_IS_CONNECTED != 0)) && gTimerMS > 2000)
    {
        Painter::Draw4SymbolsInRectC(x + 87, Grid::Bottom() + 2, SYMBOL_ETHERNET, CLIENT_LAN_IS_CONNECTED ? COLOR_FILL : Color::FLASH_01);
    }

    if ((CLIENT_VCP_IS_CONNECTED != 0) || (CABLE_VCP_IS_CONNECTED != 0))
    {
        Painter::Draw4SymbolsInRectC(x + 72, Grid::Bottom() + 2, SYMBOL_USB, CLIENT_VCP_IS_CONNECTED ? COLOR_FILL : Color::FLASH_01);
    }
    
    Painter::SetColor(COLOR_FILL);
    // Пиковый детектор
    if(!PEAKDET_IS_DISABLE)
    {
       Painter::DrawChar(x + 38, Grid::Bottom() + 11, '\x12');
       Painter::DrawChar(x + 46, Grid::Bottom() + 11, '\x13');
    }

    if (MODE_WORK_IS_DIRECT)
    {
        Painter::SetFont(TypeFont::_5);
        WriteStringAndNumber("СГЛАЖ.:", x + 57, Grid::Bottom() + 10, Smoothing::NumPoints());
        Painter::SetFont(TypeFont::_8);
    }
}


void Display::DrawTimeForFrame(uint timeTicks)
{
    if(!SHOW_STATS)
    {
        return;
    }
    static char buffer[10];
    static bool first = true;
    static uint timeMSstartCalculation = 0;
    static int numFrames = 0;
    static float numMS = 0.0F;
    if(first)
    {
        timeMSstartCalculation = gTimerMS;
        first = false;
    }
    numMS += timeTicks / 120000.0F;
    numFrames++;
    
    if((gTimerMS - timeMSstartCalculation) >= 500)
    {
        std::sprintf(buffer, "%.1fms/%d", numMS / numFrames, numFrames * 2);
        timeMSstartCalculation = gTimerMS;
        numMS = 0.0F;
        numFrames = 0;
    }

    Rectangle(84, 10).Draw(Grid::Left(), Grid::FullBottom() - 10, COLOR_FILL);
    Region(82, 8).Fill(Grid::Left() + 1, Grid::FullBottom() - 9, COLOR_BACK);
    Text(buffer).Draw(Grid::Left() + 2, Grid::FullBottom() - 9, COLOR_FILL);

    char message[20] = {0};
    std::sprintf(message, "%d", Storage::NumElementsWithSameSettings());
    std::strcat(message, "/");
    char numAvail[10] = {0};
    std::sprintf(numAvail, "%d", Storage::NumberAvailableEntries());
    std::strcat(message, numAvail);
    Text(message).Draw(Grid::Left() + 50, Grid::FullBottom() - 9);
}


void Display::DisableShowLevelRShiftA()
{
    SHOW_LEVEL_RSHIFT_0 = 0;
    Timer::Disable(TypeTimer::ShowLevelRShift0);
}


void Display::DisableShowLevelRShiftB()
{
    SHOW_LEVEL_RSHIFT_1 = 0;
    Timer::Disable(TypeTimer::ShowLevelRShift1);
}


void Display::DisableShowLevelTrigLev()
{
    SHOW_LEVEL_TRIGLEV = 0;
    Timer::Disable(TypeTimer::ShowLevelTrigLev);
}


void Display::EnableTrigLabel(bool enable)
{
    TRIG_ENABLE = enable ? 1U : 0U;
}


void Display::ResetP2Ppoints(bool empty)
{
    dataP2PIsEmpty = empty;
    lastP2Pdata = 0;
    std::memset(dataP2P_0, AVE_VALUE, NUM_P2P_POINTS);
    std::memset(dataP2P_1, AVE_VALUE, NUM_P2P_POINTS);
}


void Display::AddPoints(uint8 data00, uint8 data01, uint8 data10, uint8 data11)
{
    dataP2PIsEmpty = false;
    if (SET_SELFRECORDER)
    {
        if (lastP2Pdata == NUM_P2P_POINTS)
        {
            std::memcpy(dataP2P_0, dataP2P_0 + 2, NUM_P2P_POINTS - 2); //-V743
            std::memcpy(dataP2P_1, dataP2P_1 + 2, NUM_P2P_POINTS - 2); //-V743
        }
    }

    dataP2P_0[lastP2Pdata] = data00;
    dataP2P_1[lastP2Pdata++] = data10;
    
    dataP2P_0[lastP2Pdata] = data01;
    dataP2P_1[lastP2Pdata++] = data11;
    if (!SET_SELFRECORDER && lastP2Pdata >= NUM_P2P_POINTS)
    {
        lastP2Pdata = 0;
    }
}



void Display::SetDrawMode(DrawMode::E mode, pFuncVV func)
{
    funcOnHand = (mode == DrawMode::Auto) ? 0 : func;
}



void Display::SetAddDrawFunction(pFuncVV func)
{
    funcAdditionDraw = func;
}


void Display::RemoveAddDrawFunction()
{
    funcAdditionDraw = 0;
}


void Display::Clear()
{
    Region(Display::WIDTH - 1, Display::HEIGHT - 2).Fill(0, 0, COLOR_BACK);
}


void Display::ShiftScreen(int delta)
{
    LIMITATION(SHIFT_IN_MEMORY, static_cast<int16>(SHIFT_IN_MEMORY + delta), 0, static_cast<int16>(sMemory_GetNumPoints(false) - 282));
}


void Display::ChangedRShiftMarkers()
{
    DRAW_RSHIFT_MARKERS = ALT_MARKERS_HIDE ? 0U : 1U;
    Timer::Enable(TypeTimer::RShiftMarkersAutoHide, 5000, FuncOnTimerRShiftMarkersAutoHide);
}

void Display::FuncOnTimerRShiftMarkersAutoHide()
{
    Display::OnRShiftMarkersAutoHide();
}


void Display::OnRShiftMarkersAutoHide()
{
    DRAW_RSHIFT_MARKERS = 0;
    Timer::Disable(TypeTimer::RShiftMarkersAutoHide);
}


int Display::FirstEmptyString()
{
    for(int i = 0; i < MAX_NUM_STRINGS; i++)
    {
        if(strings[i] == 0)
        {
            return i;
        }
    }
    return MAX_NUM_STRINGS - 1;
}


int Display::CalculateFreeSize()
{
    int firstEmptyString = FirstEmptyString();
    if(firstEmptyString == 0)
    {
        return SIZE_BUFFER_FOR_STRINGS;
    }
    return static_cast<int>(SIZE_BUFFER_FOR_STRINGS - (strings[firstEmptyString - 1] - bufferForStrings) - std::strlen(strings[firstEmptyString - 1]) - 1);
}


void Display::DeleteFirstString()
{
    if(FirstEmptyString() < 2)
    {
        return;
    }
    int delta = static_cast<int>(std::strlen(strings[0])) + 1;
    int numStrings = FirstEmptyString();
    for(int i = 1; i < numStrings; i++)
    {
        strings[i - 1] = strings[i] - delta;
    }
    for(int i = numStrings - 1; i < MAX_NUM_STRINGS; i++)
    {
        strings[i] = 0; //-V557
    }
    for(int i = 0; i < SIZE_BUFFER_FOR_STRINGS - delta; i++)
    {
        bufferForStrings[i] = bufferForStrings[i + delta];
    }
}



void Display::AddString(const char *string)
{
    if(CONSOLE_IN_PAUSE)
    {
        return;
    }
    static int num = 0;
    char buffer[100];
    std::sprintf(buffer, "%d\x11", num++);
    std::strcat(buffer, string);
    int size = static_cast<int>(std::strlen(buffer)) + 1;
    while(CalculateFreeSize() < size)
    {
        DeleteFirstString();
    }
    if(!strings[0])
    {
        strings[0] = bufferForStrings;
        std::strcpy(strings[0], buffer);
    }
    else
    {
        char *addressLastString = strings[FirstEmptyString() - 1];
        char *address = addressLastString + std::strlen(addressLastString) + 1;
        strings[FirstEmptyString()] = address;
        std::strcpy(address, buffer);
    }
}



void Display::AddStringToIndicating(const char *string)
{
    if(FirstEmptyString() == (MAX_NUM_STRINGS - 1))
    {
        DeleteFirstString();
    }

    AddString(string);
}



void Display::SetPauseForConsole(bool pause)
{
    if(pause)
    {
        lastStringForPause = FirstEmptyString() - 1;
    }
    else
    {
        lastStringForPause = -1;
    }
}



void Display::OneStringUp()
{
    if(!CONSOLE_IN_PAUSE)
    {
    }
    else if(lastStringForPause > NUM_STRINGS - 1)
    {
        lastStringForPause--;
    }
}



void Display::OneStringDown()
{
    if(!CONSOLE_IN_PAUSE)
    {
    }
    else if(lastStringForPause < FirstEmptyString() - 1)
    {
        lastStringForPause++;
    }
}



void Display::DrawConsole()
{
    int count = 0;
    Painter::SetFont(sDebug_GetSizeFontForConsole() == 5 ? TypeFont::_5 : TypeFont::_8);
    int height = Font::GetSize();

    int lastString = FirstEmptyString() - 1;
    int numStr = NUM_STRINGS;
    if(height == 8 && numStr > 22)
    {
        numStr = 22;
    }
    int delta = 0;
    if(SHOW_STRING_NAVIGATION)
    {
        numStr -= ((height == 8) ? 1 : 2);
        delta = 10;
    }
    int firstString = lastString - numStr + 1;
    if(firstString < 0)
    {
        firstString = 0;
    }

    for(int numString = firstString; numString <= lastString; numString++)
    {
        int width = Font::GetLengthText(strings[numString]);
        Region(width, height + 1).Fill(Grid::Left() + 1, Grid::TOP + 1 + count * (height + 1) + delta, COLOR_BACK);
        int y = Grid::TOP + 5 + count * (height + 1) - 4;
        if(Font::GetSize() == 5)
        {
            y -= 3;
        }
        Text(strings[numString]).Draw(Grid::Left() + 2, y + delta, COLOR_FILL);
        count++;
    }

    Painter::SetFont(TypeFont::_8);
}


void Display::ShowWarn(const char *message)
{
    if (warnings[0] == 0)
    {
        Timer::Enable(TypeTimer::ShowMessages, 100, OnTimerShowWarning);
    }
    bool alreadyStored = false;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (warnings[i] == 0 && !alreadyStored)
        {
            warnings[i] = message;
            timeWarnings[i] = gTimerMS;
            alreadyStored = true;
        }
        else if (warnings[i] == message)
        {
            timeWarnings[i] = gTimerMS;
            return;
        }
    }
}


void Display::OnTimerShowWarning()
{
    uint time = gTimerMS;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (time - timeWarnings[i] > static_cast<uint>(TIME_MESSAGES * 1000))
        {
            timeWarnings[i] = 0;
            warnings[i] = 0;
        }
    }

    int pointer = 0;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (warnings[i] != 0)
        {
            warnings[pointer] = warnings[i];
            timeWarnings[pointer] = timeWarnings[i];
            if (pointer != i)
            {
                timeWarnings[i] = 0;
                warnings[i] = 0;
            }
            pointer++;
        }
    }

    if (pointer == 0)
    {
        Timer::Disable(TypeTimer::ShowMessages);
    }
}


void Display::ClearFromWarnings()
{
    Timer::Disable(TypeTimer::ShowMessages);
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        warnings[i] = 0;
        timeWarnings[i] = 0;
    }
}



void Display::ShowWarningBad(Warning::E warning)
{
    Painter::ResetFlash();
    ShowWarn(Tables_GetWarning(warning));
    Sound::WarnBeepBad();
}



void Display::ShowWarningGood(Warning::E warning)
{
    Painter::ResetFlash();
    ShowWarn(Tables_GetWarning(warning));
    Sound::WarnBeepGood();
}


void Display::DrawStringInRectangle(int, int y, char const *text)
{
    int width = Font::GetLengthText(text);
    int height = 8;
    Rectangle(width + 4, height + 4).Draw(Grid::Left(), y, COLOR_FILL);
    Rectangle(width + 2, height + 2).Draw(Grid::Left() + 1, y + 1, COLOR_BACK);
    Region(width, height).Fill(Grid::Left() + 2, y + 2, Color::FLASH_10);
    Text(text).Draw(Grid::Left() + 3, y + 2, Color::FLASH_01);
}


void Display::DrawWarnings()
{
    int delta = 12;
    int y = Grid::BottomMessages();
    for(int i = 0; i < 10; i++)
    {
        if(warnings[i] != 0)
        {
            DrawStringInRectangle(Grid::Left(), y, warnings[i]);
            y -= delta;
        }
    }
}


void Display::RunAfterDraw(pFuncVV func)
{
    funcAfterDraw = func;
}
