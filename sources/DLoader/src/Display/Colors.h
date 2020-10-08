#pragma once
#include "defines.h"
#include "DisplayTypes.h"


// ���������� ���� �������������.
Color::E ColorTrig(void);
// ���� ��������� ��������, inShade - �������� �� ��������.
Color::E ColorMenuTitle(bool inShade);
// ���� �������� ����.
Color::E ColorMenuItem(bool inShade);
// ���� ��������� ����.
Color::E ColorBorderMenu(bool inShade);
// ���������� ����, ����������� � color. ����� ���� ����� ��� ������.
Color::E ColorContrast(Color::E color);
// ������� ���� � ����.
Color::E LightShadingTextColor(void);


#define ColorMenuField()            COLOR_MENU_FIELD
// ���� ����� ������� ����, ��� ���� �������� ����. ������������ ��� �������� ������� ������.
#define ColorMenuItemLessBright()   Color::MENU_ITEM_DARK
// ���� ����� ������� ����, ��� ���� ��������� ��������. ������������ ��� �������� ������� ������.
#define ColorMenuTitleBrighter()    COLOR_MENU_TITLE_BRIGHT
// ���� ����� ������� ����, ��� ���� ��������� ��������. ������������ ��� �������� ������� ������.
#define ColorMenuTitleLessBright()  COLOR_MENU_TITLE_DARK
// ���� ����� ������� ����, ��� ���� �������� ����. ������������ ��� �������� ������� ������.
#define ColorMenuItemBrighter()     COLOR_MENU_ITEM_BRIGHT
// Ҹ���� ���� � ����.
#define DarkShadingTextColor()      COLOR_MENU_TITLE_DARK


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

// ������� � ��� �������� �����.
void Color_Log(Color::E color);

void Color_BrightnessChange(ColorType *colorType, int delta);

void Color_SetBrightness(ColorType *colorType, float brightness);
// ���� forced == false, �� ������������� ���������� ������ ��� ������ ������ �������
void Color_Init(ColorType *colorType, bool forced);

void Color_ComponentChange(ColorType *colorType, int delta);

const char* NameColorFromValue(uint16 colorValue);

const char* NameColor(Color::E color);

#define MAKE_COLOR(r, g, b) static_cast<uint16>(((b) & 0x1f) + (((g) & 0x3f) << 5) + (((r) & 0x1f) << 11))
#define R_FROM_COLOR(color) (((uint16)(color) >> 11) & (uint16)0x1f)
#define G_FROM_COLOR(color) (((uint16)(color) >> 5) & (uint16)0x3f)
#define B_FROM_COLOR(color) ((uint16)(color) & 0x1f)

extern Color::E gColorFill;
extern Color::E gColorBack;
extern Color::E gColorGrid;
extern Color::E gColorChan[4];
