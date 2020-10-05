#pragma once

// Здесь ID всех представленных в меню страниц
enum NamePage
{
    NamePage::Display,               // ДИСПЕЙ
    NamePage::DisplayAccumulation,   // ДИСПЛЕЙ - НАКОПЛЕНИЕ
    NamePage::DisplayAverage,        // ДИСПЛЕЙ - УСРЕДНЕНИЕ
    NamePage::DisplayGrid,           // ДИСПЛЕЙ - СЕТКА
    NamePage::DisplayRefresh,        // ДИСПЛЕЙ - ОБНОВЛЕНИЕ
    NamePage::MainPage,              // Главная страница меню. Из неё растут все остальные
    NamePage::Debug,                 // ОТЛАДКА
    NamePage::DebugConsole,          // ОТЛАДКА - КОНСОЛЬ
    NamePage::DebugADC,              // ОТЛАДКА - АЦП
    NamePage::DebugADCbalance,       // ОТЛАДКА - АЦП - БАЛАНС
    NamePage::DebugADCstretch,       // ОТЛАДКА - АЦП - РАСТЯЖКА
    NamePage::DebugADCrShift,        // ОТЛАДКА - АЦП - ДОП. СМЕЩЕНИЕ
    NamePage::DebugADCaltShift,      // ОТЛАДКА - АЦП - ДОП. СМЕЩ. ПАМ.
    NamePage::DebugShowRegisters,    // ОТЛАДКА - РЕГИСТРЫ
    NamePage::DebugRandomizer,       // ОТЛАДКА - РАНД-ТОР
    NamePage::DebugInformation,      // ОТЛАДКА - ИНФОРМАЦИЯ
    NamePage::DebugSound,            // ОТЛАДКА - ЗВУК
    NamePage::DebugAltera,           // ОТЛАДКА - АЛЬТЕРА
    NamePage::Channel0,              // КАНАЛ 1
    NamePage::Channel1,              // КАНАЛ 2
    NamePage::Cursors,               // КУРСОРЫ
    NamePage::Trig,                  // СИНХР.
    NamePage::Memory,                // ПАМЯТЬ
    NamePage::MemoryExt,             // ПАМЯТЬ - ВНЕШН. ЗУ
    NamePage::Measures,              // ИЗМЕРЕНИЯ
    NamePage::MeasuresField,         // ИЗМЕРЕНИЯ - ЗОНА
    NamePage::Service,               // СЕРВИС
    NamePage::ServiceDisplay,        // СЕРВИС - ДИСПЛЕЙ
    NamePage::ServiceDisplayColors,  // СЕРВИС - ДИСПЛЕЙ - ЦВЕТА
    NamePage::ServiceCalibrator,     // СЕРВИС - КАЛИБРАТОР
    NamePage::ServiceEthernet,       // СЕРВИС - ETHERNET
    NamePage::Help,                  // ПОМОЩЬ
    NamePage::Time,                  // РАЗВЕРТКА
    NamePage::Math,                  // СЕРВИС - МАТЕМАТИКА
    NamePage::MathFFT,
    NamePage::TrigAuto,              // СИНХР - АВТ ПОИСК
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
    NamePage::NoPage
};             // При добавлении страниц нужно изменять инициализацию SettingsMenu в SSettings.c
