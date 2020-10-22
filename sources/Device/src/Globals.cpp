#include "defines.h"
#include "Globals.h"
#include "Display/DisplayTypes.h"
#include "Display/Grid.h"
#include "Menu/Pages/HelpContent.h"


void *extraMEM = 0;

StateFPGA gStateFPGA = 
{
    false,
    StateWorkFPGA::Stop,
    StateCalibration::None
};

void SetItemForHint(Item *item)
{
    PageHelpContent::stringForHint = 0;
    PageHelpContent::itemHint = item;
}

uint8           *gData0 = 0;
uint8           *gData1 = 0;
DataSettings    *gDSet = 0;

uint8           *gData0memInt = 0;
uint8           *gData1memInt = 0;
DataSettings    *gDSmemInt = 0;

uint8           *gData0memLast = 0;
uint8           *gData1memLast = 0;
DataSettings    *gDSmemLast = 0;

int transmitBytes = 0;
