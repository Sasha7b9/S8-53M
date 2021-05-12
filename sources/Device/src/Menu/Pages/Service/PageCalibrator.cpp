// 2021/05/12 15:57:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


static void OnChanged_Calibrator_Mode(bool)
{
    CalibratorMode::Set(CalibratorMode::Get());
}


DEF_CHOICE_3(cCalibrator_Mode, PageService::PageCalibrator::self,
    "Калибратор", "Calibrator",
    "Режим работы калибратора",
    "Mode of operation of the calibrator",
    "Перем", "DC",
    "Пост", "AC",
    "0В", "OV",
    set.service.calibrator, nullptr, OnChanged_Calibrator_Mode, nullptr
)


static void OnPress_Calibrator_Calibrate()
{
    FPGA::state.need_calibration = true;
}


DEF_BUTTON(cCalibrator_Calibrate, PageService::PageCalibrator::self,
    "Калибровать", "Calibrate",
    "Запуск процедуры калибровки",
    "Running the calibration procedure",
    nullptr, OnPress_Calibrator_Calibrate
)


DEF_PAGE_2(pageCalibrator, PageService::self, NamePage::ServiceCalibrator,
    "КАЛИБРАТОР", "CALIBRATOR",
    "Управлением калибратором и калибровка осциллографа",
    "Item of the calibrator and calibration of an oscillograph",
    cCalibrator_Mode,
    cCalibrator_Calibrate,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageService::PageCalibrator::self = &pageCalibrator;
