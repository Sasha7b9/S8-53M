// 2021/05/12 16:00:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Menu/Menu.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


static void OnPress_Information_Exit()
{
    Display::SetDrawMode(DrawMode::Auto, 0);
    Display::RemoveAddDrawFunction();
}


DEF_SMALL_BUTTON(sbInformation_Exit, PageService::PageCalibrator::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, OnPress_Information_Exit, DrawSB_Exit, nullptr
)


static void Information_Draw()
{
    Display::BeginFrame(Color::BLACK);
    int x = 100;
    int dY = 20;
    int y = 20;
    Rectangle(319, 239).Draw(0, 0, Color::FILL);
    y += dY;
    Text(LANG_RU ? "ИНФОРМАЦИЯ" : "INFORMATION").Draw(x, y);
    y += dY;
    Text(LANG_RU ? "Модель : С8-53/1" : "Model : S8-53/1").Draw(x, y);
    y += dY;

    Text(LANG_RU ? "Программное обеспечение:" : "Software:").Draw(x, y);
    y += dY;

    Text((pchar)(LANG_RU ? "версия %s" : "version %s"), NUM_VER).Draw(x, y);

    y += dY;

    Text("CRC32 : %X", HAL_CRC::Calculate()).Draw(x, y, Color::FILL);

    dY = -10;
    Text("Для получения помощи нажмите и удерживайте кнопку ПОМОЩЬ").DrawInCenterRect(0, 190 + dY, 320, 20);
    Text("Отдел маркетинга: тел./факс. 8-017-270-02-00").DrawInCenterRect(0, 205 + dY, 320, 20);
    Text("Разработчики: e-mail: mnipi-24(@)tut.by, тел. 8-017-270-02-23").DrawInCenterRect(0, 220 + dY, 320, 20);

    Menu::Draw();
    Display::EndFrame();
}


static void OnPress_Information()
{
    PageService::PageInformation::self->OpenAndSetItCurrent();
    Display::SetDrawMode(DrawMode::Hand, Information_Draw);
}


DEF_PAGE_6(pageInformation, PageService::self, NamePage::SB_Information,
    "ИНФОРМАЦИЯ", "INFORMATION",
    "Выводит на экран идентификационные данные осциллографа",
    "Displays identification data of the oscilloscope",
    sbInformation_Exit, // СЕРВИС - ИНФОРМАЦИЯ - Выход
    Item::empty,
    Item::empty,
    Item::empty,
    Item::empty,
    Item::empty,
    nullptr, OnPress_Information, nullptr, nullptr
)

const Page *PageService::PageInformation::self = &pageInformation;
