// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/Memory/ROM_.h"
#include "Display/Warnings.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include <cstdio>
#include <cstdlib>


//----------------------------------------------------------------------------------------------------------------------

DEF_PAGE_5(pageDebug, PageMain::self, NamePage::Debug,
    "Œ“À¿ƒ ¿", "DEBUG",
    "",
    "",
    *PageDebug::PageADC::self,
    *PageDebug::PageRandomizer::self,
    *PageDebug::PageConsole::self,
    *PageDebug::PageInfo::self,
    *PageDebug::PageData::self,
    nullptr, nullptr, nullptr, nullptr
);


const Page *PageDebug::self = &pageDebug;
