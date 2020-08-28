#include "Hardware.h"
#include "Sound.h"
#include "Utils/GlobalFunctions.h"
#include "Hardware/Timer.h"
#include "common/Hardware/HAL/HAL.h"
#include "common/Log.h"
#include "Ethernet/Ethernet.h"
#include "Display/Display.h"
#include "FDrive/FDrive.h"
#include "Panel/Panel.h"


#ifdef __cplusplus
extern "C" {
#endif
    
void Hardware::Init(void)
{
    HAL::Init();

    Panel::Init();

    FlashDrive::Init();
    
    pinG1.Reset();      // PG1 - ����� ����� 1, ������ �������, � ��������� ������� 0
}

#ifdef __cplusplus
}
#endif
