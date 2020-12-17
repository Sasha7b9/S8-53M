#pragma once
#include "common/Settings/SettingsTypes_c.h"


// Тип цвета
struct Color
{
    static Color BLACK;
    static Color WHITE;
    static Color GRID;
    static Color DATA_A;
    static Color DATA_B;
    static Color MENU_FIELD;
    static Color MENU_TITLE;
    static Color MENU_TITLE_DARK;       // Чуть менее светлый цвет, чем цвет заголовка страницы. Используется для создания эффекта объёма.
    static Color MENU_TITLE_BRIGHT;
    static Color MENU_ITEM;
    static Color MENU_ITEM_DARK;        // Чуть менее светлый цвет, чем цвет элемента меню. Используется для создания эффекта объёма.
    static Color MENU_ITEM_BRIGHT;      // Чуть более светлый цвет, чем цвет элемента меню. Используется для создания эффекта объёма.
    static Color MENU_SHADOW;
    static Color EMPTY;
    static Color EMPTY_A;
    static Color EMPTY_B;

    static Color Count;

    static Color FLASH_10;
    static Color FLASH_01;
    static Color FILL;
    static Color BACK;
    static Color SET_INVERSE;

    uint8 value;

    Color(uint8 v) : value(v) {}

    static void ResetFlash();

    void SetAsCurrent();

    static Color GetCurrent() { return current; };
    static Color Channel(Channel::E chan);
    static Color Cursors(Channel::E cnan);

    // Чуть более светлый цвет, чем цвет заголовка страницы. Используется для создания эффекта объёма.
    static Color MenuTitleBrighter()   { return Color::MENU_TITLE_BRIGHT; }

    static Color Trig();

    // Цвет окантовки меню.
    static Color BorderMenu()          { return Color::MENU_TITLE; }

    // Возвращает цвет, контрастный к color. Может быть белым или чёрным.
    static Color Contrast(const Color &color);

    // Светлый цвет в тени.
    static Color LightShadingText();

    bool operator==(const Color &rhs) { return (value == rhs.value); }
    bool operator!=(const Color &rhs) { return (value != rhs.value); }

private:

    static Color current;
};


// Тёмный цвет в тени.
#define DarkShadingTextColor()      Color::MENU_TITLE_DARK


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
};


void Color_BrightnessChange(ColorType *colorType, int delta);
void Color_SetBrightness(ColorType *colorType, float brightness);
void Color_Init(ColorType *colorType);
void Color_ComponentChange(ColorType *colorType, int delta);


const char* NameColor(const Color &color);
#define MAKE_COLOR(r, g, b) ((uint)(((b)) + (((g)) << 8) + (((uint)(r)) << 16)))
#define R_FROM_COLOR(color) (((uint)(color) >> 16) & (uint)0xFF)
#define G_FROM_COLOR(color) (((uint)(color) >> 8) & (uint)0xFF)
#define B_FROM_COLOR(color) ((uint)(color) & 0xFF)
#define COLOR(i) set.display.colors[i]

