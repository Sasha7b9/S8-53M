#pragma once
#include "common/Settings/SettingsTypes_.h"


// ��� �����
struct Color
{
    static const Color BLACK;
    static Color WHITE;
    static Color GRID;
    static Color DATA_A;
    static Color DATA_B;
    static Color MENU_FIELD;
    static Color MENU_TITLE;
    static Color MENU_TITLE_DARK;       // ���� ����� ������� ����, ��� ���� ��������� ��������. ������������ ��� �������� ������� ������.
    static Color MENU_TITLE_BRIGHT;     // ���� ����� ������� ����, ��� ���� ��������� ��������. ������������ ��� �������� ������� ������.
    static Color MENU_ITEM;
    static Color MENU_ITEM_DARK;        // ���� ����� ������� ����, ��� ���� �������� ����. ������������ ��� �������� ������� ������.
    static Color MENU_ITEM_BRIGHT;      // ���� ����� ������� ����, ��� ���� �������� ����. ������������ ��� �������� ������� ������.
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

    uint8 index;

    Color(uint8 i) : index(i) {}

    Color(const Color &col) : index(col.index) {}

    static void ResetFlash();

    void SetAsCurrent() const;

    static Color GetCurrent() { return Color(current); };
    static Color Channel(Channel::E chan);
    static Color Cursors(Channel::E cnan);

    static Color Trig();

    // ���� ��������� ����.
    static Color BorderMenu()          { return Color::MENU_TITLE; }

    // ���������� ����, ����������� � color. ����� ���� ����� ��� ������.
    static Color Contrast(const Color &color);

    // ������� ���� � ����.
    static Color LightShadingText()   { return Color::MENU_TITLE; } //-V524

    bool operator==(const Color &rhs) { return (index == rhs.index); }
    bool operator!=(const Color &rhs) { return (index != rhs.index); }

    static uint Make(uint8 r, uint8 g, uint8 b)  { return (static_cast<uint>(((b)) + (((g)) << 8) + (((r)) << 16))); }

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