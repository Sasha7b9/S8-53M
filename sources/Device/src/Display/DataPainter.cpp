// 2021/03/17 13:27:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Log_.h"
#include "common/Display/Painter_.h"
#include "common/Display/Primitives_.h"
#include "common/Display/Text_.h"
#include "common/Utils/GlobalFunctions_.h"
#include "common/Utils/Math_.h"
#include "Display/DataPainter.h"
#include "Display/Grid.h"
#include "FPGA/FPGA_Reader.h"
#include "FPGA/MathFPGA.h"
#include "FPGA/Storage.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Utils/ProcessingSignal.h"
#include <cstring>


using namespace Primitives;


bool DataPainter::dataP2PIsEmpty = true;
uint8 DataPainter::dataP2P_0[NUM_P2P_POINTS];
uint8 DataPainter::dataP2P_1[NUM_P2P_POINTS];
int DataPainter::lastP2Pdata = 0;


void DataPainter::DrawData()
{
    DrawDataNormal();

    //    if (Storage::AllDatas())
    //    {
    //        if (MODE_WORK_IS_MEMINT)
    //        {
    //            if (!MODE_SHOW_MEMINT_IS_DIRECT)
    //            {
    //                DrawDataMemInt();
    //            }
    //            if (!MODE_SHOW_MEMINT_IS_SAVED)
    //            {
    //                DrawDataNormal();
    //            }
    //        }
    //        else if (MODE_WORK_IS_LATEST)
    //        {
    //            DrawDataInModeWorkLatest();
    //        }
    //        else
    //        {
    //            if (PageMemory::PageInternal::showAlways)
    //            {
    //                DrawDataMemInt();
    //            }
    //            DrawDataNormal();
    //        }
    //
    //        if (NUM_MIN_MAX != 1)
    //        {
    //            DrawDataMinMax();
    //        }
    //    }

    Primitives::Rectangle(Grid::Width(), Grid::FullHeight()).Draw(Grid::Left(), Grid::TOP, Color::FILL);
}


void DataPainter::DrawDataNormal()
{
    DrawDataInModeNormal();

//    if (!dataP2PIsEmpty)
//    {
//        static const pFuncVV funcs[2] = {DrawDataInModePoint2Point, DrawDataInModeSelfRecorder};
//        funcs[(int)SET_SELFRECORDER]();
//    }
//    else
//    {
//        DrawDataInModeNormal();
//    }
}


void DataPainter::DrawDataInModeNormal()
{
    DrawBothChannels();

    //    static void* prevAddr = 0;
    //
    //    uint8 *data0 = 0;
    //    uint8 *data1 = 0;
    //    DataSettings *ds = 0;
    //    Processing::GetData(&data0, &data1, &ds);
    //
    //    int16 numSignals = (int16)Storage::NumElementsWithSameSettings();
    //    LIMITATION(numSignals, numSignals, 1, NUM_ACCUM);
    //    if (numSignals == 1 || ENUM_ACCUM_IS_INFINITY || MODE_ACCUM_IS_RESET || sTime_RandomizeModeEnabled())
    //    {
    //        DrawBothChannels(0, 0);
    //        if (prevAddr == 0 || prevAddr != ds->addrPrev)
    //        {
    //            numDrawingSignals++;
    //            prevAddr = ds->addrPrev;
    //        }
    //    }
    //    else
    //    {
    //        for (int i = 0; i < numSignals; i++)
    //        {
    //            DrawBothChannels(Storage::GetData(ChA, i), Storage::GetData(ChB, i));
    //        }
    //    }
}


void DataPainter::DrawBothChannels()
{
    if (LAST_AFFECTED_CHANNEL_IS_B)
    {
        DrawDataChannel(ReaderFPGA::data_a, ChA, &ReaderFPGA::ds, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(ReaderFPGA::data_b, ChB, &ReaderFPGA::ds, Grid::TOP, Grid::ChannelBottom());
    }
    else
    {
        DrawDataChannel(ReaderFPGA::data_b, ChB, &ReaderFPGA::ds, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(ReaderFPGA::data_a, ChA, &ReaderFPGA::ds, Grid::TOP, Grid::ChannelBottom());
    }
}


void DataPainter::DrawDataChannel(uint8 *data, Channel::E chan, DataSettings *ds, int min_y, int max_y)
{
    float scaleY = static_cast<float>(max_y - min_y) / (MAX_VALUE - MIN_VALUE);
    float scaleX = Grid::Width() / 280.0F;

    int firstPoint = 0;
    int lastPoint = 0;

    SettingsDisplay::PointsOnDisplay(&firstPoint, &lastPoint);

    Color::Channel(chan).SetAsCurrent();

    DrawSignalPointed(data, ds, firstPoint, lastPoint, min_y, max_y, scaleY, scaleX);
}


bool DataPainter::ChannelNeedForDraw(puchar data, Channel::E chan, const DataSettings *ds)
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
        if ((chan == ChA && ds->enableCh0 == 0) || (chan == ChB && ds->enableCh1 == 0))
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


void DataPainter::DrawMarkersForMeasure(float scale, Channel::E chan)
{
    if (chan == Channel::Math)
    {
        return;
    }
    Color::Cursors(chan).SetAsCurrent();
    for (int numMarker = 0; numMarker < 2; numMarker++)
    {
        int pos = Processing::GetMarkerHorizontal(chan, numMarker);
        if (pos != ERROR_VALUE_INT && pos > 0 && pos < 200)
        {
            DashedHLine(3, 2).Draw(Grid::FullBottom() - static_cast<int>(pos * scale), Grid::Left(), Grid::Right(), 0);
        }

        pos = Processing::GetMarkerVertical(chan, numMarker);
        if (pos != ERROR_VALUE_INT && pos > 0 && pos < Grid::Right())
        {
            DashedVLine(3, 2).Draw(Grid::Left() + static_cast<int>(pos * scale), Grid::TOP, Grid::FullBottom(), 0);
        }

    }
}


#define CONVERT_DATA_TO_DISPLAY(out, in)                    \
    out = (uint8)(max_y - ((in) - MIN_VALUE) * scale_y);    \
    if(out < min_y)          { out = (uint8)min_y; }        \
    else if (out > max_y)    { out = (uint8)max_y; };


void DataPainter::DrawSignalLined(puchar data, const DataSettings *ds, int start_i, int end_i, int min_y,
    int max_y, float scale_y, float scale_x, bool calculate_filtr)
{
    if (end_i < start_i)
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
        for (int i = start_i; i < end_i; i++)
        {
            float x0 = gridLeft + (i - start_i) * scale_x;
            if (x0 >= gridLeft && x0 <= gridRight)
            {
                int index = i - start_i;
                int y = calculate_filtr ? Math::CalculateFiltr(data, i, numPoints, numSmoothing) : data[i];
                CONVERT_DATA_TO_DISPLAY(dataCD[index], y);
            }
        }
    }
    else
    {
        int shift = static_cast<int>(ds->length1channel);

        int yMinNext = -1;
        int yMaxNext = -1;

        for (int i = start_i; i < end_i; i++)
        {
            float x = gridLeft + (i - start_i) * scale_x;
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

    if (end_i - start_i < 281)
    {
        int _numPoints = 281 - (end_i - start_i);
        for (int i = 0; i < _numPoints; i++)
        {
            int index = end_i - start_i + i;
            CONVERT_DATA_TO_DISPLAY(dataCD[index], MIN_VALUE);
        }
    }

    if (ds->peakDet == PeackDetMode::Disable)
    {
        CONVERT_DATA_TO_DISPLAY(dataCD[280], data[end_i]);
        Painter::DrawSignal(Grid::Left(), dataCD, true);
    }
}


void DataPainter::DrawSignalPointed(puchar data, const DataSettings *ds, int start_i, int end_i, int min_y,
    int max_y, float scale_y, float scale_x)
{
    int numPoints = sMemory_GetNumPoints(false);
    int numSmoothing = Smoothing::NumPoints();

    uint8 dataCD[281];

    if (scale_x == 1.0F) //-V550 //-V2550
    {
        for (int i = start_i; i < end_i; i++)
        {
            int index = i - start_i;
            CONVERT_DATA_TO_DISPLAY(dataCD[index], Math::CalculateFiltr(data, i, numPoints, numSmoothing));
        }
        Painter::DrawSignal(Grid::Left(), dataCD, false);

        if (ds->peakDet)
        {
            int size = end_i - start_i;
            start_i += numPoints;
            end_i = start_i + size;;

            for (int i = start_i; i < end_i; i++)
            {
                int index = i - start_i;
                CONVERT_DATA_TO_DISPLAY(dataCD[index], Math::CalculateFiltr(data, i, numPoints, numSmoothing));
            }
            Painter::DrawSignal(Grid::Left(), dataCD, false);
        }
    }
    else
    {
        for (int i = start_i; i < end_i; i++)
        {
            int index = i - start_i;
            int dat = 0;
            CONVERT_DATA_TO_DISPLAY(dat, Math::CalculateFiltr(data, i, numPoints, numSmoothing));
            Point().Draw(Grid::Left() + static_cast<int>(index * scale_x), dat);
        }
    }
}


void DataPainter::DrawMath()
{
    if (DISABLED_DRAW_MATH || Storage::GetData(ChA, 0) == 0 || Storage::GetData(ChB, 0) == 0)
    {
        return;
    }

    uint8 *dataRel0 = 0;
    uint8 *dataRel1 = 0;
    DataSettings *ds = 0;
    Processing::GetData(&dataRel0, &dataRel1, &ds);

    float dataAbs0[FPGA_MAX_POINTS];
    float dataAbs1[FPGA_MAX_POINTS];

    MathFPGA::PointsRelToVoltage(dataRel0, static_cast<int>(ds->length1channel), ds->range[ChA],
        static_cast<int16>(ds->rShiftCh0), dataAbs0);
    MathFPGA::PointsRelToVoltage(dataRel1, static_cast<int>(ds->length1channel), ds->range[ChB],
        static_cast<int16>(ds->rShiftCh1), dataAbs1);

    MathFPGA::CalculateMathFunction(dataAbs0, dataAbs1, static_cast<int>(ds->length1channel));

    uint8 points[FPGA_MAX_POINTS];
    MathFPGA::PointsVoltageToRel(dataAbs0, static_cast<int>(ds->length1channel), SET_RANGE_MATH, SET_RSHIFT_MATH,
        points);

    DrawDataChannel(points, Channel::Math, ds, Grid::MathTop(), Grid::MathBottom());

    static const int w = 71;
    static const int h = 10;
    int delta = (SHOW_STRING_NAVIGATION && MODE_DRAW_MATH_IS_TOGETHER) ? 10 : 0;
    Primitives::Rectangle(w, h).Draw(Grid::Left(), Grid::MathTop() + delta, Color::FILL);
    Region(w - 2, h - 2).Fill(Grid::Left() + 1, Grid::MathTop() + 1 + delta, Color::BACK);
    Divider::E multiplier = MATH_MULTIPLIER;
    Text(sChannel_Range2String(SET_RANGE_MATH, multiplier)).Draw(Grid::Left() + 2, Grid::MathTop() + 1 + delta,
        Color::FILL);
    Text(":").Draw(Grid::Left() + 25, Grid::MathTop() + 1 + delta);
    char buffer[20];
    Text(sChannel_RShift2String(SET_RSHIFT_MATH, SET_RANGE_MATH, multiplier, buffer)).Draw(Grid::Left() + 27,
        Grid::MathTop() + 1 + delta);
}


void DataPainter::DrawDataMemInt()
{
    if (Storage::dsInt != 0)
    {
        DrawDataChannel(Storage::dataIntA, ChA, Storage::dsInt, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::dataIntB, ChB, Storage::dsInt, Grid::TOP, Grid::ChannelBottom());
    }
}


void DataPainter::DrawDataInModeWorkLatest()
{
    if (Storage::dsLast != 0)
    {
        DrawDataChannel(Storage::dataLastA, ChA, Storage::dsLast, Grid::TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::dataLastB, ChB, Storage::dsLast, Grid::TOP, Grid::ChannelBottom());
    }
}


void DataPainter::DrawDataInModePoint2Point()
{
    uint8 *data0 = 0;
    uint8 *data1 = 0;
    DataSettings *ds = 0;
    Processing::GetData(&data0, &data1, &ds);

    if (LAST_AFFECTED_CHANNEL_IS_B)
    {
        if (SET_SELFRECORDER || (Storage::NumElementsWithCurrentSettings() == 0))
        {
            DrawDataChannel(dataP2P_0, ChA, ds, Grid::TOP, Grid::ChannelBottom());
            DrawDataChannel(dataP2P_1, ChB, ds, Grid::TOP, Grid::ChannelBottom());
        }
        else
        {
            DrawDataChannel(data0, ChA, ds, Grid::TOP, Grid::ChannelBottom());
            DrawDataChannel(data1, ChB, ds, Grid::TOP, Grid::ChannelBottom());
        }
    }
    else
    {
        if (SET_SELFRECORDER || (Storage::NumElementsWithCurrentSettings() == 0))
        {
            DrawDataChannel(dataP2P_1, ChB, ds, Grid::TOP, Grid::ChannelBottom());
            DrawDataChannel(dataP2P_0, ChA, ds, Grid::TOP, Grid::ChannelBottom());
        }
        else
        {
            DrawDataChannel(data1, ChB, ds, Grid::TOP, Grid::ChannelBottom());
            DrawDataChannel(data0, ChA, ds, Grid::TOP, Grid::ChannelBottom());
        }
    }
}


void DataPainter::DrawDataMinMax()
{
    ModeDrawSignal::E modeDrawSignalOld = MODE_DRAW_SIGNAL;
    MODE_DRAW_SIGNAL = ModeDrawSignal::Lines;
    if (LAST_AFFECTED_CHANNEL_IS_B)
    {
        DrawDataChannel(Storage::GetLimitation(ChA, 0), ChA, Storage::set, Grid::TOP,
            Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(ChA, 1), ChA, Storage::set, Grid::TOP,
            Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(ChB, 0), ChB, Storage::set, Grid::TOP,
            Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(ChB, 1), ChB, Storage::set, Grid::TOP,
            Grid::ChannelBottom());
    }
    else
    {
        DrawDataChannel(Storage::GetLimitation(ChB, 0), ChB, Storage::set, Grid::TOP,
            Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(ChB, 1), ChB, Storage::set, Grid::TOP,
            Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(ChA, 0), ChA, Storage::set, Grid::TOP,
            Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(ChA, 1), ChA, Storage::set, Grid::TOP,
            Grid::ChannelBottom());
    }
    MODE_DRAW_SIGNAL = modeDrawSignalOld;
}


// shiftForPeakDet - если рисуем информацию с пикового детектора - то через shiftForPeakDet точек расположена
// иниформация о максимумах.
void DataPainter::DrawChannelInWindowMemory(int timeWindowRectWidth, int xVert0, int xVert1, int startI, int endI,
    puchar data, int rightX, Channel::E chan, int shiftForPeakDet)
{
    if (data == dataP2P_0 && data == dataP2P_1)
    {

    }
    else
    {
        DrawDataInRect(1, xVert0 - 1, &(data[0]), startI, chan,
            shiftForPeakDet);
        DrawDataInRect(xVert0 + 2, timeWindowRectWidth - 2, &(data[startI]), 281, chan,
            shiftForPeakDet);
        DrawDataInRect(xVert1 + 2, rightX - xVert1 + 2, &(data[endI + 1]), sMemory_GetNumPoints(false) - endI, chan,
            shiftForPeakDet);
    }
}


// shiftForPeakDet - если рисуем информацию с пикового детектора - то через shiftForPeakDet точек расположена
// иниформация о максимумах.
void DataPainter::DrawDataInRect(int x, int width, puchar data, int numElems, Channel::E chan, int shiftForPeakDet)
{
    if (numElems == 0)
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

#define ORDINATE(x) bottom - scale * Math::Limitation<int>((x) - MIN_VALUE, 0, 200)

#define NUM_POINTS (300 * 2)
    uint8 points[NUM_POINTS];

    points[0] = static_cast<uint8>(ORDINATE(max[0]));
    points[1] = static_cast<uint8>(ORDINATE(min[0]));

    for (int i = 1; i < width; i++)
    {
        int value0 = min[i] > max[i - 1] ? max[i - 1] : min[i];
        int value1 = max[i] < min[i - 1] ? min[i - 1] : max[i];
        points[i * 2] = static_cast<uint8>(ORDINATE(value1));
        points[i * 2 + 1] = static_cast<uint8>(ORDINATE(value0));
    }
    if (width < 256)
    {
        VLineArray().Draw(x, width, points, Color::Channel(chan));
    }
    else
    {
        VLineArray().Draw(x, 255, points, Color::Channel(chan));
        VLineArray().Draw(x + 255, width - 255, points + 255 * 2, Color::Channel(chan));
    }
}


void DataPainter::DrawMemoryWindow()
{
    uint8 *dat0 = Storage::dataIntA;
    uint8 *dat1 = Storage::dataIntB;
    DataSettings *ds = Storage::dsInt;

    if (MODE_WORK_IS_DIRECT || MODE_WORK_IS_LATEST)
    {
        dat0 = Storage::dataA;
        dat1 = Storage::dataB;
        ds = Storage::set;
    }

    if (ds == nullptr)
    {
        return;
    }

    int leftX = 3;
    int top = 1;
    int height = Grid::TOP - 3;
    int bottom = top + height;

    static const int rightXses[3] = { 276, 285, 247 };
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
    Primitives::Rectangle(xVert1 - xVert0, bottom - top - (showFull ? 0 : 2)).Draw(xVert0, top + (showFull ? 0 : 1),
        Color::FILL);

    if (!dataP2PIsEmpty)
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

            Channel::E chanFirst = LAST_AFFECTED_CHANNEL_IS_A ? ChB : ChA;
            Channel::E chanSecond = LAST_AFFECTED_CHANNEL_IS_A ? ChA : ChB;
            puchar dataFirst = LAST_AFFECTED_CHANNEL_IS_A ? dat1 : dat0;
            puchar dataSecond = LAST_AFFECTED_CHANNEL_IS_A ? dat0 : dat1;

            int shiftForPeakDet = ds->peakDet == PeackDetMode::Disable ? 0 : (int)ds->length1channel;

            if (ChannelNeedForDraw(dataFirst, chanFirst, ds))
            {
                DrawChannelInWindowMemory(timeWindowRectWidth, xVert0, xVert1, startI, endI, dataFirst, rightX,
                    chanFirst, shiftForPeakDet);
            }
            if (ChannelNeedForDraw(dataSecond, chanSecond, ds))
            {
                DrawChannelInWindowMemory(timeWindowRectWidth, xVert0, xVert1, startI, endI, dataSecond, rightX,
                    chanSecond, shiftForPeakDet);
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

    int x[] = { leftX, (rightX - leftX) / 2 + leftX + 1, rightX };
    int x0 = x[SET_TPOS];

    // Маркер TPos
    Region(6, 6).Fill(x0 - 3, 9, Color::BACK);
    Char(Symbol::S8::TPOS_1).Draw(x0 - 3, 9, Color::FILL);

    // Маркер tShift
    float scale = (float)(rightX - leftX + 1) /
        ((float)sMemory_GetNumPoints(false) - (sMemory_GetNumPoints(false) == 281 ? 1 : 0));

    float xShift = 1 + (sTime_TPosInPoints((PeackDetMode::E)Storage::set->peakDet, (int)Storage::set->length1channel, SET_TPOS) - sTime_TShiftInPoints((PeackDetMode::E)Storage::set->peakDet)) * scale;

    if (xShift < leftX - 2)
    {
        xShift = static_cast<float>(leftX - 2);
    }

    Region(6, 6).Fill(static_cast<int>(xShift - 1), 3, Color::BACK);
    Region(4, 4).Fill(static_cast<int>(xShift), 4, Color::FILL);
    Color::BACK.SetAsCurrent();

    if (xShift == leftX - 2) //-V2550 //-V550
    {
        xShift = static_cast<float>(leftX - 2);
        Line().Draw(static_cast<int>(xShift) + 3, 5, static_cast<int>(xShift) + 3, 7);
        Line().Draw(static_cast<int>(xShift) + 1, 6, static_cast<int>(xShift) + 2, 6);
    }
    else if (xShift > rightX - 1)
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


void DataPainter::ResetP2Ppoints(bool empty)
{
    dataP2PIsEmpty = empty;
    lastP2Pdata = 0;
    std::memset(dataP2P_0, AVE_VALUE, NUM_P2P_POINTS);
    std::memset(dataP2P_1, AVE_VALUE, NUM_P2P_POINTS);
}


void DataPainter::AddPoints(uint16 data_a_1, uint16 data_a_2, uint16 data_b_1, uint16 data_b_2)
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

    dataP2P_0[lastP2Pdata] = (uint8)data_a_1;
    dataP2P_1[lastP2Pdata++] = (uint8)data_b_1;

    dataP2P_0[lastP2Pdata] = (uint8)data_a_2;
    dataP2P_1[lastP2Pdata++] = (uint8)data_b_2;
    if (!SET_SELFRECORDER && lastP2Pdata >= NUM_P2P_POINTS)
    {
        lastP2Pdata = 0;
    }
}


void DataPainter::DrawSpectrum()
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
            DRAW_SPECTRUM(Storage::dataA, numPoints, ChA);
        }
        else if (SOURCE_FFT_IS_B)
        {
            DRAW_SPECTRUM(Storage::dataB, numPoints, ChB);
        }
        else
        {
            if (LAST_AFFECTED_CHANNEL_IS_A)
            {
                DRAW_SPECTRUM(Storage::dataB, numPoints, ChB);
                DRAW_SPECTRUM(Storage::dataA, numPoints, ChA);
            }
            else
            {
                DRAW_SPECTRUM(Storage::dataA, numPoints, ChA);
                DRAW_SPECTRUM(Storage::dataB, numPoints, ChB);
            }
        }
    }

    HLine().Draw(Grid::ChannelBottom(), Grid::Left(), Grid::Right(), Color::FILL);
    HLine().Draw(Grid::MathBottom(), Grid::Left(), Grid::Right());
}


void DataPainter::DRAW_SPECTRUM(puchar data, int numPoints, Channel::E channel)
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

    MathFPGA::PointsRelToVoltage(data, numPoints, Storage::set->range[channel], channel == ChA ?
        static_cast<int16>(Storage::set->rShiftCh0) :
        static_cast<int16>(Storage::set->rShiftCh1), dataR);

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


void DataPainter::DrawSpectrumChannel(const float *spectrum, Color color)
{
    color.SetAsCurrent();
    int gridLeft = Grid::Left();
    int gridBottom = Grid::MathBottom();
    int gridHeight = Grid::MathHeight();
    for (int i = 0; i < 256; i++)
    {
        HLine().Draw(gridLeft + i, gridBottom, gridBottom - static_cast<int>(gridHeight * spectrum[i]));
    }
}


void DataPainter::WriteParametersFFT(Channel::E chan, float freq0, float density0, float freq1, float density1)
{
    int x = Grid::Left() + 259;
    int y = Grid::ChannelBottom() + 5;
    int dY = 10;

    Color::FILL.SetAsCurrent();
    GF::Freq2String(freq0, false).Draw(x, y);
    y += dY;
    GF::Freq2String(freq1, false).Draw(x, y);
    if (chan == ChA)
    {
        y += dY + 2;
    }
    else
    {
        y += dY * 3 + 4;
    }
    Color::Channel(chan).SetAsCurrent();
    Text(SCALE_FFT_IS_LOG ? GF::Float2Db(density0, 4).c_str() : GF::Float2String(density0, false, 7).c_str()).Draw(x, y);
    y += dY;
    Text(SCALE_FFT_IS_LOG ? GF::Float2Db(density1, 4).c_str() : GF::Float2String(density1, false, 7).c_str()).Draw(x, y);
}


void DataPainter::DrawDataInModeSelfRecorder()
{
    LOG_TRACE
}
