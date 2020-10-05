#pragma once


// Идентификаторы кнопок.
struct PanelButton { enum E
{
    Empty,      // кнопка не нажата
    Channel0,   // КАНАЛ 1
    Service,    // СЕРВИС
    Channel1,   // КАНАЛ 2
    Display,    // ДИСПЛЕЙ
    Time,       // РАЗВ
    Memory,     // ПАМЯТЬ
    Trig,       // СИНХР
    Start,      // ПУСК/СТАРТ
    Cursors,    // КУРСОРЫ
    Measures,   // ИЗМЕР
    Power,      // ПИТАНИЕ
    Help,       // ПОМОЩЬ
    Menu,       // МЕНЮ
    F1,         // 1
    F2,         // 2
    F3,         // 3
    F4,         // 4
    F5,         // 5
    Count            // общее количество кнопок
};};

PanelButton::E& operator++(PanelButton::E &button);

// Идентификаторы регуляторов.
struct Regulator { enum E
{
    Empty     = 0,    // регулятор не вращался
    Range0    = 20,   // ВОЛЬТ/ДЕЛ кан. 1
    RShift0,
    Range1,           // ВОЛЬТ/ДЕЛ кан. 2
    RShift1,
    TBase,            // ВРЕМЯ/ДЕЛ
    TShift,
    TrigLev,          // УРОВЕНЬ
    Set               // УСТАНОВКА
};};


const char* NameButton(PanelButton::E button);
