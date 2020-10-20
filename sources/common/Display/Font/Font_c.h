#pragma once


struct Symbol
{
    uchar width;
    uchar bytes[8];
};


struct TypeFont { enum E
{
	_5,
	_8,
	_UGO,
	_UGO2,
	_Count,
	_None
};};

struct Font
{
    int height;
    Symbol symbol[256];

    static int GetSize();
    static int GetLengthText(const char *text);
    static int GetHeightSymbol();
    static int GetLengthSymbol(uchar symbol);
    static void Set(TypeFont::E typeFont);

    static const Font *font;
    static const Font *fonts[TypeFont::_Count];

    static const uchar font5display[3080];
    static const uchar font8display[3080];
    static const uchar fontUGOdisplay[3080];
    static const uchar fontUGO2display[3080];
};
