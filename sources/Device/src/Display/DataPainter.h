// 2021/03/17 13:26:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Settings/SettingsTypes_.h"
#include "FPGA/FPGA_Types.h"


struct DataSettings;


class DataPainter
{
public:

    static bool dataP2PIsEmpty;

#define NUM_P2P_POINTS (FPGA_MAX_POINTS)
    static uint8 dataP2P_0[NUM_P2P_POINTS];
    static uint8 dataP2P_1[NUM_P2P_POINTS];

    static int lastP2Pdata;

    // Функция должна вызываться для очистки поточечной памяти. Вызывается при изменении масштаба по напряжению.
    static void ResetP2Ppoints(bool empty);

    // Добавляет точки в режиме поточечного вывода и самописца data00, data01 - для первого канала; data10, data11 - для второго канала
    static void AddPoints(uint16 data00, uint16 data01, uint16 data10, uint16 data11);

    static void DrawData();

private:

    static void DrawDataNormal();

    static void DrawDataInModeNormal();

    static void DrawBothChannels();

    static void DrawDataChannel(uint8 *data, Channel::E chan, DataSettings *ds, int minY, int maxY);

    static bool ChannelNeedForDraw(puchar data, Channel::E chan, const DataSettings *ds);

    static void DrawMarkersForMeasure(float scale, Channel::E chan);

    static void DrawSignalLined(puchar data, const DataSettings *ds, int startPoint, int endPoint, int minY, int maxY,
        float scaleY, float scaleX, bool calculateFiltr);

    static void DrawSignalPointed(puchar data, const DataSettings *ds, int startPoint, int endPoint, int minY, int maxY,
        float scaleY, float scaleX);

    static void DrawMath();

    static void DrawDataMemInt();

    static void DrawDataInModeWorkLatest();

    static void DrawDataInModePoint2Point();

    static void DrawDataMinMax();

    static void DrawChannelInWindowMemory(int timeWindowRectWidth, int xVert0, int xVert1, int startI, int endI,
        puchar data, int rightX, Channel::E chan, int shiftForPeakDet);

    static void DrawDataInRect(int x, int width, puchar data, int numElems, Channel::E chan, int shiftForPeakDet);

    // Нарисовать окно памяти
    static void DrawMemoryWindow();
};
