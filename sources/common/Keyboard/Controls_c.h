#pragma once


struct Key { enum E
{
        None,               // 
        F1,                 // 1
        F2,                 // 2
        F3,                 // 3
        F4,                 // 4
        F5,                 // 5
        ChannelA,           // КАНАЛ 1
        ChannelB,           // КАНАЛ 2
        Time,               // РАЗВ
        Synchronization,    // СИНХР
        Cursors,            // КУРСОРЫ
        Measures,           // ИЗМЕР
        Display,            // ДИСПЛЕЙ
        Help,               // ПОМОЩЬ
        Start,              // ПУСК/СТОП
        Memory,             // ПАМЯТЬ
        Service,            // СЕРВИС
        Menu,               // МЕНЮ
        RangeA,             // ВОЛЬТ/ДЕЛ 1
        RangeB,             // ВОЛЬТ/ДЕЛ 2
        RShiftA,            // 
        RShiftB,            // 
        TBase,              // ВРЕМЯ/ДЕЛ
        TShift,             // 
        TrigLev,            // УРОВЕНЬ
        Setting,            // УСТАНОВКА
        Count
    };

    Key(E v) : value(v) { }

    pString Name() const;

    E value;

    static Key::E FromCode(uint16 code);
    static uint16 ToCode(Key::E key);
};

Key::E& operator++(Key::E &right);


struct Action { enum E
{
        Down,         // Нажатие кнопки
        Up,           // Отпускание кнопки
        Long,         // "Длинное" нажатие
        RotateLeft,   // Поворото ручки влево
        RotateRight,  // Поворот ручки вправо
        Count
    };

    static Action::E FromCode(uint16 code);
    static uint16 ToCode(Action::E action);
};


struct KeyboardEvent
{
    Key::E key;
    Action::E action;

    bool Is(Key::E c) const           { return (c == key); };
    bool Is(Key::E c, Action::E a) const { return (key == c) && (action == a); };
       
    bool IsUp() const   { return (action == Action::Up); }
    bool IsDown() const { return (action == Action::Down); }
    bool IsLong() const { return (action == Action::Long); }
    // Возвращает true в случае отпускания кнопки или "длинного" нажатия
    bool IsRelease() const;

    // true, если функциональная клавиша
    bool IsFunctional() const;
    
    bool operator==(const KeyboardEvent &rhl) const
    {
        return (rhl.key == key) && (rhl.action == action);
    }
};
