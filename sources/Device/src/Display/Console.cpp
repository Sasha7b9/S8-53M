// 2021/03/30 14:02:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "Display/Console.h"
#include "Display/Screen/Grid.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"
#include <cstdio>
#include <cstring>


#define MAX_NUM_STRINGS         35
#define SIZE_BUFFER_FOR_STRINGS 2000
static char *strings[MAX_NUM_STRINGS] = { 0 };
static char                     bufferForStrings[SIZE_BUFFER_FOR_STRINGS] = { 0 };
static int                      lastStringForPause = -1;


void Console::Draw()
{
    int count = 0;
    Font::Set(SettingsNRST::GetSizeFontForConsole() == 5 ? TypeFont::S5 : TypeFont::S8);
    int height = Font::GetSize();

    int last_string = FirstEmptyString() - 1;
    int numStr = setNRST.console.num_srings;

    if (height == 8 && numStr > 22)
    {
        numStr = 22;
    }

    int delta = 0;

    if (set.display.show_string_navigation)
    {
        numStr -= ((height == 8) ? 1 : 2);
        delta = 10;
    }

    int first_string = last_string - numStr + 1;

    if (first_string < 0)
    {
        first_string = 0;
    }

    for (int num_string = first_string; num_string <= last_string; num_string++)
    {
        int width = Font::GetLengthText(strings[num_string]);
        Region(width, height + 1).Fill(Grid::Left() + 1, Grid::TOP + 1 + count * (height + 1) + delta, Color::BACK);
        int y = Grid::TOP + 5 + count * (height + 1) - 4;

        if (Font::GetSize() == 5)
        {
            y -= 3;
        }

        Text(strings[num_string]).Draw(Grid::Left() + 2, y + delta, Color::FILL);

        count++;
    }

    Font::Set(TypeFont::S8);
}


int Console::FirstEmptyString()
{
    for (int i = 0; i < MAX_NUM_STRINGS; i++)
    {
        if (strings[i] == 0)
        {
            return i;
        }
    }
    return MAX_NUM_STRINGS - 1;
}


int Console::CalculateFreeSize()
{
    int firstEmptyString = FirstEmptyString();

    if (firstEmptyString == 0)
    {
        return SIZE_BUFFER_FOR_STRINGS;
    }

    return (int)(SIZE_BUFFER_FOR_STRINGS - (strings[firstEmptyString - 1] - bufferForStrings) -
        std::strlen(strings[firstEmptyString - 1]) - 1);
}


void Console::DeleteFirstString()
{
    if (FirstEmptyString() < 2)
    {
        return;
    }

    int delta = (int)(std::strlen(strings[0])) + 1;
    int numStrings = FirstEmptyString();

    for (int i = 1; i < numStrings; i++)
    {
        strings[i - 1] = strings[i] - delta;
    }

    for (int i = numStrings - 1; i < MAX_NUM_STRINGS; i++)
    {
        strings[i] = 0;
    }

    for (int i = 0; i < SIZE_BUFFER_FOR_STRINGS - delta; i++)
    {
        bufferForStrings[i] = bufferForStrings[i + delta];
    }
}


void Console::AddString(pchar string)
{
    if (setNRST.console_in_pause)
    {
        return;
    }

    static int num = 0;
    char buffer[100];
    std::sprintf(buffer, "%d\x11", num++);
    std::strcat(buffer, string);
    int size = (int)(std::strlen(buffer)) + 1;

    while (CalculateFreeSize() < size)
    {
        DeleteFirstString();
    }

    if (!strings[0])
    {
        strings[0] = bufferForStrings;
        std::strcpy(strings[0], buffer);
    }
    else
    {
        char *addressLastString = strings[FirstEmptyString() - 1];
        char *address = addressLastString + std::strlen(addressLastString) + 1;
        strings[FirstEmptyString()] = address;
        std::strcpy(address, buffer);
    }
}


void Console::AddStringToIndicating(pchar string)
{
    if (FirstEmptyString() == (MAX_NUM_STRINGS - 1))
    {
        DeleteFirstString();
    }

    AddString(string);
}


void Console::SetPauseForConsole(bool pause)
{
    if (pause)
    {
        lastStringForPause = FirstEmptyString() - 1;
    }
    else
    {
        lastStringForPause = -1;
    }
}


void Console::OneStringUp()
{
    if (!setNRST.console_in_pause)
    {
    }
    else if (lastStringForPause > setNRST.console.num_srings - 1)
    {
        lastStringForPause--;
    }
}


void Console::OneStringDown()
{
    if (!setNRST.console_in_pause)
    {
    }
    else if (lastStringForPause < FirstEmptyString() - 1)
    {
        lastStringForPause++;
    }
}
