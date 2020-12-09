#pragma once


struct DataSmallButton
{
    pFuncVV           funcOnPress;    // Эта функция вызвается для обработки нажатия кнопки. Возвращает true, если надо за
    pFuncVII          funcOnDraw;     // Эта функция вызывается для отрисовки кнопки в месте с координатами x, y.
    const arrayHints *hintUGO;
};

// Описывает кнопку для дополнительного режима меню.
class SmallButton : public Item
{
public:

    static const int WIDTH = 19;
    static const int LEFT = 299;

    SmallButton(const DataItem *const data) : Item(data) {};

    const DataSmallButton *OwnData() const {
        return static_cast<const DataSmallButton *>(data->ad); //-V2571
    }

    virtual void Draw(int x, int y, bool opened = false) const;

    virtual void ShortPress();

    void DrawHints(int x, int y, int width);
};
