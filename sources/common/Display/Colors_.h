// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"


struct Channel;


// Тип цвета
struct Color
{
    static const Color BLACK;
    static Color WHITE;
    static Color GRID;
    static Color DATA_A;
    static Color DATA_B;
    static Color MENU_FIELD;
    static Color MENU_TITLE;
    static Color MENU_TITLE_DARK;       // Чуть менее светлый цвет, чем цвет заголовка страницы. Используется для
                                        // создания эффекта объёма.
    static Color MENU_TITLE_BRIGHT;     // Чуть более светлый цвет, чем цвет заголовка страницы. Используется для
                                        // создания эффекта объёма.
    static Color MENU_ITEM;
    static Color MENU_ITEM_DARK;        // Чуть менее светлый цвет, чем цвет элемента меню. Используется для создания
                                        // эффекта объёма.
    static Color MENU_ITEM_BRIGHT;      // Чуть более светлый цвет, чем цвет элемента меню. Используется для создания
                                        // эффекта объёма
    static Color MENU_SHADOW;
    static Color EMPTY;
    static Color EMPTY_A;
    static Color EMPTY_B;

    static const uint8 Count = 16;

    static Color FLASH_10;
    static Color FLASH_01;
    static Color FILL;
    static Color BACK;
    static Color SET_INVERSE;

    uint8 index;

    Color(uint8 i) : index(i) {}

    Color(const Color &col) : index(col.index) {}

    static void ResetFlash();

    void SetAsCurrent() const;

    static Color GetCurrent() { return Color(current); };
    static Color Cursors(const Channel &ch);

    static Color Trig();

    // Цвет окантовки меню.
    static Color BorderMenu()       { return Color::MENU_TITLE; }

    // Светлый цвет в тени.
    static Color LightShadingText() { return BorderMenu(); }

    // Возвращает цвет, контрастный к color. Может быть белым или чёрным.
    static Color Contrast(const Color &color);

    bool operator==(const Color &rhs) { return (index == rhs.index); }
    bool operator!=(const Color &rhs) { return (index != rhs.index); }
    Color &operator=(const Color &rhs) { this->index = rhs.index; return *this; }

    static uint Make(uint8 r, uint8 g, uint8 b)  { return ((uint)(((b)) + (((g)) << 8) + (((r)) << 16))); }

private:

    static Color current;
};


struct ColorType
{
    float   red;
    float   green;
    float   blue;
    float   stepRed;
    float   stepGreen;
    float   stepBlue;
    float   brightness;
    uint8   color;
    int8    currentField;
    bool    alreadyUsed;

    void Init();
    void BrightnessChange(int delta);
    void SetBrightness(float brightness);
    void ComponentChange(int delta);

private:
    void SetBrightness();
    void Set();
    void CalcSteps();
};


#define R_FROM_COLOR(color) (((uint)(color) >> 16) & (uint)0xFF)
#define G_FROM_COLOR(color) (((uint)(color) >> 8) & (uint)0xFF)
#define B_FROM_COLOR(color) ((uint)(color) & 0xFF)
#define COLOR(i) set.display.colors[i]
