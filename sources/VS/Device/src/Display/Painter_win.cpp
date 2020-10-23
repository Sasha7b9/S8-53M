#include "defines.h"
#pragma warning(push, 0)
#include "GUI/Application.h"
#include "GUI/Controls/Governor.h"

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

#include "common/Display/Painter_c.h"
#include "common/Display/Primitives_c.h"
#include "common/Utils/Math_c.h"
#include "common/Hardware/Timer_c.h"
#include "Menu/Menu.h"
#include "Panel/Panel.h"


#undef uint   
#undef int8   
#undef uint8  
#undef int16  
#undef uint16 
#undef uint   
#undef uchar  
#undef pString


using namespace Primitives;

static wxBitmap bitmapScreen(Display::WIDTH, Display::HEIGHT);
// Здесь будем рисовать
wxMemoryDC memDC;

// Цвета
static uint colors[256];

static bool needStartTimerLong = false;
static bool needStopTimerLong = false;
// Здесь имя нажатой кнопки
static Key::E pressedKey = Key::None;


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
    if (c == Color::Count)
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
    Application::CreateFrame();
}


void Application::CreateFrame()
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

    CreateGovernors(frame);

    frame->Show(true);
}


void Frame::OnDown(wxCommandEvent &event)
{
    Key::E key = static_cast<Key::E>(event.GetId());

    event.Skip();

    int code = Key::ToCode(key) | Action::ToCode(Action::Down);

    Panel::ProcessingCommandFromPIC(static_cast<uint16>(code));

    needStartTimerLong = true;

    pressedKey = key;
}


void Frame::OnUp(wxCommandEvent &event)
{
    Key::E key = static_cast<Key::E>(event.GetId());

    event.Skip();

    int code = Key::ToCode(key) | Action::ToCode(Action::Up);

    Panel::ProcessingCommandFromPIC(static_cast<uint16>(code));

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
