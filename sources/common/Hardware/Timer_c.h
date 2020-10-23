#pragma once 
#include "Utils/GlobalFunctions.h"


struct TypeTimer { enum E
{
    PressKey,                 // Нужно устанавливать, когда приходит нажатие клавиши.
    ShowLevelRShift0,         // Нужно устанавливать, когда изменяется положение ручки смещения канала 1.
    ShowLevelRShift1,         // Нужно устанавливать, когда изменяется положение ручки смещения канала 2.
    ShowLevelTrigLev,         // Нужно устанавливать, когда изменяется положение ручки уровня синхронизации
    NumSignalsInSec,          // Для установки количества считываний сигнала в секунду.
    FlashDisplay,             // Таймер для мерцающих участков экрана чёрно-белого дисплея.
    P2P,                      // Таймер для чтения данных в режиме поточечного вывода.
    ShowMessages,             // Таймер для засекания время показа информационных и предупреждающих сообщений.
    MenuAutoHide,             // Таймер для отсчёта времени скрывания меню.
    RShiftMarkersAutoHide,    // Таймер для отсчёта времени скрывания дополнительных боковых меток.
    Temp,                     // Вспомогательный, для общих нужд.
    StopSound,                // Выключить звук
    TemporaryPauseFPGA,       // Временная пауза для фиксации сигнала на экране после поворота ручек
    TimerDrawHandFunction,    // Этот таймер могут использовать функции, использующие свою процедуру рисования
    Count                     // Общее количество таймеров.
};};


class Timer
{
public:

    static void PauseOnTicks(uint numTicks);
    // Функция вызывается по прерыванию системного таймера для работы таймеров. Увеличивает внутренее время таймеров на 1мс.
    static void Update1ms();
         
    static void Enable(TypeTimer::E type, int timeInMS, pFuncVV);
    // Остановить таймер.
    static void Disable(TypeTimer::E type);
    // Устанавливает стартовую точку логгирования. Далее вызовы Timer_LogPoint засекают временные интервалы от это точки.
    static void StartLogging();
    
    static uint LogPointUS(char *name);
    
    static uint LogPointMS(char *name);
};
