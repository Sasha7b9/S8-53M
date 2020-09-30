#pragma once


// Идентификаторы кнопок.
enum PanelButton
{
    B_Empty,        // кнопка не нажата
    B_Channel0,     // КАНАЛ 1
    B_Service,      // СЕРВИС
    B_Channel1,     // КАНАЛ 2
    B_Display,      // ДИСПЛЕЙ
    B_Time,         // РАЗВ
    B_Memory,       // ПАМЯТЬ
    B_Trig,         // СИНХР
    B_Start,        // ПУСК/СТАРТ
    B_Cursors,      // КУРСОРЫ
    B_Measures,     // ИЗМЕР
    B_Power,        // ПИТАНИЕ
    B_Help,         // ПОМОЩЬ
    B_Menu,         // МЕНЮ
    B_F1,           // 1
    B_F2,           // 2
    B_F3,           // 3
    B_F4,           // 4
    B_F5,           // 5
    B_NumButtons            // общее количество кнопок
};

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


const char* NameButton(PanelButton button);
