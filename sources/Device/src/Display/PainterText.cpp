#include "defines.h"
#include "common/Log.h"
#include "Display/Painter.h"
#include "common/Display/Primitives.h"
#include "common/Display/Text.h"
#include "common/Display/Font/Font.h"
#include "Hardware/Timer.h"
#include "Utils/Math.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>


using namespace Primitives;


bool ByteFontNotEmpty(int eChar, int byte)
{
    static pUCHAR bytes = 0;
    static int prevChar = -1;
    if (eChar != prevChar)
    {
        prevChar = eChar;
        bytes = font->symbol[prevChar].bytes;
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
        prevByte = font->symbol[eChar].bytes[numByte];
        prevChar = eChar;
        prevNumByte = numByte;
    }
    return prevByte & (1 << bit);
}


int Painter::DrawTextOnBackground(int x, int y, const char *text, Color::E colorBackground)
{
    int width = Font::GetLengthText(text);
    int height = Font::GetSize();

    Color::E colorText = Color::GetCurrent();
    Region(width, height).Fill(x - 1, y, colorBackground);
    Color::SetCurrent(colorText);

    return Text(text).Draw(x, y);
}


int Painter::DrawCharWithLimitation(int eX, int eY, uchar symbol, int limitX, int limitY, int limitWidth, int limitHeight)
{
    int8 width = static_cast<int8>(font->symbol[symbol].width);
    int8 height = static_cast<int8>(font->height);

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
                    if ((x >= limitX) && (x <= (limitX + limitWidth)) && (y >= limitY) && (y <= limitY + limitHeight))
                    {
                        Point().Draw(x, y);
                    }
                }
                x++;
            }
        }
    }

    return eX + width + 1;
}


int Painter::DrawTextWithLimitationC(int x, int y, const char* text, Color::E color, int limitX, int limitY, int limitWidth, int limitHeight)
{
    Color::SetCurrent(color);
    int retValue = x;
    while (*text)
    {
        x = DrawCharWithLimitation(x, y, static_cast<uint8>(*text), limitX, limitY, limitWidth, limitHeight);
        retValue += Font::GetLengthSymbol(static_cast<uint8>(*text));
        text++;
    }
    return retValue + 1;
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

    return isLetter[static_cast<uint8>(symbol)];
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

    return isConsonat[static_cast<uint8>(symbol)];
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

    *lettersInSyllable = static_cast<int8>(std::strlen(letters)); //-V1029
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

    uint size = std::strlen(letters);
    for (uint i = 0; i < size; i++)
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


// Возвращает часть слова до слога numSyllable(включительн) вместе со знаком переноса
static char* PartWordForTransfer(char *word, const int8* lengthSyllables, int, int numSyllable, char buffer[30])
{
    int length = 0;
    for (int i = 0; i <= numSyllable; i++)
    {
        length += lengthSyllables[i];
    }
    std::memcpy(static_cast<void*>(buffer), static_cast<void*>(word), static_cast<uint>(length));
    buffer[length] = '-';
    buffer[length + 1] = '\0';
    return buffer;
}


// Если draw == false, то рисовать символ не надо, фунция используется только для вычислений
int Painter::DrawPartWord(char *word, int x, int y, int xRight, bool draw)
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
        char *subString = PartWordForTransfer(word, lengthSyllables, numSyllabels, i, buffer);
        int length = Font::GetLengthText(subString);
        if (xRight - x > length - 5)
        {
            if (draw)
            {
                Text(subString).Draw(x, y);
            }
            return static_cast<int>(std::strlen(subString)) - 1;
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
    int numSymb = static_cast<int>(std::strlen(text));

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (y < bottom && curSymbol < numSymb)
    {
        while (x < right - 1 && curSymbol < numSymb)
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
                x = Char(symbol).Draw(x, y);
            }
            else                                            // А здесь найдено по крайней мере два буквенных символа, т.е. найдено слово
            {
                int lengthString = Font::GetLengthText(word);
                if (x + lengthString > right + 5)
                {
                    int numSymbols = DrawPartWord(word, x, y, right, true);
                    x = right;
                    curSymbol += numSymbols;
                    continue;
                }
                else
                {
                    curSymbol += length;
                    x = Text(word).Draw(x, y);
                }
            }
        }
        x = left;
        y += 9;
    }

    return y;
}


// Возвращает высоту экрана, которую займёт текст text, при выводе от left до right в переменной height. Если bool == false, то текст не влезет на экран 
bool Painter::GetHeightTextWithTransfers(int left, int top, int right, const char *text, int *height)
{
    char buffer[20];
    int numSymb = static_cast<int>(std::strlen(text));

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (y < 231 && curSymbol < numSymb)
    {
        while (x < right - 1 && curSymbol < numSymb)
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
                x += Font::GetLengthSymbol(static_cast<uint8>(symbol));
            }
            else                                            // А здесь найдено по крайней мере два буквенных символа, т.е. найдено слово
            {
                int lengthString = Font::GetLengthText(word);
                if (x + lengthString > right + 5)
                {
                    int numSymbols = DrawPartWord(word, x, y, right, false);
                    x = right;
                    curSymbol += numSymbols;
                    continue;
                }
                else
                {
                    curSymbol += length;
                    x += Font::GetLengthText(word);
                }
            }
        }
        x = left;
        y += 9;
    }

    LIMITATION(*height, y - top + 4, 0, 239);

    return curSymbol == numSymb;
}


int Painter::DrawTextInBoundedRectWithTransfers(int x, int y, int width, const char *text, Color::E colorBackground, Color::E colorFill)
{
    int height = 0;
    GetHeightTextWithTransfers(x + 3, y + 3, x + width - 8, text, &height);

    Rectangle(width, height).Draw(x, y, colorFill);
    Region(width - 2, height - 2).Fill(x + 1, y + 1, colorBackground);
    DrawTextInRectWithTransfersC(x + 3, y + 3, width - 8, height, text, colorFill);
    return y + height;
}


int Painter::DrawTextInRectWithTransfersC(int x, int y, int width, int height, const char *text, Color::E color)
{
    Color::SetCurrent(color);
    return DrawTextInRectWithTransfers(x, y, width, height, text);
}


int Painter::DrawFormatText(int x, int y, Color::E color, char *text, ...)
{
#undef SIZE_BUFFER
#define SIZE_BUFFER 200
    char buffer[SIZE_BUFFER];
    std::va_list args;
    va_start(args, text);
    std::vsprintf(buffer, text, args);
    va_end(args);
    return Text(buffer).Draw(x, y, color);
#undef SIZE_BUFFER
}


int Painter::DrawStringInCenterRect(int eX, int eY, int width, int eHeight, const char *text)
{
    int lenght = Font::GetLengthText(text);
    int height = Font::GetHeightSymbol();
    int x = eX + (width - lenght) / 2;
    int y = eY + (eHeight - height) / 2;
    return Text(text).Draw(x, y);
}


int Painter::DrawStringInCenterRectC(int x, int y, int width, int height, const char *text, Color::E color)
{
    Color::SetCurrent(color);
    return DrawStringInCenterRect(x, y, width, height, text);
}


void Painter::DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color::E colorText, int widthBorder, 
                                                 Color::E colorBackground)
{
    int lenght = Font::GetLengthText(text);
    int eX = DrawStringInCenterRectC(x, y, width, height, text, colorBackground);
    int w = lenght + widthBorder * 2 - 2;
    int h = 7 + widthBorder * 2 - 1;
    Region(w, h).Fill(eX - lenght - widthBorder, y - widthBorder);
    DrawStringInCenterRectC(x, y, width, height, text, colorText);
}


int Painter::DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color::E colorBackground, Color::E colorFill)
{
    Rectangle(width, height).Draw(x, y, colorFill);
    Region(width - 2, height - 2).Fill(x + 1, y + 1, colorBackground);
    Color::SetCurrent(colorFill);
    return DrawStringInCenterRect(x, y, width, height, text);
}


void Painter::DrawHintsForSmallButton(int x, int y, int width, void *smallButton)
{
    SmallButton *sb = static_cast<SmallButton*>(smallButton);
    Region(width, 239 - y).Fill(x, y, COLOR_BACK);
    Rectangle(width, 239 - y).Draw(x, y, COLOR_FILL);
    const StructHelpSmallButton *structHelp = &(*sb->hintUGO)[0];
    x += 3;
    y += 3;
    while (structHelp->funcDrawUGO)
    {
        Rectangle(WIDTH_SB, WIDTH_SB).Draw(x, y);
        structHelp->funcDrawUGO(x, y);
        int yNew = DrawTextInRectWithTransfers(x + 23, y + 1, width - 30, 20, structHelp->helpUGO[set.common.lang]);
        y = ((yNew - y) < 22) ? (y + 22) : yNew;
        structHelp++;
    }
}


void Painter::DrawTextRelativelyRight(int xRight, int y, const char *text)
{
    int lenght = Font::GetLengthText(text);
    Text(text).Draw(xRight - lenght, y);
}


void Painter::DrawTextRelativelyRightC(int xRight, int y, const char *text, Color::E color)
{
    Color::SetCurrent(color);
    DrawTextRelativelyRight(xRight, y, text);
}


void Painter::Draw2SymbolsC(int x, int y, char symbol1, char symbol2, Color::E color1, Color::E color2)
{
    Char(symbol1).Draw(x, y, color1);
    Char(symbol2).Draw(x, y, color2);
}


void Painter::Draw4SymbolsInRect(int x, int y, char eChar)
{
    for (int i = 0; i < 2; i++)
    {
        Char(static_cast<char>(eChar + i)).Draw(x + 8 * i, y);
        Char(static_cast<char>(eChar + i + 16)).Draw(x + 8 * i, y + 8);
    }
}


void Painter::Draw4SymbolsInRectC(int x, int y, char eChar, Color::E color)
{
    Color::SetCurrent(color);
    Draw4SymbolsInRect(x, y, eChar);
}


void Painter::Draw10SymbolsInRect(int x, int y, char eChar)
{
    for (int i = 0; i < 5; i++)
    {
        Char(static_cast<char>(eChar + i)).Draw(x + 8 * i, y);
        Char(static_cast<char>(eChar + i + 16)).Draw(x + 8 * i, y + 8);
    }
}
