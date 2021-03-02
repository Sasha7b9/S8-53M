#include "defines.h"
#include "common/Log_.h"
#include "common/Display/Painter_.h"
#include "common/Display/Primitives_.h"
#include "common/Display/Text_.h"
#include "common/Display/Font/Font_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/Timer_.h"
#include "common/Utils/Math_.h"
#include "Display/Display.h"
#include "FDrive/FDrive.h"
#include "FPGA/DataSettings.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/MathFPGA.h"
#include "FPGA/Storage.h"
#include "LAN/LAN.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "Utils/ProcessingSignal.h"
#include "VCP/VCP.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>

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
static const char               *warnings[NUM_WARNINGS] = {0};      // ����� ��������������� ���������.
static uint                     timeWarnings[NUM_WARNINGS] = {0};   // ����� �����, ����� ��������������� ��������� ��������� �� �����.

static pFuncVV funcOnHand       = 0;
static pFuncVV funcAdditionDraw = 0;
static pFuncVV funcAfterDraw    = 0;

static bool showLevelRShiftA = false;    // ����� �� �������� �������������� ����� ������ �������� ������� ������
static bool showLevelRShiftB = false;
static bool showLevelTrigLev = false;    // ����� �� �������� �������������� ����� ������ �������� ������ �������������
static bool trigEnable = false;
static bool drawRShiftMarkers = false;
int Display::topMeasures = Grid::Bottom();
static bool needFinishDraw = true;      // ���� 1, �� ������� ��������� � �����������
static uint numDrawingSignals = 0;      // ����� ������������ �������� ��� ������ ����������



void Display::DrawStringNavigation() 
{
    char buffer[100];
    char *string = Menu::StringNavigation(buffer);
    if(string) 
    {
        int length = Font::GetLengthText(string);
        int height = 10;
        Primitives::Rectangle(length + 2, height).Draw(Grid::Left(), Grid::TOP, Color::FILL);
        Region(length, height - 2).Fill(Grid::Left() + 1, Grid::TOP + 1, Color::BACK);
        Text(string).Draw(Grid::Left() + 2, Grid::TOP + 1, Color::FILL);
    }
}


void Display::RotateRShift(Channel::E chan)
{
    ResetP2Ppoints(true);
    LAST_AFFECTED_CHANNEL = chan;
    if(TIME_SHOW_LEVELS)
    {
        if (chan == Channel::A) { showLevelRShiftA = true; }
        else                    { showLevelRShiftB = true; }
        Timer::Enable((chan == Channel::A) ? TypeTimer::ShowLevelRShift0 : TypeTimer::ShowLevelRShift1, TIME_SHOW_LEVELS  * 1000, (chan == Channel::A) ? FuncOnTimerDisableShowLevelRShiftA :
                     FuncOnTimerDisableShowLevelRShiftB);
    };
    Redraw();
};


void Display::FuncOnTimerDisableShowLevelRShiftA()
{
    DisableShowLevelRShiftA();
}


void Display::FuncOnTimerDisableShowLevelRShiftB()
{
    DisableShowLevelRShiftB();
}


void Display::RotateTrigLev()
{
    if (TIME_SHOW_LEVELS)
    {
        showLevelTrigLev = true;
        Timer::Enable(TypeTimer::ShowLevelTrigLev, TIME_SHOW_LEVELS * 1000, FuncOnTimerDisableShowLevelTrigLev);
    }
    Redraw();
}


void Display::FuncOnTimerDisableShowLevelTrigLev()
{
    DisableShowLevelTrigLev();
}


void Display::Redraw()
{
    needFinishDraw = true;
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
    Color::Cursors(chan).SetAsCurrent();
    for(int numMarker = 0; numMarker < 2; numMarker++)
    {
        int pos = Processing::GetMarkerHorizontal(chan, numMarker);
        if(pos != ERROR_VALUE_INT && pos > 0 && pos < 200)
        {
            DashedHLine(3, 2).Draw(Grid::FullBottom() - static_cast<int>(pos * scale), Grid::Left(), Grid::Right(), 0); //-V2564
        }

        pos = Processing::GetMarkerVertical(chan, numMarker);
        if (pos != ERROR_VALUE_INT && pos > 0 && pos < Grid::Right())
        {
            DashedVLine(3, 2).Draw(Grid::Left() + static_cast<int>(pos * scale), Grid::TOP, Grid::FullBottom(), 0); //-V2564
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
            float x0 = gridLeft + (i - startPoint) * scaleX; //-V2564
            if (x0 >= gridLeft && x0 <= gridRight) //-V2564
            {
                int index = i - startPoint;
                int y = calculateFiltr ? Math::CalculateFiltr(data, i, numPoints, numSmoothing) : data[i]; //-V2563
                CONVERT_DATA_TO_DISPLAY(dataCD[index], y); //-V2516 //-V2564
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
            float x = gridLeft + (i - startPoint) * scaleX; //-V2564
            if (x >= gridLeft && x <= gridRight) //-V2564
            {
                int yMin = yMinNext;
                if (yMin == -1)
                {
                    CONVERT_DATA_TO_DISPLAY(yMin, data[i + shift]); //-V2516 //-V2563 //-V2564
                }
                int yMax = yMaxNext;
                if (yMax == -1)
                {
                    CONVERT_DATA_TO_DISPLAY(yMax, data[i]); //-V2516 //-V2563 //-V2564
                }

                CONVERT_DATA_TO_DISPLAY(yMaxNext, data[i + 1]); //-V2516 //-V2563 //-V2564
                if (yMaxNext < yMin)
                {
                    yMin = yMaxNext + 1;
                }

                CONVERT_DATA_TO_DISPLAY(yMinNext, data[i + shift + 1]); //-V2516 //-V2563 //-V2564
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
            CONVERT_DATA_TO_DISPLAY(dataCD[index], MIN_VALUE); //-V2516 //-V2564
        }
    }

    if(ds->peakDet == PeackDetMode::Disable)
    {
        CONVERT_DATA_TO_DISPLAY(dataCD[280], data[endPoint]); //-V2516 //-V2563 //-V2564
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
            CONVERT_DATA_TO_DISPLAY(dataCD[index], Math::CalculateFiltr(data, i, numPoints, numSmoothing)); //-V2516 //-V2564
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
                CONVERT_DATA_TO_DISPLAY(dataCD[index], Math::CalculateFiltr(data, i, numPoints, numSmoothing)); //-V2516 //-V2564
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
            CONVERT_DATA_TO_DISPLAY(dat, Math::CalculateFiltr(data, i, numPoints, numSmoothing)); //-V2516 //-V2564
            Point().Draw(Grid::Left() + static_cast<int>(index * scaleX), dat); //-V2564
        }
    }
}



// ���� data == 0, �� ������ ����� �� GetData
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

    float scaleY = static_cast<float>(maxY - minY) / (MAX_VALUE - MIN_VALUE); //-V2564
    float scaleX = Grid::Width() / 280.0F; //-V2564

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
        else if(lastPoint > lastP2Pdata) //-V2516
        {
            lastPoint = lastP2Pdata;
        }
    }

    Color::Channel(chan).SetAsCurrent();
    if(MODE_DRAW_IS_SIGNAL_LINES)
    {
        /*
        if (set.display.numAveraging > ENumAveraging::_1)
        {
            Color::GRID.SetAsCurrent();
            DrawSignalLined(DS_GetData(chan, 0), ds, firstPoint, lastPoint, minY, maxY, scaleY, scaleX, calculateFiltr);    // WARN
        }
        Color::SetCurrent(ColorChannel(chan));
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

    MathFPGA::PointsRelToVoltage(dataRel0, static_cast<int>(ds->length1channel), ds->range[Channel::A], static_cast<int16>(ds->rShiftCh0), dataAbs0);
    MathFPGA::PointsRelToVoltage(dataRel1, static_cast<int>(ds->length1channel), ds->range[Channel::B], static_cast<int16>(ds->rShiftCh1), dataAbs1);

    MathFPGA::CalculateMathFunction(dataAbs0, dataAbs1, static_cast<int>(ds->length1channel));
    
    uint8 points[FPGA_MAX_POINTS];
    MathFPGA::PointsVoltageToRel(dataAbs0, static_cast<int>(ds->length1channel), SET_RANGE_MATH, SET_RSHIFT_MATH, points);

    DrawDataChannel(points, Channel::Math, ds, Grid::MathTop(), Grid::MathBottom());

    static const int w = 71;
    static const int h = 10;
    int delta = (SHOW_STRING_NAVIGATION && MODE_DRAW_MATH_IS_TOGETHER) ? 10 : 0;
    Primitives::Rectangle(w, h).Draw(Grid::Left(), Grid::MathTop() + delta, Color::FILL);
    Region(w - 2, h - 2).Fill(Grid::Left() + 1, Grid::MathTop() + 1 + delta, Color::BACK);
    Divider::E multiplier = MATH_MULTIPLIER;
    Text(sChannel_Range2String(SET_RANGE_MATH, multiplier)).Draw(Grid::Left() + 2, Grid::MathTop() + 1 + delta, Color::FILL);
    Text(":").Draw(Grid::Left() + 25, Grid::MathTop() + 1 + delta);
    char buffer[20];
    Text(sChannel_RShift2String(SET_RSHIFT_MATH, SET_RANGE_MATH, multiplier, buffer)).Draw(Grid::Left() + 27, Grid::MathTop() + 1 + delta);
}



void Display::DrawSpectrumChannel(const float *spectrum, Color color)
{
    color.SetAsCurrent();
	int gridLeft = Grid::Left();
	int gridBottom = Grid::MathBottom();
	int gridHeight = Grid::MathHeight();
    for (int i = 0; i < 256; i++) 
    {
        HLine().Draw(gridLeft + i, gridBottom, gridBottom - static_cast<int>(gridHeight * spectrum[i])); //-V2563 //-V2564
    }
}



void Display::WriteParametersFFT(Channel::E chan, float freq0, float density0, float freq1, float density1)
{
    int x = Grid::Left() + 259;
    int y = Grid::ChannelBottom() + 5;
    int dY = 10;

    char buffer[20];
    Color::FILL.SetAsCurrent();
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
    Color::Channel(chan).SetAsCurrent();
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

    MathFPGA::PointsRelToVoltage(data, numPoints, Storage::set->range[channel], channel == Channel::A ? static_cast<int16>(Storage::set->rShiftCh0) : static_cast<int16>(Storage::set->rShiftCh1), dataR);
    MathFPGA::CalculateFFT(dataR, numPoints, spectrum, &freq0, &density0, &freq1, &density1, &y0, &y1);
    DrawSpectrumChannel(spectrum, Color::Channel(channel));
    if (!Menu::IsShown() || Menu::IsMinimize())
    {
        int s = 2;

        Color color = Color::FILL;
        WriteParametersFFT(channel, freq0, density0, freq1, density1);
        Primitives::Rectangle(s * 2, s * 2).Draw(FFT_POS_CURSOR_0 + Grid::Left() - s, y0 - s, color);
        Primitives::Rectangle(s * 2, s * 2).Draw(FFT_POS_CURSOR_1 + Grid::Left() - s, y1 - s);

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

    HLine().Draw(Grid::Right(), Grid::ChannelBottom() + 1, Grid::MathBottom() - 1, Color::BACK);

    if (MODE_WORK_IS_DIRECT)
    {
        int numPoints = sMemory_GetNumPoints(false);
        if (numPoints < 512)
        {
            numPoints = 256;
        }

        if (SOURCE_FFT_IS_A)
        {
            DRAW_SPECTRUM(Storage::dataA, numPoints, Channel::A);
        }
        else if (SOURCE_FFT_IS_B)
        {
            DRAW_SPECTRUM(Storage::dataB, numPoints, Channel::B);
        }
        else
        {
            if (LAST_AFFECTED_CHANNEL_IS_A)
            {
                DRAW_SPECTRUM(Storage::dataB, numPoints, Channel::B);
                DRAW_SPECTRUM(Storage::dataA, numPoints, Channel::A);
            }
            else
            {
                DRAW_SPECTRUM(Storage::dataA, numPoints, Channel::A);
                DRAW_SPECTRUM(Storage::dataB, numPoints, Channel::B);
            }
        }
    }

    HLine().Draw(Grid::ChannelBottom(), Grid::Left(), Grid::Right(), Color::FILL);
    HLine().Draw(Grid::MathBottom(), Grid::Left(), Grid::Right());
}



void Display::DrawBothChannels(uint8 *data0, uint8 *data1)
{
	if (LAST_AFFECTED_CHANNEL_IS_B)
    {
        DrawDataChannel(data0, Channel::A, Storage::set, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(data1, Channel::B, Storage::set, Grid::TOP, Grid::ChannelBottom());
    }
    else
    {
        DrawDataChannel(data1, Channel::B, Storage::set, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(data0, Channel::A, Storage::set, Grid::TOP, Grid::ChannelBottom());
    }
}



void Display::DrawDataMemInt()
{
    if(Storage::dsInt != 0)
     {
        DrawDataChannel(Storage::dataIntA, Channel::A, Storage::dsInt, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::dataIntB, Channel::B, Storage::dsInt, Grid::TOP, Grid::ChannelBottom());
    }
}



void Display::DrawDataInModeWorkLatest()
{
    if (Storage::dsLast != 0)
    {
        DrawDataChannel(Storage::dataLastA, Channel::A, Storage::dsLast, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::dataLastB, Channel::B, Storage::dsLast, Grid::TOP, Grid::ChannelBottom());
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

    int16 numSignals = (int16)Storage::NumElementsWithSameSettings(); //-V2533
    LIMITATION(numSignals, numSignals, 1, NUM_ACCUM); //-V2516
    if (numSignals == 1 || ENUM_ACCUM_IS_INFINITY || MODE_ACCUM_IS_RESET || sTime_RandomizeModeEnabled())
    {
        DrawBothChannels(0, 0);
        if (prevAddr == 0 || prevAddr != ds->addrPrev)
        {
            numDrawingSignals++;
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
        DrawDataChannel(Storage::GetLimitation(Channel::A, 0), Channel::A, Storage::set, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(Channel::A, 1), Channel::A, Storage::set, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(Channel::B, 0), Channel::B, Storage::set, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(Channel::B, 1), Channel::B, Storage::set, Grid::TOP, Grid::ChannelBottom());
    }
    else
    {
        DrawDataChannel(Storage::GetLimitation(Channel::B, 0), Channel::B, Storage::set, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(Channel::B, 1), Channel::B, Storage::set, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(Channel::A, 0), Channel::A, Storage::set, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(Channel::A, 1), Channel::A, Storage::set, Grid::TOP, Grid::ChannelBottom());
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
            if (PageMemory::PageInternal::showAlways)
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

    Primitives::Rectangle(Grid::Width(), Grid::FullHeight()).Draw(Grid::Left(), Grid::TOP, Color::FILL);
}


void Display::DrawTime(int x, int y)
{
    int dField = 10;
    int dSeparator = 2;

    PackedTime time = HAL_RTC::GetPackedTime();
    
    char buffer[20];
    
    Color::FILL.SetAsCurrent();
    
    if (MODE_WORK_IS_MEMINT || MODE_WORK_IS_LATEST)
    {
        DataSettings *ds = MODE_WORK_IS_MEMINT ? Storage::dsInt : Storage::dsLast;

        if (ds != 0)
        {
            y -= 9;
            time.day = ds->time.day;
            time.hours = ds->time.hours;
            time.minutes = ds->time.minutes;
            time.seconds = ds->time.seconds;
            time.month = ds->time.month;
            time.year = ds->time.year;
            Text(Int2String((int)time.day, false, 2, buffer)).Draw(x, y); //-V2533
            Text(":").Draw(x + dField, y);
            Text(Int2String((int)time.month, false, 2, buffer)).Draw(x + dField + dSeparator, y); //-V2533
            Text(":").Draw(x + 2 * dField + dSeparator, y);
            Text(Int2String((int)time.year + 2000, false, 4, buffer)).Draw(x + 2 * dField + 2 * dSeparator, y); //-V2533
            y += 9;
        }
        else
        {
            return;
        }
    }
    
    
    Text(Int2String((int)time.hours, false, 2, buffer)).Draw(x, y); //-V2533
    Text(":").Draw(x + dField, y);
    Text(Int2String((int)time.minutes, false, 2, buffer)).Draw(x + dField + dSeparator, y); //-V2533
    Text(":").Draw(x + 2 * dField + dSeparator, y);
    Text(Int2String((int)time.seconds, false, 2, buffer)).Draw(x + 2 * dField + 2 * dSeparator, y); //-V2533
}


void Display::DrawHiPart()
{
    WriteCursors();
    DrawHiRightPart();
}


// shiftForPeakDet - ���� ������ ���������� � �������� ��������� - �� ����� shiftForPeakDet ����� ����������� ����������� � ����������.
void Display::DrawDataInRect(int x, int width, pUCHAR data, int numElems, Channel::E chan, int shiftForPeakDet)
{
    if(numElems == 0)
    {
        return;
    }

    width--;
    float elemsInColumn = (float)numElems / (float)width; //-V2533
    uint8 min[300];
    uint8 max[300];


    if ((SET_TBASE >= TBase::_20ms) && (PEAKDET != PeackDetMode::Disable))
    {
        for (int col = 0; col < width; col++)
        {
            float firstElem = col * elemsInColumn; //-V2564
            float lastElem = firstElem + elemsInColumn - 1; //-V2564
            min[col] = data[(int)firstElem]; //-V2533 //-V2563
            max[col] = data[(int)firstElem + 1]; //-V2533 //-V2563
            for (int elem = static_cast<int>(firstElem) + 2; elem <= lastElem; elem += 2) //-V2564
            {
                SET_MIN_IF_LESS(data[elem], min[col]); //-V2563
                SET_MAX_IF_LARGER(data[elem + 1], max[col]); //-V2563
            }
        }
    }
    else if (shiftForPeakDet == 0)
    {
        uint8 *iMin = &min[0];
        uint8 *iMax = &max[0];

        for (int col = 0; col < width; col++, iMin++, iMax++) //-V2528
        {
            int firstElem = static_cast<int>(col * elemsInColumn); //-V2564
            int lastElem = firstElem + static_cast<int>(elemsInColumn) - 1;
            *iMin = data[firstElem]; //-V2563
            *iMax = data[firstElem]; //-V2563
            for (int elem = firstElem + 1; elem <= lastElem; elem++)
            {
                SET_MIN_IF_LESS(data[elem], *iMin); //-V2563
                SET_MAX_IF_LARGER(data[elem], *iMax); //-V2563
            }
        }
    }
    else        // � ����� �����, ���� ���. ���. �������
    {
        for (int col = 0; col < width; col++)
        {
            float firstElem = col * elemsInColumn; //-V2564
            float lastElem = firstElem + elemsInColumn - 1; //-V2564
            min[col] = data[(int)firstElem]; //-V2533 //-V2563
            max[col] = data[(int)firstElem + shiftForPeakDet]; //-V2533 //-V2563
            for (int elem = static_cast<int>(firstElem) + 1; elem <= lastElem; elem++) //-V2564
            {
                SET_MIN_IF_LESS(data[elem], min[col]); //-V2563
                SET_MAX_IF_LARGER(data[elem + shiftForPeakDet], max[col]); //-V2563
            }
        }
    }

    int bottom = 16;
    int height = 14;
    float scale = (float)height / (float)(MAX_VALUE - MIN_VALUE); //-V2533

#define ORDINATE(x) bottom - scale * LimitationInt((x) - MIN_VALUE, 0, 200)

#define NUM_POINTS (300 * 2)
    uint8 points[NUM_POINTS];

    points[0] = static_cast<uint8>(ORDINATE(max[0])); //-V2564
    points[1] = static_cast<uint8>(ORDINATE(min[0])); //-V2564



    for(int i = 1; i < width; i++)
    {
        int value0 = min[i] > max[i - 1] ? max[i - 1] : min[i];
        int value1 = max[i] < min[i - 1] ? min[i - 1] : max[i];
        points[i * 2] = static_cast<uint8>(ORDINATE(value1)); //-V2564
        points[i * 2 + 1] = static_cast<uint8>(ORDINATE(value0)); //-V2564
    }
	if(width < 256)
    {
        VLineArray().Draw(x, width, points, Color::Channel(chan));
	}
    else
    {
        VLineArray().Draw(x, 255, points, Color::Channel(chan));
        VLineArray().Draw(x + 255, width - 255, points + 255 * 2, Color::Channel(chan)); //-V2563
	}
}



// shiftForPeakDet - ���� ������ ���������� � �������� ��������� - �� ����� shiftForPeakDet ����� ����������� ����������� � ����������.
void Display::DrawChannelInWindowMemory(int timeWindowRectWidth, int xVert0, int xVert1, int startI, int endI, pUCHAR data, int rightX, Channel::E chan, int shiftForPeakDet)
{
    if(data == dataP2P_0 && data == dataP2P_1)
    {

    }
    else
    {
        DrawDataInRect(1,          xVert0 - 1,              &(data[0]),        startI,                             chan, shiftForPeakDet); //-V2563
        DrawDataInRect(xVert0 + 2, timeWindowRectWidth - 2, &(data[startI]),   281,                                chan, shiftForPeakDet); //-V2563
        DrawDataInRect(xVert1 + 2, rightX - xVert1 + 2,     &(data[endI + 1]), sMemory_GetNumPoints(false) - endI, chan, shiftForPeakDet); //-V2563
    }
}



void Display::DrawMemoryWindow()
{
    uint8 *dat0 = Storage::dataIntA;
    uint8 *dat1 = Storage::dataIntB;
    DataSettings *ds = Storage::dsInt;
    
    if(MODE_WORK_IS_DIRECT || MODE_WORK_IS_LATEST)
    {
        dat0 = Storage::dataA;
        dat1 = Storage::dataB;
        ds = Storage::set;
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

    int timeWindowRectWidth = static_cast<int>((rightX - leftX) * (282.0F / sMemory_GetNumPoints(false))); //-V2564
    float scaleX = (float)(rightX - leftX + 1) / sMemory_GetNumPoints(false); //-V2533 //-V2564

    int16 shiftInMemory = SHIFT_IN_MEMORY;
    
    const int xVert0 = static_cast<int>(leftX + shiftInMemory * scaleX); //-V2564
    const int xVert1 = leftX + static_cast<int>(shiftInMemory * scaleX) + timeWindowRectWidth; //-V2564
    bool showFull = set.display.showFullMemoryWindow;
    Primitives::Rectangle(xVert1 - xVert0, bottom - top - (showFull ? 0 : 2)).Draw(xVert0, top + (showFull ? 0 : 1), Color::FILL);

    if(!dataP2PIsEmpty)
    {
        dat0 = dataP2P_0;
        dat1 = dataP2P_1;
    }

    if (showFull)
    {
        if (Storage::dataA || Storage::dataB || (!dataP2PIsEmpty))
        {
            int startI = shiftInMemory;
            int endI = startI + 281;

            Channel::E chanFirst = LAST_AFFECTED_CHANNEL_IS_A ? Channel::B : Channel::A;
            Channel::E chanSecond = LAST_AFFECTED_CHANNEL_IS_A ? Channel::A : Channel::B;
            pUCHAR dataFirst = LAST_AFFECTED_CHANNEL_IS_A ? dat1 : dat0;
            pUCHAR dataSecond = LAST_AFFECTED_CHANNEL_IS_A ? dat0 : dat1;

            int shiftForPeakDet = ds->peakDet == PeackDetMode::Disable ? 0 : (int)ds->length1channel; //-V2533

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
        HLine().Draw(leftX - 2, top, bottom, Color::FILL);
        HLine().Draw(rightX + 2, top, bottom);
        HLine().Draw((bottom + top) / 2 - 3, leftX, xVert0 - 2);
        HLine().Draw((bottom + top) / 2 + 3, leftX, xVert0 - 2);
        HLine().Draw((bottom + top) / 2 + 3, xVert1 + 2, rightX);
        HLine().Draw((bottom + top) / 2 - 3, xVert1 + 2, rightX);
    }

    int x[] = {leftX, (rightX - leftX) / 2 + leftX + 1, rightX};
    int x0 = x[SET_TPOS];

    // ������ TPos
    Region(6, 6).Fill(x0 - 3, 9, Color::BACK);
    Char(Symbol::S8::TPOS_1).Draw(x0 - 3, 9, Color::FILL);

    // ������ tShift
    float scale = (float)(rightX - leftX + 1) / ((float)sMemory_GetNumPoints(false) - (sMemory_GetNumPoints(false) == 281 ? 1 : 0)); //-V2533 //-V2564
    float xShift = 1 + (sTime_TPosInPoints((PeackDetMode::E)Storage::set->peakDet, (int)Storage::set->length1channel, SET_TPOS) - sTime_TShiftInPoints((PeackDetMode::E)Storage::set->peakDet)) * scale; //-V2533 //-V2564
    
    if(xShift < leftX - 2) //-V2564
    {
        xShift = static_cast<float>(leftX - 2);
    }

    Region(6, 6).Fill(static_cast<int>(xShift - 1), 3, Color::BACK); //-V2564
    Region(4, 4).Fill(static_cast<int>(xShift), 4, Color::FILL);
    Color::BACK.SetAsCurrent();

    if(xShift == leftX - 2) //-V2550 //-V550 //-V2564
    {
        xShift = static_cast<float>(leftX - 2);
        Line().Draw(static_cast<int>(xShift) + 3, 5, static_cast<int>(xShift) + 3, 7);
        Line().Draw(static_cast<int>(xShift) + 1, 6, static_cast<int>(xShift) + 2, 6);
    }
    else if(xShift > rightX - 1) //-V2564
    {
        xShift = static_cast<float>(rightX - 2);
        Line().Draw(static_cast<int>(xShift) + 1, 5, static_cast<int>(xShift) + 1, 7);
        Line().Draw(static_cast<int>(xShift) + 2, 6, static_cast<int>(xShift) + 3, 6);
    }
    else
    {
        Line().Draw(static_cast<int>(xShift) + 1, 5, static_cast<int>(xShift) + 3, 5);
        Line().Draw(static_cast<int>(xShift) + 2, 6, static_cast<int>(xShift) + 2, 7);
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

        HLine().Draw(x, 1, Grid::TOP - 2, Color::FILL);
        x += 3;
        Channel::E source = CURS_SOURCE;
        Color colorText = Color::Channel(source);
        if(!CURS_CNTRL_U_IS_DISABLE(source))
        {
            Text("1:").Draw(x, y1, colorText);
            Text("2:").Draw(x, y2);
            x += 7;
            Text(sCursors_GetCursVoltage(source, 0, buffer)).Draw(x, y1);
            Text(sCursors_GetCursVoltage(source, 1, buffer)).Draw(x, y2);
            x = startX + 49;
            float pos0 = MathFPGA::VoltageCursor(sCursors_GetCursPosU(source, 0), SET_RANGE(source), SET_RSHIFT(source));
            float pos1 = MathFPGA::VoltageCursor(sCursors_GetCursPosU(source, 1), SET_RANGE(source), SET_RSHIFT(source));
            float delta = std::fabsf(pos1 - pos0);
            Text(":dU=").Draw(x, y1);
            Text(Voltage2String(delta, false, buffer)).Draw(x + 17, y1);
            Text(":").Draw(x, y2);
            Text(sCursors_GetCursorPercentsU(source, buffer)).Draw(x + 10, y2);
        }

        x = startX + 101;
        HLine().Draw(x, 1, Grid::TOP - 2, Color::FILL);
        x += 3;
        if(!CURS_CNTRL_T_IS_DISABLE(source))
        {
            colorText.SetAsCurrent();
            Text("1:").Draw(x, y1);
            Text("2:").Draw(x, y2);
            x+=7;
            Text(sCursors_GetCursorTime(source, 0, buffer)).Draw(x, y1);
            Text(sCursors_GetCursorTime(source, 1, buffer)).Draw(x, y2);
            x = startX + 153;
            float pos0 = MathFPGA::TimeCursor(CURS_POS_T0(source), SET_TBASE);
            float pos1 = MathFPGA::TimeCursor(CURS_POS_T1(source), SET_TBASE);
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
                Primitives::Rectangle(width, 12).Draw(x0, Grid::TOP, Color::FILL);
                Region(width - 2, 10).Fill(x0 + 1, Grid::TOP + 1, Color::BACK);
                Text("1/dT=").Draw(x0 + 1, Grid::TOP + 2, colorText);
                char buff[20];
                Text(Freq2String(1.0F / delta, false, buff)).Draw(x0 + 25, Grid::TOP + 2);
            }
        }
    }
}


void Display::DrawHiRightPart()
    {
    // �������������
    int y = 2;

    static const int xses[3] = {280, 271, 251};
    int x = xses[MODE_WORK];

    if (!MODE_WORK_IS_LATEST)
    {
        HLine().Draw(x, 1, Grid::TOP - 2, Color::FILL);

        x += 2;

        if (trigEnable)
        {
            Region(Grid::TOP - 3, Grid::TOP - 7).Fill(x, 1 + y);
            Text(LANG_RU ? "��" : "Tr").Draw(x + 3, 3 + y, Color::BACK);
        }
    }

    // ����� ������
    static const char *strings_[][2] =
    {
        {"���",     "MEAS"},
        {"����",    "LAST"},
        {"����",    "INT"}
    };

    if(!MODE_WORK_IS_DIRECT)
    {
        x += 18;
        HLine().Draw(x, 1, Grid::TOP - 2, Color::FILL);
        x += 2;
        Text(LANG_RU ? "�����" : "mode").Draw(LANG_RU ? x : x + 3, -1);
        Text(strings_[MODE_WORK][LANG]).DrawInCenterRect(x + 1, 9, 25, 8);
    }
    else
    {
        x -= 9;
    }

    if (!MODE_WORK_IS_LATEST)
    {

        x += 27;
        HLine().Draw(x, 1, Grid::TOP - 2, Color::FILL);

        x += 2;
        y = 1;
        if (FPGA::CurrentStateWork() == StateWorkFPGA::Work)
        {
            Char(Symbol::S8::PLAY).Draw4SymbolsInRect(x, 1);
        }
        else if (FPGA::CurrentStateWork() == StateWorkFPGA::Stop)
        {
            Region(10, 10).Fill(x + 3, y + 3);
        }
        else if (FPGA::CurrentStateWork() == StateWorkFPGA::Wait) //-V2516
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

    if (needFinishDraw)
    {
        needFinishDraw = false;
        return true;
    }

    if (MODE_ACCUM_IS_RESET && (numDrawingSignals >= static_cast<uint>(numAccum))) //-V560
    {
        numDrawingSignals = 0;
        return true;
    }
    return false;
}


static void CalculateCoord(int &x, int &y)
{
    x++;

    if (x > 200)
    {
        x = 0;
    }

    y++;

    if (y > 200)
    {
        y = 0;
    }
}


void Display::Update(bool endScene)
{
    static uint prev_time = 0;

    if (HAL_TIM2::TimeMS() - prev_time < 100)
    {
        return;
    }

    static int x = 0;
    static int y = 0;

    CalculateCoord(x, y);

    Painter::BeginScene(Color::BLACK);

    Region(40, 40).Fill(x, y, Color::WHITE);

//    Painter::EndScene();

    prev_time = HAL_TIM2::TimeMS();

    return;
    
    
	uint timeStart = gTimerTics;
    if (funcOnHand != 0)
    {
        funcOnHand();
        return;
    }

    bool needClear = NeedForClearScreen();

    if (needClear)
    {
        Painter::BeginScene(Color::BACK);
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

    Color::FILL.SetAsCurrent();

    Painter::EndScene(endScene);

    if (PageMemory::needForSaveToFlashDrive)
    {
        if (Painter::SaveScreenToFlashDrive())
        {
            ShowWarningGood(Warning::FileIsSaved);
        }
        PageMemory::needForSaveToFlashDrive = false;
    }

    if (funcAfterDraw)
    {
        funcAfterDraw();
        funcAfterDraw = 0;
    }
}


void Display::WriteValueTrigLevel()
{
    if (showLevelTrigLev && MODE_WORK_IS_DIRECT)
    {
        float trigLev = RSHIFT_2_ABS(TRIG_LEVEL_SOURCE, SET_RANGE(TRIG_SOURCE));     // WARN ����� ��� ������� ������������� ����������� �������������� �������.
        TrigSource::E trigSource = TRIG_SOURCE;
        if (TRIG_INPUT_IS_AC && trigSource <= TrigSource::B)
        {
            int16 rShift = SET_RSHIFT(trigSource);
            float rShiftAbs = RSHIFT_2_ABS(rShift, SET_RANGE(trigSource)); //-V2533
            trigLev += rShiftAbs;
        }
        char buffer[20];
        std::strcpy(buffer, LANG_RU ? "�� ����� = " : "Trig lvl = "); //-V2513
        char bufForVolt[20];
        std::strcat(buffer, Voltage2String(trigLev, true, bufForVolt)); //-V2513
        int width = 96;
        int x = (Grid::Width() - width) / 2 + Grid::Left();
        int y = Grid::BottomMessages() - 20;
        Primitives::Rectangle(width, 10).Draw(x, y, Color::FILL);
        Region(width - 2, 8).Fill(x + 1, y + 1, Color::BACK);
        Text(buffer).Draw(x + 2, y + 1, Color::FILL);
    }
}


void Display::DrawGridSpectrum()
{
    if (SCALE_FFT_IS_LOG)
    {
        static const int nums[] = {4, 6, 8};
        static const char *strs[] = {"0", "-10", "-20", "-30", "-40", "-50", "-60", "-70"};
        int numParts = nums[FFT_MAX_DB];
        float scale = (float)Grid::MathHeight() / numParts; //-V2533 //-V2564
        for (int i = 1; i < numParts; i++)
        {
            int y = static_cast<int>(Grid::MathTop() + i * scale); //-V2564
            HLine().Draw(y, Grid::Left(), Grid::Left() + 256, Color::GRID);
            if (!Menu::IsMinimize())
            {
                Color::FILL.SetAsCurrent();
                Text(strs[i]).Draw(3, y - 4);
            }
        }
        if (!Menu::IsMinimize())
        {
            Color::FILL.SetAsCurrent();
            Text("��").Draw(5, Grid::MathTop() + 1);
        }
    }
    else if (SCALE_FFT_IS_LINEAR) //-V2516
    {
        static const char *strs[] = {"1.0", "0.8", "0.6", "0.4", "0.2"};
        float scale = (float)Grid::MathHeight() / 5; //-V2533 //-V2564
        for (int i = 1; i < 5; i++)
        {
            int y = static_cast<int>(Grid::MathTop() + i * scale); //-V2564
            HLine().Draw(y, Grid::Left(), Grid::Left() + 256, Color::GRID);
            if (!Menu::IsMinimize())
            {
                Text(strs[i]).Draw(5, y - 4, Color::FILL);
            }
        }
    }
    HLine().Draw(Grid::Left() + 256, Grid::MathTop(), Grid::MathBottom(), Color::FILL);
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
        HLine().Draw(Grid::TOP + Grid::FullHeight() / 2, Grid::Left(), Grid::Left() + Grid::Width(), Color::FILL);
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
    masX[0] = (uint16)(left + 1); //-V2533
    for (int i = 1; i < 7; i++)
    {
        masX[i] = static_cast<uint16>(left + deltaX * i); //-V2564
    }
    for (int i = 7; i < 10; i++)
    {
        masX[i] = static_cast<uint16>(centerX - 8 + i); //-V2564
    }
    for (int i = 10; i < 16; i++)
    {
        masX[i] = static_cast<uint16>(centerX + deltaX * (i - 9)); //-V2564
    }
    masX[16] = (uint16)(right - 1); //-V2533

    MultiVPointLine(17, masX, static_cast<int>(stepY), CalculateCountV()).Draw(static_cast<int>(top + stepY), Color::GRID); //-V2564

    uint8 mas[13];
    mas[0] = (uint8)(top + 1); //-V2533
    for (int i = 1; i < 5; i++)
    {
        mas[i] = static_cast<uint8>(top + deltaY * i); //-V2564
    }
    for (int i = 5; i < 8; i++)
    {
        mas[i] = static_cast<uint8>(centerY - 6 + i); //-V2564
    }
    for (int i = 8; i < 12; i++)
    {
        mas[i] = static_cast<uint8>(centerY + deltaY * (i - 7)); //-V2564
    }
    mas[12] = (uint8)(bottom - 1); //-V2533

    MultiHPointLine(13, mas, static_cast<int>(stepX), CalculateCountH()).Draw(static_cast<int>(left + stepX), Color::GRID); //-V2564
}



void Display::DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY)
{ 
    uint16 masX[15];
    masX[0] = (uint16)(left + 1); //-V2533
    for (int i = 1; i < 14; i++)
    {
        masX[i] = (uint16)(left + deltaX * i); //-V2533
    }
    masX[14] = (uint16)(right - 1); //-V2533
    MultiVPointLine(15, masX, stepY, CalculateCountV()).Draw(top + stepY, Color::GRID);

    uint8 mas[11];
    mas[0] = (uint8)(top + 1); //-V2533
    for (int i = 1; i < 10; i++)
    {
        mas[i] = (uint8)(top + deltaY * i); //-V2533
    }
    mas[10] = (uint8)(bottom - 1); //-V2533
    MultiHPointLine(11, mas, stepX, CalculateCountH()).Draw(left + stepX, Color::GRID);
}

void Display::DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY, int stepX, int stepY)
{
    HPointLine(static_cast<float>(stepX)).Draw(centerY, left + stepX, right);
    uint8 masY[6] = {(uint8)(top + 1), (uint8)(top + 2), (uint8)(centerY - 1), (uint8)(centerY + 1), (uint8)(bottom - 2), (uint8)(bottom - 1)}; //-V2533
    MultiHPointLine(6, masY, deltaX, (right - top) / deltaX).Draw(left + deltaX, Color::GRID);
    VPointLine(static_cast<float>(stepY)).Draw(centerX, top + stepY, bottom, Color::GRID);
    uint16 masX[6] = {(uint16)(left + 1), (uint16)(left + 2), (uint16)(centerX - 1), (uint16)(centerX + 1), (uint16)(right - 2), (uint16)(right - 1)}; //-V2533
    MultiVPointLine(6, masX, deltaY, (bottom - top) / deltaY).Draw(top + deltaY, Color::GRID);
}

void Display::DrawGrid(int left, int top, int width, int height)
{
    int right = left + width;
    int bottom = top + height;

    Color::FILL.SetAsCurrent();

    if (top == Grid::TOP)
    {
        HLine().Draw(top, 1, left - 2);
        HLine().Draw(top, right + 2, WIDTH - 2);

        if (!Menu::IsMinimize() || !Menu::IsShown())
        {
            VLine().Draw(1, top + 2, bottom - 2);
            VLine().Draw(318, top + 2, bottom - 2);
        }
    }

    float deltaX = Grid::DeltaX() *(float)width / width; //-V2533 //-V2564
    float deltaY = Grid::DeltaY() * (float)height / height; //-V2533 //-V2564
    float stepX = deltaX / 5; //-V2564
    float stepY = deltaY / 5; //-V2564
    
    int centerX = left + width / 2;
    int centerY = top + height / 2;

    Color::GRID.SetAsCurrent();
    if (TYPE_GRID_IS_1)
    {
        DrawGridType1(left, top, right, bottom, static_cast<float>(centerX), static_cast<float>(centerY), deltaX, deltaY, stepX, stepY);
    }
    else if (TYPE_GRID_IS_2)
    {
        DrawGridType2(left, top, right, bottom, static_cast<int>(deltaX), static_cast<int>(deltaY), static_cast<int>(stepX), static_cast<int>(stepY));
    }
    else if (TYPE_GRID_IS_3) //-V2516
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
        Line().Draw(x + 1, levels[i], x2 - 1, levels[i], Color::FILL);
    }
}


void Display::DrawCursorsWindow()
{
    if((!Menu::IsMinimize() || !Menu::IsShown()) && drawRShiftMarkers)
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
    float scale = 1.0F / ((TrigLevMax - TrigLevMin) / 2.0F / Grid::ChannelHeight()); //-V2564
    int y0 = static_cast<int>((Grid::TOP + Grid::ChannelBottom()) / 2 + scale * (TrigLevZero - TrigLevMin)); //-V2564
    int y = static_cast<int>(y0 - scale * (trigLev - TrigLevMin)); //-V2564

    y = (y - Grid::ChannelCenterHeight()) + Grid::ChannelCenterHeight();

    int x = Grid::Right();
    Color::Trig().SetAsCurrent();
    if(y > Grid::ChannelBottom())
    {
        Char(Symbol::S8::TRIG_LEV_LOWER).Draw(x + 3, Grid::ChannelBottom() - 11);;
        Point().Draw(x + 5, Grid::ChannelBottom() - 2);
        y = Grid::ChannelBottom() - 7;
        x--;
    }
    else if(y < Grid::TOP)
    {
        Char(Symbol::S8::TRIG_LEV_ABOVE).Draw(x + 3, Grid::TOP + 2);
        Point().Draw(x + 5, Grid::TOP + 2);
        y = Grid::TOP + 7;
        x--;
    }
    else
    {
        Char(Symbol::S8::TRIG_LEV_NORMAL).Draw(x + 1, y - 4);
    }
    Font::Set(TypeFont::S5);

    const char simbols[3] = {'1', '2', '�'};

    Char(simbols[TRIG_SOURCE]).Draw(x + 5, y - 9, Color::BACK);
    Font::Set(TypeFont::S8);

    if (drawRShiftMarkers && !Menu::IsMinimize())
    {
        DrawScaleLine(WIDTH - 11, true);
        int left = Grid::Right() + 9;
        int height = Grid::ChannelHeight() - 2 * DELTA;
        int shiftFullMin = RShiftMin + TrigLevMin;
        int shiftFullMax = RShiftMax + TrigLevMax;
        scale = (float)height / (shiftFullMax - shiftFullMin); //-V2533 //-V2564
        int shiftFull = TRIG_LEVEL_SOURCE + (TRIG_SOURCE_IS_EXT ? 0 : SET_RSHIFT(chan));
        int yFull = static_cast<int>(Grid::TOP + DELTA + height - scale * (shiftFull - RShiftMin - TrigLevMin) - 4); //-V2564
        Region(4, 6).Fill(left + 2, yFull + 1, Color::Trig());
        Font::Set(TypeFont::S5);
        Char(simbols[TRIG_SOURCE]).Draw(left + 3, yFull - 5, Color::BACK);
        Font::Set(TypeFont::S8);
    }
}


void Display::DrawCursorRShift(Channel::E chan)
{
    float x = static_cast<float>(Grid::Right() - Grid::Width() - Measure::GetDeltaGridLeft());

    if (chan == Channel::Math)
    {
        int rShift = SET_RSHIFT_MATH;
        float scale = (float)Grid::MathHeight() / 960; //-V2533 //-V2564
        float y = (Grid::MathTop() + Grid::MathBottom()) / 2.0F - scale * (rShift - RShiftZero); //-V2564
        Char(Symbol::S8::RSHIFT_NORMAL).Draw(static_cast<int>(x - 9), static_cast<int>(y - 4), Color::FILL); //-V2564
        Char('m').Draw(static_cast<int>(x - 8), static_cast<int>(y - 5), Color::BACK); //-V2564
        return;
    }
    if(!sChannel_Enabled(chan))
    {
        return;
    }

    int rShift = SET_RSHIFT(chan);
 
    float scale = Grid::ChannelHeight() / (STEP_RSHIFT * 200.0F); //-V2564
    float y = Grid::ChannelCenterHeight() - scale * (rShift - RShiftZero); //-V2564

    if(y > Grid::ChannelBottom()) //-V2564
    {
        Char(Symbol::S8::RSHIFT_LOWER).Draw(static_cast<int>(x - 7), Grid::ChannelBottom() - 11, Color::Channel(chan)); //-V2564
        Point().Draw(static_cast<int>(x - 5), Grid::ChannelBottom() - 2); //-V2564
        y = static_cast<float>(Grid::ChannelBottom() - 7);
        x++;
    }
    else if(y < Grid::TOP) //-V2564
    {
        Char(Symbol::S8::RSHIFT_ABOVE).Draw(static_cast<int>(x - 7), Grid::TOP + 2, Color::Channel(chan)); //-V2564
        Point().Draw(static_cast<int>(x - 5), Grid::TOP + 2); //-V2564
        y = Grid::TOP + 7; //-V2564
        x++;
    }
    else
    {
        Char(Symbol::S8::RSHIFT_NORMAL).Draw(static_cast<int>(x - 8), static_cast<int>(y - 4), Color::Channel(chan)); //-V2564
        if(((chan == Channel::A) ? showLevelRShiftA : showLevelRShiftB) && MODE_WORK_IS_DIRECT) //-V2570
        {
            DashedHLine(7, 3).Draw(static_cast<int>(y), Grid::Left(), Grid::Right(), 0);
        }
    }

    Font::Set(TypeFont::S5);

    if((!Menu::IsMinimize() || !Menu::IsShown()) && drawRShiftMarkers)
    {
        float scaleFull = (float)Grid::ChannelHeight() / (RShiftMax - RShiftMin) * (sService_MathEnabled() ? 0.9F : 0.91F); //-V2533 //-V2564
        float yFull = Grid::ChannelCenterHeight() - scaleFull * (rShift - RShiftZero); //-V2564

        Region(4, 6).Fill(4, static_cast<int>(yFull - 3), Color::Channel(chan)); //-V2564
        Char(chan == Channel::A ? '1' : '2').Draw(5, static_cast<int>(yFull - 9), Color::BACK); //-V2564
    }
    Char(chan == Channel::A ? '1' : '2').Draw(static_cast<int>(x - 7), static_cast<int>(y - 9), Color::BACK); //-V2564
    Font::Set(TypeFont::S8);
}


void Display::DrawCursorTShift()
{
    int firstPoint = 0;
    int lastPoint = 0;
    SettingsDisplay::PointsOnDisplay(&firstPoint, &lastPoint);

    if (Storage::set == nullptr)
    {
        return;
    }

    // ������ TPos
    int shiftTPos = sTime_TPosInPoints((PeackDetMode::E)Storage::set->peakDet, (int)Storage::set->length1channel, SET_TPOS) - SHIFT_IN_MEMORY; //-V2533
    float scale = static_cast<float>((lastPoint - firstPoint) / Grid::Width());
    int gridLeft = Grid::Left();
    int x = static_cast<int>(gridLeft + shiftTPos * scale - 3); //-V2564
    if (IntInRange(x + 3, gridLeft, Grid::Right() + 1))
    {
        Char(Symbol::S8::TPOS_2).Draw2SymbolsInPosition(x, Grid::TOP - 1, Symbol::S8::TPOS_3, Color::BACK, Color::FILL);
    };

    // ������ tShift
    int shiftTShift = sTime_TPosInPoints((PeackDetMode::E)Storage::set->peakDet, (int)Storage::set->length1channel, SET_TPOS) - sTime_TShiftInPoints((PeackDetMode::E)Storage::set->peakDet); //-V2533
    if(IntInRange(shiftTShift, firstPoint, lastPoint))
    {
        x = gridLeft + shiftTShift - firstPoint - 3;
        Char(Symbol::S8::TSHIFT_NORM_1).Draw2SymbolsInPosition(x, Grid::TOP - 1, Symbol::S8::TSHIFT_NORM_2, Color::BACK, Color::FILL);
    }
    else if(shiftTShift < firstPoint)
    {
        Char(Symbol::S8::TSHIFT_LEFT_1).Draw2SymbolsInPosition(gridLeft + 1, Grid::TOP, Symbol::S8::TSHIFT_LEFT_2, Color::BACK, Color::FILL);
        Line().Draw(Grid::Left() + 9, Grid::TOP + 1, Grid::Left() + 9, Grid::TOP + 7, Color::BACK);
    }
    else if(shiftTShift > lastPoint) //-V2516
    {
        Char(Symbol::S8::TSHIFT_RIGHT_1).Draw2SymbolsInPosition(Grid::Right() - 8, Grid::TOP, Symbol::S8::TSHIFT_RIGHT_2, Color::BACK, Color::FILL);
        Line().Draw(Grid::Right() - 9, Grid::TOP + 1, Grid::Right() - 9, Grid::TOP + 7, Color::BACK);
    }
}


void Display::DrawHorizontalCursor(int y, int xTearing)
{
    y += Grid::TOP;
    if(xTearing == -1)
    {
        DashedHLine(1, 1).Draw(y, Grid::Left() + 2, Grid::Right() - 1, 0);
    }
    else
    {
        DashedHLine(1, 1).Draw(y, Grid::Left() + 2, xTearing - 2, 0);
        DashedHLine(1, 1).Draw(y, xTearing + 2, Grid::Right() - 1, 0);
    }
    Primitives::Rectangle(2, 2).Draw(Grid::Left() - 1, y - 1);
    Primitives::Rectangle(2, 2).Draw(Grid::Right() - 1, y - 1);
}


void Display::DrawVerticalCursor(int x, int yTearing)
{
    x += Grid::Left();
    if(yTearing == -1)
    {
        DashedVLine(1, 1).Draw(x, Grid::TOP + 2, Grid::ChannelBottom() - 1, 0);
    }
    else
    {
        DashedVLine(1, 1).Draw(x, Grid::TOP + 2, yTearing - 2, 0);
        DashedVLine(1, 1).Draw(x, yTearing + 2, Grid::ChannelBottom() - 1, 0);
    }
    Primitives::Rectangle(2, 2).Draw(x - 1, Grid::TOP - 1);
    Primitives::Rectangle(2, 2).Draw(x - 1, Grid::ChannelBottom() - 1);
}


void Display::DrawCursors()
{
    Channel::E source = CURS_SOURCE;
    Color::Cursors(source).SetAsCurrent();
    if (sCursors_NecessaryDrawCursors())
    {
        bool bothCursors = !CURS_CNTRL_T_IS_DISABLE(source) && !CURS_CNTRL_U_IS_DISABLE(source);  // ������� ����, ��� �������� � ������������ � 
                                                                            // �������������� ������� - ���� ���������� �������� � ������ �����������
        int x0 = -1;
        int x1 = -1;
        int y0 = -1;
        int y1 = -1;

        if (bothCursors)
        {
            x0 = static_cast<int>(Grid::Left() + CURS_POS_T0(source)); //-V2564
            x1 = static_cast<int>(Grid::Left() + CURS_POS_T1(source)); //-V2564
            y0 = static_cast<int>(Grid::TOP + sCursors_GetCursPosU(source, 0)); //-V2564
            y1 = static_cast<int>(Grid::TOP + sCursors_GetCursPosU(source, 1)); //-V2564

            Primitives::Rectangle(4, 4).Draw(x0 - 2, y0 - 2);
            Primitives::Rectangle(4, 4).Draw(x1 - 2, y1 - 2);
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
        topMeasures = Grid::Bottom();
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
        Primitives::Rectangle(x1 - x0, y1 - y0).Draw(x0, y0, Color::FILL);
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
            Measure::E meas = Measure::Type(str, elem);
            if(meas != Measure::None)
            {
                Region(dX, dY).Fill(x, y, Color::BACK);
                Primitives::Rectangle(dX, dY).Draw(x, y, Color::FILL);
                topMeasures = Math::MinFrom2Int(topMeasures, y);
            }
            if(active)
            {
                Region(dX - 4, dY - 4).Fill(x + 2, y + 2, Color::FILL);
            }
            if(meas != Measure::None)
            {
                char buffer[20];
                Text(Measure::Name(str, elem)).Draw(x + 4, y + 2, active ? Color::BACK : Color::FILL);
                if(meas == MEAS_MARKED)
                {
                    Region(dX - 2, 9).Fill(x + 1, y + 1, active ? Color::BACK : Color::FILL);
                    Text(Measure::Name(str, elem)).Draw(x + 4, y + 2, active ? Color::FILL : Color::BACK);
                }
                if(MEAS_SOURCE_IS_A)
                {
                    Text(Processing::GetStringMeasure(meas, Channel::A, buffer)).Draw(x + 2, y + 11, Color::Channel(Channel::A));
                }
                else if(MEAS_SOURCE_IS_B)
                {
                    Text(Processing::GetStringMeasure(meas, Channel::B, buffer)).Draw(x + 2, y + 11, Color::Channel(Channel::B));
                }
                else
                {
                    Text(Processing::GetStringMeasure(meas, Channel::A, buffer)).Draw(x + 2, y + 11, Color::Channel(Channel::A));
                    Text(Processing::GetStringMeasure(meas, Channel::B, buffer)).Draw(x + 2, y + 20, Color::Channel(Channel::B));
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

    bool inverse = SET_INVERSE(chan);
    ModeCouple::E modeCouple = SET_COUPLE(chan);
    Divider::E multiplier = SET_DIVIDER(chan);
    Range::E range = SET_RANGE(chan);
    uint rShift = (uint)SET_RSHIFT(chan);
    bool enable = SET_ENABLED(chan);

    if (!MODE_WORK_IS_DIRECT)
    {
        DataSettings *ds = MODE_WORK_IS_DIRECT ? Storage::set : Storage::dsInt;
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
        Color color = Color::Channel(chan);
        Color colorDraw = inverse ? Color::WHITE : color;

        if(inverse)
        {
            const int widthField = 91;
            const int heightField = 8;
            Region(widthField, heightField).Fill(x, y, color);
        }

        char buffer[100] = {0};

        std::sprintf(buffer, "%s\xa5%s\xa5%s", (chan == Channel::A) ? (LANG_RU ? "1�" : "1c") : (LANG_RU ? "2�" : "2c"), couple[modeCouple],
            sChannel_Range2String(range, multiplier));

        Text(buffer).Draw(x + 1, y, colorDraw);

        char bufferTemp[20];
        std::sprintf(buffer, "\xa5%s", sChannel_RShift2String((int16)rShift, range, multiplier, bufferTemp)); //-V2533
        Text(buffer).Draw(x + 46, y);
    }
}



void Display::WriteStringAndNumber(pCHAR text, int x, int y, int number)
{
    char buffer[100];
    Text(text).Draw(x, y, Color::FILL);
    if(number == 0)
    {
        std::sprintf(buffer, "-");
    }
    else
    {
        std::sprintf(buffer, "%d", number);
    }
    Text(buffer).DrawRelativelyRight(x + 41, y);
}



void Display::DrawLowPart()
{
    int y0 = HEIGHT - 19;
    int y1 = HEIGHT - 10;
    int x = -1;

    HLine().Draw(Grid::ChannelBottom(), 1, Grid::Left() - Measure::GetDeltaGridLeft() - 2, Color::FILL);
    HLine().Draw(Grid::FullBottom(), 1, Grid::Left() - Measure::GetDeltaGridLeft() - 2);

    WriteTextVoltage(Channel::A, x + 2, y0);

    WriteTextVoltage(Channel::B, x + 2, y1);

    VLine().Draw(x + 95, Grid::Bottom() + 2, HEIGHT - 2, Color::FILL);

    x += 98;
    char buffer[100] = {0};

    TBase::E tBase = SET_TBASE;
    int16 tShift = TSHIFT;

    if (!MODE_WORK_IS_DIRECT)
    {
        DataSettings *ds = MODE_WORK_IS_LATEST ? Storage::dsLast : Storage::dsInt;
        if (ds != 0)
        {
            tBase = ds->tBase;
            tShift = ds->tShift;
        }
    }

    std::sprintf(buffer, "�\xa5%s", Tables::GetTBaseString(tBase));
    Text(buffer).Draw(x, y0);

    buffer[0] = 0;
    char bufForVal[20];
    std::sprintf(buffer, "\xa5%s", FPGA::GetTShiftString(tShift, bufForVal));
    Text(buffer).Draw(x + 35, y0);

    buffer[0] = 0;
    if (MODE_WORK_IS_DIRECT)
    {
        const char *source[3] = { "1", "2", "\x82" };
        std::sprintf(buffer, "�\xa5\x10%s", source[TRIG_SOURCE]);
    }

    Text(buffer).Draw(x, y1, Color::Trig());

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
        Char(filtr[TRIG_INPUT][0]).Draw(x + 45, y1); //-V2563
        Char(filtr[TRIG_INPUT][1]).Draw(x + 53, y1); //-V2563
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
    
    VLine().Draw(x + 79, Grid::Bottom() + 2, HEIGHT - 2, Color::FILL);

    HLine().Draw(Grid::Bottom(), Grid::Right() + 2, WIDTH - 2);
    HLine().Draw(Grid::ChannelBottom(), Grid::Right() + 2, WIDTH - 2);

    x += 82;
    y0 = y0 - 3;
    y1 = y1 - 6;
    Font::Set(TypeFont::S5);
    
    if (MODE_WORK_IS_DIRECT)
    {
        int y2 = y1 + 6;
        WriteStringAndNumber("������", x, y0, NUM_ACCUM);
        WriteStringAndNumber("������", x, y1, NUM_AVE);
        WriteStringAndNumber("��\x93���", x, y2, NUM_MIN_MAX);
    }

    x += 42;
    VLine().Draw(x, Grid::Bottom() + 2, HEIGHT - 2);

    Font::Set(TypeFont::S8);

    if (MODE_WORK_IS_DIRECT)
    {
        char mesFreq[20] = "\x7c=";
        char buf[20];
        float freq = FPGA::GetFreq();
        if (freq == -1.0F) //-V550 //-V2550
        {
            std::strcat(mesFreq, "******"); //-V2513
        }
        else
        {
            std::strcat(mesFreq, Freq2String(freq, false, buf)); //-V2513
        }
        Text(mesFreq).Draw(x + 3, Grid::Bottom() + 2);
    }

    DrawTime(x + 3, Grid::Bottom() + 11);

    HLine().Draw(x + 55, Grid::Bottom() + 2, HEIGHT - 2);

    Font::Set(TypeFont::UGO2);

    // ������
    if (FDrive::isConnected)
    {
        Char(Symbol::S8::FLASH_DRIVE).Draw4SymbolsInRect(x + 57, Grid::Bottom() + 2);
    }

    // Ethernet
    if ((LAN::clientIsConnected || LAN::cableIsConnected) && gTimerMS > 2000)
    {
        Char(Symbol::S8::ETHERNET).Draw4SymbolsInRect(x + 87, Grid::Bottom() + 2, LAN::clientIsConnected ? Color::FILL : Color::FLASH_01);
    }

    if (VCP::connectToHost || VCP::cableIsConnected)
    {
        Char(Symbol::UGO2::USB).Draw4SymbolsInRect(x + 72, Grid::Bottom() + 2, VCP::connectToHost ? Color::FILL : Color::FLASH_01);
    }
    
    Color::FILL.SetAsCurrent();
    // ������� ��������
    if(!PEAKDET_IS_DISABLE)
    {
       Char('\x12').Draw(x + 38, Grid::Bottom() + 11);
       Char('\x13').Draw(x + 46, Grid::Bottom() + 11);
    }

    if (MODE_WORK_IS_DIRECT)
    {
        Font::Set(TypeFont::S5);
        WriteStringAndNumber("�����.:", x + 57, Grid::Bottom() + 10, Smoothing::NumPoints());
        Font::Set(TypeFont::S8);
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
    numMS += timeTicks / 120000.0F; //-V2564
    numFrames++;
    
    if((gTimerMS - timeMSstartCalculation) >= 500)
    {
        std::sprintf(buffer, "%.1fms/%d", numMS / numFrames, numFrames * 2); //-V2564
        timeMSstartCalculation = gTimerMS;
        numMS = 0.0F;
        numFrames = 0;
    }

    Primitives::Rectangle(84, 10).Draw(Grid::Left(), Grid::FullBottom() - 10, Color::FILL);
    Region(82, 8).Fill(Grid::Left() + 1, Grid::FullBottom() - 9, Color::BACK);
    Text(buffer).Draw(Grid::Left() + 2, Grid::FullBottom() - 9, Color::FILL);

    char message[20] = {0};
    std::sprintf(message, "%d", Storage::NumElementsWithSameSettings());
    std::strcat(message, "/"); //-V2513
    char numAvail[10] = {0};
    std::sprintf(numAvail, "%d", Storage::NumberAvailableEntries());
    std::strcat(message, numAvail); //-V2513
    Text(message).Draw(Grid::Left() + 50, Grid::FullBottom() - 9);
}


void Display::DisableShowLevelRShiftA()
{
    showLevelRShiftA = false;
    Timer::Disable(TypeTimer::ShowLevelRShift0);
}


void Display::DisableShowLevelRShiftB()
{
    showLevelRShiftB = false;
    Timer::Disable(TypeTimer::ShowLevelRShift1);
}


void Display::DisableShowLevelTrigLev()
{
    showLevelTrigLev = false;
    Timer::Disable(TypeTimer::ShowLevelTrigLev);
}


void Display::EnableTrigLabel(bool enable)
{
    trigEnable = enable;
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
            std::memcpy(dataP2P_0, dataP2P_0 + 2, NUM_P2P_POINTS - 2); //-V743 //-V2563
            std::memcpy(dataP2P_1, dataP2P_1 + 2, NUM_P2P_POINTS - 2); //-V743 //-V2563
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
    Region(WIDTH - 1, HEIGHT - 2).Fill(0, 0, Color::BACK);
}


void Display::ShiftScreen(int delta)
{
    LIMITATION(SHIFT_IN_MEMORY, static_cast<int16>(SHIFT_IN_MEMORY + delta), 0, static_cast<int16>(sMemory_GetNumPoints(false) - 282)); //-V2516
}


void Display::ChangedRShiftMarkers()
{
    drawRShiftMarkers = ALT_MARKERS_HIDE;
    Timer::Enable(TypeTimer::RShiftMarkersAutoHide, 5000, FuncOnTimerRShiftMarkersAutoHide);
}

void Display::FuncOnTimerRShiftMarkersAutoHide()
{
    OnRShiftMarkersAutoHide();
}


void Display::OnRShiftMarkersAutoHide()
{
    drawRShiftMarkers = false;
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
    return static_cast<int>(SIZE_BUFFER_FOR_STRINGS - (strings[firstEmptyString - 1] - bufferForStrings) - std::strlen(strings[firstEmptyString - 1]) - 1); //-V2513
}


void Display::DeleteFirstString()
{
    if(FirstEmptyString() < 2)
    {
        return;
    }
    int delta = static_cast<int>(std::strlen(strings[0])) + 1; //-V2513
    int numStrings = FirstEmptyString();
    for(int i = 1; i < numStrings; i++)
    {
        strings[i - 1] = strings[i] - delta; //-V2563
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
    std::strcat(buffer, string); //-V2513
    int size = static_cast<int>(std::strlen(buffer)) + 1; //-V2513
    while(CalculateFreeSize() < size)
    {
        DeleteFirstString();
    }
    if(!strings[0])
    {
        strings[0] = bufferForStrings;
        std::strcpy(strings[0], buffer); //-V2513
    }
    else
    {
        char *addressLastString = strings[FirstEmptyString() - 1];
        char *address = addressLastString + std::strlen(addressLastString) + 1; //-V2513 //-V2563
        strings[FirstEmptyString()] = address;
        std::strcpy(address, buffer); //-V2513
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
    else if(lastStringForPause > NUM_STRINGS - 1) //-V2516
    {
        lastStringForPause--;
    }
}



void Display::OneStringDown()
{
    if(!CONSOLE_IN_PAUSE)
    {
    }
    else if(lastStringForPause < FirstEmptyString() - 1) //-V2516
    {
        lastStringForPause++;
    }
}



void Display::DrawConsole()
{
    int count = 0;
    Font::Set(sDebug_GetSizeFontForConsole() == 5 ? TypeFont::S5 : TypeFont::S8);
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
        Region(width, height + 1).Fill(Grid::Left() + 1, Grid::TOP + 1 + count * (height + 1) + delta, Color::BACK);
        int y = Grid::TOP + 5 + count * (height + 1) - 4;
        if(Font::GetSize() == 5)
        {
            y -= 3;
        }
        Text(strings[numString]).Draw(Grid::Left() + 2, y + delta, Color::FILL);
        count++;
    }

    Font::Set(TypeFont::S8);
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
        else if (warnings[i] == message) //-V2516
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
    Color::ResetFlash();
    ShowWarn(Tables::GetWarning(warning));
    Sound::WarnBeepBad();
}



void Display::ShowWarningGood(Warning::E warning)
{
    Color::ResetFlash();
    ShowWarn(Tables::GetWarning(warning));
    Sound::WarnBeepGood();
}


void Display::DrawStringInRectangle(int, int y, char const *text)
{
    int width = Font::GetLengthText(text);
    int height = 8;
    Primitives::Rectangle(width + 4, height + 4).Draw(Grid::Left(), y, Color::FILL);
    Primitives::Rectangle(width + 2, height + 2).Draw(Grid::Left() + 1, y + 1, Color::BACK);
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
