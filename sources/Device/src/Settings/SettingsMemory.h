#pragma once


#define FILE_NAME_MASK              (set.memory.fileNameMask)                       // SettingsMemory.fileNameMask

#define FILE_NAME                   (set.memory.fileName)                           // SettingsMemory.fileName

#define INDEX_SYMBOL                (set.memory.indexCurSymbolNameMask)             // SettingsMemory.indexCurSymbolNameMask

#define FLASH_AUTOCONNECT           (set.memory.flashAutoConnect)                   // SettingsMemory.flashAutoConnect

#define MODE_SAVE_SIGNAL            (set.memory.modeSaveSignal)                     // SettingsMemory.modeSaveSignal
#define MODE_SAVE_SIGNAL_IS_BMP     (MODE_SAVE_SIGNAL == ModeSaveSignal::BMP)


struct StructMemoryLast
{
    bool isActiveModeSelect;
};

// Настройки МЕНЮ->ПАМЯТЬ
struct SettingsMemory
{
#define MAX_SYMBOLS_IN_FILE_NAME 35
    ENUM_POINTS_FPGA::E enum_points_fpga;                       // Число точек.
    ModeWork::E         modeWork;                               // Режим работы.
    FileNamingMode::E   fileNamingMode;                         // Режим именования файлов.
    char                fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]; // Здесь маска для автоматического именования файлов\n
         // Правила именования.\n
         // %y('\x42') - год, %m('\x43') - месяц, %d('\x44') - день, %H('\x45') - часы, %M('\x46') - минуты, %S('\x47') - секунды\n
         // %Nn('\x48''n') - порядковый номер, котрый занимает не менее n знакомест, например, 7 в %3N будет преобразовано в 007\n
         // Примеры\n
         // name_%4N_%y_%m_%d_%H_%M_%S будет генерировать файлы вида name_0043_2014_04_25_14_45_32\n
         // При этом обратите внимание, что если спецификатор %4N стоИт после временнЫх параметров, то, скорее всего, этот параметр будет всегда равен 0001, т.к. для определения номера просматриваются только символы ДО него.
    char                fileName[MAX_SYMBOLS_IN_FILE_NAME];     // Имя файла для режима ручного задания
    int8                indexCurSymbolNameMask;                 // Индекс текущего символа в режиме задания маски или выбора имени.
    StructMemoryLast    strMemoryLast;
    ModeShowIntMem::E   modeShowIntMem;                         // Какие сигналы показывать в режиме внутреннего ЗУ
    bool                flashAutoConnect;                       // Если true, при подлючении флеш автоматически выводится NC (Нортон Коммандер)
    ModeBtnMemory::E    modeBtnMemory;
    ModeSaveSignal::E   modeSaveSignal;                         // В каком виде сохранять сигнал.

    static int GetNumPoints(bool forCalculate);
    static ENUM_POINTS_FPGA::E IntNumPoints2FPGA_NUM_POINTS(int numPoints);
};
