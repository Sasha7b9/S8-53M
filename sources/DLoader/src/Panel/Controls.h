#pragma once


// Идентификаторы кнопок.
struct PanelButton { enum E
{
    Empty,        // кнопка не нажата
    Channel0,     // КАНАЛ 1
    Service,      // СЕРВИС
    Channel1,     // КАНАЛ 2
    Display,      // ДИСПЛЕЙ
    Time,         // РАЗВ
    Memory,       // ПАМЯТЬ
    Trig,         // СИНХР
    Start,        // ПУСК/СТАРТ
    Cursors,      // КУРСОРЫ
    Measures,     // ИЗМЕР
    Power,        // ПИТАНИЕ
    Help,         // ПОМОЩЬ
    Menu,         // МЕНЮ
    F1,           // 1
    F2,           // 2
    F3,           // 3
    F4,           // 4
    F5,           // 5
    Count            // общее количество кнопок
};};

// Идентификаторы регуляторов.
enum Regulator
{
    R_Empty     = 0,    // регулятор не вращался
    R_Range0    = 20,   // ВОЛЬТ/ДЕЛ кан. 1
    R_RShift0   = 21,   
    R_Range1    = 22,   // ВОЛЬТ/ДЕЛ кан. 2
    R_RShift1   = 23,
    R_TBase     = 24,   // ВРЕМЯ/ДЕЛ
    R_TShift    = 25,
    R_TrigLev   = 26,   // УРОВЕНЬ
    R_Set       = 27    // УСТАНОВКА
};

// Направления вращения регуляторов.
enum RegulatorDirection
{
    Direction_Left,     // вращение регулятора влево
    Direction_Right,    // вращение регулятора вправо
    Direction_None      // вращение отсутствует
};


#define NUM_REGULATORS (R_Set - R_Range0 + 1)


const char* NameButton(PanelButton::E button);
