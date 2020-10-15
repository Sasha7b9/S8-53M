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
#include "common/Display/Primitives.h"
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
#include "common/Utils/Math.h"


using namespace Primitives;

static wxBitmap bitmapScreen(Display::WIDTH, Display::HEIGHT);
// Здесь будем рисовать
wxMemoryDC memDC;

// Здесь хранятся указатели на кнопки
static wxButton *buttons[Key::Count] = { nullptr };
// Цвета
static uint colors[256];

static bool needStartTimerLong = false;
static bool needStopTimerLong = false;
// Здесь имя нажатой кнопки
static Key::E pressedKey = Key::None;

// Создаёт окно приложения. Возвращает хэндл виджета для отрисовки
static void CreateFrame();
// Создаёт все кнопки
static void CreateButtons(Frame *frame);
// Создаёт одну кнопку
static void CreateButton(Key::E key, Frame *frame, const wxPoint &pos, const wxSize &size, pString title);

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
        wxImage image = bitmapScreen.ConvertToImage();
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
    memDC.SelectObject(bitmapScreen);
    wxBrush brush({ 0, 0, 0 }, wxTRANSPARENT);
    memDC.SetBrush(brush);
    Color::SetCurrent(color);
    Region(Display::WIDTH, Display::HEIGHT).Fill(0, 0, color);
}


void Painter::EndScene(bool)
{
    memDC.SelectObject(wxNullBitmap);
    screen->Refresh();
}


void Color::SetCurrent(Color::E c)
{
    if (c == Color::NUM)
    {
        return;
    }

    current = c;

    uint colorValue = Color::FromSettings(c);
    uint8 b = colorValue & 0xFF;
    uint8 g = (colorValue >> 8) & 0xFF;
    uint8 r = (colorValue >> 16) & 0xFF;
    wxColour color = wxColour(r, g, b);
    memDC.SetPen(wxPen(color));
}


void Display::Init()
{
    CreateFrame();
}


static void CreateFrame()
{
    Frame *frame = new Frame("");

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
    sizer->AddSpacer(0);
    vSizer->AddSpacer(20);
    sizer->Add(vSizer);
    screen = new Screen(frame);

    vSizer->Add(screen);

    frame->SetSizer(sizer);

    CreateButtons(frame);

    frame->Show(true);
}


static void CreateButtons(Frame *frame)
{
    // Рисуем кнопки меню и функциональные

    Key::E keys[5] = 
    {
        Key::F1, Key::F2, Key::F3, Key::F4, Key::F5
    };

    int x = 640;
    int y0 = 167;

    int dY = 29;

    int width = 58;
    int height = 25;

    wxSize size = {width, height};

    for (int i = 0; i < 5; i++)
    {
        Key::E key = keys[i];
        CreateButton(key, frame, { x, y0 + (height + dY) * i }, size, Key(key).Name());
    }

    int y = 45;

    CreateButton(Key::Cursors, frame, { 845, y }, size, Key(Key::Cursors).Name());

//    CreateButton(Key::Menu, frame, { x, 101 }, size, Key(Key::Menu).Name());
//
//    CreateButton(Key::Menu, frame, { x, 101 }, size, Key(Key::Menu).Name());
//
//    CreateButton(Key::Menu, frame, { x, 101 }, size, Key(Key::Menu).Name());
//
//    CreateButton(Key::Menu, frame, { x, 101 }, size, Key(Key::Menu).Name());
//
//    CreateButton(Key::Menu, frame, { x, 101 }, size, Key(Key::Menu).Name());
//
//    CreateButton(Key::Menu, frame, { x, 101 }, size, Key(Key::Menu).Name());
}


static void CreateButton(Key::E key, Frame *frame, const wxPoint &pos, const wxSize &size, pString title)
{
    wxButton *button = new wxButton(frame, static_cast<wxWindowID>(key), title, pos, size);

    button->Connect(static_cast<wxWindowID>(key), wxEVT_LEFT_DOWN, wxCommandEventHandler(Frame::OnDown));
    button->Connect(static_cast<wxWindowID>(key), wxEVT_LEFT_UP, wxCommandEventHandler(Frame::OnUp));

    buttons[key] = button;
}


void Frame::OnDown(wxCommandEvent &event)
{
    Key::E key = static_cast<Key::E>(event.GetId());

    event.Skip();

//    BufferButtons::Push(KeyEvent(key, TypePress::Press));

    needStartTimerLong = true;

    pressedKey = key;
}


void Frame::OnUp(wxCommandEvent &event)
{
//    Key::E key = static_cast<Key::E>(event.GetId());

    event.Skip();

//    BufferButtons::Push(KeyEvent(key, TypePress::Release));

    needStopTimerLong = true;

    pressedKey = Key::None;
}


void Frame::OnTimerLong(wxTimerEvent&)
{
//    BufferButtons::Push(KeyEvent(pressedKey, TypePress::Long));

    pressedKey = Key::None;
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
