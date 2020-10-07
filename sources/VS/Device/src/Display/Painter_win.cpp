#include "defines.h"
#pragma warning(push, 0)
#include "GUI/Application.h"

#define uint    unsigned int
#define int8    signed char
#define uint8   unsigned char
#define int16   signed short
#define uint16  unsigned short
#define uint    unsigned int
#define uchar   unsigned char
#define pString const char * const

#include <wx/display.h>

#pragma warning(pop)

#include "Display/Painter.h"
#include "Hardware/Timer.h"


#undef uint   
#undef int8   
#undef uint8  
#undef int16  
#undef uint16 
#undef uint   
#undef uchar  
#undef pString


#include "defines.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"


using namespace Primitives;


static wxBitmap bitmapButton(Display::WIDTH, Display::HEIGHT);
// Здесь будем рисовать
wxMemoryDC memDC;

// Здесь хранятся указатели на кнопки
static wxButton *buttons[PanelButton::Count] = { nullptr };
// Цвета
static uint colors[256];

static bool needStartTimerLong = false;
static bool needStopTimerLong = false;
// Здесь имя нажатой кнопки
static PanelButton::E pressedKey = PanelButton::Empty;

// Создаёт окно приложения. Возвращает хэндл виджета для отрисовки
static void CreateFrame();
// Создаёт все кнопки
static void CreateButtons(Frame *frame);
// Создаёт одну кнопку
static void CreateButton(PanelButton::E key, Frame *frame, const wxPoint &pos, const wxSize &size, pString title);
// Создаёт кнопки для меню канала
static void CreateButtonsChannel(Frame *frame, const char *title, int x, int y, PanelButton::E keyChannel, PanelButton::E keyRangeLess, PanelButton::E keyRangeMore, PanelButton::E keyRShiftLess, PanelButton::E keyRShiftMore);
// Создаёт кнопки группы синхронизации
static void CreateButtonsTrig(Frame *frame, int x, int y);

// Нарисовать одну вертикальную лиинию из count точек c расстоянием delta между соседнимит точками
static void DrawVPointLine(int x, int y, int count, int delta);
// Нарисовать одну горизонтальную лиинию из count точек c расстоянием delta между соседнимит точками
static void DrawHPointLine(int x, int y, int count, int delta);


class Screen : public wxPanel
{
public:
    Screen(wxWindow *parent) : wxPanel(parent)
    {
        SetMinSize({ Frame::WIDTH, Frame::HEIGHT });
        SetDoubleBuffered(true);
        Bind(wxEVT_PAINT, &Screen::OnPaint, this);
    }

    void OnPaint(wxPaintEvent &)
    {
        wxPaintDC dc(this);
        wxImage image = bitmapButton.ConvertToImage();
        image = image.Rescale(Frame::WIDTH, Frame::HEIGHT);
        wxBitmap bitmap(image);
        dc.DrawBitmap(bitmap, 0, 0);
    }
};


static Screen *screen = nullptr;


void Painter::Init()
{
    CreateFrame();
}


void Painter::BeginScene(Color::E color)
{
    memDC.SelectObject(bitmapButton);
    wxBrush brush({ 0, 0, 0 }, wxTRANSPARENT);
    memDC.SetBrush(brush);
    SetColor(color);
    Region(Display::WIDTH, Display::HEIGHT).Fill(0, 0, color);
}


void Painter::EndScene(bool)
{
    memDC.SelectObject(wxNullBitmap);
    screen->Refresh();
}


void Painter::SetColor(Color::E color)
{
    if (color == Color::NUM)
    {
        return;
    }

    if (color == Color::BLACK)
    {
        wxColour colorDraw = wxColour(0, 0, 0);
        memDC.SetPen(wxPen(colorDraw));
    }
    else
    {
        wxColour colorDraw = wxColour(0xFF, 0xFF, 0xFF);
        memDC.SetPen(wxPen(colorDraw));
    }
}


void Primitives::Region::Fill(int x, int y, Color::E color)
{
    Painter::SetColor(color);
    wxBrush brush = memDC.GetBrush();
    wxPen pen = memDC.GetPen();
    memDC.SetBrush(wxBrush(pen.GetColour()));
    memDC.DrawRectangle({ x, y, width + 1, height + 1 });
    memDC.SetBrush(brush);
}



void Primitives::Rectangle::Draw(int x, int y, Color::E color)
{
    Painter::SetColor(color);

    memDC.DrawRectangle({ x, y, width + 1, height + 1 });
}


void Display::Init()
{
    CreateFrame();
}


static void CreateFrame()
{
    Frame *frame = new Frame("");

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    screen = new Screen(frame);

    sizer->Add(screen);

    frame->SetSizer(sizer);

    CreateButtons(frame);

    frame->Show(true);
}


static void CreateButtons(Frame *frame)
{
    // Рисуем кнопки меню и функциональные

    PanelButton::E keys[2][5] = 
    {
        { PanelButton::F1,      PanelButton::F2,      PanelButton::F3,     PanelButton::F4,       PanelButton::F5 },
        { PanelButton::Cursors, PanelButton::Display, PanelButton::Memory, PanelButton::Measures, PanelButton::Service }
    };

    int x0 = 37;
    int y0 = Frame::HEIGHT + 10;

    int dX = 68;
    int dY = 5;

    int width = 58;
    int height = 25;

    wxSize size = {width, height};

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            PanelButton::E key = keys[j][i];
            CreateButton(key, frame, {x0 + (width + dX) * i, y0 + (height + dY) * j}, size, PanelButton(key).Name());
        }
    }

    // Рисуем кнопки управления

    dX = 5;

    int x00 = 145;

    width = 25;
    height = 25;

    x0 = x00 + 160;
    y0 = Frame::HEIGHT + 100;

    size.SetWidth(width);
    size.SetHeight(height);

    CreateButton(PanelButton::Menu, frame, {x0, y0}, size, "E");
//    CreateButton(PanelButton::Left, frame, {x0 - dX - width, y0}, size, "L");
//    CreateButton(PanelButton::Right, frame, {x0 + dX + width, y0}, size, "R");
//    CreateButton(PanelButton::Up, frame, {x0, y0 - height - dY}, size, "U");
//    CreateButton(PanelButton::Down, frame, {x0, y0 + height + dY}, size, "D");

    // Кнопки времени

    width = 51;
    x0 = x00 + 5;

    size.SetWidth(width);

//    CreateButton(PanelButton::TBaseLess, frame, {x0 + width + dY, y0}, size, "мс");
//    CreateButton(PanelButton::TBaseMore, frame, {x0, y0}, size, "с");
    y0 += height + dY;
//    CreateButton(PanelButton::TShiftLess, frame, {x0, y0}, size, "<-");
//    CreateButton(PanelButton::TShiftMore, frame, {x0 + width + dY, y0}, size, "->");

    int x = x00 + 5 + (2 * width + dX) / 2 - width / 2;

    CreateButton(PanelButton::Time, frame, {x, y0 - height - dY - height - dY}, size, "Развёртка");

    // Кнопки канала A

    int y = Frame::HEIGHT + 200;

//    CreateButtonsChannel(frame, "Канал 1", 5 + x00, y, PanelButton::ChannelA, PanelButton::RangeLessA, PanelButton::RangeMoreA, PanelButton::RShiftLessA, PanelButton::RShiftMoreA);

    // Кнопки канала B

//    CreateButtonsChannel(frame, "Канал 1", 120 + x00, y, PanelButton::ChannelB, PanelButton::RangeLessB, PanelButton::RangeMoreB, PanelButton::RShiftLessB, PanelButton::RShiftMoreB);

    CreateButtonsTrig(frame, 235 + x00, y - 130);

    CreateButton(PanelButton::Start, frame, { 230 + x00, Frame::HEIGHT + 198 }, { 80, 25 }, "ПУСК/СТОП");
}


static void CreateButton(PanelButton::E key, Frame *frame, const wxPoint &pos, const wxSize &size, pString title)
{
    wxButton *button = new wxButton(frame, static_cast<wxWindowID>(key), title, pos, size);

    button->Connect(static_cast<wxWindowID>(key), wxEVT_LEFT_DOWN, wxCommandEventHandler(Frame::OnDown));
    button->Connect(static_cast<wxWindowID>(key), wxEVT_LEFT_UP, wxCommandEventHandler(Frame::OnUp));

    buttons[key] = button;
}


//static void CreateButtonsChannel(Frame *frame, const char *title, int x, int y, PanelButton::E keyChannel, PanelButton::E keyRangeLess, PanelButton::E keyRangeMore, PanelButton::E keyRShiftLess, PanelButton::E keyRShiftMore)
//{
//    int width = 45;
//    int height = 20;
//
//    int dX = 5;
//    int dY = 5;
//
//    wxSize size = {width, height};
//
//    CreateButton(keyRangeLess, frame, {x, y}, size, "мВ");
//    CreateButton(keyRangeMore, frame, {x, y + height + dY}, size, "В");
//
//    CreateButton(keyRShiftMore, frame, {x + width + 2 * dX, y}, size, "+");
//    CreateButton(keyRShiftLess, frame, {x + width + 2 * dX, y + height + dY}, size, "-");
//
//    size.SetHeight(25);
//    size.SetWidth(width + width + dX * 2);
//
//    wxPoint pos = {x, y - dY - size.GetHeight()};
//
//    CreateButton(keyChannel, frame, pos, size, title);
//}


static void CreateButtonsTrig(Frame *frame, int x, int y)
{
    int width = 45;
    int height = 20;

    wxSize size = { width, height };

    CreateButton(PanelButton::Trig, frame, { x, y }, size, "СИНХР");
//    CreateButton(PanelButton::TrigLevMore, frame, { x, y + 30 }, size, "больше");
//    CreateButton(PanelButton::TrigLevLess, frame, { x, y + 60 }, size, "меньше");
}


void Frame::OnDown(wxCommandEvent &event)
{
    PanelButton::E key = static_cast<PanelButton::E>(event.GetId());

    event.Skip();

//    BufferButtons::Push(KeyEvent(key, TypePress::Press));

    needStartTimerLong = true;

    pressedKey = key;
}


void Frame::OnUp(wxCommandEvent &event)
{
//    PanelButton::E key = static_cast<PanelButton::E>(event.GetId());

    event.Skip();

//    BufferButtons::Push(KeyEvent(key, TypePress::Release));

    needStopTimerLong = true;

    pressedKey = PanelButton::Empty;
}


void Frame::OnTimerLong(wxTimerEvent&)
{
//    BufferButtons::Push(KeyEvent(pressedKey, TypePress::Long));

    pressedKey = PanelButton::Empty;
}


void Frame::HandlerEvents()
{
    if (needStartTimerLong)
    {
        timerLongPress.StartOnce(500);
        needStartTimerLong = false;
    }

    if (needStopTimerLong)
    {
        timerLongPress.Stop();
        needStopTimerLong = false;
    }
}


void Primitives::HLine::Draw(int y, int x1, int x2, Color::E color)
{
    Painter::SetColor(color);
    memDC.DrawLine({ x1, y }, { x2, y });
}


void Primitives::Point::Draw(int x, int y)
{
    memDC.DrawPoint({ x, y });
}


void Primitives::VLine::Draw(int x, int y0, int y1, Color::E color)
{
    Painter::SetColor(color);
    memDC.DrawLine({ x, y0 }, { x, y1 });
}


Primitives::MultiVPointLine::MultiVPointLine(int _numLines, uint16 *_x0, int _delta, int _count) : numLines(_numLines), x0(_x0), delta(_delta), count(_count)
{
}


void Primitives::MultiVPointLine::Draw(int y, Color::E color)
{
    Painter::SetColor(color);

    for (int i = 0; i < numLines; i++)
    {
        DrawVPointLine(x0[i], y, count, delta);
    }
}


static void DrawVPointLine(int x, int y, int count, int delta)
{
    for (int i = 0; i < count; i++)
    {
        memDC.DrawPoint({ x, y });
        y += delta;
    }
}


void Primitives::MultiHPointLine::Draw(int x, Color::E color)
{
    Painter::SetColor(color);

    for (int i = 0; i < numLines; i++)
    {
        DrawHPointLine(x, y[i], count, delta);
    }
}


static void DrawHPointLine(int x, int y, int count, int delta)
{
    for (int i = 0; i < count; i++)
    {
        memDC.DrawPoint({ x, y });
        x += delta;
    }
}
