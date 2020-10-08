#pragma once
#include "defines.h"


struct Symbol
{
    uchar width;
    uchar bytes[8];
};


struct Font
{
	int height;
    Symbol symbol[256];

	static int GetSize();
	static int GetLengthText(pString text);
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


extern const Font *font;
extern const Font *fonts[TypeFont::_Count];

extern const uchar font5display[3080];
extern const uchar font8display[3080];
extern const uchar fontUGOdisplay[3080];
extern const uchar fontUGO2display[3080];



int Font_GetHeightSymbol(char symbol);
int Font_GetLengthSymbol(uchar symbol);
