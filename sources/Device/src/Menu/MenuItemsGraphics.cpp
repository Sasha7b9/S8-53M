#include "MenuItemsGraphics.h"
#include "MenuItemsLogic.h"
#include "Menu.h"
#include "Hardware/Hardware.h"
#include "common/Hardware/HAL/HAL.h"
#include "common/Display/Colors.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "common/Display/Primitives.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include "common/Log.h"
#include <cstring>
#include <cstdio>


using namespace Primitives;

 
static void Governor_DrawOpened(Governor *governor, int x, int y);
static void ItemMACaddress_DrawOpened(MACaddress *mac, int x, int y);
static void ItemIPaddress_DrawOpened(IPaddress *ip, int x, int y);


void DrawGovernorChoiceColorFormulaHiPart(void *item, int x, int y, bool pressed, bool shade, bool opened)
{

    int delta = pressed && !shade ? 1 : 0;
    int width = MI_WIDTH_VALUE;

    if (Menu::TypeMenuItem(item) == TypeItem::IP && opened && ((IPaddress*)item)->port != 0)
    {
        width += MOI_WIDTH_D_IP;
    }

    Color::E color = shade ? ColorMenuTitleLessBright() : (IS_COLOR_SCHEME_WHITE_LETTERS ? Color::WHITE : Color::BLACK);
    HLine().Draw(y + 1, x, x + width + 3, ColorBorderMenu(false));

    if (shade)
    {
        Region(width + 2, MI_HEIGHT_VALUE + 3).Fill(x + 1, y + 2, ColorMenuItem(false));
    }
    else
    {
        Painter::DrawVolumeButton(x + 1, y + 2, width + 2, MI_HEIGHT_VALUE + 3, 2, ColorMenuItem(false), ColorMenuItemBrighter(), ColorMenuItemLessBright(), pressed, shade);
    }

    Text(Menu::TitleItem(item)).Draw(x + 6 + delta, y + 6 + delta, color);
    
    TypeItem::E type = Menu::TypeMenuItem(item);

    if(Menu::CurrentItem() == item)
    {
        char symbol = 0;

        if (type == TypeItem::Governor)
        {
            symbol = GetSymbolForGovernor(*((Governor*)item)->cell);
            ADDRESS_GOVERNOR = (uint)item;
        }
        else if (type == TypeItem::ChoiceReg || (Menu::ItemIsOpened(item) && type == TypeItem::Choice))
        {
            symbol = GetSymbolForGovernor(*((Choice*)item)->cell);
        }
        else if (type == TypeItem::Time)
        {
            Time *time = (Time*)item;
            if ((Menu::OpenedItem() == item) && (*time->curField != iEXIT) && (*time->curField != iSET))
            {
                int8 values[7] =
                {
                    0,
                    *time->day,
                    *time->month,
                    *time->year,
                    *time->hours,
                    *time->minutes,
                    *time->seconds
                };
                symbol = GetSymbolForGovernor(values[*time->curField]);
            }
        }

        Painter::Draw4SymbolsInRectC(x + MI_WIDTH - 13, y + 5 + (Menu::ItemIsOpened(item) ? 0 : 15), symbol, IS_COLOR_SCHEME_WHITE_LETTERS ? COLOR_BACK : COLOR_FILL);
    }
}

void DrawGovernorLowPart(Governor *governor, int x, int y, bool, bool shade)
{
    char buffer[20];
    
    Color::E colorTextDown = COLOR_BACK;

    Painter::DrawVolumeButton(x + 1, y + 17, MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3, 2, ColorMenuField(), 
        ColorMenuItemBrighter(), ColorMenuItemLessBright(), true, shade);
    if(shade)
    {
        colorTextDown = ColorMenuItem(false);
    }

    x = Text("\x80").Draw(x + 4, y + 21, colorTextDown);
    if(Menu::OpenedItem() != governor)
    {
        float delta = Governor_Step(governor);
        if(delta == 0.0F) //-V2550 //-V550
        {
            x = Text(Int2String(*governor->cell, false, 1, buffer)).Draw(x + 1, y + 21);
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
                x = Painter::DrawTextWithLimitationC(drawX, static_cast<int>(y + 21 - delta), Int2String(*governor->cell, false, 1, buffer),
                                            COLOR_BACK, limX, limY, limWidth, limHeight);
                Painter::DrawTextWithLimitationC(drawX, static_cast<int>(y + 21 + 10 - delta), Int2String(Governor_NextValue(governor), false, 1, buffer),
                                            COLOR_BACK, limX, limY, limWidth, limHeight);
            }
            if(delta < 0.0F)
            {
                x = Painter::DrawTextWithLimitationC(drawX, static_cast<int>(y + 21 - delta), Int2String(*governor->cell, false, 1, buffer),
                                            COLOR_BACK, limX, limY, limWidth, limHeight);
                Painter::DrawTextWithLimitationC(drawX, static_cast<int>(y + 21 - 10 - delta), Int2String(Governor_PrevValue(governor), false, 1, buffer),
                    COLOR_BACK, limX, limY, limWidth, limHeight);
            }
        }
    }
    else
    {
        x = Text(Int2String(*governor->cell, false, 1, buffer)).Draw(x + 1, y + 21, COLOR_FILL);
    }
    Text("\x81").Draw(x + 1, y + 21, colorTextDown);
}

static void DrawIPaddressLowPart(const IPaddress *ip, int x, int y, bool, bool shade)
{
    char buffer[20];

    Color::E colorTextDown = COLOR_BACK;

    Painter::DrawVolumeButton(x + 1, y + 17, MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3, 2, ColorMenuField(),
                             ColorMenuItemBrighter(), ColorMenuItemLessBright(), true, shade);
    if (shade)
    {
        colorTextDown = ColorMenuItem(false);
    }

    std::sprintf(buffer, "%03d.%03d.%03d.%03d", *ip->ip0, *ip->ip1, *ip->ip2, *ip->ip3);

    if (Menu::OpenedItem() != ip)
    {
        Text(buffer).Draw(x + 4, y + 21, colorTextDown);
    }
    else
    {
        Text(buffer).Draw(x + 4, y + 21, COLOR_FILL);
    }
}

static void DrawMACaddressLowPart(const MACaddress *mac, int x, int y, bool, bool shade)
{
    char buffer[20];

    Color::E colorTextDown = COLOR_BACK;

    Painter::DrawVolumeButton(x + 1, y + 17, MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3, 2, ColorMenuField(),
                             ColorMenuItemBrighter(), ColorMenuItemLessBright(), true, shade);
    if (shade)
    {
        colorTextDown = ColorMenuItem(false);
    }

    std::sprintf(buffer, "%02X.%02X.%02X.%02X.%02X.%02X", *mac->mac0, *mac->mac1, *mac->mac2, *mac->mac3, *mac->mac4, *mac->mac5);

    if (Menu::OpenedItem() != mac)
    {

        Text(buffer).Draw(x + 4, y + 21, colorTextDown);
    }
    else
    {
        Text(buffer).Draw(x + 4, y + 21, COLOR_FILL);
    }
}

void WriteTextFormula(const Formula *formula, int x, int y, bool)
{
    Function::E function = (Function::E)(*formula->function);
    
    if (function != Function::Mul && function != Function::Sum)
    {
        return;
    }

    bool funcIsMul = function == Function::Mul;
    int8 koeff1 = funcIsMul ? *formula->koeff1mul : *formula->koeff1add;
    int8 koeff2 = funcIsMul ? *formula->koeff2mul : *formula->koeff2add;
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

void DrawFormulaLowPart(const Formula *formula, int x, int y, bool, bool shade)
{
    Color::E colorTextDown = COLOR_BACK;

    Painter::DrawVolumeButton(x + 1, y + 17, MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3, 2, ColorMenuField(),
                     ColorMenuItemBrighter(), ColorMenuItemLessBright(), true, shade);
    if (shade)
    {
        colorTextDown = ColorMenuItem(false);
    }
    Painter::SetColor(colorTextDown);
    WriteTextFormula(formula, x + 6, y + 21, false);
}

static void Governor_DrawClosed(Governor *governor, int x, int y)
{
    bool pressed = Menu::IsPressed(governor);
    bool shade = Menu::IsShade(governor) || !Menu::ItemIsActive(governor);
    DrawGovernorLowPart(governor, x, y, pressed, shade);
    DrawGovernorChoiceColorFormulaHiPart(governor, x, y, pressed, shade, false);
}

static void ItemIPaddress_DrawClosed(IPaddress *ip, int x, int y)
{
    bool pressed = Menu::IsPressed(ip);
    bool shade = Menu::IsShade(ip) || !Menu::ItemIsActive(ip);
    DrawIPaddressLowPart(ip, x, y, pressed, shade);
    DrawGovernorChoiceColorFormulaHiPart(ip, x, y, pressed, shade, false);
}

static void ItemMACaddress_DrawClosed(MACaddress *mac, int x, int y)
{
    bool pressed = Menu::IsPressed(mac);
    bool shade = Menu::IsShade(mac) || !Menu::ItemIsActive(mac);
    DrawMACaddressLowPart(mac, x, y, pressed, shade);
    DrawGovernorChoiceColorFormulaHiPart(mac, x, y, pressed, shade, false);
}

void Formula_DrawClosed(Formula *formula, int x, int y)
{
    bool pressed = Menu::IsPressed(formula);
    bool shade = Menu::IsShade(formula) || !Menu::ItemIsActive(formula);
    DrawFormulaLowPart(formula, x, y, pressed, shade);
    DrawGovernorChoiceColorFormulaHiPart(formula, x, y, pressed, shade, false);
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

void DrawGovernorValue(int x, int y, const Governor *governor)
{
    char buffer[20];

    int startX = x + 40;
    int16 value = *governor->cell;
    int signGovernor = *governor->cell < 0 ? -1 : 1;
    if(signGovernor == -1)
    {
        value = -value;
    }
    Font::Set(TypeFont::_5);
    bool sign = governor->minValue < 0;
    Text(Int2String(governor->maxValue, sign, 1, buffer)).Draw(x + 55, y - 5, COLOR_FILL);
    Text(Int2String(governor->minValue, sign, 1, buffer)).Draw(x + 55, y + 2);
    Font::Set(TypeFont::_8);

    DrawValueWithSelectedPosition(startX, y, value, Governor_NumDigits(governor), gCurDigit, true, true);

    if(sign)
    {
       Char(signGovernor < 0 ? '\x9b' : '\x9a').Draw(startX - 1, y);
    }
}

static void DrawIPvalue(int x, int y, IPaddress *ip)
{
    if (gCurDigit > (ip->port == 0 ? 11 : 16))
    {
        gCurDigit = 0;
    }

    uint8 *bytes = ip->ip0;

    x += 15;

    y += 1;

    int numIP = 0;
    int selPos = 0;

    IPaddress_GetNumPosIPvalue(&numIP, &selPos);

    for (int i = 0; i < 4; i++)
    {
        DrawValueWithSelectedPosition(x, y, bytes[i], 3, numIP == i ? selPos : -1, false, true);
        if (i != 3)
        {
            Char('.').Draw(x + 5, y, COLOR_FILL);
        }
        x += 19;
    }

    if (ip->port != 0)
    {
        Char(':').Draw(x - 13, y, COLOR_FILL);
        DrawValueWithSelectedPosition(x + 14, y, *ip->port, 5, numIP == 4 ? selPos : -1, false, true);
    }
}

static void DrawMACvalue(int x, int y, MACaddress *mac)
{
    if (gCurDigit > 5)
    {
        gCurDigit = 0;
    }
    uint8 *bytes = mac->mac0;
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

void ItemGovernor_Draw(Governor *governor, int x, int y, bool opened)
{
    if (governor->funcBeforeDraw)
    {
        governor->funcBeforeDraw();
    }
    if(opened)
    {
        Governor_DrawOpened(governor, x, y);
    }
    else
    {
        Governor_DrawClosed(governor, x, y);
    }
}

void ItemIPaddress_Draw(IPaddress *ip, int x, int y, bool opened)
{
    if (opened)
    {
        ItemIPaddress_DrawOpened(ip, x - (ip->port == 0 ? 0 : MOI_WIDTH_D_IP), y);
    }
    else
    {
        ItemIPaddress_DrawClosed(ip, x, y);
    }
}

void ItemMACaddress_Draw(MACaddress *mac, int x, int y, bool opened)
{
    if (opened)
    {
        ItemMACaddress_DrawOpened(mac, x, y);
    }
    else
    {
        ItemMACaddress_DrawClosed(mac, x, y);
    }
}

void ItemFormula_Draw(Formula *formula, int x, int y, bool opened)
{
    if (opened)
    {
        
    }
    else
    {
        Formula_DrawClosed(formula, x, y);
    }
}

void DrawGovernorColorValue(int x, int y, GovernorColor *govColor, int delta)
{
    char buffer[20];
    
    ColorType *ct = govColor->colorType;
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

static void GovernorColor_DrawOpened(GovernorColor *gov, int x, int y)
{
    static const int delta = 43;
    x -= delta;
    Color_Init(gov->colorType);
    Rectangle(MI_WIDTH + delta + 2, MI_HEIGHT + 2).Draw(x - 1, y - 1, Color::BLACK);
    Rectangle(MI_WIDTH + delta, MI_HEIGHT).Draw(x, y, ColorMenuTitle(false));
    Painter::DrawVolumeButton(x + 1, y + 1, MI_WIDTH_VALUE + 2 + delta, MI_HEIGHT_VALUE + 3, 2, ColorMenuItem(false), 
        ColorMenuItemBrighter(), ColorMenuItemLessBright(), Menu::IsPressed(gov), Menu::IsShade(gov));
    HLine().Draw(y + MI_HEIGHT / 2 + 2, x, x + MI_WIDTH + delta, ColorMenuTitle(false));
    Painter::DrawStringInCenterRectC(x + (Menu::IsPressed(gov) ? 2 : 1), y + (Menu::IsPressed(gov) ? 2 : 1), MI_WIDTH + delta, MI_HEIGHT / 2 + 2, Menu::TitleItem(gov), Color::WHITE);
    DrawGovernorColorValue(x + 1, y + 19, gov, delta);
}

static void GovernorColor_DrawClosed(GovernorColor *gov, int x, int y)
{
    Color_Init(gov->colorType);
    DrawGovernorChoiceColorFormulaHiPart(gov, x, y, Menu::IsPressed(gov), Menu::IsShade(gov) || !Menu::ItemIsActive(gov), true);
    Region(MI_WIDTH_VALUE, MI_HEIGHT_VALUE - 1).Fill(x + 2, y + 20, gov->colorType->color);
}

void GovernorColor_Draw(GovernorColor *govColor, int x, int y, bool opened)
{
    if(opened)
    {
        GovernorColor_DrawOpened(govColor, x, y);
    }
    else
    {
        GovernorColor_DrawClosed(govColor, x, y);
    }
}

void ItemChoice_DrawOpened(Choice *choice, int x, int y)
{
    int height = Menu::HeightOpenedItem(choice);

    Rectangle(MP_TITLE_WIDTH + 2, height + 3).Draw(x - 1, y - 1, COLOR_BACK);
    
    DrawGovernorChoiceColorFormulaHiPart(choice, x - 1, y - 1, Menu::IsPressed(choice), false, true);
    Rectangle(MP_TITLE_WIDTH + 1, height + 1).Draw(x - 1, y, ColorMenuTitle(false));
 
    HLine().Draw(y + MOI_HEIGHT_TITLE - 1, x, x + MOI_WIDTH);
    Painter::DrawVolumeButton(x, y + MOI_HEIGHT_TITLE, MOI_WIDTH - 1, height - MOI_HEIGHT_TITLE, 1, Color::BLACK, ColorMenuTitleBrighter(),
                        ColorMenuTitleLessBright(), false, false);
    int index = *((int8*)choice->cell);
    for(int i = 0; i < choice->NumSubItems(); i++)
    {
        int yItem = y + MOI_HEIGHT_TITLE + i * MOSI_HEIGHT + 1;
        bool pressed = i == index;
        if(pressed)
        {
            Painter::DrawVolumeButton(x + 1, yItem, MOI_WIDTH - 2 , MOSI_HEIGHT - 2, 2, ColorMenuField(), ColorMenuTitleBrighter(),
                ColorMenuTitleLessBright(), pressed, false);
        }
        Text(choice->NameSubItem(i)).Draw(x + 4, yItem + 2, pressed ? Color::BLACK : ColorMenuField());
    }
}

void ItemTime_DrawOpened(Time *time, int x, int y)
{
    char buffer[20];
    
    int width = MI_WIDTH_VALUE + 3;
    int height = 61;
    Rectangle(width + 2, height + 3).Draw(x - 1, y - 1, COLOR_BACK);
    DrawGovernorChoiceColorFormulaHiPart(time, x - 1, y - 1, Menu::IsPressed(time), false, true);

    Rectangle(width + 1, height + 1).Draw(x - 1, y, ColorMenuTitle(false));

    HLine().Draw(y + MOI_HEIGHT_TITLE - 1, x, x + MOI_WIDTH);
    Painter::DrawVolumeButton(x, y + MOI_HEIGHT_TITLE, MOI_WIDTH - 1, height - MOI_HEIGHT_TITLE, 1, Color::BLACK, ColorMenuTitleBrighter(),
                             ColorMenuTitleLessBright(), false, false);

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

    char strI[8][20];
    std::strcpy(strI[iEXIT], "Не сохранять");
    std::strcpy(strI[iDAY], Int2String(*time->day, false, 2, buffer));
    std::strcpy(strI[iMONTH], Int2String(*time->month, false, 2, buffer));
    std::strcpy(strI[iYEAR], Int2String(*time->year, false, 2, buffer));
    std::strcpy(strI[iHOURS], Int2String(*time->hours, false, 2, buffer));
    std::strcpy(strI[iMIN], Int2String(*time->minutes, false, 2, buffer));
    std::strcpy(strI[iSEC], Int2String(*time->seconds, false, 2, buffer));
    std::strcpy(strI[iSET], "Сохранить");

    Text("д м г - ").Draw(x + 3, y + y0, COLOR_FILL);
    Text("ч м с - ").Draw(x + 3, y + y1);

    for (int i = 0; i < 8; i++)
    {
        if (*time->curField == i)
        {
            Region(strPaint[i].width, 8).Fill(x + strPaint[i].x - 1, y + strPaint[i].y, Color::FLASH_10);
        }
        Text(strI[i]).Draw(x + strPaint[i].x, y + strPaint[i].y, *time->curField == i ? Color::FLASH_01 : COLOR_FILL);
    }
}

static void GovernorIpCommon_DrawOpened(void *item, int x, int y, int dWidth)
{
    int height = 34;
    Rectangle(MP_TITLE_WIDTH + 2 + dWidth, height + 3).Draw(x - 1, y - 1, COLOR_BACK);
    Rectangle(MP_TITLE_WIDTH + 1 + dWidth, height + 1).Draw(x - 1, y, ColorMenuTitle(false));
    HLine().Draw(y + MOI_HEIGHT_TITLE - 1, x, x + MOI_WIDTH + dWidth);
    DrawGovernorChoiceColorFormulaHiPart(item, x - 1, y - 1, Menu::IsPressed(item), false, true);
    Painter::DrawVolumeButton(x, y + MOI_HEIGHT_TITLE, MOI_WIDTH - 1 + dWidth, height - MOI_HEIGHT_TITLE, 1, Color::BLACK, ColorMenuTitleBrighter(),
                             ColorMenuTitleLessBright(), false, false);
}

static void Governor_DrawOpened(Governor *governor, int x, int y)
{
    GovernorIpCommon_DrawOpened(governor, x, y, 0);
    DrawGovernorValue(x, y + 22, governor);
}

static void ItemIPaddress_DrawOpened(IPaddress *ip, int x, int y)
{
    GovernorIpCommon_DrawOpened(ip, x, y, ip->port == 0 ? 0 : MOI_WIDTH_D_IP);
    DrawIPvalue(x, y + 22, ip);
}

static void ItemMACaddress_DrawOpened(MACaddress *mac, int x, int y)
{
    GovernorIpCommon_DrawOpened(mac, x, y, 0);
    DrawMACvalue(x, y + 22, mac);
}

void ItemChoice_DrawClosed(Choice *choice, int x, int y)
{
    bool pressed = Menu::IsPressed(choice);
    bool shade = Menu::IsShade(choice) || !Menu::ItemIsActive(choice);
        
    if (shade)
    {
        Region(MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3).Fill(x + 1, y + 17, ColorMenuTitleLessBright());
    }
    else
    {
        Painter::DrawVolumeButton(x + 1, y + 17, MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3, 2, ColorMenuField(), ColorMenuItemBrighter(), ColorMenuItemLessBright(), true, shade);
    }

    float deltaY = choice->Step();
    Color::E colorText = shade ? LightShadingTextColor() : COLOR_BACK;
    Painter::SetColor(colorText);
    if(deltaY == 0.0F) //-V2550 //-V550
    {
        Text(choice->NameCurrentSubItem()).Draw(x + 4, y + 21);
    }
    else
    {
        Painter::DrawTextWithLimitationC(x + 4, static_cast<int>(y + 21 - deltaY), choice->NameCurrentSubItem(), colorText, x, y + 19, MI_WIDTH_VALUE, MI_HEIGHT_VALUE - 1);
        HLine().Draw(static_cast<int>(y + (deltaY > 0 ? 31 : 19) - deltaY), x + 3, x + MI_WIDTH_VALUE + 1, Color::BLACK);
        Painter::DrawTextWithLimitationC(x + 4, static_cast<int>(y + (deltaY > 0 ? 33 : 9) - deltaY), deltaY > 0 ? choice->NameNextSubItem() : choice->NamePrevSubItem(), colorText, x, y + 19, MI_WIDTH_VALUE, MI_HEIGHT_VALUE - 1);
    }
    HLine().Draw(y + MI_HEIGHT + 1, x, x + MI_WIDTH, ColorBorderMenu(false));

    DrawGovernorChoiceColorFormulaHiPart(choice, x, y, pressed, shade, false);

    choice->FuncForDraw(x, y);
}

void ItemChoice_Draw(Choice *choice, int x, int y, bool opened)
{
    if(opened)
    {
        ItemChoice_DrawOpened(choice, x, y);
    }
    else
    {
        ItemChoice_DrawClosed(choice, x, y);
    }
}

void ItemTime_DrawClosed(Time *item, int x, int y)
{
    char buffer[20];
    
    bool pressed = Menu::IsPressed(item);
    bool shade = Menu::IsShade(item);
    DrawGovernorChoiceColorFormulaHiPart(item, x, y, pressed, shade, false);

    Painter::DrawVolumeButton(x + 1, y + 17, MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3, 2, shade ? ColorMenuTitleLessBright() : ColorMenuField(), 
        ColorMenuItemBrighter(), ColorMenuItemLessBright(), true, shade);
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

void ItemTime_Draw(Time *time, int x, int y, bool opened)
{
    if(opened)
    {
        ItemTime_DrawOpened(time, x, y);
    }
    else
    {
        ItemTime_DrawClosed(time, x, y);
    }
}

void ItemButton_Draw(Button *button, int x, int y)
{
    bool pressed = Menu::IsPressed(button);
    bool shade = Menu::IsShade(button) || !Menu::ItemIsActive(button);

    HLine().Draw(y + 1, x, x + MI_WIDTH, ColorMenuTitle(shade));
    Color::E color = shade ? Color::MENU_SHADOW : Color::WHITE;
    Region(MI_WIDTH - 2, MI_HEIGHT - 2).Fill(x + 1, y + 2, ColorMenuItem(false));
    Painter::DrawVolumeButton(x + 4, y + 5, MI_WIDTH - 8, MI_HEIGHT - 8, 3, ColorMenuItem(false), ColorMenuItemBrighter(), 
                            ColorMenuItemLessBright(), pressed, shade);

    int delta = (pressed && (!shade)) ? 2 : 1;
    
    Painter::DrawStringInCenterRectC(x + delta, y + delta, MI_WIDTH, MI_HEIGHT, Menu::TitleItem(button), color);
}

void ItemSmallButton_Draw(SmallButton *smallButton, int x, int y)
{
    if (Menu::ItemIsActive(smallButton))
    {
        if (Menu::IsPressed(smallButton))
        {
            Region(WIDTH_SB, WIDTH_SB).Fill(x, y, COLOR_FILL);
            Painter::SetColor(COLOR_BACK);
        }
        else
        {
            Rectangle(WIDTH_SB, WIDTH_SB).Draw(x, y, COLOR_FILL);
            Painter::SetColor(COLOR_FILL);
        }
        smallButton->funcOnDraw(x, y);
    }
    else
    {
        Rectangle(WIDTH_SB, WIDTH_SB).Draw(x, y, COLOR_FILL);
    }
}

void ItemPage_Draw(Page *page, int x, int y)
{
    bool isShade = Menu::IsShade(page) || !Menu::ItemIsActive(page);
    bool isPressed = Menu::IsPressed(page);
    HLine().Draw(y + 1, x, x + MI_WIDTH, ColorBorderMenu(false));
    if (isShade)
    {
        Region(MI_WIDTH - 2, MI_HEIGHT - 2).Fill(x + 1, y + 2, ColorMenuTitleLessBright());
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
    Painter::DrawStringInCenterRectC(x + delta, y + delta, MI_WIDTH, MI_HEIGHT, Menu::TitleItem(page), colorText);
}
