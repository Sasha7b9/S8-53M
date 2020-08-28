#include "defines.h"
#include "SCPI.h"
#include "Hardware/Hardware.h"
#include "Hardware/HAL/HAL.h"
#include "VCP/VCP.h"
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SCPI::COMMON::IDN(uint8 *)
{
    SCPI_SEND("MNIPI,S8-53/1,V%S,%X", NUM_VER, HAL_CRC::Calculate());
}



void SCPI::COMMON::RUN(uint8 *)
{

}



void SCPI::COMMON::STOP(uint8 *)
{

}



void SCPI::COMMON::RESET(uint8 *)
{
    NEED_RESET_SETTINGS = 1;
}



void SCPI::COMMON::AUTOSCALE(uint8 *)
{

}



void SCPI::COMMON::REQUEST(uint8 *)
{
    SCPI_SEND("S8-53/1");
}
