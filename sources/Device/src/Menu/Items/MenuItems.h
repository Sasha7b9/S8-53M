// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Colors_.h"
#include "Display/Display.h"
#include "Display/Screen/Grid.h"
#include "Menu/Items/MenuItemsDefs.h"


class Page;
class SmallButton;
class TimeItem;
class Governor;
class Choice;


#define MAX_NUM_CHOICE_SMALL_BUTTON 6    // Максимальное количество вариантов маленькой кнопки + 1

                                            
// Разные виды пунктов меню
struct TypeItem { enum E
{
    None,           
    Choice,        // Пункт выбора - позволяет выбрать одно из нескольких заданных значений.
    Button,        // Кнопка.
    Page,          // Страница.
    Governor,      // Регулятор - позволяет выбрать любое целое числовое значение из заранее заданного диапазаона.
    Time,          // Позволяет ввести время.
    IP,            // Позволяет ввести IP-адрес.
    SwitchButton,
    GovernorColor, // Позволяет выбрать цвет.
    Formula,       // Позволяет выбрать знак и коэффициенты для математической формулы (умножение и сложение)
    MAC,           // MAC-адрес
    ChoiceReg,     // Элемент выбора, в котором выбор осуществляется не кнопкой, а ручкой
    SmallButton,   // Кнопка для режима малых кнопок
    Count
};};


// Здесь ID всех представленных в меню страниц
struct NamePage { enum E
{
    Empty,
    Display,               // ДИСПЕЙ
    DisplayAccumulation,   // ДИСПЛЕЙ - НАКОПЛЕНИЕ
    DisplayAverage,        // ДИСПЛЕЙ - УСРЕДНЕНИЕ
    DisplayGrid,           // ДИСПЛЕЙ - СЕТКА
    DisplayRefresh,        // ДИСПЛЕЙ - ОБНОВЛЕНИЕ
    MainPage,              // Главная страница меню. Из неё растут все остальные
    Debug,                 // ОТЛАДКА
    DebugConsole,          // ОТЛАДКА - КОНСОЛЬ
    DebugADC,              // ОТЛАДКА - АЦП
    DebugADCbalance,       // ОТЛАДКА - АЦП - БАЛАНС
    DebugADCstretch,       // ОТЛАДКА - АЦП - РАСТЯЖКА
    DebugADCrShift,        // ОТЛАДКА - АЦП - ДОП. СМЕЩЕНИЕ
    DebugADCaltShift,      // ОТЛАДКА - АЦП - ДОП. СМЕЩ. ПАМ.
    DebugShowRegisters,    // ОТЛАДКА - РЕГИСТРЫ
    DebugRandomizer,       // ОТЛАДКА - РАНД-ТОР
    DebugInformation,      // ОТЛАДКА - ИНФОРМАЦИЯ
    DebugSound,            // ОТЛАДКА - ЗВУК
    DebugAltera,           // ОТЛАДКА - АЛЬТЕРА
    ChannelA,              // КАНАЛ 1
    ChannelB,              // КАНАЛ 2
    Cursors,               // КУРСОРЫ
    Trig,                  // СИНХР.
    Memory,                // ПАМЯТЬ
    MemoryExt,             // ПАМЯТЬ - ВНЕШН. ЗУ
    Measures,              // ИЗМЕРЕНИЯ
    MeasuresField,         // ИЗМЕРЕНИЯ - ЗОНА
    Service,               // СЕРВИС
    ServiceDisplay,        // СЕРВИС - ДИСПЛЕЙ
    ServiceDisplayColors,  // СЕРВИС - ДИСПЛЕЙ - ЦВЕТА
    ServiceCalibrator,     // СЕРВИС - КАЛИБРАТОР
    ServiceEthernet,       // СЕРВИС - ETHERNET
    Help,                  // ПОМОЩЬ
    Time,                  // РАЗВЕРТКА
    Math,                  // СЕРВИС - МАТЕМАТИКА
    MathFFT,
    TrigAuto,              // СИНХР - АВТ ПОИСК
    SB_Curs,               // Открыта страница КУРСОРЫ - Измерения
    SB_MeasTuneMeas,       // Открыта страница ИЗМЕРЕНИЯ - Настроить
    SB_MeasTuneField,      // Открыта страница ИЗМЕРЕНИЯ - ЗОНА - Задать
    SB_MemLatest,          // Открыта страница ПАМЯТЬ - Последние
    SB_MemInt,             // Открыта страница ПАМЯТЬ - Внутр ЗУ
    SB_MemExtSetMask,      // Открыта страница ПАМЯТЬ - ВНЕШН ЗУ - Маска
    SB_MemExtSetName,      // Появляется перед сохранением файла на флешку для задания имени файла при соответствующей
                           // опции ВНЕШН ЗУ - Имя файла
    SB_FileManager,        // Открыта страница ПАМЯТЬ - ВНЕШН ЗУ - Обзор
    SB_Help,               // Открыта страница HELP
    SB_MathCursorsFFT,     // Открыта страница СЕРВИС - Математика - Курсоры
    SB_MathFunction,       // СЕРВИС - МАТЕМАТИКА - ФУНКЦИЯ
    SB_Information,        // СЕРВИС - ИНФОРМАЦИЯ
    SB_SerialNumber,       // ОТАДКА - С/Н
    Count,
    NoPage
};};             // При добавлении страниц нужно изменять инициализацию SettingsMenu в SSettings.c


#define TITLE(item) ((item)->data->titleHint[LANG])
#define HINT(item) ((item)->data->titleHint[2 + LANG])


class StructHelpSmallButton
{
public:
    pFuncVII funcDrawUGO;    // Указатель на функцию отрисовки изображения варианта кнопки
    pchar    helpUGO[2];     // Подпись к данному изображению.
};

typedef StructHelpSmallButton arrayHints[MAX_NUM_CHOICE_SMALL_BUTTON];

#include "Menu/Items/Item.h"
#include "Menu/Items/ItemPage.h"
#include "Menu/Items/ItemButton.h"
#include "Menu/Items/ItemSmallButton.h"
#include "Menu/Items/ItemGovernor.h"
#include "Menu/Items/ItemIPaddress.h"
#include "Menu/Items/ItemMACaddress.h"
#include "Menu/Items/ItemFormula.h"
#include "Menu/Items/ItemGovernorColor.h"
#include "Menu/Items/ItemChoice.h"
#include "Menu/Items/ItemTime.h"
