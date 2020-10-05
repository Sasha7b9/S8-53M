#pragma once

// Здесь ID всех представленных в меню страниц
enum NamePage
{
    Page::EDisplay,               // ДИСПЕЙ
    Page::EDisplayAccumulation,   // ДИСПЛЕЙ - НАКОПЛЕНИЕ
    Page::EDisplayAverage,        // ДИСПЛЕЙ - УСРЕДНЕНИЕ
    Page::EDisplayGrid,           // ДИСПЛЕЙ - СЕТКА
    Page::EDisplayRefresh,        // ДИСПЛЕЙ - ОБНОВЛЕНИЕ
    Page::EMainPage,              // Главная страница меню. Из неё растут все остальные
    Page::EDebug,                 // ОТЛАДКА
    Page::EDebugConsole,          // ОТЛАДКА - КОНСОЛЬ
    Page::EDebugADC,              // ОТЛАДКА - АЦП
    Page::EDebugADCbalance,       // ОТЛАДКА - АЦП - БАЛАНС
    Page::EDebugADCstretch,       // ОТЛАДКА - АЦП - РАСТЯЖКА
    Page::EDebugADCrShift,        // ОТЛАДКА - АЦП - ДОП. СМЕЩЕНИЕ
    Page::EDebugADCaltShift,      // ОТЛАДКА - АЦП - ДОП. СМЕЩ. ПАМ.
    Page::EDebugShowRegisters,    // ОТЛАДКА - РЕГИСТРЫ
    Page::EDebugRandomizer,       // ОТЛАДКА - РАНД-ТОР
    Page::EDebugInformation,      // ОТЛАДКА - ИНФОРМАЦИЯ
    Page::EDebugSound,            // ОТЛАДКА - ЗВУК
    Page::EDebugAltera,           // ОТЛАДКА - АЛЬТЕРА
    Page::EChannel0,              // КАНАЛ 1
    Page::EChannel1,              // КАНАЛ 2
    Page::ECursors,               // КУРСОРЫ
    Page::ETrig,                  // СИНХР.
    Page::EMemory,                // ПАМЯТЬ
    Page::EMemoryExt,             // ПАМЯТЬ - ВНЕШН. ЗУ
    Page::EMeasures,              // ИЗМЕРЕНИЯ
    Page::EMeasuresField,         // ИЗМЕРЕНИЯ - ЗОНА
    Page::EService,               // СЕРВИС
    Page::EServiceDisplay,        // СЕРВИС - ДИСПЛЕЙ
    Page::EServiceDisplayColors,  // СЕРВИС - ДИСПЛЕЙ - ЦВЕТА
    Page::EServiceCalibrator,     // СЕРВИС - КАЛИБРАТОР
    Page::EServiceEthernet,       // СЕРВИС - ETHERNET
    Page::EHelp,                  // ПОМОЩЬ
    Page::ETime,                  // РАЗВЕРТКА
    Page::EMath,                  // СЕРВИС - МАТЕМАТИКА
    Page::EMathFFT,
    Page::ETrigAuto,              // СИНХР - АВТ ПОИСК
    NamePage::SB_Curs,               // Открыта страница КУРСОРЫ - Измерения
    NamePage::SB_MeasTuneMeas,       // Открыта страница ИЗМЕРЕНИЯ - Настроить
    NamePage::SB_MeasTuneField,      // Открыта страница ИЗМЕРЕНИЯ - ЗОНА - Задать
    NamePage::SB_MemLatest,          // Открыта страница ПАМЯТЬ - Последние
    NamePage::SB_MemInt,             // Открыта страница ПАМЯТЬ - Внутр ЗУ
    NamePage::SB_MemExtSetMask,      // Открыта страница ПАМЯТЬ - ВНЕШН ЗУ - Маска
    NamePage::SB_MemExtSetName,      // Появляется перед сохранением файла на флешку для задания имени файла при соответствующей опции ВНЕШН ЗУ - Имя файла
    NamePage::SB_FileManager,        // Открыта страница ПАМЯТЬ - ВНЕШН ЗУ - Обзор
    NamePage::SB_Help,               // Открыта страница HELP
    NamePage::SB_MathCursorsFFT,     // Открыта страница СЕРВИС - Математика - Курсоры
    NamePage::SB_MathFunction,       // СЕРВИС - МАТЕМАТИКА - ФУНКЦИЯ
    NamePage::SB_Information,        // СЕРВИС - ИНФОРМАЦИЯ
    NamePage::SB_SerialNumber,       // ОТАДКА - С/Н
    NamePage::Count,
    Page::ENoPage
};             // При добавлении страниц нужно изменять инициализацию SettingsMenu в SSettings.c
