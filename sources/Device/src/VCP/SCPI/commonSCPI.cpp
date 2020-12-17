#include "defines.h"
#include "common/Hardware/HAL/HAL_c.h"
#include "device.h"
#include "LAN/LAN.h"
#include "VCP/VCP.h"
#include "VCP/SCPI/SCPI.h"
#include "Settings/Settings.h"



void SCPI::COMMON::IDN(pUCHAR )
{
    SCPI_SEND("MNIPI,S8-53/1,V%S,%X", NUM_VER, HAL_CRC::Calculate());
}



void SCPI::COMMON::RUN(pUCHAR )
{

}



void SCPI::COMMON::STOP(pUCHAR )
{

}



void SCPI::COMMON::RESET(pUCHAR )
{
    Device::needResetSettings = true;
}



void SCPI::COMMON::AUTOSCALE(pUCHAR )
{

}



void SCPI::COMMON::REQUEST(pUCHAR )
{
    SCPI_SEND("S8-53/1");
}
