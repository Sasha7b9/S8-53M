#pragma once
#include "common/Settings/SettingsTypes_c.h"


// ��� �����
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
        Count,
        FLASH_10,
        FLASH_01,
        SET_INVERSE
    };

    static uint FromSettings(Color::E);
    static void ResetFlash();

    static void SetCurrent(Color::E color);
    static Color::E GetCurrent() { return current; };
    static Color::E Channel(Channel::E chan);
    static Color::E Cursors(Channel::E cnan);
    static Color::E MenuField();
    // ���� ����� ������� ����, ��� ���� ��������� ��������. ������������ ��� �������� ������� ������.
    static Color::E MenuTitleLessBright();
    static Color::E Trig();
    static Color::E MenuTitle(bool inShade);
    static Color::E MenuItem(bool inShade);
    static Color::E BorderMenu(bool inShade);       // ���� ��������� ����.
    static Color::E Contrast(Color::E color);       // ���������� ����, ����������� � color. ����� ���� ����� ��� ������.
    static Color::E LightShadingText();             // ������� ���� � ����.
    static Color::E FILL;
    static Color::E BACK;

private:
    static Color::E current;
};


// ���� ����� ������� ����, ��� ���� �������� ����. ������������ ��� �������� ������� ������.
#define ColorMenuItemLessBright()   Color::MENU_ITEM_DARK
// ���� ����� ������� ����, ��� ���� ��������� ��������. ������������ ��� �������� ������� ������.
#define ColorMenuTitleBrighter()    Color::MENU_TITLE_BRIGHT
// ���� ����� ������� ����, ��� ���� �������� ����. ������������ ��� �������� ������� ������.
#define ColorMenuItemBrighter()     Color::MENU_ITEM_BRIGHT
// Ҹ���� ���� � ����.
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

void Color_Log(Color::E color);           // ������� � ��� �������� �����.
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

