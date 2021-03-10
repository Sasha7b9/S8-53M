#pragma once


struct Key { enum E
{
        None,       // 
        F1,         // 1
        F2,         // 2
        F3,         // 3
        F4,         // 4
        F5,         // 5
        ChannelA,   // КАНАЛ 1
        ChannelB,   // КАНАЛ 2
        Time,       // РАЗВ
        Trig,       // СИНХР
        Cursors,    // КУРСОРЫ
        Measures,   // ИЗМЕР
        Display,    // ДИСПЛЕЙ
        Help,       // ПОМОЩЬ
        Start,      // ПУСК/СТОП
        Memory,     // ПАМЯТЬ
        Service,    // СЕРВИС
        Menu,       // МЕНЮ
        Power,      // ВЫКЛ
        RangeA,     // ВОЛЬТ/ДЕЛ 1
        RangeB,     // ВОЛЬТ/ДЕЛ 2
        RShiftA,    // 
        RShiftB,    // 
        TBase,      // ВРЕМЯ/ДЕЛ
        TShift,     // 
        TrigLev,    // УРОВЕНЬ
        Setting,    // УСТАНОВКА
        Count
    };

    Key(E v) : value(v) { }

    pString Name() const;

    E value;

    static Key::E FromCode(uint16 code);
    static uint16 ToCode(Key::E key);
    static bool IsGovernor(Key::E key);
    static bool IsButton(Key::E key);
    static bool IsFunctionalButton(Key::E key);
};

Key::E& operator++(Key::E &right);


struct Action { enum E {
        Down,         // Нажатие кнопки
        Up,           // Отпускание кнопки
        Long,         // "Длинное" нажатие
        RotateLeft,   // Поворото ручки влево
        RotateRight,  // Поворот ручки вправо
        Count
    } value;

    Action(uint8 v) : value((E)v) {}
    static Action::E FromCode(uint16 code);
    static uint16 ToCode(Action::E action);
    bool IsDown() const { return value == Down; }
    bool IsUp() const { return value == Up; }
    bool IsLong() const { return value == Long; }
    bool IsRotateLeft() const { return value == RotateLeft; }
    bool IsRotateRight() const { return value == RotateRight; }
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

    bool IsRotate() const { return (action == Action::RotateRight) || (action == Action::RotateLeft); }
    
    bool operator==(const KeyboardEvent &rhl) const
    {
        return (rhl.key == key) && (rhl.action == action);
    }
};
