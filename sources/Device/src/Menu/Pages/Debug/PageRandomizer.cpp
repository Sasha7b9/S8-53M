// 2021/05/12 12:01:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"


//----------------------------------------------------------------------------------------------------------------------

static void OnChanged_SamplesForGates()
{
    FPGA::Randomizer::SetNumberMeasuresForGates(setNRST.num_measures_for_gates);
}


DEF_GOVERNOR(mgSamplesForGates, PageDebug::PageRandomizer::self,
    "Выб-к/ворота", "Samples/gates",
    "",
    "",
    setNRST.num_measures_for_gates, 1, 2500, nullptr, OnChanged_SamplesForGates, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

static void OnChanged_AltTShift0()
{
    TShift::SetDelta(setNRST.shift_T0);
}


DEF_GOVERNOR(mgAltTShift0, PageDebug::PageRandomizer::self,
    "tShift доп.", "tShift alt.",
    "",
    "",
    setNRST.shift_T0, 0, 510, nullptr, OnChanged_AltTShift0, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_GOVERNOR(mgAverage, PageDebug::PageRandomizer::self,
    "Усредн.", "Average",
    "",
    "",
    setNRST.num_ave_for_rand, 1, 32, nullptr, nullptr, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_PAGE_3(pageRandomizer, PageDebug::self, NamePage::DebugRandomizer,
    "РАНД-ТОР", "RANDOMIZER",
    "",
    "",
    mgSamplesForGates,
    mgAltTShift0,
    mgAverage,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDebug::PageRandomizer::self = &pageRandomizer;
