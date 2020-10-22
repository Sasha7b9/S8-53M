#pragma once
#include "common/Keyboard/Controls_c.h"
#include "Settings/SettingsTypes.h"
#include "Settings/SettingsChannel.h"
#include "Settings/SettingsTime.h"
#include "common/Hardware/HAL/HAL_c.h"


class Item;


#define RETURN_TO_MAIN_MENU             0U
#define RETURN_TO_LAST_MEM              1U
#define RETURN_TO_INT_MEM               2U
#define RETURN_TO_DISABLE_MENU          3U


struct StateCalibration { enum E
{
    None,
    ADCinProgress,
    RShift0start,
    RShift0inProgress,
    RShift1start,
    RShift1inProgress,
    ErrorCalibration0,
    ErrorCalibration1
};};

struct StateWorkFPGA { enum E
{
    Stop,    // СТОП - не занимается считыванием информации.
    Wait,    // Ждёт поступления синхроимпульса.
    Work,    // Идёт работа.
    Pause    // Это состояние, когда временно приостановлен прибор, например, для чтения данных или для записи значений регистров.
};};


struct DataSettings
{
    void*         addrNext;               // Адрес следующей записи.
    void*         addrPrev;               // Адрес предыдущей записи.
    uint          rShiftCh0       : 10;   // Смещение по напряжению
    uint          rShiftCh1       : 10;
    uint          trigLevCh0      : 10;   // Уровень синхронизации
    int16         tShift;                 // Смещение по времени
    ModeCouple::E modeCouple1     : 2;
    Range::E      range[2];               // Масштаб по напряжению обоих каналов.

    uint          trigLevCh1      : 10;
    uint          length1channel  : 11;   // Сколько занимает в байтах длина измерения одного канала
    TBase::E      tBase           : 5;    // Масштаб по времени
    ModeCouple::E modeCouple0     : 2;    // Режим канала по входу
    uint          peakDet         : 2;    // Включен ли пиковый детектор
    uint          enableCh0       : 1;    // Включён ли канал 0
    uint          enableCh1       : 1;    // Включен ли канал 1

    uint          inverseCh0      : 1;
    uint          inverseCh1      : 1;
    Divider::E    multiplier0     : 1;
    Divider::E    multiplier1     : 1;
    PackedTime    time;
};


extern DataSettings *gDSmemInt;       //--
extern uint8        *gData0memInt;    // | Здесь данные из ППЗУ, которые должны выводиться на экран
extern uint8        *gData1memInt;    //-/

extern DataSettings *gDSmemLast;
extern uint8        *gData0memLast;
extern uint8        *gData1memLast;

extern void *extraMEM;      // Это специальный указатель. Используется для выделения памяти переменным, которые не нужны всё время выполения программы,
                            // но нужны болеее чем в одной функции. Перед использованием с помощью вызова malloc() выделяется необходимое количество
                            // памяти, которая затем освобождается вызвом free()

#define MALLOC_EXTRAMEM(NameStruct, name)   extraMEM = std::malloc(sizeof(NameStruct));    \
                                            NameStruct *name = (NameStruct*)extraMEM
#define ACCESS_EXTRAMEM(NameStruct, name)   NameStruct *name = (NameStruct*)extraMEM //-V2546 //-V1003
#define FREE_EXTRAMEM()                     std::free(extraMEM)
