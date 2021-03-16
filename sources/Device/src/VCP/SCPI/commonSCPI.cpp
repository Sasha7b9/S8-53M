#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "device.h"
#include "LAN/LAN.h"
#include "VCP/VCP.h"
#include "VCP/SCPI/SCPI.h"
#include "Settings/Settings.h"



void SCPI::COMMON::IDN(puchar )
{
    SCPI_SEND("MNIPI,S8-53/1,V%S,%X", NUM_VER, HAL_CRC::Calculate());
}



void SCPI::COMMON::RUN(puchar )
{

}



void SCPI::COMMON::STOP(puchar )
{

}



void SCPI::COMMON::RESET(puchar )
{
    Device::needResetSettings = true;
}



void SCPI::COMMON::AUTOSCALE(puchar )
{

}



void SCPI::COMMON::REQUEST(puchar )
{
    SCPI_SEND("S8-53/1");
}
