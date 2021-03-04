#pragma once


struct DataGovernorColor
{
    ColorType *colorType;                  // Структура для описания цвета.
};


class GovernorColor : public Item
{
public:
    GovernorColor(const DataItem *const data) : Item(data) {};

    const DataGovernorColor *OwnData() const {
        return static_cast<const DataGovernorColor *>(data->ad);
    }

    virtual void Draw(int x, int y, bool opened = false) const;
    // Изменить яркость цвета в governor
    void ChangeValue(int delta);

    virtual void ShortPress();

private:
    void DrawOpened(int x, int y) const;
    void DrawClosed(int x, int y) const;
    void DrawValue(int x, int y, int delta) const;
};
