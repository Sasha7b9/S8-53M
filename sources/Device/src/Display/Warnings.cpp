// 2021/03/30 13:42:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/Timer_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Display/Warnings.h"
#include "Display/Screen/Grid.h"
#include "Settings/SettingsMain.h"


#define NUM_WARNINGS            10
static const char *warnings[NUM_WARNINGS] = { 0 };      // Здесь предупреждающие сообщения.
static uint                     timeWarnings[NUM_WARNINGS] = { 0 };   // Здесь время, когда предупреждающее сообщение
                                                                    // поступило на экран.


void Warnings::Draw()
{
    int delta = 12;
    int y = Grid::BottomMessages();
    for (int i = 0; i < 10; i++)
    {
        if (warnings[i] != 0)
        {
            DrawStringInRectangle(Grid::Left(), y, warnings[i]);
            y -= delta;
        }
    }
}


void Warnings::ShowWarn(pchar message)
{
    if (warnings[0] == 0)
    {
        Timer::Enable(TypeTimer::ShowMessages, 100, OnTimerShowWarning);
    }
    bool alreadyStored = false;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (warnings[i] == 0 && !alreadyStored)
        {
            warnings[i] = message;
            timeWarnings[i] = TIME_MS;
            alreadyStored = true;
        }
        else if (warnings[i] == message)
        {
            timeWarnings[i] = TIME_MS;
            return;
        }
    }
}


void Warnings::OnTimerShowWarning()
{
    uint time = TIME_MS;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (time - timeWarnings[i] > (uint)(set.display.time_messages * 1000))
        {
            timeWarnings[i] = 0;
            warnings[i] = 0;
        }
    }

    int pointer = 0;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (warnings[i] != 0)
        {
            warnings[pointer] = warnings[i];
            timeWarnings[pointer] = timeWarnings[i];
            if (pointer != i)
            {
                timeWarnings[i] = 0;
                warnings[i] = 0;
            }
            pointer++;
        }
    }

    if (pointer == 0)
    {
        Timer::Disable(TypeTimer::ShowMessages);
    }
}


void Warnings::ClearFromWarnings()
{
    Timer::Disable(TypeTimer::ShowMessages);

    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        warnings[i] = 0;
        timeWarnings[i] = 0;
    }
}


void Warnings::ShowWarningBad(Warning::E warning)
{
    Color::ResetFlash();
    ShowWarn(Tables::GetWarning(warning));
    Sound::WarnBeepBad();
}


void Warnings::ShowWarningGood(Warning::E warning)
{
    Color::ResetFlash();
    ShowWarn(Tables::GetWarning(warning));
    Sound::WarnBeepGood();
}


void Warnings::DrawStringInRectangle(int, int y, char const *text)
{
    int width = Font::GetLengthText(text);
    int height = 8;
    Rectangle(width + 4, height + 4).Draw(Grid::Left(), y, Color::FILL);
    Rectangle(width + 2, height + 2).Draw(Grid::Left() + 1, y + 1, Color::BACK);
    Region(width, height).Fill(Grid::Left() + 2, y + 2, Color::FLASH_10);
    Text(text).Draw(Grid::Left() + 3, y + 2, Color::FLASH_01);
}
