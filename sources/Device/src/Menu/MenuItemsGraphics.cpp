#include "defines.h"
#include "common/Display/Primitives_c.h"
#include "common/Display/Text_c.h"
#include "common/Utils/Math_c.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include <cstdio>
#include <cstring>


using namespace Primitives;


Key::E GetFuncButtonFromY(int _y)
{
    int y = Grid::TOP + Grid::Height() / 12;
    int step = Grid::Height() / 6;
    Key::E button = Key::None;
    for (int i = 0; i < 6; i++)
    {
        if (_y < y)
        {
            return button;
        }
        ++button;
        y += step;
    }
    return  Key::F5;
}

 
void DrawGovernorChoiceColorFormulaHiPart(const Item *item, int x, int y, bool pressed, bool shade, bool opened)
{

    int delta = pressed && !shade ? 1 : 0;
    int width = MI_WIDTH_VALUE;

    if (item->Type() == TypeItem::IP && opened && ((IPaddress*)item)->OwnData()->port != 0)
    {
        width += MOI_WIDTH_D_IP;
    }

    Color::E color = shade ? Color::MenuTitleLessBright() : (IS_COLOR_SCHEME_WHITE_LETTERS ? Color::WHITE : Color::BLACK);
    HLine().Draw(y + 1, x, x + width + 3, ColorBorderMenu(false));

    if (shade)
    {
        Region(width + 2, MI_HEIGHT_VALUE + 3).Fill(x + 1, y + 2, ColorMenuItem(false));
    }
    else
    {
        Painter::DrawVolumeButton(x + 1, y + 2, width + 2, MI_HEIGHT_VALUE + 3, 2, ColorMenuItem(false), ColorMenuItemBrighter(), ColorMenuItemLessBright(), pressed, shade);
    }

    Text(item->Title()).Draw(x + 6 + delta, y + 6 + delta, color);
    
    TypeItem::E type = item->Type();

    if(Menu::CurrentItem() == item)
    {
        char symbol = 0;

        if (type == TypeItem::Governor)
        {
            symbol = GetSymbolForGovernor(*((Governor*)item)->OwnData()->cell);
            ADDRESS_GOVERNOR = (uint)item;
        }
        else if (type == TypeItem::ChoiceReg || (item->IsOpened() && type == TypeItem::Choice))
        {
            symbol = GetSymbolForGovernor(*((Choice*)item)->OwnData()->cell);
        }
        else if (type == TypeItem::Time)
        {
            TimeItem *time = (TimeItem *)item;
            const DataTime *own = time->OwnData();
            if ((Menu::OpenedItem() == item) && (*own->curField != iEXIT) && (*own->curField != iSET))
            {
                int8 values[7] =
                {
                    0,
                    *own->day,
                    *own->month,
                    *own->year,
                    *own->hours,
                    *own->minutes,
                    *own->seconds
                };
                symbol = GetSymbolForGovernor(values[*own->curField]);
            }
        }

        Painter::Draw4SymbolsInRectC(x + MI_WIDTH - 13, y + 5 + (item->IsOpened() ? 0 : 15), symbol, IS_COLOR_SCHEME_WHITE_LETTERS ? COLOR_BACK : COLOR_FILL);
    }
}

void Governor::DrawLowPart(int x, int y, bool, bool shade) const
{
    char buffer[20];

    const DataGovernor *own = OwnData();
    
    Color::E colorTextDown = COLOR_BACK;

    Painter::DrawVolumeButton(x + 1, y + 17, MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3, 2, Color::MenuField(), 
        ColorMenuItemBrighter(), ColorMenuItemLessBright(), true, shade);
    if(shade)
    {
        colorTextDown = ColorMenuItem(false);
    }

    x = Text("\x80").Draw(x + 4, y + 21, colorTextDown);
    if(Menu::OpenedItem() != this)
    {
        float delta = Step();
        if(delta == 0.0F) //-V2550 //-V550
        {
            x = Text(Int2String(*own->cell, false, 1, buffer)).Draw(x + 1, y + 21);
        }
        else
        {
            int drawX = x + 1;
            int limX = x + 1;
            int limY = y + 19;
            int limWidth = MI_WIDTH_VALUE;
            int limHeight = MI_HEIGHT_VALUE - 1;
            if(delta > 0.0F)
            {
                x = Painter::DrawTextWithLimitationC(drawX, static_cast<int>(y + 21 - delta), Int2String(*own->cell, false, 1, buffer), COLOR_BACK, limX, limY, limWidth, limHeight);
                Painter::DrawTextWithLimitationC(drawX, static_cast<int>(y + 21 + 10 - delta), Int2String(NextValue(), false, 1, buffer), COLOR_BACK, limX, limY, limWidth, limHeight);
            }
            if(delta < 0.0F)
            {
                x = Painter::DrawTextWithLimitationC(drawX, static_cast<int>(y + 21 - delta), Int2String(*own->cell, false, 1, buffer), COLOR_BACK, limX, limY, limWidth, limHeight);
                Painter::DrawTextWithLimitationC(drawX, static_cast<int>(y + 21 - 10 - delta), Int2String(PrevValue(), false, 1, buffer), COLOR_BACK, limX, limY, limWidth, limHeight);
            }
        }
    }
    else
    {
        x = Text(Int2String(*own->cell, false, 1, buffer)).Draw(x + 1, y + 21, COLOR_FILL);
    }
    Text("\x81").Draw(x + 1, y + 21, colorTextDown);
}

void IPaddress::DrawLowPart(int x, int y, bool, bool shade) const
{
    char buffer[20];

    Color::E colorTextDown = COLOR_BACK;

    Painter::DrawVolumeButton(x + 1, y + 17, MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3, 2, Color::MenuField(), ColorMenuItemBrighter(), ColorMenuItemLessBright(), true, shade);
    if (shade)
    {
        colorTextDown = ColorMenuItem(false);
    }

    const DataIPaddress *own = OwnData();

    std::sprintf(buffer, "%03d.%03d.%03d.%03d", *own->ip0, *own->ip1, *own->ip2, *own->ip3);

    if (Menu::OpenedItem() != this)
    {
        Text(buffer).Draw(x + 4, y + 21, colorTextDown);
    }
    else
    {
        Text(buffer).Draw(x + 4, y + 21, COLOR_FILL);
    }
}

void MACaddress::DrawLowPart(int x, int y, bool, bool shade) const
{
    char buffer[20];

    Color::E colorTextDown = COLOR_BACK;

    Painter::DrawVolumeButton(x + 1, y + 17, MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3, 2, Color::MenuField(), ColorMenuItemBrighter(), ColorMenuItemLessBright(), true, shade);
    if (shade)
    {
        colorTextDown = ColorMenuItem(false);
    }

    const DataMACaddress *own = OwnData();

    std::sprintf(buffer, "%02X.%02X.%02X.%02X.%02X.%02X", *own->mac0, *own->mac1, *own->mac2, *own->mac3, *own->mac4, *own->mac5);

    if (Menu::OpenedItem() != this)
    {

        Text(buffer).Draw(x + 4, y + 21, colorTextDown);
    }
    else
    {
        Text(buffer).Draw(x + 4, y + 21, COLOR_FILL);
    }
}

void Formula::WriteText(int x, int y, bool) const
{
    const DataFormula *own = OwnData();

    if (*own->function != Function::Mul && *own->function != Function::Sum)
    {
        return;
    }

    bool funcIsMul = (*own->function == Function::Mul);
    int8 koeff1 = funcIsMul ? *own->koeff1mul : *own->koeff1add;
    int8 koeff2 = funcIsMul ? *own->koeff2mul : *own->koeff2add;
    if (koeff1 != 0)
    {
       Char(koeff1 < 0 ? '-' : '+').Draw(x, y);
    }
    Char((char)(koeff1 + 0x30)).Draw(x + 5, y);
    Char('*').Draw(x + 10, y);
    Text("K1").Draw(x + 14, y);
    Char(funcIsMul ? '*' : '+').Draw(x + 27, y);
    if (koeff2 != 0)
    {
       Char(koeff2 < 0 ? '-' : '+').Draw(x + 30, y);
    }
    Char((char)(koeff2 + 0x30)).Draw(x + 39, y);
    Char('*').Draw(x + 44, y);
    Text("K2").Draw(x + 48, y);
}

void Formula::DrawLowPart(int x, int y, bool, bool shade) const
{
    Color::E colorTextDown = COLOR_BACK;

    Painter::DrawVolumeButton(x + 1, y + 17, MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3, 2, Color::MenuField(), ColorMenuItemBrighter(), ColorMenuItemLessBright(), true, shade);
    if (shade)
    {
        colorTextDown = ColorMenuItem(false);
    }
    Color::SetCurrent(colorTextDown);
    WriteText(x + 6, y + 21, false);
}

void Governor::DrawClosed(int x, int y) const
{
    bool pressed = IsPressed();
    bool shade = IsShade() || !IsActive();
    DrawLowPart(x, y, pressed, shade);
    DrawGovernorChoiceColorFormulaHiPart(this, x, y, pressed, shade, false);
}

void IPaddress::DrawClosed(int x, int y) const
{
    bool pressed = IsPressed();
    bool shade = IsShade() || !IsActive();
    DrawLowPart(x, y, pressed, shade);
    DrawGovernorChoiceColorFormulaHiPart(this, x, y, pressed, shade, false);
}

void MACaddress::DrawClosed(int x, int y) const
{
    bool pressed = IsPressed();
    bool shade = IsShade() || !IsActive();
    DrawLowPart(x, y, pressed, shade);
    DrawGovernorChoiceColorFormulaHiPart(this, x, y, pressed, shade, false);
}

void Formula::DrawClosed(int x, int y) const
{
    bool pressed = IsPressed();
    bool shade = IsShade() || !IsActive();
    DrawLowPart(x, y, pressed, shade);
    DrawGovernorChoiceColorFormulaHiPart(this, x, y, pressed, shade, false);
}

static void DrawValueWithSelectedPosition(int x, int y, int value, int numDigits, int selPos, bool hLine, bool fillNull) // Если selPos == -1, подсвечивать не нужно
{
    int firstValue = value;
    int height = hLine ? 9 : 8;
    for (int i = 0; i < numDigits; i++)
    {
        int rest = value % 10;
        value /= 10;
        if (selPos == i)
        {
            Region(5, height).Fill(x - 1, y, COLOR_FILL);
        }
        if (!(rest == 0 && value == 0) || (firstValue == 0 && i == 0))
        {
            Char(static_cast<char>(rest + 48)).Draw(x, y, selPos == i ? COLOR_BACK : COLOR_FILL);
        }
        else if (fillNull)
        {
            Char('0').Draw(x, y, selPos == i ? COLOR_BACK : COLOR_FILL);
        }
        if (hLine)
        {
            Painter::DrawLineC(x, y + 9, x + 3, y + 9, COLOR_FILL);
        }
        x -= 6;
    }
}

void Governor::DrawValue(int x, int y) const
{
    const DataGovernor *own = OwnData();

    char buffer[20];

    int startX = x + 40;
    int16 value = *own->cell;
    int signGovernor = *own->cell < 0 ? -1 : 1;
    if(signGovernor == -1)
    {
        value = -value;
    }
    Font::Set(TypeFont::_5);
    bool sign = own->minValue < 0;
    Text(Int2String(own->maxValue, sign, 1, buffer)).Draw(x + 55, y - 5, COLOR_FILL);
    Text(Int2String(own->minValue, sign, 1, buffer)).Draw(x + 55, y + 2);
    Font::Set(TypeFont::_8);

    DrawValueWithSelectedPosition(startX, y, value, NumDigits(), gCurDigit, true, true);

    if(sign)
    {
       Char(signGovernor < 0 ? '\x9b' : '\x9a').Draw(startX - 1, y);
    }
}

void IPaddress::DrawValue(int x, int y)
{
    const DataIPaddress *own = OwnData();

    if (gCurDigit > (own->port == 0 ? 11 : 16))
    {
        gCurDigit = 0;
    }

    uint8 *bytes = own->ip0;

    x += 15;

    y += 1;

    int numIP = 0;
    int selPos = 0;

    IPaddress::GetNumPosIPvalue(&numIP, &selPos);

    for (int i = 0; i < 4; i++)
    {
        DrawValueWithSelectedPosition(x, y, bytes[i], 3, numIP == i ? selPos : -1, false, true);
        if (i != 3)
        {
            Char('.').Draw(x + 5, y, COLOR_FILL);
        }
        x += 19;
    }

    if (own->port != 0)
    {
        Char(':').Draw(x - 13, y, COLOR_FILL);
        DrawValueWithSelectedPosition(x + 14, y, *own->port, 5, numIP == 4 ? selPos : -1, false, true);
    }
}

void MACaddress::DrawValue(int x, int y)
{
    if (gCurDigit > 5)
    {
        gCurDigit = 0;
    }
    uint8 *bytes = OwnData()->mac0;
    x += MOI_WIDTH - 14;
    y++;
    for (int num = 5; num >= 0; num--)
    {
        int value = (int)(*(bytes + num));
        if (gCurDigit == num)
        {
            Region(10, 8).Fill(x - 1, y, COLOR_FILL);
        }
        char buffer[20];
        std::sprintf(buffer, "%02X", value);
        Text(buffer).Draw(x, y, gCurDigit == num ? COLOR_BACK : COLOR_FILL);
        x -= 12;
    }
}

void Governor::Draw(int x, int y, bool opened)
{
    if (OwnData()->funcBeforeDraw)
    {
        OwnData()->funcBeforeDraw();
    }
    if(opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

void IPaddress::Draw(int x, int y, bool opened)
{
    if (opened)
    {
        DrawOpened(x - (OwnData()->port == 0 ? 0 : MOI_WIDTH_D_IP), y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

void MACaddress::Draw(int x, int y, bool opened)
{
    if (opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

void Formula::Draw(int x, int y, bool opened)
{
    if (opened)
    {
        
    }
    else
    {
        DrawClosed(x, y);
    }
}

void GovernorColor::DrawValue(int x, int y, int delta)
{
    const DataGovernorColor *own = OwnData();

    char buffer[20];
    
    ColorType *ct = own->colorType;
    int8 field = ct->currentField;
    char *texts[4] = {"Яр", "Сн", "Зл", "Кр"};
    uint16 color = set.display.colors[ct->color];
    int red = R_FROM_COLOR(color);
    int green = G_FROM_COLOR(color);
    int blue = B_FROM_COLOR(color);
    if(!ct->alreadyUsed)
    {
        Color_Init(ct);
    }

    int vals[4] = { static_cast<int>(ct->brightness * 100), blue, green, red };

    Region(MI_WIDTH + delta - 2, MI_HEIGHT / 2 - 3).Fill(x, y, Color::BLACK);
    x += 92;
    
    for(int i = 0; i < 4; i++)
    {
        Color::E colorBack = (field == i) ? Color::WHITE : Color::BLACK;
        Color::E colorDraw = (field == i) ? Color::BLACK : Color::WHITE;
        Region(29, 10).Fill(x - 1, y + 1, colorBack);
        Text(texts[i]).Draw(x, y + 2, colorDraw);
        Text(Int2String(vals[i], false, 1, buffer)).Draw(x + 14, y + 2);
        x -= 30;
    }
    
}

void GovernorColor::DrawOpened(int x, int y)
{
    static const int delta = 43;
    x -= delta;
    Color_Init(OwnData()->colorType);
    Rectangle(MI_WIDTH + delta + 2, MI_HEIGHT + 2).Draw(x - 1, y - 1, Color::BLACK);
    Rectangle(MI_WIDTH + delta, MI_HEIGHT).Draw(x, y, Color::MenuTitle(false));
    Painter::DrawVolumeButton(x + 1, y + 1, MI_WIDTH_VALUE + 2 + delta, MI_HEIGHT_VALUE + 3, 2, ColorMenuItem(false), 
        ColorMenuItemBrighter(), ColorMenuItemLessBright(), IsPressed(), IsShade());
    HLine().Draw(y + MI_HEIGHT / 2 + 2, x, x + MI_WIDTH + delta, Color::MenuTitle(false));
    Painter::DrawStringInCenterRectC(x + (IsPressed() ? 2 : 1), y + (IsPressed() ? 2 : 1), MI_WIDTH + delta, MI_HEIGHT / 2 + 2, Title(), Color::WHITE);
    DrawValue(x + 1, y + 19, delta);
}

void GovernorColor::DrawClosed(int x, int y)
{
    Color_Init(OwnData()->colorType);
    DrawGovernorChoiceColorFormulaHiPart(this, x, y, IsPressed(), IsShade() || !IsActive(), true);
    Region(MI_WIDTH_VALUE, MI_HEIGHT_VALUE - 1).Fill(x + 2, y + 20, OwnData()->colorType->color);
}

void GovernorColor::Draw(int x, int y, bool opened)
{
    if(opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

void Choice::DrawOpened(int x, int y) const
{
    int height = HeightOpened();

    Rectangle(MP_TITLE_WIDTH + 2, height + 3).Draw(x - 1, y - 1, COLOR_BACK);
    
    DrawGovernorChoiceColorFormulaHiPart(this, x - 1, y - 1, IsPressed(), false, true);
    Rectangle(MP_TITLE_WIDTH + 1, height + 1).Draw(x - 1, y, Color::MenuTitle(false));
 
    HLine().Draw(y + MOI_HEIGHT_TITLE - 1, x, x + MOI_WIDTH);
    Painter::DrawVolumeButton(x, y + MOI_HEIGHT_TITLE, MOI_WIDTH - 1, height - MOI_HEIGHT_TITLE, 1, Color::BLACK, ColorMenuTitleBrighter(), Color::MenuTitleLessBright(), false, false);
    int index = *((int8*)OwnData()->cell);
    for(int i = 0; i < NumSubItems(); i++)
    {
        int yItem = y + MOI_HEIGHT_TITLE + i * MOSI_HEIGHT + 1;
        bool pressed = i == index;
        if(pressed)
        {
            Painter::DrawVolumeButton(x + 1, yItem, MOI_WIDTH - 2 , MOSI_HEIGHT - 2, 2, Color::MenuField(), ColorMenuTitleBrighter(), Color::MenuTitleLessBright(), pressed, false);
        }
        Text(NameSubItem(i)).Draw(x + 4, yItem + 2, pressed ? Color::BLACK : Color::MenuField());
    }
}

void TimeItem::DrawOpened(int x, int y) const
{
    char buffer[20];
    
    int width = MI_WIDTH_VALUE + 3;
    int height = 61;
    Rectangle(width + 2, height + 3).Draw(x - 1, y - 1, COLOR_BACK);
    DrawGovernorChoiceColorFormulaHiPart(this, x - 1, y - 1, IsPressed(), false, true);

    Rectangle(width + 1, height + 1).Draw(x - 1, y, Color::MenuTitle(false));

    HLine().Draw(y + MOI_HEIGHT_TITLE - 1, x, x + MOI_WIDTH);
    Painter::DrawVolumeButton(x, y + MOI_HEIGHT_TITLE, MOI_WIDTH - 1, height - MOI_HEIGHT_TITLE, 1, Color::BLACK, ColorMenuTitleBrighter(), Color::MenuTitleLessBright(), false, false);

    int y0 = 21;
    int y1 = 31;
    int y2 = 41;
    int y3 = 51;

    int x0 = 41;
    int dX = 13;
    int wS = 10;

    struct StructPaint
    {
        int x;
        int y;
        int width;
    } strPaint[8] =
    {
        {3,             y3, 60},    // Не сохранять
        {x0,            y0, wS},    // день
        {x0 + dX,       y0, wS},    // месяц
        {x0 + 2 * dX,   y0, wS},    // год
        {x0,            y1, wS},    // часы
        {x0 + dX,       y1, wS},    // мин
        {x0 + 2 * dX,   y1, wS},    // сек
        {3,             y2, 46}     // Сохранить
    };

    const DataTime *own = OwnData();

    char strI[8][20];
    std::strcpy(strI[iEXIT], "Не сохранять");
    std::strcpy(strI[iDAY], Int2String(*own->day, false, 2, buffer));
    std::strcpy(strI[iMONTH], Int2String(*own->month, false, 2, buffer));
    std::strcpy(strI[iYEAR], Int2String(*own->year, false, 2, buffer));
    std::strcpy(strI[iHOURS], Int2String(*own->hours, false, 2, buffer));
    std::strcpy(strI[iMIN], Int2String(*own->minutes, false, 2, buffer));
    std::strcpy(strI[iSEC], Int2String(*own->seconds, false, 2, buffer));
    std::strcpy(strI[iSET], "Сохранить");

    Text("д м г - ").Draw(x + 3, y + y0, COLOR_FILL);
    Text("ч м с - ").Draw(x + 3, y + y1);

    for (int i = 0; i < 8; i++)
    {
        if (*own->curField == i)
        {
            Region(strPaint[i].width, 8).Fill(x + strPaint[i].x - 1, y + strPaint[i].y, Color::FLASH_10);
        }
        Text(strI[i]).Draw(x + strPaint[i].x, y + strPaint[i].y, *own->curField == i ? Color::FLASH_01 : COLOR_FILL);
    }
}

static void GovernorIpCommon_DrawOpened(const Item *item, int x, int y, int dWidth)
{
    int height = 34;
    Rectangle(MP_TITLE_WIDTH + 2 + dWidth, height + 3).Draw(x - 1, y - 1, COLOR_BACK);
    Rectangle(MP_TITLE_WIDTH + 1 + dWidth, height + 1).Draw(x - 1, y, Color::MenuTitle(false));
    HLine().Draw(y + MOI_HEIGHT_TITLE - 1, x, x + MOI_WIDTH + dWidth);
    DrawGovernorChoiceColorFormulaHiPart(item, x - 1, y - 1, item->IsPressed(), false, true);
    Painter::DrawVolumeButton(x, y + MOI_HEIGHT_TITLE, MOI_WIDTH - 1 + dWidth, height - MOI_HEIGHT_TITLE, 1, Color::BLACK, ColorMenuTitleBrighter(), Color::MenuTitleLessBright(), false, false);
}

void Governor::DrawOpened(int x, int y) const
{
    GovernorIpCommon_DrawOpened(this, x, y, 0);
    DrawValue(x, y + 22);
}

void IPaddress::DrawOpened(int x, int y)
{
    GovernorIpCommon_DrawOpened(this, x, y, (OwnData()->port == 0) ? 0 : MOI_WIDTH_D_IP);
    DrawValue(x, y + 22);
}

void MACaddress::DrawOpened(int x, int y)
{
    GovernorIpCommon_DrawOpened(this, x, y, 0);
    DrawValue(x, y + 22);
}

void Choice::DrawClosed(int x, int y) const
{
    bool pressed = IsPressed();
    bool shade = IsShade() || !IsActive();
        
    if (shade)
    {
        Region(MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3).Fill(x + 1, y + 17, Color::MenuTitleLessBright());
    }
    else
    {
        Painter::DrawVolumeButton(x + 1, y + 17, MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3, 2, Color::MenuField(), ColorMenuItemBrighter(), ColorMenuItemLessBright(), true, shade);
    }

    float deltaY = Step();
    Color::E colorText = shade ? LightShadingTextColor() : COLOR_BACK;
    Color::SetCurrent(colorText);
    if(deltaY == 0.0F) //-V2550 //-V550
    {
        Text(NameCurrentSubItem()).Draw(x + 4, y + 21);
    }
    else
    {
        Painter::DrawTextWithLimitationC(x + 4, static_cast<int>(y + 21 - deltaY), NameCurrentSubItem(), colorText, x, y + 19, MI_WIDTH_VALUE, MI_HEIGHT_VALUE - 1);
        HLine().Draw(static_cast<int>(y + (deltaY > 0 ? 31 : 19) - deltaY), x + 3, x + MI_WIDTH_VALUE + 1, Color::BLACK);
        Painter::DrawTextWithLimitationC(x + 4, static_cast<int>(y + (deltaY > 0 ? 33 : 9) - deltaY), deltaY > 0 ? NameNextSubItem() : NamePrevSubItem(), colorText, x, y + 19, MI_WIDTH_VALUE, MI_HEIGHT_VALUE - 1);
    }
    HLine().Draw(y + MI_HEIGHT + 1, x, x + MI_WIDTH, ColorBorderMenu(false));

    DrawGovernorChoiceColorFormulaHiPart(this, x, y, pressed, shade, false);

    FuncForDraw(x, y);
}

void Choice::Draw(int x, int y, bool opened)
{
    if(opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

void TimeItem::DrawClosed(int x, int y) const
{
    char buffer[20];
    
    bool pressed = IsPressed();
    bool shade = IsShade();
    DrawGovernorChoiceColorFormulaHiPart(this, x, y, pressed, shade, false);

    Painter::DrawVolumeButton(x + 1, y + 17, MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3, 2, shade ? Color::MenuTitleLessBright() : Color::MenuField(), ColorMenuItemBrighter(), ColorMenuItemLessBright(), true, shade);
    //int delta = 0;

    int deltaField = 10;
    int deltaSeparator = 2;
    int startX = 3;
    y += 21;
    PackedTime time = HAL_RTC::GetPackedTime();
    Text(Int2String((int)time.hours, false, 2, buffer)).Draw(x + startX, y, COLOR_BACK);
    Text(":").Draw(x + startX + deltaField, y);
    Text(Int2String((int)time.minutes, false, 2, buffer)).Draw(x + startX + deltaField + deltaSeparator, y);
    Text(":").Draw(x + startX + 2 * deltaField + deltaSeparator, y);
    Text(Int2String((int)time.seconds, false, 2, buffer)).Draw(x + startX + 2 * deltaField + 2 * deltaSeparator, y);

    startX = 44;
    Text(Int2String((int)time.day, false, 2, buffer)).Draw(x + startX, y);
    Text(":").Draw(x + startX + deltaField, y);
    Text(Int2String((int)time.month, false, 2, buffer)).Draw(x + startX + deltaField + deltaSeparator, y);
    Text(":").Draw(x + startX + 2 * deltaField + deltaSeparator, y);
    Text(Int2String((int)time.year, false, 2, buffer)).Draw(x + startX + 2 * deltaField + 2 * deltaSeparator, y);
}

void TimeItem::Draw(int x, int y, bool opened)
{
    if(opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

void Button::Draw(int x, int y, bool)
{
    bool pressed = IsPressed();
    bool shade = IsShade() || !IsActive();

    HLine().Draw(y + 1, x, x + MI_WIDTH, Color::MenuTitle(shade));
    Color::E color = shade ? Color::MENU_SHADOW : Color::WHITE;
    Region(MI_WIDTH - 2, MI_HEIGHT - 2).Fill(x + 1, y + 2, ColorMenuItem(false));
    Painter::DrawVolumeButton(x + 4, y + 5, MI_WIDTH - 8, MI_HEIGHT - 8, 3, ColorMenuItem(false), ColorMenuItemBrighter(), 
                            ColorMenuItemLessBright(), pressed, shade);

    int delta = (pressed && (!shade)) ? 2 : 1;
    
    Painter::DrawStringInCenterRectC(x + delta, y + delta, MI_WIDTH, MI_HEIGHT, Title(), color);
}

void SmallButton::Draw(int x, int y, bool)
{
    if (IsActive())
    {
        if (IsPressed())
        {
            Region(WIDTH_SB, WIDTH_SB).Fill(x, y, COLOR_FILL);
            Color::SetCurrent(COLOR_BACK);
        }
        else
        {
            Rectangle(WIDTH_SB, WIDTH_SB).Draw(x, y, COLOR_FILL);
            Color::SetCurrent(COLOR_FILL);
        }
        OwnData()->funcOnDraw(x, y);
    }
    else
    {
        Rectangle(WIDTH_SB, WIDTH_SB).Draw(x, y, COLOR_FILL);
    }
}

void Page::Draw(int x, int y, bool)
{
    bool isShade = IsShade() || !IsActive();
    bool isPressed = IsPressed();
    HLine().Draw(y + 1, x, x + MI_WIDTH, ColorBorderMenu(false));
    if (isShade)
    {
        Region(MI_WIDTH - 2, MI_HEIGHT - 2).Fill(x + 1, y + 2, Color::MenuTitleLessBright());
    }
    else
    {
        Painter::DrawVolumeButton(x + 1, y + 2, MI_WIDTH - 2, MI_HEIGHT - 2, 2, ColorMenuItem(isShade), ColorMenuItemBrighter(), ColorMenuItemLessBright(), isPressed, isShade);
    }
    Color::E colorText = isShade ? LightShadingTextColor() : Color::BLACK;
    int delta = 0;
    if(isPressed && (!isShade))
    {
        colorText = COLOR_FILL;
        delta = 1;
    }
    Painter::DrawStringInCenterRectC(x + delta, y + delta, MI_WIDTH, MI_HEIGHT, Title(), colorText);
}


void Page::DrawOpened(int layer, int yTop)
{
    DrawTitle(layer, yTop);
    DrawItems(layer, yTop + MP_TITLE_HEIGHT);
    if (Menu::CurrentItemIsOpened(GetName()))
    {
        int8 posCurItem = PosCurrentItem();
        Item *item = GetItem(posCurItem);
        for (int i = 0; i < 5; i++)
        {
            if (Menu::itemUnderButton[i + Key::F1] != item)
            {
                Menu::itemUnderButton[i + Key::F1] = 0;
            }
        }

        item->Draw(CalculateX(1), OpenedPosY(), true);
    }

    if (OwnData()->funcOnDraw)
    {
        OwnData()->funcOnDraw();
    }
}


void Page::DrawTitle(int layer, int yTop)
{
    int x = CalculateX(layer);
    if (IsSB())
    {
        SmallButonFrom(0)->Draw(LEFT_SB, yTop + 3);
        return;
    }
    int height = HeightOpened();
    bool shade = Menu::CurrentItemIsOpened(GetName());
    Region(MP_TITLE_WIDTH + 2, height + 2).Fill(x - 1, yTop, COLOR_BACK);
    Rectangle(MP_TITLE_WIDTH + 1, height + 1).Draw(x, yTop, ColorBorderMenu(shade));

    if (shade)
    {
        Region(MP_TITLE_WIDTH - 1, MP_TITLE_HEIGHT - 1).Fill(x + 1, yTop + 1, Color::MenuTitleLessBright());
        Region(MP_TITLE_WIDTH - 7, MP_TITLE_HEIGHT - 7).Fill(x + 4, yTop + 4, Color::MENU_TITLE_DARK);
    }
    else
    {
        Painter::DrawVolumeButton(x + 1, yTop + 1, MP_TITLE_WIDTH - 1, MP_TITLE_HEIGHT - 1, 3, Color::MenuTitle(false), ColorMenuTitleBrighter(), Color::MenuTitleLessBright(), shade, false);
    }

    VLine().Draw(x, yTop, yTop + HeightOpened(), ColorBorderMenu(false));
    bool condDrawRSet = NumSubPages() > 1 && Menu::CurrentItem()->Type() != TypeItem::ChoiceReg && Menu::CurrentItem()->Type() != TypeItem::Governor && Menu::OpenedItem()->Type() == TypeItem::Page;
    int delta = condDrawRSet ? -10 : 0;
    Color::E colorText = shade ? LightShadingTextColor() : Color::BLACK;
    x = Painter::DrawStringInCenterRectC(x, yTop, MP_TITLE_WIDTH + 2 + delta, MP_TITLE_HEIGHT, Title(), colorText);
    if (condDrawRSet)
    {
        Painter::Draw4SymbolsInRectC(x + 4, yTop + 11, GetSymbolForGovernor(GetCurrentSubPage()), colorText);
    }

    Menu::itemUnderButton[GetFuncButtonFromY(yTop)] = this;

    delta = 0;

    Color::SetCurrent(colorText);
    DrawPagesUGO(CalculateX(layer) + MP_TITLE_WIDTH - 3 + delta, yTop + MP_TITLE_HEIGHT - 2 + delta);
}


void Page::DrawItems(int layer, int yTop) const
{
    int posFirstItem = Menu::PosItemOnTop(this);
    int posLastItem = posFirstItem + MENU_ITEMS_ON_DISPLAY - 1;
    LIMITATION(posLastItem, posLastItem, 0, NumItems() - 1);
    int count = 0;
    for (int posItem = posFirstItem; posItem <= posLastItem; posItem++)
    {
        Item *item = GetItem(posItem);
        int top = yTop + MI_HEIGHT * count;
        item->Draw(CalculateX(layer), top);
        count++;
        Menu::itemUnderButton[GetFuncButtonFromY(top)] = item;
    }
}


void Governor::Draw(int x, int y)
{
    Draw(x, y, false);
}


int Item::CalculateX(int layer) const
{
    return MP_X - layer * Grid::DELTA / 4;
}


void Page::DrawPagesUGO(int right, int bottom) const
{
    int size = 4;
    int delta = 2;

    int allPages = (NumItems() - 1) / MENU_ITEMS_ON_DISPLAY + 1;
    int currentPage = GetCurrentSubPage();

    int left = right - (size + 1) * allPages - delta + (3 - allPages);
    int top = bottom - size - delta;

    for (int p = 0; p < allPages; p++)
    {
        int x = left + p * (size + 2);
        if (p == currentPage)
        {
            Region(size, size).Fill(x, top);
        }
        else
        {
            Rectangle(size, size).Draw(x, top);
        }
    }
}
