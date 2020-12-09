#include "defines.h"
#include "VCP/SCPI/Utils/StringsSCPI.h"
#include <ctype.h>


static bool ChooseSymbols(pUCHAR *string);    // Возвращает false, если выбор невозможен - строка кончилась.
static bool ChooseSpaces(pUCHAR *string);     // Возвращает false, если выбор невозможен - строка кончилась.


int GetNumWordsInString(pUCHAR )
{
    /*
    ChooseSpaces(&string);

    int numWords = 0;
    
    while (ChooseSymbols(&string))
    {
        numWords++;
        ChooseSpaces(&string);
    }
    
    return numWords;
    */
    
    return 0;
}

bool GetWord(pUCHAR string, Word *word, const int numWord) //-V2506
{
    ChooseSpaces(&string);

    int currentWord = 0;

    while (true)
    {
        if (currentWord == numWord)
        {
            word->address = (uint8*)string;
            ChooseSymbols(&string);
            word->numSymbols = static_cast<int8>(string - word->address);
            
            uint8 *pointer = word->address;
            int numSymbols = word->numSymbols;
            for (int i = 0; i < numSymbols; i++)
            {
                *pointer = (uint8)toupper((int8)*pointer);
                pointer++;
            }
            return true;
        }
        if (ChooseSymbols(&string))
        {
            currentWord++;
        }
        else
        {
            return false;
        }
        ChooseSpaces(&string);
    }
}

bool WordEqualZeroString(Word *word, char* string) //-V2506
{
    char *ch = string;
    char *w = (char*)(word->address);

    while (*ch != 0)
    {
        if (*ch++ != *w++)
        {
            return false;
        }
    }

    return (ch - string) == word->numSymbols;
}

#define  SYMBOL(x) (*(*(x)))

bool ChooseSymbols(pUCHAR *string) //-V2506
{
    if (SYMBOL(string) == 0x0d && SYMBOL(string + 1) == 0x0a)
    {
        return false;
    }

    while (SYMBOL(string) != ' ' && SYMBOL(string) != 0x0d && SYMBOL(string + 1) != 0x0a)
    {
        (*string)++;
    }

    return true;
}

bool ChooseSpaces(pUCHAR *string) //-V2506
{
    if (SYMBOL(string) == 0x0d && SYMBOL(string + 1) == 0x0a)
    {
        return false;
    }

    while (SYMBOL(string) == ' ')
    {
        (*string)++;
    }

    return true;
}

#undef SYMBOL

bool EqualsStrings(pCHAR str1, pCHAR str2, int size) //-V2506
{
    for (int i = 0; i < size; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
}

bool EqualsZeroStrings(char *str1, char *str2) //-V2506
{
    while ((*str1) == (*str2))
    {
        if ((*str1) == '\0')
        {
            return true;
        }
        str1++;
        str2++;
    }
    return false;
}
