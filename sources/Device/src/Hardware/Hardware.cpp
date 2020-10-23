#include "defines.h"
#include "common/Hardware/HAL/HAL_c.h"
#include "FDrive/FDrive.h"
#include "Hardware/Hardware.h"
#include "Panel/Panel.h"


#ifdef __cplusplus
extern "C" {
#endif
    
void Hardware::Init()
{
    HAL::Init();

    Panel::Init();

    FDrive::Init();
    
    Pin::G1.Reset();      // PG1 - когда равен 1, чтение дисплея, в остальных случаях 0
}

#ifdef __cplusplus
}
#endif
