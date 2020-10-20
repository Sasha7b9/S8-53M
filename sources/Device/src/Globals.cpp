#include "defines.h"
#include "Globals.h"
#include "Display/DisplayTypes.h"
#include "Display/Grid.h"


void *extraMEM = 0;

BitField gBF =
{
    0,  // showLevelRShift0
    0,  // showLevelRShift1
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
    
    // Governor
    0,
    0,
    0,

    // Flash drive
    0,  // flashDriveIsConnected
    1,  // cursorInDirs
    1,  // needRedrawFileManager

    0,  // settingsLoaded
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

const char *gStringForHint = 0;
Item *gItemHint = 0;


void SetItemForHint(Item *item)
{
    gStringForHint = 0;
    gItemHint = item;
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
