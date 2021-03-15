#pragma once


#define SCALE_FFT                   (set.math.scaleFFT)                 // SettingMath.scaleFFT
#define SCALE_FFT_IS_LOG            (SCALE_FFT == ScaleFFT::Log)
#define SCALE_FFT_IS_LINEAR         (SCALE_FFT == ScaleFFT::Linear)

#define SOURCE_FFT                  (set.math.sourceFFT)                // SettingsMath.sourceFFT
#define SOURCE_FFT_IS_A             (SOURCE_FFT == SourceFFT::A_)
#define SOURCE_FFT_IS_B             (SOURCE_FFT == SourceFFT::B_)

#define WINDOW_FFT                  (set.math.windowFFT)                // SettingsMath.windowFFT
#define WINDOW_FFT_IS_HAMMING       (WINDOW_FFT == WindowFFT::Hamming)
#define WINDOW_FFT_IS_BLACKMAN      (WINDOW_FFT == WindowFFT::Blackman)
#define WINDOW_FFT_IS_HANN          (WINDOW_FFT == WindowFFT::Hann)

#define FFT_MAX_DB                  (set.math.fftMaxDB)                 // SettingsMath.fftMaxDB

#define MATH_FUNC                   (set.math.func)                     // SettingsMath.func
#define MATH_FUNC_IS_SUM            (MATH_FUNC == Function::Sum)
#define MATH_FUNC_IS_MUL            (MATH_FUNC == Function::Mul)

#define FFT_CUR_CURSOR              (set.math.currentCursor)            // SettingsMath.currentCursor
#define FFT_CUR_CURSOR_IS_0         (FFT_CUR_CURSOR == 0)

#define FFT_POS_CURSOR(num)         (set.math.posCur[num])              // SettingsMath.currentCursor
#define FFT_POS_CURSOR_0            (FFT_POS_CURSOR(0))
#define FFT_POS_CURSOR_1            (FFT_POS_CURSOR(1))

#define ENABLED_FFT                 (set.math.enableFFT)                // SettingsMath.enableFFT

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
    uint8           currentCursor;  // Определяет, каким курсором спектра управляет ручка УСТАНОВКА.
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
};
