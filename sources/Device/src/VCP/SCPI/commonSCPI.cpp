#include "defines.h"
#include "SCPI.h"
#include "Hardware/Hardware.h"
#include "common/Hardware/HAL/HAL.h"
#include "VCP/VCP.h"
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
    NEED_RESET_SETTINGS = 1;
}



void SCPI::COMMON::AUTOSCALE(pUCHAR )
{

}



void SCPI::COMMON::REQUEST(pUCHAR )
{
    SCPI_SEND("S8-53/1");
}
