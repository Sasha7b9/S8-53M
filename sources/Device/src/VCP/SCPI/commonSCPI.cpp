#include "defines.h"
#include "SCPI.h"
#include "Hardware/Hardware.h"
#include "common/Hardware/HAL/HAL.h"
#include "VCP/VCP.h"
#include "Settings/Settings.h"



void SCPI::COMMON::IDN(const uint8 *)
{
    SCPI_SEND("MNIPI,S8-53/1,V%S,%X", NUM_VER, HAL_CRC::Calculate());
}



void SCPI::COMMON::RUN(const uint8 *)
{

}



void SCPI::COMMON::STOP(const uint8 *)
{

}



void SCPI::COMMON::RESET(const uint8 *)
{
    NEED_RESET_SETTINGS = 1;
}



void SCPI::COMMON::AUTOSCALE(const uint8 *)
{

}



void SCPI::COMMON::REQUEST(const uint8 *)
{
    SCPI_SEND("S8-53/1");
}
