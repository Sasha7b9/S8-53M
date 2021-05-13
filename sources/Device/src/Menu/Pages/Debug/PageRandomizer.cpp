// 2021/05/12 12:01:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"


//----------------------------------------------------------------------------------------------------------------------

DEF_GOVERNOR(gNumAverages, PageDebug::PageRandomizer::self,
    "Усредн.", "Average",
    "",
    "",
    setNRST.num_ave_for_rand, 1, 32, nullptr, nullptr, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_GOVERNOR(gNumSmooth, PageDebug::PageRandomizer::self,
    "Сглаживание", "Smoothing",
    "",
    "",
    setNRST.rand.num_smooth, 1, 10, nullptr, nullptr, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

static void OnChanged_SamplesForGates()
{
    FPGA::Randomizer::SetNumberMeasuresForGates(setNRST.rand.num_measures_for_gates);
}


DEF_GOVERNOR(gSamplesForGates, PageDebug::PageRandomizer::self,
    "Выб-к/ворота", "Samples/gates",
    "",
    "",
    setNRST.rand.num_measures_for_gates, 1, 2500, nullptr, OnChanged_SamplesForGates, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

static void OnChanged_AltTShift0()
{
    TShift::SetDelta(setNRST.rand.shift_T0);
}


DEF_GOVERNOR(gAltTShift0, PageDebug::PageRandomizer::self,
    "tShift доп.", "tShift alt.",
    "",
    "",
    setNRST.rand.shift_T0, 0, 510, nullptr, OnChanged_AltTShift0, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

DEF_PAGE_4(pageRandomizer, PageDebug::self, NamePage::DebugRandomizer,
    "РАНД-ТОР", "RANDOMIZER",
    "",
    "",
    gNumAverages,
    gNumSmooth,
    gSamplesForGates,
    gAltTShift0,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDebug::PageRandomizer::self = &pageRandomizer;
