// 2021/05/12 15:29:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Hardware/Sound_.h"
#include "common/Utils/Math_.h"
#include "Display/Warnings.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


DEF_SMALL_BUTTON(sbMath_Function_Exit, PageService::PageMath::PageFunction::self,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu",
    nullptr, nullptr, DrawSB_Exit, nullptr
)


static void OnPress_Math_Function_ModeDraw()
{
    if (set.math.fft.enabled)
    {
        Warnings::ShowWarningBad(Warning::ImpossibleEnableMathFunction);
    }
    else
    {
        Math::CircleIncrease<int8>((int8 *)&set.math.modeDraw, 0, 2);
    }
}


static void Draw_Math_Function_ModeDraw_Disable(int x, int y)
{
    Text(LANG_RU ? "���" : "Dis").Draw(x + 2 + (LANG_RU ? 2 : 0), y + 5);
}


static void Draw_Math_Function_ModeDraw_Separate(int x, int y)
{
    Rectangle(13, 9).Draw(x + 3, y + 5);
    HLine().Draw(y + 9, x + 3, x + 16);
    HLine().Draw(y + 10, x + 3, x + 16);
}


static void Draw_Math_Function_ModeDraw_Together(int x, int y)
{
    Rectangle(13, 9).Draw(x + 3, y + 5);
}


static void Draw_Math_Function_ModeDraw(int x, int y)
{
    static const pFuncVII funcs[3] =
    {
        Draw_Math_Function_ModeDraw_Disable,
        Draw_Math_Function_ModeDraw_Separate,
        Draw_Math_Function_ModeDraw_Together
    };
    funcs[ModeDrawMath::Get()](x, y);
}


static const arrayHints hintsMath_Function_ModeDraw =
{
    {Draw_Math_Function_ModeDraw_Disable,  "����� �������������� ������� ��������",
                                            "The conclusion of mathematical function is disconnected"},
    {Draw_Math_Function_ModeDraw_Separate, "������� � �������������� ������� ��������� � ������ �����",
                                            "Signals and mathematical function are removed in different windows"},
    {Draw_Math_Function_ModeDraw_Together, "������� � �������������� ������� ��������� � ����� ����",
                                            "Signals and mathematical function are removed in one window"}
};


DEF_SMALL_BUTTON(sbMath_Function_ModeDraw, PageService::PageMath::PageFunction::self,
    "�����", "Display",
    "�������� ����� ����������� ��������������� �������",
    "Chooses the mode of display of a mathematical signal",
    nullptr, OnPress_Math_Function_ModeDraw, Draw_Math_Function_ModeDraw, &hintsMath_Function_ModeDraw
)


static void OnPress_Math_Function_Type()
{
    Math::CircleIncrease<int8>((int8 *)&set.math.func, 0, 1);
}


static void Draw_Math_Function_Type_Sum(int x, int y)
{
    HLine().Draw(y + 9, x + 4, x + 14);
    HLine().Draw(x + 9, y + 4, y + 14);
}


static void Draw_Math_Function_Type_Mul(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::MATH_FUNC_MUL).Draw4SymbolsInRect(x + 4, y + 3);
    Font::Set(TypeFont::S8);
}


static void Draw_Math_Function_Type(int x, int y)
{
    const pFuncVII funcs[2] = { Draw_Math_Function_Type_Sum, Draw_Math_Function_Type_Mul };
    funcs[set.math.func](x, y);
}


static const arrayHints hintsMath_Function_Type =
{
    { Draw_Math_Function_Type_Sum,      "��������",     "Addition"       },
    { Draw_Math_Function_Type_Mul,      "���������",    "Multiplication" }
};


DEF_SMALL_BUTTON(sbMath_Function_Type, PageService::PageMath::PageFunction::self,
    "���", "Type",
    "����� �������������� �������",
    "Choice of mathematical function",
    nullptr, OnPress_Math_Function_Type, Draw_Math_Function_Type, &hintsMath_Function_Type
)


static void OnPress_Math_Function_ModeRegSet()
{
    Math::CircleIncrease<int8>((int8 *)&set.math.modeRegSet, 0, 1);
}


static void Draw_Math_Function_ModeRegSet_Range(int x, int y)
{
    Char(LANG_RU ? 'M' : 'S').Draw(x + 7, y + 5);
}


static void Draw_Math_Function_ModeRegSet_RShift(int x, int y)
{
    Text(LANG_RU ? "��" : "Shif").Draw(x + 5 - (LANG_EN ? 3 : 0), y + 5);
}


static void Draw_Math_Function_ModeRegSet(int x, int y)
{
    const pFuncVII funcs[2] = { Draw_Math_Function_ModeRegSet_Range, Draw_Math_Function_ModeRegSet_RShift };
    funcs[ModeRegSet::Get()](x, y);
}


static const arrayHints hintsMath_Function_ModeRegSet =
{
    {Draw_Math_Function_ModeRegSet_Range,  "���������� ���������", "Management of scale"},
    {Draw_Math_Function_ModeRegSet_RShift, "���������� ���������", "Management of shift"}
};


DEF_SMALL_BUTTON(sbMath_Function_ModeRegSet, PageService::PageMath::PageFunction::self,
    "����� ����� ���������", "Mode regulator SET",
    "����� ������ ����� ��������� - ���������� ��������� ��� ���������",
    "Choice mode regulcator ��������� - management of scale or shift",
    nullptr, OnPress_Math_Function_ModeRegSet, Draw_Math_Function_ModeRegSet, &hintsMath_Function_ModeRegSet
)


static void OnPress_Math_Function_RangeA()
{
    set.math.range = set.chan[ChA].range;
    set.math.multiplier = set.chan[ChA].divider;
}


static void Draw_Math_Function_RangeA(int x, int y)
{
    Char('1').Draw(x + 8, y + 5);
}


DEF_SMALL_BUTTON(sbMath_Function_RangeA, PageService::PageMath::PageFunction::self,
    "������� 1-�� ������", "Scale of the 1st channel",
    "���� ������� ��� ��������������� ������� �� ������� ������",
    "Takes scale for a mathematical signal from the first channel",
    nullptr, OnPress_Math_Function_RangeA, Draw_Math_Function_RangeA, nullptr
)


static void OnPress_Math_Function_RangeB()
{
    set.math.range = set.chan[ChB].range;
    set.math.multiplier = set.chan[ChB].divider;
}


static void Draw_Math_Function_RangeB(int x, int y)
{
    Char('2').Draw(x + 8, y + 5);
}


DEF_SMALL_BUTTON(sbMath_Function_RangeB, PageService::PageMath::PageFunction::self,
    "������� 2-�� ������", "Scale of the 2nd channel",
    "���� ������� ��� ��������������� ������� �� ������� ������",
    "Takes scale for a mathematical signal from the second channel",
    nullptr, OnPress_Math_Function_RangeB, Draw_Math_Function_RangeB, nullptr
)


static void OnPress_Math_Function()
{
    if (set.math.fft.enabled)
    {
        Warnings::ShowWarningBad(Warning::ImpossibleEnableMathFunction);
    }
}


static void OnRegSet_Math_Function(int delta)
{
    if (ModeDrawMath::IsDisabled())
    {
        return;
    }

    if (ModeRegSet::IsRShift())
    {
        int16 prevRShift = RShift::GetMath();
        int16 rShift = prevRShift;
        if (delta > 0)
        {
            if (rShift < RShift::MAX)
            {
                rShift += 4 * RShift::STEP();
                Math::LimitAbove(&rShift, RShift::MAX);
                if (prevRShift < RShift::ZERO && rShift > RShift::ZERO)
                {
                    rShift = RShift::ZERO;
                }
                Sound::RegulatorShiftRotate();
                set.math.rShift = rShift;
            }
        }
        else if (delta < 0)
        {
            if (rShift > RShift::MIN)
            {
                rShift -= 4 * RShift::STEP();
                Math::LimitBelow(&rShift, RShift::MIN);
                if (prevRShift > RShift::ZERO && rShift < RShift::ZERO)
                {
                    rShift = RShift::ZERO;
                }
                Sound::RegulatorShiftRotate();
                set.math.rShift = rShift;
            }
        }
    }
    if (ModeRegSet::IsRange())
    {
        static int sum = 0;
        sum -= delta;

        float rShiftAbs = RShift::ToAbs(RShift::GetMath(), set.math.range);

        if (sum > 2)
        {
            if (set.math.range < Range::Count - 1)
            {
                ++set.math.range.value;
                set.math.rShift = (int16)RShift::ToRel(rShiftAbs, set.math.range);
                Sound::RegulatorSwitchRotate();
            }
            sum = 0;
        }
        else if (sum < -2)
        {
            if (set.math.range > 0)
            {
                --set.math.range.value;
                set.math.rShift = (int16)RShift::ToRel(rShiftAbs, set.math.range);
                Sound::RegulatorSwitchRotate();
            }
            sum = 0;
        }
    }
}


static bool IsActive_Math_Function()
{
    return !set.math.fft.enabled;
}


DEF_PAGE_6(pageMathFunction, PageService::PageMath::self, NamePage::SB_MathFunction,
    "�������", "FUNCTION",
    "��������� � ����� �������������� ������� - �������� ��� ���������",
    "Installation and selection of mathematical functions - addition or multiplication",
    sbMath_Function_Exit,
    sbMath_Function_ModeDraw,
    sbMath_Function_Type,
    sbMath_Function_ModeRegSet,
    sbMath_Function_RangeA,
    sbMath_Function_RangeB,
    IsActive_Math_Function, OnPress_Math_Function, nullptr, OnRegSet_Math_Function
);


const Page *PageService::PageMath::PageFunction::self = &pageMathFunction;
