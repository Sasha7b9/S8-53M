#pragma once
#include <ff.h>
#include "Display/Display.h"
    

// Key configuration
#define KEY_START_ADDRESS   (uint)0x0
#define KEY_PAGE_NUMBER     20
#define KEY_VALUE           0xAAAA5555

// Flash configuration

#define MAIN_PROGRAM_PAGE_NUMBER    21
#define NUM_OF_PAGES                256
#define FLASH_PAGE_SIZE             2048

#define TIME_WAIT   5000    // Время работы заставки


struct State { enum E
{
    Start,            // Исходное состояние
    Mount,            // Монтирование флешки
    WrongFlash,       // Флешка есть, но прочитать нельзя
    RequestAction,    // Что делать - апгрейдить или нет
    NotFile,          // Если диск примонтирован, но обновления на нём нету
    Upgrade,          // Процесс апгрейда
    Ok                // Обновление удачно завершено
};};

struct StateDisk { enum E
{
    Idle,
    Start
};};

struct FDrive
{
    FATFS USBDISKFatFS;
    char USBDISKPath[4];
    StateDisk::E state;
    FIL file;
    int connection;
    int active;
};

struct MainStruct
{
    FDrive drive;
    Display display;
    float percentUpdate;
    State::E state;
};


// Данная структура используется во всех модулях программы для уменьшения расхода ОЗУ
// Память для деё должна быть выделена с помощью malloc в начале программы и возвращена в момент перехода на основную программу
extern MainStruct *ms; //-V707
