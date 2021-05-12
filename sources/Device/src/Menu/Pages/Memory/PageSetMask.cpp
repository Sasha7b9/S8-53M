// 2021/05/12 14:19:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Hardware/Sound_.h"
#include "common/Utils/Math_.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include <cstring>


static void PressSB_Exit()
{
    Display::RemoveAddDrawFunction();
}


DEF_SMALL_BUTTON(sbExit, PageMemory::PageSetMask::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, PressSB_Exit, DrawSB_Exit, nullptr
)


static void PressSB_Delete()
{
    set.memory.file_name_mask[0] = '\0';
}


static void DrawSB_Delete(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::DEL).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}


DEF_SMALL_BUTTON(sbDelete, PageMemory::PageSetMask::self,
    "Удалить", "Delete",
    "Удаляет все введённые символы",
    "Deletes all entered symbols",
    nullptr, PressSB_Delete, DrawSB_Delete, nullptr
)


static void PressSB_Backspace()
{
    int size = (int)(std::strlen(set.memory.file_name_mask));
    if (size > 0)
    {
        if (size > 1 && set.memory.file_name_mask[size - 2] == 0x07)
        {
            set.memory.file_name_mask[size - 2] = '\0';
        }
        else
        {
            set.memory.file_name_mask[size - 1] = '\0';
        }
    }
}


static void DrawSB_Backspace(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::BACKSPACE).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}


DEF_SMALL_BUTTON(sbBackspace, PageMemory::PageSetMask::self,
    "Backspace", "Backspace",
    "Удаляет последний введённый символ",
    "Deletes the last entered symbol",
    nullptr, PressSB_Backspace, DrawSB_Backspace, nullptr
)


static void PressSB_Insert()
{
    int index = set.memory.index_cur_symbol_name_mask;
    int size = (int)(std::strlen(set.memory.file_name_mask));

    if (size == MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        return;
    }

    if (index < 0x41)
    {
        set.memory.file_name_mask[size] = Tables::symbolsAlphaBet[index][0];
        set.memory.file_name_mask[size + 1] = '\0';
    }
    else
    {
        index -= 0x40;

        if (index == 0x07)  // Для %nN - отдельный случай
        {
            if (size < MAX_SYMBOLS_IN_FILE_NAME - 2 && size > 0)
            {
                if (set.memory.file_name_mask[size - 1] >= 0x30 &&
                    set.memory.file_name_mask[size - 1] <= 0x39) // Если ранее введено число
                {
                    set.memory.file_name_mask[size] = set.memory.file_name_mask[size - 1] - 0x30;
                    set.memory.file_name_mask[size - 1] = 0x07;
                    set.memory.file_name_mask[size + 1] = '\0';
                }
            }
        }
        else
        {
            set.memory.file_name_mask[size] = (int8)index;
            set.memory.file_name_mask[size + 1] = '\0';
        }
    }
}


static void DrawSB_Insert(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::INSERT).Draw4SymbolsInRect(x + 2, y + 2);
    Font::Set(TypeFont::S8);
}


DEF_SMALL_BUTTON(sbInsert, PageMemory::PageSetMask::self,
    "Вставить", "Insert",
    "Вставляет выбранный символ",
    "Inserts the chosen symbol",
    nullptr, PressSB_Insert, DrawSB_Insert, nullptr
)


static void DrawFileMask(int x, int y)
{
    char *ch = set.memory.file_name_mask;

    Color::FILL.SetAsCurrent();
    while (*ch != '\0')
    {
        if (*ch >= 32)
        {
            x = Char(*ch).Draw(x, y);
        }
        else
        {
            if (*ch == 0x07)
            {
                x = Char('%').Draw(x, y);
                x = Char((char)(0x30 | *(ch + 1))).Draw(x, y);
                x = Char('N').Draw(x, y);
                ch++;
            }
            else
            {
                x = Text(Tables::symbolsAlphaBet[*ch + 0x40]).Draw(x, y);
            }
        }
        ch++;
    }
    Region(5, 8).Fill(x, y, Color::FLASH_10);
}


// Эта функция рисует, когда выбран режим задания маски
static void DrawSetMask()
{
    int x0 = Grid::Left() + 40;
    int y0 = Grid::TOP + 20;
    int width = Grid::Width() - 80;
    int height = 160;

    Rectangle(width, height).Draw(x0, y0, Color::FILL);
    Region(width - 2, height - 2).Fill(x0 + 1, y0 + 1, Color::BACK);

    int index = 0;
    int position = 0;
    int deltaX = 10;
    int deltaY0 = 5;
    int deltaY = 12;

    // Рисуем большие буквы английского алфавита
    while (Tables::symbolsAlphaBet[index][0] != ' ')
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0);
        index++;
        position++;
    }

    // Теперь рисуем цифры и пробел
    position = 0;
    while (Tables::symbolsAlphaBet[index][0] != 'a')
    {
        Tables::DrawStr(index, x0 + deltaX + 50 + position * 7, y0 + deltaY0 + deltaY);
        index++;
        position++;
    }

    // Теперь рисуем малые буквы алфавита
    position = 0;
    while (Tables::symbolsAlphaBet[index][0] != '%')
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0 + deltaY * 2);
        index++;
        position++;
    }

    // Теперь рисуем спецсимволы
    position = 0;
    while ((uint)index < (sizeof(Tables::symbolsAlphaBet) / 4))
    {
        Tables::DrawStr(index, x0 + deltaX + 26 + position * 20, y0 + deltaY0 + deltaY * 3);
        index++;
        position++;
    }

    DrawFileMask(x0 + deltaX, y0 + 65);

    static const char *strings[] =
    {
        "%y - год, %m - месяц, %d - день",
        "%H - часы, %M - минуты, %S - секунды",
        "%nN - порядковый номер, где",
        "n - минимальное количество знаков для N"
    };

    deltaY--;
    Color::FILL.SetAsCurrent();
    for (uint i = 0; i < sizeof(strings) / 4; i++)
    {
        Text(strings[i]).Draw(x0 + deltaX, (int)(y0 + 100 + deltaY * i));
    }
}


static void OnPress()
{
    PageMemory::PageSetMask::self->OpenAndSetItCurrent();
    Display::SetAddDrawFunction(DrawSetMask);
}


static void OnRegSet(int angle)
{
    PageMemory::PageExternal::OnSetMaskNameRegSet(angle, sizeof(Tables::symbolsAlphaBet) / 4);
}


static bool IsActive()
{
    return set.memory.file_naming_mode.IsMask();
}


DEF_PAGE_6(pageSetMask, PageMemory::PageExternal::self, NamePage::SB_MemExtSetMask,
    "МАСКА", "MASK",
    "Режим ввода маски для автоматического именования файлов",
    "Input mode mask for automatic file naming",
    sbExit,
    sbDelete,
    Item::empty,
    Item::empty,
    sbBackspace,
    sbInsert,
    IsActive, OnPress, nullptr, OnRegSet
)

const Page *PageMemory::PageSetMask::self = &pageSetMask;
