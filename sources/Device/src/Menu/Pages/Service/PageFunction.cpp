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


DEF_SMALL_BUTTON(sbExit, PageService::PageMath::PageFunction::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, nullptr, DrawSB_Exit, nullptr
)


static void OnPress_ModeDraw()
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


static void Draw_ModeDraw_Disable(int x, int y)
{
    Text(LANG_RU ? "Вык" : "Dis").Draw(x + 2 + (LANG_RU ? 2 : 0), y + 5);
}


static void Draw_ModeDraw_Separate(int x, int y)
{
    Rectangle(13, 9).Draw(x + 3, y + 5);
    HLine().Draw(y + 9, x + 3, x + 16);
    HLine().Draw(y + 10, x + 3, x + 16);
}


static void Draw_ModeDraw_Together(int x, int y)
{
    Rectangle(13, 9).Draw(x + 3, y + 5);
}


static void Draw_ModeDraw(int x, int y)
{
    static const pFuncVII funcs[3] =
    {
        Draw_ModeDraw_Disable,
        Draw_ModeDraw_Separate,
        Draw_ModeDraw_Together
    };
    funcs[ModeDrawMath::Get()](x, y);
}


static const arrayHints hintsModeDraw =
{
    {Draw_ModeDraw_Disable,  "Вывод математической функции отключён",
                                            "The conclusion of mathematical function is disconnected"},
    {Draw_ModeDraw_Separate, "Сигналы и математическая функция выводятся в разных окнах",
                                            "Signals and mathematical function are removed in different windows"},
    {Draw_ModeDraw_Together, "Сигналы и математическая функция выводятся в одном окне",
                                            "Signals and mathematical function are removed in one window"}
};


DEF_SMALL_BUTTON(sbModeDraw, PageService::PageMath::PageFunction::self,
    "Экран", "Display",
    "Выбирает режим отображения математического сигнала",
    "Chooses the mode of display of a mathematical signal",
    nullptr, OnPress_ModeDraw, Draw_ModeDraw, &hintsModeDraw
)


static void OnPress_Type()
{
    Math::CircleIncrease<int8>((int8 *)&set.math.func, 0, 1);
}


static void Draw_Type_Sum(int x, int y)
{
    HLine().Draw(y + 9, x + 4, x + 14);
    HLine().Draw(x + 9, y + 4, y + 14);
}


static void Draw_Type_Mul(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::MATH_FUNC_MUL).Draw4SymbolsInRect(x + 4, y + 3);
    Font::Set(TypeFont::S8);
}


static void Draw_Type(int x, int y)
{
    const pFuncVII funcs[2] = { Draw_Type_Sum, Draw_Type_Mul };
    funcs[set.math.func](x, y);
}


static const arrayHints hintsType =
{
    { Draw_Type_Sum,      "Сложение",     "Addition"       },
    { Draw_Type_Mul,      "Умножение",    "Multiplication" }
};


DEF_SMALL_BUTTON(sbType, PageService::PageMath::PageFunction::self,
    "Вид", "Type",
    "Выбор математической функции",
    "Choice of mathematical function",
    nullptr, OnPress_Type, Draw_Type, &hintsType
)


static void OnPress_ModeRegSet()
{
    Math::CircleIncrease<int8>((int8 *)&set.math.modeRegSet, 0, 1);
}


static void Draw_ModeRegSet_Range(int x, int y)
{
    Char(LANG_RU ? 'M' : 'S').Draw(x + 7, y + 5);
}


static void Draw_ModeRegSet_RShift(int x, int y)
{
    Text(LANG_RU ? "См" : "Shif").Draw(x + 5 - (LANG_EN ? 3 : 0), y + 5);
}


static void Draw_Math_Function_ModeRegSet(int x, int y)
{
    const pFuncVII funcs[2] = { Draw_ModeRegSet_Range, Draw_ModeRegSet_RShift };
    funcs[ModeRegSet::Get()](x, y);
}


static const arrayHints hintsModeRegSet =
{
    {Draw_ModeRegSet_Range,  "Управление масштабом", "Management of scale"},
    {Draw_ModeRegSet_RShift, "Управление смещением", "Management of shift"}
};


DEF_SMALL_BUTTON(sbModeRegSet, PageService::PageMath::PageFunction::self,
    "Режим ручки УСТАНОВКА", "Mode regulator SET",
    "Выбор режима ручки УСТАНОВКА - управление масштабом или смещением",
    "Choice mode regulcator УСТАНОВКА - management of scale or shift",
    nullptr, OnPress_ModeRegSet, Draw_Math_Function_ModeRegSet, &hintsModeRegSet
)


static void OnPress_RangeA()
{
    set.math.range = set.chan[ChA].range;
    set.math.multiplier = set.chan[ChA].divider;
}


static void Draw_RangeA(int x, int y)
{
    Char('1').Draw(x + 8, y + 5);
}


DEF_SMALL_BUTTON(sbRangeA, PageService::PageMath::PageFunction::self,
    "Масштаб 1-го канала", "Scale of the 1st channel",
    "Берёт масштаб для математического сигнала из первого канала",
    "Takes scale for a mathematical signal from the first channel",
    nullptr, OnPress_RangeA, Draw_RangeA, nullptr
)


static void OnPress_RangeB()
{
    set.math.range = set.chan[ChB].range;
    set.math.multiplier = set.chan[ChB].divider;
}


static void Draw_RangeB(int x, int y)
{
    Char('2').Draw(x + 8, y + 5);
}


DEF_SMALL_BUTTON(sbRangeB, PageService::PageMath::PageFunction::self,
    "Масштаб 2-го канала", "Scale of the 2nd channel",
    "Берёт масштаб для математического сигнала из второго канала",
    "Takes scale for a mathematical signal from the second channel",
    nullptr, OnPress_RangeB, Draw_RangeB, nullptr
)


static void OnPress()
{
    if (set.math.fft.enabled)
    {
        Warnings::ShowWarningBad(Warning::ImpossibleEnableMathFunction);
    }
}


static void OnRegSet(int delta)
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


static bool IsActive()
{
    return !set.math.fft.enabled;
}


DEF_PAGE_6(pageMathFunction, PageService::PageMath::self, NamePage::SB_MathFunction,
    "ФУНКЦИЯ", "FUNCTION",
    "Установка и выбор математической функции - сложения или умножения",
    "Installation and selection of mathematical functions - addition or multiplication",
    sbExit,
    sbModeDraw,
    sbType,
    sbModeRegSet,
    sbRangeA,
    sbRangeB,
    IsActive, OnPress, nullptr, OnRegSet
);


const Page *PageService::PageMath::PageFunction::self = &pageMathFunction;
