#pragma once


struct DataItem
{
    TypeItem::E  type;
    const Page  *keeper;
    pFuncBV      funcOfActive;
    const char **titleHint;
    const void  *ad;
};


class Item
{
public:

    static const int TITLE_WIDTH = 79;
    static const int TITLE_HEIGHT = 34;
    static const int HEIGHT = 33;
    static const int WIDTH = TITLE_WIDTH + 1;
    static const int HEIGHT_VALUE = 13;
    static const int OPENED_HEIGHT = 14;
    static const int OPENED_WIDTH = TITLE_WIDTH;
    static const int OPENED_HEIGHT_TITLE = 19;
    static const int WIDTH_VALUE = WIDTH - 4;


    const DataItem *data;

    Item(const DataItem *const data = nullptr);

    // Возвращает true, если кнопка, соответствующая данному элементу меню, находится в нажатом положении.
    bool IsPressed() const;

    // Вовзращает true, если элемент меню item является ативным, т.е. может быть нажат.
    bool IsActive() const;

    TypeItem::E Type() const;

    // Возвращает адрес элемента, которому принадлежит элемент по адресу item.
    Page *Keeper() const;

    // Открыть/закрыть элемент меню по адрему item.
    void Open(bool open) const;

    // Сделать/разделать текущим пункт страницы.
    void SetCurrent(bool active) const;

    // Возвращает true, если элемент меню по адрему item открыт.
    bool IsOpened() const;

    int OpenedPosY() const;

    // Уменьшает или увеличивает значение Governor, GovernorColor или Choice по адресу item в зависимости от знака delta
    void Change(int delta);

    bool ChangeOpened(int delta);

    // Возвращает высоту в пикселях открытого элемента Choice или NamePage::E по адресу item.
    int HeightOpened() const;

    // Возвращает название элемента по адресу item, как оно выглядит на дисплее прибора.
    pString Title() const;

    void DrawVolumeButton(int x, int y, int width, int height, int thickness, Color::E normal, Color::E bright, Color::E dark, bool isPressed) const;

    // Возвращает изображение регулятора, соответствующее его текущему положению.
    char GetSymbolForGovernor() const;

    bool IsPage() const          { return data->type == TypeItem::Page; }
    bool IsChoice() const        { return data->type == TypeItem::Choice; }
    bool IsChoiceReg() const     { return data->type == TypeItem::ChoiceReg; }
    bool IsGovernor() const      { return data->type == TypeItem::Governor; }
    bool IsGovernorColor() const { return data->type == TypeItem::GovernorColor; }
    bool IsButton() const        { return data->type == TypeItem::Button; }
    bool IsSmallButton() const   { return data->type == TypeItem::SmallButton; }
    bool IsIP() const            { return data->type == TypeItem::IP; }
    bool IsMAC() const           { return data->type == TypeItem::MAC; }
    bool IsTime() const          { return data->type == TypeItem::Time; }

    const TimeItem *ReinterpretToTime()     const { return (TimeItem *)this; }
    const Governor *ReinterpretToGovernor() const { return (Governor *)this; }
    const Choice *ReinterpretToChoice()     const { return (Choice *)this; }
    const Page *ReinterpretToPage()         const { return (Page *)this; }

    static DataItem emptyData;

    static Item empty;

    // Обработка короткого нажатия
    virtual void ShortPress() {};

    // Обработка длинного нажатия на элемент меню item.
    virtual void LongPress();

    virtual void Draw(int, int, bool = false) {};

protected:
    static int8 gCurDigit;
};
