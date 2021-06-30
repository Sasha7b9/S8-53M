// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <ff.h>
#include "Display/Display.h"
#include "FDrive/FDrive.h"
    

// Key configuration
#define KEY_START_ADDRESS   (uint)0x0
#define KEY_PAGE_NUMBER     20
#define KEY_VALUE           0xAAAA5555

// Flash configuration

#define MAIN_PROGRAM_PAGE_NUMBER    21
#define NUM_OF_PAGES                256
#define FLASH_PAGE_SIZE             2048

#define TIME_WAIT   10000    // Время работы заставки


struct State { enum E {
    NoDrive,            // Стартовое значение. Если оно, то не обнаружена флешка
    Start,              // Исходное состояние
    NeedMount,          // Требуется монтирование флешки
    Mounted,            // Флешка примонтирована
    WrongFlash,         // Флешка есть, но прочитать нельзя
    RequestAction,      // Что делать - апгрейдить или нет
    NotFile,            // Если диск примонтирован, но обновления на нём нету
    Upgrade,            // Процесс апгрейда
    Ok                  // Обновление удачно завершено
}; };


struct MainStruct
{
    static float percentUpdate;
    static State::E state;
};
