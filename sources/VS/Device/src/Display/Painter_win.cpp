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

#include "common/Display/Primitives_c.h"
#include "common/Utils/Math_c.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"
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

// Здесь хранятся указатели на кнопки
static wxButton *buttons[Key::Count] = { nullptr };
static GovernorGUI *governors[Key::Count] = { nullptr };
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
static void CreateButton(Key::E key, Frame *frame, const wxPoint &pos, const wxSize &size);

static void CreateGovernors(Frame *frame);

static void CreateGovernor(Key::E key, Frame *frame, const wxPoint &pos);

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

    CreateGovernors(frame);

    frame->Show(true);
}


static void CreateButtons(Frame *frame)
{
    int width = 58;
    int height = 25;

    wxSize size = { width, height };

    for (int i = 0; i < 5; i++)
    {
        Key::E keys[5] =
        {
            Key::F1, Key::F2, Key::F3, Key::F4, Key::F5
        };

        CreateButton(keys[i], frame, { 640, 167 + (height + 29) * i }, size);
    }

    for (int row = 0; row < 2; row++)
    {
        Key::E keys[2][3] =
        {
            { Key::Cursors,  Key::Display, Key::Memory },
            { Key::Measures, Key::Help,    Key::Service }
        };

        for (int col = 0; col < 3; col++)
        {
            CreateButton(keys[row][col], frame, { 845 + col * (width + 5), 45 + 55 * row }, size);
        }
    }

    CreateButton(Key::Start, frame, { 1047, 71 }, size);

    for (int i = 0; i < 4; i++)
    {
        Key::E keys[4] = { Key::ChannelA, Key::ChannelB, Key::Time, Key::Synchronization };

        int x[4] = { 760, 882, 1030, 1150 };

        CreateButton(keys[i], frame, { x[i], 197 }, size);
    }

    CreateButton(Key::Menu, frame, { 640, 102 }, size);
}


static void CreateGovernors(Frame *frame)
{
    int x0 = 750;

    CreateGovernor(Key::Setting, frame, { x0, 53 });

    for (int row = 0; row < 2; row++)
    {
        for (int col = 0; col < 2; col++)
        {
            Key::E keys[2][2] =
            {
                { Key::RShiftA, Key::RShiftB },
                { Key::RangeA,  Key::RangeB }
            };

            CreateGovernor(keys[row][col], frame, { x0 + col * 133, 250 + row * 120 });
        }
    }

    for (int row = 0; row < 2; row++)
    {
        for (int col = 0; col < 2; col++)
        {
            Key::E keys[2][2] =
            {
                { Key::TShift, Key::TrigLev},
                { Key::TBase,  Key::None}
            };

            if (keys[row][col] != Key::None)
            {
                CreateGovernor(keys[row][col], frame, { 1030 + col * 125, 250 + row * 120 });
            }
        }
    }
}


static void CreateGovernor(Key::E key, Frame *frame, const wxPoint &pos)
{
    governors[key] = new GovernorGUI(frame, pos, key);
}


static void CreateButton(Key::E key, Frame *frame, const wxPoint &pos, const wxSize &size)
{
    wxButton *button = new wxButton(frame, static_cast<wxWindowID>(key), "", /*Key(key).Name(), */ pos, size);

    button->Connect(static_cast<wxWindowID>(key), wxEVT_LEFT_DOWN, wxCommandEventHandler(Frame::OnDown));
    button->Connect(static_cast<wxWindowID>(key), wxEVT_LEFT_UP, wxCommandEventHandler(Frame::OnUp));

    buttons[key] = button;
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
