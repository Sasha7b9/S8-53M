// 2021/03/17 13:26:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Colors_.h"
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

    // ������� ������ ���������� ��� ������� ���������� ������. ���������� ��� ��������� �������� �� ����������.
    static void ResetP2Ppoints(bool empty);

    // ��������� ����� � ������ ����������� ������ � ���������
    // data_a_1, data_a_2 - ��� ������� ������; data_b_1, data_b_2 - ��� ������� ������
    static void AddPoints(uint16 data_a_1, uint16 data_a_2, uint16 data_b_1, uint16 data_b_2);

    static void DrawData();

private:

    static void DrawBothChannels();

    static void DrawDataChannel(uint8 *data, Channel::E chan, DataSettings *ds, int min_y, int max_y);

    static bool ChannelNeedForDraw(puchar data, Channel::E chan, const DataSettings *ds);

    static void DrawMarkersForMeasure(float scale, Channel::E chan);

    static void DrawSignalLined(puchar data, const DataSettings *ds, int start_i, int end_i, int min_y,
        int max_y, float scale_y, float scale_x, bool calculate_filtr);

    static void DrawSignalPointed(puchar data, const DataSettings *ds, int start_i, int end_i, int min_y,
        int max_y, float scale_y, float scale_x);

    static void DrawMath();

    static void DrawDataMemInt();

    static void DrawDataInModeWorkLatest();

    static void DrawDataInModePoint2Point();

    static void DrawDataMinMax();

    static void DrawChannelInWindowMemory(int timeWindowRectWidth, int xVert0, int xVert1, int startI, int endI,
        puchar data, int rightX, Channel::E chan, int shiftForPeakDet);

    static void DrawDataInRect(int x, int width, puchar data, int numElems, Channel::E chan, int shiftForPeakDet);

    // ���������� ���� ������
    static void DrawMemoryWindow();

    static void DrawSpectrum();

    static void DRAW_SPECTRUM(puchar data, int numPoints, Channel::E channel);

    static void DrawSpectrumChannel(const float *spectrum, Color color);

    static void WriteParametersFFT(Channel::E chan, float freq0, float density0, float freq1, float density1);

    static void DrawDataInModeSelfRecorder();

    // ���������� ������ �������
    // start_x    - ���������� x ������ �����
    // T ����� ���� uint8 - ���� ������ ��������� �� ���������� ���, ��� uint16 - ���� ������ ��������� �� ������� ���
    static void DrawPoints(uint8 *y, const int start_x, int num_points, const Color &color = Color::Count);
    // ���������� DrawPoints(), ������ � y ���� ������� ���������� ���� � �����
    static void DrawLines(uint8 *y, const int start_x, int num_lines, const Color &color = Color::Count);
};
