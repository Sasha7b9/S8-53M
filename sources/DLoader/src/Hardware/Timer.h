#pragma once 
#include "defines.h"
#include "Utils/GlobalFunctions.h"


struct TypeTimer { enum E
{
    PressKey,                   // Нужно устанавливать, когда приходит нажатие клавиши.
    ShowLevelRShift0,           // Нужно устанавливать, когда изменяется положение ручки смещения канала 1.
    ShowLevelRShift1,           // Нужно устанавливать, когда изменяется положение ручки смещения канала 2.
    ShowLevelTrigLev,           // Нужно устанавливать, когда изменяется положение ручки уровня синхронизации
    NumSignalsInSec,            // Для установки количества считываний сигнала в секунду.
    FlashDisplay,               // Таймер для мерцающих участков экрана чёрно-белого дисплея.
    P2P,                        // Таймер для чтения данных в режиме поточечного вывода.
    ShowMessages,               // Таймер для засекания время показа информационных и предупреждающих сообщений.
    MenuAutoHide,               // Таймер для отсчёта времени скрывания меню.
    RShiftMarkersAutoHide,      // Таймер для отсчёта времени скрывания дополнительных боковых меток.
    Temp,                       // Вспомогательный, для общих нужд.
    StopSound,                  // Выключить звук
    TemporaryPauseFPGA,         // Временная пауза для фиксации сигнала на экране после поворота ручек
    TimerDrawHandFunction,      // Этот таймер могут использовать функции, использующие свою процедуру рисования
    Count                       // Общее количество таймеров.
};};


// Приостанавливает выполнение программы на timeMS микросекунд
void Timer_PauseOnTime(uint timeMS);
// Приостанавливает выполнение программы на numTicks тиков
void Timer_PauseOnTicks(uint numTicks);
// Функция вызывается по прерыванию системного таймера для работы таймеров. Увеличивает внутренее время таймеров на 1мс.
void Timer_Update1ms(void);
// Функция вызывается по прерыванию системного таймера для работы таймеров. Увеличивает внутреннее время таймеров на 10мс.
void Timer_Update10ms(void);
// Запуск таймера
void Timer_Enable(TypeTimer::E type,   // Тип таймера
                  int timeInMS,     // Время срабатывания таймера
                  pFuncVV func      // Функция, которая вызывается каждый раз, когда срабатывает таймер
                  );
// Остановить таймер
void Timer_Disable(TypeTimer::E type);
// Приостановить таймер. Запустить снова можно функцией Timer_Continue()
void Timer_Pause(TypeTimer::E type);
// Продолжить работу таймера, ранее приостановленного функцией Timer_Pause()
void Timer_Continue(TypeTimer::E type);
// С помощью этой функции можно узнать, работает ли таймер
bool Timer_IsRun(TypeTimer::E type);
// Устанавливает стартовую точку логгирования. Далее вызовы Timer_LogPoint засекают временные интервалы от это точки.
void Timer_StartLogging(void);               

uint Timer_LogPointUS(char *name);

uint Timer_LogPointMS(char *name);

struct Timer
{
    static void Update1ms();
};
