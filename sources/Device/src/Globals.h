#pragma once
#include "common/Keyboard/Controls_c.h"
#include "Settings/SettingsTypes.h"
#include "Settings/SettingsChannel.h"
#include "Settings/SettingsTime.h"
#include "common/Hardware/HAL/HAL_c.h"


class Item;


#define NEED_OPEN_FILE_MANAGER      (gBF.needOpenFileMananger)
#define FLASH_DRIVE_IS_CONNECTED    (gBF.flashDriveIsConnected)
#define NEED_CLOSE_PAGE_SB          (gBF.needClosePageSB)
#define NEED_RESET_SETTINGS         (gBF.needResetSettings)
#define CURSORS_IN_DIRS             (gBF.cursorInDirs)
#define SHOW_HELP_HINTS             (gBF.showHelpHints)
#define SHOW_DEBUG_MENU             (gBF.showDebugMenu)


struct BitField
{
    uint flashDriveIsConnected      : 1;
    uint cursorInDirs               : 1;    // Если 1, то ручка УСТАНОВКА перемещает по каталогам
    uint showHelpHints              : 1;    // Если 1, то при нажатии кнопки вместо выполнения её фунции выводится подсказка о её назначении
    uint showDebugMenu              : 1;
    uint needOpenFileMananger       : 1;    // Если 1, то нужно открыть файловый менеджер (сработало автоподключение)
    uint needClosePageSB            : 1;    // Если 1, нужно закрывать страницу малых кнопок
    uint needResetSettings          : 1;    // Если 1, то необходим сброс настроек
};


extern BitField gBF;

#define CURRENT_NUM_LATEST_SIGNAL       (gMemory.currentNumLatestSignal)
#define CURRENT_NUM_INT_SIGNAL          (gMemory.currentNumIntSignal)
#define EXIT_FROM_INT_TO_LAST           (gMemory.exitFromIntToLast)
#define RUN_FPGA_AFTER_SMALL_BUTTONS    (gMemory.runningFPGAbeforeSmallButtons)
#define INT_SHOW_ALWAYS                 (gMemory.showAlways)
#define NEED_SAVE_TO_DRIVE              (gMemory.needForSaveToFlashDrive)

#define EXIT_FROM_SET_NAME_TO           (gMemory.exitFromModeSetNameTo)
#define RETURN_TO_MAIN_MENU             0U
#define RETURN_TO_LAST_MEM              1U
#define RETURN_TO_INT_MEM               2U
#define RETURN_TO_DISABLE_MENU          3U
#define EXIT_FROM_SET_NAME_TO_MAIN_MENU (EXIT_FROM_SET_NAME_TO == RETURN_TO_MAIN_MENU)
#define EXIT_FROM_SET_NAME_TO_LAST      (EXIT_FROM_SET_NAME_TO == RETURN_TO_LAST_MEM)
#define EXIT_FROM_SET_NAME_TO_INT       (EXIT_FROM_SET_NAME_TO == RETURN_TO_INT_MEM)
#define EXIT_FROM_SET_NAME_TO_DIS_MENU  (EXIT_FROM_SET_NAME_TO == RETURN_TO_DISABLE_MENU)

struct GMemory
{
    int16   currentNumLatestSignal;                 // Текущий номер последнего сигнала в режиме ПАМЯТЬ - Последние
    int8    currentNumIntSignal;                    // Текущий номер сигнала, сохранённого в ППЗУ
    uint    showAlways                      : 1;    // Если 1, то показывать всегда выбранный в режиме "Внутр. ЗУ" сигнал
    uint    runningFPGAbeforeSmallButtons   : 1;    // Здесь сохраняется информация о том, работала ли ПЛИС перед переходом в режим работы с памятью
    uint    exitFromIntToLast               : 1;    // Если 1, то выходить из страницы внутренней памяти нужно не стандартно, а в меню последних
    uint    exitFromModeSetNameTo           : 2;    // Куда возвращаться из окна установки имени при сохранении : 0 - в основное меню, 1 - в окно 
                                                    // последних, 2 - в окно Внутр ЗУ, 3 - в основно окно в выключенным меню
    uint    needForSaveToFlashDrive         : 1;    // Если 1, то нужно сохранить после отрисовки на флешку.
};


extern GMemory gMemory;


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
    Pause    // Это состояние, когда временно приостановлен прибор, например, для чтения данных или для 
             // записи значений регистров.
};};


struct StateFPGA
{
    bool needCalibration;				        // Установленное в true значение означает, что необходимо произвести калибровку.
    StateWorkFPGA::E stateWorkBeforeCalibration;
    StateCalibration::E stateCalibration;          // Текущее состояние калибровки. Используется в процессе калибровки.
};

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


void SetItemForHint(Item *item);

extern StateFPGA gStateFPGA;

extern uint8        *gData0;    // Указатель на данные первого канала, который надо рисовать на экране
extern uint8        *gData1;    // Указатель на данные второго канала, который надо рисовать на экране
extern DataSettings *gDSet;     // Указатель на параметры рисуемых сигналов

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

extern int transmitBytes;
