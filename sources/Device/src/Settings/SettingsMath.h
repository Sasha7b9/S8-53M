#pragma once


#define MODE_DRAW_MATH              (set.math.modeDraw)                 // SettingsMath.modeDraw
#define DISABLED_DRAW_MATH          (MODE_DRAW_MATH == ModeDrawMath::Disable)
#define MODE_DRAW_MATH_IS_TOGETHER  (MODE_DRAW_MATH == ModeDrawMath::Together)
#define MODE_DRAW_MATH_IS_SEPARATE  (MODE_DRAW_MATH == ModeDrawMath::Separate)

#define MATH_MODE_REG_SET           (set.math.modeRegSet)               // SettingsMath.modeRegSet
#define MATH_MODE_REG_SET_IS_RSHIFT (MATH_MODE_REG_SET == ModeRegSet::RShift)
#define MATH_MODE_REG_SET_IS_RANGE  (MATH_MODE_REG_SET == ModeRegSet::Range)

#define MATH_MULTIPLIER             (set.math.multiplier)               // SettingsMath.multiplier


struct SettingsMath
{
    ScaleFFT::E     scaleFFT;       // ������� ����� ��� ����������� �������.
    SourceFFT::E    sourceFFT;      // �������� ������� ��� ������� � ����������� �������.
    WindowFFT::E    windowFFT;      // ����� ��� ����, �� ������� ���������� ������ ����� �������� �������.
    FFTmaxDB::E     fftMaxDB;       // ����������� �������� �� ������������ ���������� ������� �������.
    Function::E     func;           // ���������� �������������� �������.
    uint8           cursorFFT;      // ����������, ����� �������� ������� ��������� ����� ���������.
    uint8           posCur[2];      // ������� ������� �������. ���������� 0...256.
    int8            koeff1add;      // ����������� ��� ������ ��������� ��� ��������.
    int8            koeff2add;
    int8            koeff1mul;
    int8            koeff2mul;
    bool            enableFFT;      // \c true, ���� ������� ������.
    ModeDrawMath::E modeDraw;       // ���������� ��� ����� ������� � ������ �������������� �������.
    ModeRegSet::E   modeRegSet;     // ������� ����� ��������� - ������� �� ������� ��� �������� �� ���������.
    Range::E        range;          // ������� �� ���������� ��������������� �������.
    Divider::E      multiplier;     // ��������� �������� ��������������� �������.
    int16           rShift;         // �������� �� ������������ ��� ��������������� �������.

    static bool IsMathEnabled();
    static uint8 CursorFFT();
    static uint8 PositionCursorFFT(int num);
    static bool IsEnbaledFFT();
};
