// 2021/05/12 12:01:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"


static void OnChanged_Randomizer_SamplesForGates()
{
    FPGA::Randomizer::SetNumberMeasuresForGates(setNRST.num_measures_for_gates);
}


DEF_GOVERNOR(mgSamplesForGates, PageDebug::PageRandomizer::self,
    "Âûá-ê/âîğîòà", "Samples/gates",
    "",
    "",
    setNRST.num_measures_for_gates, 1, 2500, nullptr, OnChanged_Randomizer_SamplesForGates, nullptr
)


static void OnChanged_Randomizer_AltTShift0()
{
    TShift::SetDelta(setNRST.shift_T0);
}


DEF_GOVERNOR(mgRandomizer_AltTShift0, PageDebug::PageRandomizer::self,
    "tShift äîï.", "tShift alt.",
    "",
    "",
    setNRST.shift_T0, 0, 510, nullptr, OnChanged_Randomizer_AltTShift0, nullptr
)


DEF_GOVERNOR(mgRandomizer_Average, PageDebug::PageRandomizer::self,
    "Óñğåäí.", "Average",
    "",
    "",
    setNRST.num_ave_for_rand, 1, 32, nullptr, nullptr, nullptr
)


DEF_PAGE_3(pageRandomizer, PageDebug::self, NamePage::DebugRandomizer,
    "ĞÀÍÄ-ÒÎĞ", "RANDOMIZER",
    "",
    "",
    mgSamplesForGates,   // ÎÒËÀÄÊÀ - ĞÀÍÄ-ÒÎĞ - Âûá-ê/âîğîòà
    mgRandomizer_AltTShift0,        // ÎÒËÀÄÊÀ - ĞÀÍÄ-ÒÎĞ - tShift äîï.
    mgRandomizer_Average,           // ÎÒËÀÄÊÀ - ĞÀÍÄ-ÒÎĞ - Óñğåäí.
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDebug::PageRandomizer::self = &pageRandomizer;
