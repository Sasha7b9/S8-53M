#pragma once


struct DataGovernor
{
    int16 *cell;
    int         minValue;               // Минмальное значение, которое может принимать регулятор.
    int16       maxValue;               // Максимальное значение.
    pFuncVV     funcOfChanged;          // Функция, которую нужно вызывать после того, как значение регулятора изменилось.
    pFuncVV     funcBeforeDraw;         // Функция, которая вызывается перед отрисовкой
};


// Описывает регулятор.
class Governor : public Item
{
public:

    Governor(const DataItem *const data) : Item(data) {};
    const DataGovernor *OwnData() const {
        return static_cast<const DataGovernor *>(data->ad); //-V2571
    }
    // Обработка короткого нажатия на элемент Governor с адресом governor.
    virtual void ShortPress();
    void StartChange(int detla);        // Запускает процессс анимации инкремента или декремента элемента меню типа Governor (в зависимости от знака delta)
    float Step() const;                 // Рассчитывате следующий кадр анимации.
    void  ChangeValue(int delta);       // Изменяет значение в текущей позиции при раскрытом элементе
    int16 NextValue() const;            // Возвращает следующее большее значение, которое может принять governor.
    int16 PrevValue() const;            // Возвращает следующее меньшее значение, которое может принять governor.

    virtual void Draw(int x, int y, bool opened = false) const;
    void Draw(int x, int y);

    static Item *address;

private:
    void DrawOpened(int x, int y) const;
    void DrawClosed(int x, int y) const;
    void DrawValue(int x, int y) const;

    // Возвращает число знакомест в поле для ввода элемента governor. Зависит от максимального значения, которое может принимать governor.
    int  NumDigits() const;
    // При открытом элементе переставляет курсор на следующую позицию
    void NextPosition();
    void DrawLowPart(int x, int y) const;

    static bool inMoveIncrease;
    static bool inMoveDecrease;
    static uint timeStartMS;
};
