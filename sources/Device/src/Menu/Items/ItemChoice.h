// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct DataChoice
{
    pchar    *names;             // Варианты выбора на русском и английском языках.
    int8     *cell;              // Адрес ячейки, в которой хранится позиция текущего выбора.
    pFuncVB	  funcOnChanged;     // Функция должна вызываться после изменения значения элемента.
    pFuncVII  funcForDraw;       // Функция вызывается после отрисовки элемента. 
};


class Choice : public Item
{
public:
    Choice(const DataItem *const data) : Item(data) {};

    const DataChoice *OwnData() const {
        return (const DataChoice *)(data->ad);
    }

    pchar NameSubItem(int i) const;
    // Возвращает имя текущего варианта выбора элемента choice, как оно записано в исходном коде программы.
    pchar NameCurrentSubItem() const;
    // Возвращает имя следующего варианта выбора элемента choice, как оно записано в исходном коде программы.
    pchar NameNextSubItem() const;

    pchar NamePrevSubItem() const;
    // Возвращает количество вариантов выбора в элементе по адресу choice.
    int NumSubItems() const;

    void StartChange(int delta);
    // Рассчитывает следующий кадр анимации.
    float Step() const;
    // Изменяет значение choice в зависимости от величины и знака delta.
    void ChangeOpened(int delta);

    void FuncOnChanged(bool active) const;

    void FuncForDraw(int x, int y) const;

    virtual void Draw(int x, int y, bool opened = false) const;
    // Обработка короткого нажатия
    virtual void ShortPress();

private:
    void DrawClosed(int x, int y) const;

    void DrawOpened(int x, int y) const;
};
