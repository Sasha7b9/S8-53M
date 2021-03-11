#include "defines.h"
#include "FPGA/FPGA.h"
#include "FPGA/Storage.h"


static int additionShift = 0;

StateFPGA FPGA::state;


void FPGA::WriteToDAC(TypeWriteDAC::E, uint16)
{

}


void FPGA::WriteToAnalog(TypeWriteAnalog::E, uint)
{

}


void FPGA::SetAdditionShift(int shift)
{
    additionShift = shift;
}


float FPGA::GetFreq()
{
    return 0.0F;
}


void FPGA::Update()
{

}


void FPGA::TemporaryPause()
{

}


void FPGA::FindAndSetTrigLevel()
{

}


void FPGA::Start()
{

}


void FPGA::Stop(bool)
{

}


bool FPGA::IsRunning()
{
    return false;
}


StateWorkFPGA::E FPGA::CurrentStateWork()
{
    return StateWorkFPGA::Stop;
}


void FPGA::FillDataPointer(DataSettings *)
{

}


void FPGA::OnPressStartStop()
{

}


void FPGA::WriteToHardware(uint16 *const, uint16, bool)
{

}


void FPGA::SetNumberMeasuresForGates(int)
{

}


void FPGA::SetNumSignalsInSec(int)
{

}


void FPGA::Init()
{

}


void FPGA::StartAutoFind()
{

}


void FPGA::ClearData()
{

}


void FPGA::Write(TypeRecord::E, uint16 *, uint, bool)
{

}
