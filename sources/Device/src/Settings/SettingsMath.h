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
    ScaleFFT::E     scaleFFT;       // Масштаб шкалы для отображения спектра.
    SourceFFT::E    sourceFFT;      // Источник сигнала для расчёта и отображения спектра.
    WindowFFT::E    windowFFT;      // Задаёт вид окна, на которое умножается сигнал перед расчётом спектра.
    FFTmaxDB::E     fftMaxDB;       // Минимальное значение на вертикальной координате графика спектра.
    Function::E     func;           // Включённая математическая функция.
    uint8           cursorFFT;      // Определяет, каким курсором спектра управляет ручка УСТАНОВКА.
    uint8           posCur[2];      // Позиция курсора спектра. Изменятеся 0...256.
    int8            koeff1add;      // Коэффициент при первом слагаемом для сложения.
    int8            koeff2add;
    int8            koeff1mul;
    int8            koeff2mul;
    bool            enableFFT;      // \c true, если включён спектр.
    ModeDrawMath::E modeDraw;       // Раздельный или общий дисплей в режиме математической функции.
    ModeRegSet::E   modeRegSet;     // Функция ручки УСТАНОВКА - масштаб по времени или смещение по вертикали.
    Range::E        range;          // Масштаб по напряжению математического сигнала.
    Divider::E      multiplier;     // Множитель масштаба математического сигнала.
    int16           rShift;         // Смещение по вертикальной оси математического сигнала.

    static bool IsMathEnabled();
    static uint8 CursorFFT();
    static uint8 PositionCursorFFT(int num);
    static bool IsEnbaledFFT();
};
