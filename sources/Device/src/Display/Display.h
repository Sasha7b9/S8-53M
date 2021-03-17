#pragma once
#include "Tables.h"


struct DataSettings;

struct DrawMode { enum E
{
    Auto,
    Hand
};};


class Display
{
public:

    static const int WIDTH = 320;
    static const int HEIGHT = 240;

    static int topMeasures;    // ���� ������� ������ ���������. ��� �������� ����� ��� ����������� ������ ��������� �� ����� - ����� ��� ������ �� �����������

    static void Init();

    static uint8 *GetBuffer();
    static uint8 *GetBufferEnd();
    static void BeginFrame(const Color &color);
    static void EndFrame();

    static void Update();

    static void RotateRShift(Channel::E channel);

    static void RotateTrigLev();

    static void Redraw();

    static void EnableTrigLabel(bool enable);
    // ������� ������ ���������� ��� ������� ���������� ������. ���������� ��� ��������� �������� �� ����������.
    static void ResetP2Ppoints(bool empty);
    // ��������� ����� � ������ ����������� ������ � ��������� data00, data01 - ��� ������� ������; data10, data11 - ��� ������� ������
    static void AddPoints(uint16 data00, uint16 data01, uint16 data10, uint16 data11);

    static void ShowWarningBad(Warning::E warning);

    static void ShowWarningGood(Warning::E warning);

    static void ClearFromWarnings();

    static void SetDrawMode(DrawMode::E mode, pFuncVV func);

    static void SetAddDrawFunction(pFuncVV func);

    static void RemoveAddDrawFunction();

    static void Clear();

    static void ShiftScreen(int delta);

    static void ChangedRShiftMarkers();

    static void AddStringToIndicating(const char *string);

    static void OneStringUp();

    static void OneStringDown();

    static void SetPauseForConsole(bool pause);
    // ����� ��������� ���������� ������ ��� ������� ����� ������� ���� ���.
    static void RunAfterDraw(pFuncVV func);
    // ��������� ��������������� ����� ������� �������� �� ���������� ������� ������.
    static void DisableShowLevelRShiftA();
    // ��������� ��������������� ����� ������� �������� �� ���������� ������� ������.
    static void DisableShowLevelRShiftB();
    
    static void DisableShowLevelTrigLev();

    static void OnRShiftMarkersAutoHide();

private:

    static void ShowWarn(const char *message);
    // ���������� �����.
    static void DrawGrid(int left, int top, int width, int height);
    // ���������� ������ �����.
    static void DrawFullGrid();

    static void DrawCursorsWindow();

    static void DrawCursorsRShift();
    // ���������� ������� �������� �� ����������
    static void DrawCursorRShift(Channel::E chan);
    // ���������� ������ ������ �������������.
    static void DrawCursorTrigLevel();
    // ���������� ������ �������� �� �������.
    static void DrawCursorTShift();
    // �������� ���������� ��� ������ - � ������ ����� �������.
    static void DrawLowPart();

    static void DrawHiPart();

    static void DrawHiRightPart();

    static void DrawMath();

    static void DrawSpectrum();
    // ������� ������� �����.
    static void DrawTime(int x, int y);
    // ���������� ������� ��������� ���������.
    static void DrawCursors();
    // ������� �������� �������� ��������� ���������.
    static void WriteCursors();
    // ������� �������� �������������� ���������.
    static void DrawMeasures();
    // �������� ��������� ���������� �������.
    static void DrawConsole();
    // �������� ��������������.
    static void DrawWarnings();
    
    static int CalculateCountV();
    
    static int CalculateFreeSize();
    
    static bool ChannelNeedForDraw(puchar data, Channel::E chan, const DataSettings *ds);
    
    static void DrawDataChannel(uint8 *data, Channel::E chan, DataSettings *ds, int minY, int maxY);
    
    static void DrawBothChannels();
    
    static void DrawDataMemInt();
    
    static void DrawDataInModeWorkLatest();
    
    static void DrawDataInModePoint2Point();
    
    static void DrawDataInModeNormal();
    
    static void DrawDataMinMax();
    
    static void DrawDataNormal();
    
    static void DrawData();
    // ���������� ���� ������
    static void DrawMemoryWindow();
    
    static void DRAW_SPECTRUM(puchar data, int numPoints, Channel::E channel);
    
    static void DrawGridSpectrum();
    
    static void DrawMarkersForMeasure(float scale, Channel::E chan);
    
    static void DrawScaleLine(int x, bool forTrigLev);
    
    static void WriteParametersFFT(Channel::E chan, float freq0, float density0, float freq1, float density1);
    
    static void WriteTextVoltage(Channel::E chan, int x, int y);
    
    static void FuncOnTimerDisableShowLevelRShiftA();
    
    static void FuncOnTimerDisableShowLevelRShiftB();
    
    static void FuncOnTimerDisableShowLevelTrigLev();
    
    static void FuncOnTimerRShiftMarkersAutoHide();
    
    static void OnTimerShowWarning();
    
    static void DrawSignalLined(puchar data, const DataSettings *ds, int startPoint, int endPoint, int minY, int maxY, float scaleY, 
        float scaleX, bool calculateFiltr);
    
    static void DrawSignalPointed(puchar data, const DataSettings *ds, int startPoint, int endPoint, int minY, int maxY, float scaleY, 
        float scaleX);
    
    static void DrawSpectrumChannel(const float *spectrum, Color color);
    
    static void DrawStringInRectangle(int x, int y, char const *text);
    
    static void DrawStringNavigation();
    
    static int FirstEmptyString();
    
    static bool NeedForClearScreen();
    
    static void WriteStringAndNumber(pchar text, int x, int y, int number);
    
    static void DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY, int stepX, int stepY);
    
    static void DrawChannelInWindowMemory(int timeWindowRectWidth, int xVert0, int xVert1, int startI, int endI, puchar data, int rightX, 
        Channel::E chan, int shiftForPeakDet);
    
    static void DrawDataInModeSelfRecorder();
    
    static void DrawDataInRect(int x, int width, puchar data, int numElems, Channel::E chan, int shiftForPeakDet);

    static void DrawTimeForFrame(uint timeMS);
    
    static void DeleteFirstString();
    // ���������� �������������� ������ ��������� ���������.
    static void DrawHorizontalCursor
                                (int y,             // �������� �������� �������.
                                int xTearing        // ���������� �����, � ������� ���������� ������� ������ ��� �������� �����������.
                                );
    // ���������� ������������ ������ ��������� ���������.
    static void DrawVerticalCursor
                                (int x,             // �������� �������� �������.
                                int yTearing        // ���������� �����, � ������� ���������� ������� ������ ��� �������� �����������.
                                );
    // ������� �������� ������ �������������. 
    static void WriteValueTrigLevel();

    static void AddString(const char *string);
                                
    static int CalculateCountH();
                                
    static void DrawGridType1(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX, float deltaY, float stepX, float stepY);

    static void DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY);
};

