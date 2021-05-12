// 2021/05/12 11:01:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"


static void OnPress_Exit()
{
    Display::RemoveAddDrawFunction();
    //    FREE_EXTRAMEM();
}

DEF_SMALL_BUTTON(bExit, PageDebug::PageSerialNumber::self,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu",
    nullptr, OnPress_Exit, DrawSB_Exit, nullptr
)


static void OnPress_Change()
{
    //    ACCESS_EXTRAMEM(StructForSN, s);
    //    ++s->curDigt;
    //    s->curDigt %= 2;
    //    Color::ResetFlash();
}

static void Draw_Change(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::TAB).Draw4SymbolsInRect(x + 2, y + 2);
    Font::Set(TypeFont::S8);
}

DEF_SMALL_BUTTON(bChange, PageDebug::PageSerialNumber::self,
    "��������", "Insert",
    "��������� �������� ������",
    "Inserts the chosen symbol",
    nullptr, OnPress_Change, Draw_Change, nullptr
)


static void OnPress_Save()
{
    //    ACCESS_EXTRAMEM(StructForSN, s);
    //
    //    char stringSN[20];
    //
    //    std::snprintf(stringSN, 19, "%02d %04d", s->number, s->year);
    //
    //    if (!HAL_OTP::SaveSerialNumber(stringSN))
    //    {
    //        Warnings::ShowWarningBad(Warning::FullyCompletedOTP);
    //    }
}


static void Draw_Save(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::SAVE_TO_MEM).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}


DEF_SMALL_BUTTON(bSave, PageDebug::PageSerialNumber::self,
    "���������", "Save",
    "���������� �������� ����� � OTP",
    "Records the serial number in OTP",
    nullptr, OnPress_Save, Draw_Save, nullptr
)


static void Draw_Enter()
{
    //    int x0 = Grid::Left() + 40;
    //    int y0 = Grid::TOP + 20;
    //    int width = Grid::Width() - 80;
    //    int height = 160;
    //
    //    Primitives::Rectangle(width, height).Draw(x0, y0, Color::FILL);
    //    Region(width - 2, height - 2).Fill(x0 + 1, y0 + 1, Color::BACK);
    //
    //    int deltaX = 10;
    //
    //    ACCESS_EXTRAMEM(StructForSN, s);
    //
    //    bool selNumber = s->curDigt == 0;
    //
    //    char buffer[20];
    //    std::snprintf(buffer, 19, "%02d", s->number);
    //
    //    Color colorText = Color::FILL;
    //    Color colorBackground = Color::BACK;
    //
    //    if (selNumber)
    //    {
    //        colorText = Color::FLASH_01;
    //        colorBackground = Color::FLASH_10;
    //    }
    //
    //    int y = y0 + 50;
    //
    //    colorText.SetAsCurrent();
    //    int x = Text(buffer).DrawOnBackground(x0 + deltaX, y, colorBackground);
    //
    //    colorText = Color::FLASH_01;
    //    colorBackground = Color::FLASH_10;
    //
    //    if (selNumber)
    //    {
    //        colorText = Color::FILL;
    //        colorBackground = Color::BACK;
    //    }
    //
    //    std::snprintf(buffer, 19, "%04d", s->year);
    //
    //    colorText.SetAsCurrent();
    //    Text(buffer).DrawOnBackground(x + 5, y, colorBackground);
    //
    //    // ������ ������� ���������� �� ���������� ����� � OTP-������ ��� ������
    //
    //    int allShots = HAL_OTP::GetSerialNumber(buffer);
    //
    //    Text("������� ���������� ����� %s", buffer[0] == 0 ? "-- ----" : buffer).Draw(x0 + deltaX, y0 + 130, Color::FILL);
    //
    //    Text("�������� ����� ��� %d �������", allShots).Draw(x0 + deltaX, y0 + 100, Color::FILL);
}


static void OnPress()
{
    PageDebug::PageSerialNumber::self->OpenAndSetItCurrent();
    Display::SetAddDrawFunction(Draw_Enter);
    //    MALLOC_EXTRAMEM(StructForSN, s);
    //    s->number = 1; 522
    //    s->year = 2017;
    //    s->curDigt = 0;
}


static void OnRegSet(int /*angle*/)
{
    //    typedef int(*pFunc)(int *, int, int);
    //
    //    pFunc p = angle > 0 ? Math::CircleIncrease<int> : Math::CircleDecrease<int>;
    //
    //    ACCESS_EXTRAMEM(StructForSN, s);
    //
    //    if (s->curDigt == 0)
    //    {
    //        p(&s->number, 1, 99);
    //    }
    //    else
    //    {
    //        p(&s->year, 2014, 2050);
    //    }
    //    Sound::GovernorChangedValue();
}


DEF_PAGE_6(pageSerialNumber, PageDebug::self, NamePage::SB_SerialNumber,
    "�/�", "S/N",
    "������ ��������� ������ � OTP-������. ��������!!! ��P-������ - ������ � ����������� �������.",
    "Serial number recording in OTP-memory. ATTENTION!!! OTP memory is a one-time programming memory.",
    bExit,
    bChange,
    Item::empty,
    Item::empty,
    Item::empty,
    bSave,
    nullptr, OnPress, nullptr, OnRegSet
)


const Page *PageDebug::PageSerialNumber::self = &pageSerialNumber;
