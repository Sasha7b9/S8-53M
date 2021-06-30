// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "main.h"


typedef enum
{
    APPLICATION_IDLE = 0,
    APPLICATION_START,
    APPLICATION_RUNNING,
}MSC_ApplicationTypeDef;


extern MSC_ApplicationTypeDef Appli_state;


struct StateDisk { enum E {
    Idle,
    Start
}; };


struct FDrive
{
    static FATFS USBDISKFatFS;
    static char USBDISKPath[4];
    static StateDisk::E state;
    static FIL file;
    static bool connected;
    static bool active;

    static void Init();

    static bool Update();

    static bool FileExist(pchar fileName);

    static int OpenFileForRead(pchar fileName);

    // Считывает из открытого файла numBytes байт. Возвращает число реально считанных байт
    static uint ReadFromFile(int numBytes, uint8 *buffer);

    static void CloseOpenedFile();
};


struct State { enum E {
    Start,            // Исходное состояние
    Mount,            // Монтирование флешки
    WrongFlash,       // Флешка есть, но прочитать нельзя
    RequestAction,    // Что делать - апгрейдить или нет
    NotFile,          // Если диск примонтирован, но обновления на нём нету
    Upgrade,          // Процесс апгрейда
    Ok                // Обновление удачно завершено
}; };


struct MainStruct
{
    float percentUpdate;
    volatile State::E state;

    // Данная структура используется во всех модулях программы для уменьшения расхода ОЗУ
    // Память для деё должна быть выделена с помощью malloc в начале программы и возвращена в момент перехода на основную программу
    static MainStruct *ms;
};
