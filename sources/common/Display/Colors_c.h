#pragma once
#include "common/Settings/SettingsTypes_c.h"


// ��� �����
struct Color {
    static Color BLACK;
    static Color WHITE;
    static Color GRID;
    static Color DATA_A;
    static Color DATA_B;
    static Color MENU_FIELD;
    static Color MENU_TITLE;
    static Color MENU_TITLE_DARK;
    static Color MENU_TITLE_BRIGHT;
    static Color MENU_ITEM;
    static Color MENU_ITEM_DARK;
    static Color MENU_ITEM_BRIGHT;
    static Color MENU_SHADOW;
    static Color EMPTY;
    static Color EMPTY_A;
    static Color EMPTY_B;

    static Color Count;

    static Color FLASH_10;
    static Color FLASH_01;
    static Color SET_INVERSE;

    uint8 value;

    Color(uint8 v) : value(v) {}

    static void ResetFlash();

    void SetAsCurrent();
    static Color GetCurrent() { return current; };
    static Color Channel(Channel::E chan);
    static Color Cursors(Channel::E cnan);
    static Color MenuField();
    // ���� ����� ������� ����, ��� ���� ��������� ��������. ������������ ��� �������� ������� ������.
    static Color MenuTitleLessBright();
    static Color Trig();
    static Color MenuTitle();
    static Color MenuItem();
    static Color BorderMenu();                   // ���� ��������� ����.
    static Color Contrast(const Color &color);       // ���������� ����, ����������� � color. ����� ���� ����� ��� ������.
    static Color LightShadingText();             // ������� ���� � ����.
    static Color FILL;
    static Color BACK;

    bool operator==(const Color &rhs) { return (value == rhs.value); }
    bool operator!=(const Color &rhs) { return (value != rhs.value); }

private:

    static Color current;
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
    uint8   color;
    int8    currentField;
    bool    alreadyUsed;
};

void Color_Log(const Color &color);           // ������� � ��� �������� �����.
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

