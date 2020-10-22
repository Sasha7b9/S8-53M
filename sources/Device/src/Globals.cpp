#include "defines.h"
#include "Globals.h"
#include "Display/DisplayTypes.h"
#include "Display/Grid.h"
#include "Menu/Pages/HelpContent.h"


void *extraMEM = 0;

BitField gBF =
{
    0,  // showLevelTrigLev
    0,  // trigEnable
    0,  // drawRShiftMarkers
    1,  // needFinishDraw
    0,  // framesElapsed
    0,  // numDrawingSignals

  
    // FPGA
    0,  // FPGAtrigAutoFind
    0,  // FPGAautoFindInProgress
    0,  // FPGAtemporaryPause
    0,  // FPGAinProcessingOfRead
    1,  // FPGAcanReadData
    0,  // FPGAcritiacalSituation
    0,  // FPGAfirstAfterWrite
    
    // Flash drive
    0,  // flashDriveIsConnected
    1,  // cursorInDirs
    1,  // needRedrawFileManager

    0,  // showHelpHints

    0,  // showDebugMenu

    Grid::Bottom(), // topMeasures
 
    0,  // needOpenFileManager
    0,  // needClosePageSB
    0   // needResetSettings
};


GMemory gMemory =
{
    0,  // currentNumLatestSignal
    0,  // currentNumIntSignal
    0,  // showAlways
    0,  // runningFPGAbeforeSmallButtons
    0,  // exitFromIntToLast
    0,  // после установки имени и сохранения на флешку возвращаемся в основное рабочее окно
    0   // needForSaveToFlashDrive
};


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
