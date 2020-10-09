#pragma once
#include "common/Settings/cSettingsTypes.h"


// Тип цвета
struct Color {
    enum E
    {
        BLACK,
        WHITE,
        GRID,
        DATA_A,
        DATA_B,
        MENU_FIELD,
        MENU_TITLE,
        MENU_TITLE_DARK,
        MENU_TITLE_BRIGHT,
        MENU_ITEM,
        MENU_ITEM_DARK,
        MENU_ITEM_BRIGHT,
        MENU_SHADOW,
        EMPTY,
        EMPTY_A,
        EMPTY_B,
        NUM,
        FLASH_10,
        FLASH_01,
        SET_INVERSE
    };

    static uint FromSettings(Color::E);

    static void SetCurrent(Color::E color);
    static Color::E GetCurrent() { return current; };

private:
    static Color::E current;
};


Color::E ColorChannel(Channel::E chan);
Color::E ColorCursors(Channel::E cnan);
Color::E ColorTrig();                  // Возвращает цвет синхронизации.
Color::E ColorMenuTitle                    // Цвет заголовка страницы.
                    (bool inShade       // Затенена ли страница.
                    );
Color::E ColorMenuItem(bool inShade);      // Цвет элемента меню.
Color::E ColorBorderMenu(bool inShade);    // Цвет окантовки меню.
Color::E ColorContrast(Color::E color);       // Возвращает цвет, контрастный к color. Может быть белым или чёрным.
Color::E LightShadingTextColor();      // Светлый цвет в тени.

#define COLOR_BACK Color::BLACK
#define COLOR_FILL Color::WHITE
#define ColorGrid() Color::GRID


#define ColorMenuField() Color::MENU_FIELD
// Чуть менее светлый цвет, чем цвет элемента меню. Используется для создания эффекта объёма.
#define ColorMenuItemLessBright()   Color::MENU_ITEM_DARK
// Чуть более светлый цвет, чем цвет заголовка страницы. Используется для создания эффекта объёма.
#define ColorMenuTitleBrighter()    Color::MENU_TITLE_BRIGHT
// Чуть менее светлый цвет, чем цвет заголовка страницы. Используется для создания эффекта объёма.
#define ColorMenuTitleLessBright()  Color::MENU_TITLE_DARK
// Чуть более светлый цвет, чем цвет элемента меню. Используется для создания эффекта объёма.
#define ColorMenuItemBrighter()     Color::MENU_ITEM_BRIGHT
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
    Color::E color;
    int8    currentField;
    bool    alreadyUsed;
};

void Color_Log(Color::E color);           // Вывести в лог значение цвета.
void Color_BrightnessChange(ColorType *colorType, int delta);
void Color_SetBrightness(ColorType *colorType, float brightness);
void Color_Init(ColorType *colorType);
void Color_ComponentChange(ColorType *colorType, int delta);


const char* NameColorFromValue(uint16 colorValue);
const char* NameColor(Color::E color);
#define MAKE_COLOR(r, g, b) ((uint16)(((b) & 0x1f) + (((g) & 0x3f) << 5) + (((r) & 0x1f) << 11)))
#define R_FROM_COLOR(color) (((uint16)(color) >> 11) & (uint16)0x1f)
#define G_FROM_COLOR(color) (((uint16)(color) >> 5) & (uint16)0x3f)
#define B_FROM_COLOR(color) ((uint16)(color) & 0x1f)
#define COLOR(i) set.display.colors[i];

