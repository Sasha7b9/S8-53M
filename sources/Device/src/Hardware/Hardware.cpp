#include "defines.h"
#include "FDrive/FDrive.h"
#include "Hardware/Hardware.h"
#include "Panel/Panel.h"


#ifdef __cplusplus
extern "C" {
#endif
    
void Hardware::Init(void)
{
    HAL::Init();

    Panel::Init();

    FlashDrive::Init();
    
    pinG1.Reset();      // PG1 - когда равен 1, чтение дисплея, в остальных случаях 0
}

#ifdef __cplusplus
}
#endif
