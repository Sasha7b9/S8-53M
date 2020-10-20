#include "defines.h"
#include "common/Display/Primitives_c.h"
#include "Display/PainterText.h"
#include "DisplayTypes.h"
#include "common/Display/Font/Font_c.h"
#include "Painter.h"
#include <cstring>
#include <cstdarg>
#include <cstdio>


using namespace Primitives;


bool ByteFontNotEmpty(int eChar, int byte)
{
    static pUCHAR bytes = 0;
    static int prevChar = -1;
    if (eChar != prevChar)
    {
        prevChar = eChar;
        bytes = Font::font->symbol[prevChar].bytes;
    }
    return bytes[byte] != 0;
}


static bool BitInFontIsExist(int eChar, int numByte, int bit)
{
    static uint8 prevByte = 0;      // WARN здесь точно статики нужны?
    static int prevChar = -1;
    static int prevNumByte = -1;
    if (prevNumByte != numByte || prevChar != eChar)
    {
        prevByte = Font::font->symbol[eChar].bytes[numByte];
        prevChar = eChar;
        prevNumByte = numByte;
    }
    return prevByte & (1 << bit);
}


static void DrawCharInColorDisplay(int eX, int eY, uchar symbol)
{
    int8 width = static_cast<int8>(Font::font->symbol[symbol].width);
    int8 height = (int8)Font::font->height;

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    Painter::SetPoint(x, y);
                }
                x++;
            }
        }
    }
}


static int DrawBigChar(int eX, int eY, int size, char symbol)
{
    int8 width = static_cast<int8>(Font::font->symbol[symbol].width);
    int8 height = (int8)Font::font->height;

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b * size + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    for (int i = 0; i < size; i++)
                    {
                        for (int j = 0; j < size; j++)
                        {
                            Painter::SetPoint(x + i, y + j);
                        }
                    }
                }
                x += size;
            }
        }
    }

    return eX + width * size;
}


extern void CalculateCurrentColor(void);


int Painter::DrawChar(int x, int y, char symbol)
{
    CalculateCurrentColor();
    if (Font::GetSize() == 5)
    {
        DrawCharHardCol(x, y + 3, symbol);
    }
    else if (Font::GetSize() == 8)
    {
        DrawCharHardCol(x, y, symbol);
    }
    else
    {
        DrawCharInColorDisplay(x, y, static_cast<uchar>(symbol));
    }
    return x + Font::GetLengthSymbol(static_cast<uchar>(symbol));
}


int Painter::DrawCharC(int x, int y, char symbol, Color::E color)
{
    Painter::SetColor(color);
    return Painter::DrawChar(x, y, symbol);
}


static bool IsLetter(char symbol)
{
    static const bool isLetter[256] =
    {
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true
    };

    return isLetter[(uint8)symbol];
}


static char *GetWord(const char *firstSymbol, int *length, char buffer[20])
{
    int pointer = 0;
    *length = 0;

    while (IsLetter(*firstSymbol))
    {
        buffer[pointer] = *firstSymbol;
        pointer++;
        firstSymbol++;
        (*length)++;
    }
    buffer[pointer] = '\0';

    return buffer;
}


static bool IsConsonant(char symbol)
{
    static const bool isConsonat[256] =
    {
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, true, true, true, true, false, true, true, false, false, true, true, true, true, false, true,
        true, true, true, false, true, true, true, true, true, true, true, false, true, false, false, false,
        false, true, true, true, true, false, true, true, false, false, true, true, true, true, false, true,
        true, true, true, false, true, true, true, true, true, true, true, false, true, false, false, false
    };

    return isConsonat[(uint8)symbol];
}


static bool CompareArrays(const bool *array1, const bool *array2, int numElems)
{
    for (int i = 0; i < numElems; i++)
    {
        if (array1[i] != array2[i])
        {
            return false;
        }
    }
    return true;
}


// Находит следующий перенос. C letters начинается часть слово, где нужно найти перенос, в lettersInSyllable будет записано число букв в найденном 
// слоге. Если слово закончилось, функция возвращает false
static bool FindNextTransfer(const char *letters, int8 *lettersInSyllable)
{

#define VOWEL       0   // Гласная
#define CONSONANT   1   // Согласная

    * lettersInSyllable = (int8)std::strlen(letters);
    if (std::strlen(letters) <= 3) //-V1051
    {
        return false;
    }

    static const bool template1[3] = {false, true, true};               //     011     2   // После второго символа перенос
    static const bool template2[4] = {true, false, true, false};        //     1010    2
    static const bool template3[4] = {false, true, false, true};        //     0101    3
    static const bool template4[4] = {true, false, true, true};         //     1011    3
    static const bool template5[4] = {false, true, false, false};       //     0100    3
    static const bool template6[4] = {true, false, true, true};         //     1011    3
    static const bool template7[5] = {true, true, false, true, false};  //     11010   3
    static const bool template8[6] = {true, true, false, true, true};   //     11011   4

    bool consonant[20];

    int size = static_cast<int>(std::strlen(letters));
    for (int i = 0; i < size; i++)
    {
        consonant[i] = IsConsonant(letters[i]);
    }

    if (CompareArrays(template1, consonant, 3))
    {
        *lettersInSyllable = 2;
        return true;
    }
    if (CompareArrays(template2, consonant, 4))
    {
        *lettersInSyllable = 2;
        return true;
    }
    if (std::strlen(letters) < 5)
    {
        return false;
    }
    if (CompareArrays(template3, consonant, 4) || CompareArrays(template4, consonant, 4) || CompareArrays(template5, consonant, 4) || CompareArrays(template6, consonant, 4))
    {
        *lettersInSyllable = 3;
        return true;
    }
    if (std::strlen(letters) < 6)
    {
        return false;
    }
    if (CompareArrays(template7, consonant, 5))
    {
        *lettersInSyllable = 3;
        return true;
    }
    if (CompareArrays(template8, consonant, 5))
    {
        *lettersInSyllable = 4;
        return true;
    }
    return false;
}


static int8* BreakWord(char *word)
{
    int num = 0;
    static int8 lengthSyllables[10];
    char *position = word;
    while (FindNextTransfer(position, &(lengthSyllables[num])))
    {
        position += lengthSyllables[num];
        num++;
    }
    lengthSyllables[num + 1] = 0;
    if (std::strcmp(word, "структуру") == 0)
    {
        int8 lengths[] = {5, 2, 2, 0};
        std::memcpy(lengthSyllables, lengths, 4);
    }
    else if (std::strcmp(word, "соответствующей") == 0)
    {
        int8 lenghts[] = {4, 3, 4, 5, 3, 0};
        std::memcpy(lengthSyllables, lenghts, 6);
    }
    return lengthSyllables;
}


int Painter::DrawStringInCenterRect(int eX, int , int width, int , const char *text)
{
    int lenght = Font::GetLengthText(text);
    int x = eX + (width - lenght) / 2;
    return x;
}


int Painter::DrawStringInCenterRectC(int x, int y, int width, int height, const char *text, Color::E color)
{
    Painter::SetColor(color);
    return Painter::DrawStringInCenterRect(x, y, width, height, text);
}


void Painter::DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color::E colorText, int widthBorder, Color::E colorBackground)
{
    int lenght = Font::GetLengthText(text);
    int eX = Painter::DrawStringInCenterRectC(x, y, width, height, text, colorBackground);
    int w = lenght + widthBorder * 2 - 2;
    int h = 7 + widthBorder * 2 - 1;
    Painter::FillRegion(eX - lenght - widthBorder, y - widthBorder + 1, w, h);
    Painter::DrawStringInCenterRectC(x, y, width, height, text, colorText);
}


int Painter::DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color::E colorBackground, Color::E colorFill)
{
    Rectangle(width, height).Draw(x, y, colorFill);
    Region(width - 2, height - 2).Fill(x + 1, y + 1, colorBackground);
    Painter::SetColor(colorFill);
    return Painter::DrawStringInCenterRect(x, y, width, height, text);
}


static int GetLenghtSubString(char *text)
{
    int retValue = 0;
    while (((*text) != ' ') && ((*text) != '\0'))
    {
        retValue += Font::GetLengthSymbol(static_cast<uchar>(*text));
        text++;
    }
    return retValue;
}


static int DrawSubString(int x, int y, char *text)
{
    int numSymbols = 0;
    while (((*text) != ' ') && ((*text) != '\0'))
    {
        x = Painter::DrawChar(x, y, *text);
        numSymbols++;
        text++;
    }
    return numSymbols;
}


static int DrawSpaces(int x, int y, char *text, int *numSymbols)
{
    *numSymbols = 0;
    while (*text == ' ')
    {
        x = Painter::DrawChar(x, y, *text);
        text++;
        (*numSymbols)++;
    }
    return x;
}


void Painter::DrawTextInRect(int x, int y, int width, char *text)
{
    int xStart = x;
    int xEnd = xStart + width;

    while (*text != 0)
    {
        int length = GetLenghtSubString(text);
        if (length + x > xEnd)
        {
            x = xStart;
            y += Font::GetHeightSymbol();
        }
        int numSymbols = 0;
        numSymbols = DrawSubString(x, y, text);
        text += numSymbols;
        x += length;
        x = DrawSpaces(x, y, text, &numSymbols);
        text += numSymbols;
    }
}


void Painter::DrawTextRelativelyRight(int , int , const char *)
{
}


void Painter::Draw2SymbolsC(int x, int y, char symbol1, char symbol2, Color::E color1, Color::E color2)
{
    Painter::DrawCharC(x, y, symbol1, color1);
    Painter::DrawCharC(x, y, symbol2, color2);
}


void Painter::Draw4SymbolsInRect(int x, int y, char eChar)
{
    for (int i = 0; i < 2; i++)
    {
        Painter::DrawChar(x + 8 * i, y, (char)(eChar + i));
        Painter::DrawChar(x + 8 * i, y + 8, (char)(eChar + i + 16));
    }
}


void Painter::Draw10SymbolsInRect(int x, int y, char eChar)
{
    for (int i = 0; i < 5; i++)
    {
        Painter::DrawChar(x + 8 * i, y, (char)(eChar + i));
        Painter::DrawChar(x + 8 * i, y + 8, (char)(eChar + i + 16));
    }
}


void Painter::DrawBigText(int eX, int eY, int size, const char *text)
{
    int numSymbols = static_cast<int>(std::strlen(text));

    int x = eX;

    for (int i = 0; i < numSymbols; i++)
    {
        x = DrawBigChar(x, eY, size, text[i]);
        x += size;
    }
}


// Возвращает часть слова до слога numSyllable(включительн) вместе со знаком переноса
static char* PartWordForTransfer(char *word, const int8* lengthSyllables, int numSyllable, char buffer[30])
{
    int length = 0;
    for (int i = 0; i <= numSyllable; i++)
    {
        length += lengthSyllables[i];
    }
    std::memcpy((void*)buffer, (void*)word, static_cast<uint>(length));
    buffer[length] = '-';
    buffer[length + 1] = '\0';
    return buffer;
}


// Если draw == false, то рисовать символ не надо, фунция используется только для вычислений
static int DrawPartWord(char *word, int x, int , int xRight, bool draw)
{
    int8 *lengthSyllables = BreakWord(word);
    int numSyllabels = 0;
    char buffer[30];
    for (int i = 0; i < 10; i++)
    {
        if (lengthSyllables[i] == 0)
        {
            numSyllabels = i;
            break;
        }
    }

    for (int i = numSyllabels - 2; i >= 0; i--)
    {
        char *subString = PartWordForTransfer(word, lengthSyllables, i, buffer);
        int length = Font::GetLengthText(subString);
        if (xRight - x > length - 5)
        {
            if (draw)
            {
            }
            return static_cast<int>(std::strlen(subString) - 1);
        }
    }

    return 0;
}


int Painter::DrawTextInRectWithTransfers(int eX, int eY, int eWidth, int eHeight, const char *text)
{
    int top = eY;
    int left = eX;
    int right = eX + eWidth;
    int bottom = eY + eHeight;

    char buffer[20];
    int numSymbols = static_cast<int>(std::strlen(text));

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (y < bottom && curSymbol < numSymbols)
    {
        while (x < right - 1 && curSymbol < numSymbols)
        {
            int length = 0;
            char *word = GetWord(text + curSymbol, &length, buffer);

            if (length <= 1)                            // Нет буквенных символов или один, т.е. слово не найдено
            {
                char symbol = text[curSymbol++];
                if (symbol == '\n')
                {
                    x = right;
                    continue;
                }
                if (symbol == ' ' && x == left)
                {
                    continue;
                }
                x = Painter::DrawChar(x, y, symbol);
            }
            else                                            // А здесь найдено по крайней мере два буквенных символа, т.е. найдено слово
            {
                int lengthString = Font::GetLengthText(word);
                if (x + lengthString > right + 5)
                {
                    int nums = DrawPartWord(word, x, y, right, true);
                    x = right;
                    curSymbol += nums;
                    continue;
                }
                else
                {
                    curSymbol += length;
                }
            }
        }
        x = left;
        y += 9;
    }

    return y;
}


static int DrawBigCharInBuffer(int eX, int eY, int size, char symbol, uint8 buffer[320][240])
{
    int8 width = static_cast<int8>(Font::font->symbol[symbol].width);
    int8 height = (int8)Font::font->height;

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b * size + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    for (int i = 0; i < size; i++)
                    {
                        for (int j = 0; j < size; j++)
                        {
                            int fullX = x + i;
                            int fullY = y + j;

                            if (fullX >= 0 && fullX < 320 && fullY >= 0 && fullY < 240)
                            {
                                buffer[fullX][fullY] = 1;
                            }
                        }
                    }
                }
                x += size;
            }
        }
    }

    return eX + width * size;
}


void Painter::DrawBigTextInBuffer(int eX, int eY, int size, const char* text, uint8 buffer[320][240])
{
    for (int x = 0; x < 320; x++)
    {
        for (int y = 0; y < 240; y++)
        {
            buffer[x][y] = 0;
        }
    }

    int numSymbols = static_cast<int>(std::strlen(text));

    int x = eX;

    for (int i = 0; i < numSymbols; i++)
    {
        x = DrawBigCharInBuffer(x, eY, size, text[i], buffer);
        x += size;
    }
}
