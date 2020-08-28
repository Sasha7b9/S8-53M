#include "Hardware.h"
#include "Sound.h"
#include "FSMC.h"
#include "Utils/GlobalFunctions.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Log.h"
#include "Ethernet/Ethernet.h"
#include "Display/Display.h"
#include "FDrive/FDrive.h"
#include "Panel/Panel.h"


static void SystemClock_Config();



#ifdef __cplusplus
extern "C" {
#endif
    
void Hardware::Init(void)
{
    HAL::Init();

    Panel::Init();

    FSMC::Init();

    FlashDrive::Init();
    
    pinG1.Reset();      // PG1 - когда равен 1, чтение дисплея, в остальных случаях 0
}

#ifdef __cplusplus
}
#endif
