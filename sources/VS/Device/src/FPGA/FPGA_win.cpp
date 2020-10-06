#include "defines.h"
#include "FPGA/FPGA.h"
#include "FPGA/Storage.h"


float FPGA::GetFreq()
{
    return 0.0F;
}


void FPGA::SetCalibratorMode(CalibratorMode::E)
{

}


bool FPGA::RangeIncrease(Channel::E)
{
    return false;
}


void FPGA::Update()
{

}


void FPGA::SetTrigInput(TrigInput::E)
{

}


void FPGA::SetPeackDetMode(PeackDetMode::E)
{

}


void FPGA::TemporaryPause()
{

}


void FPGA::SetTrigSource(TrigSource::E)
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


void FPGA::LoadKoeffCalibration(Channel::E)
{

}


bool FPGA::IsRunning()
{
    return false;
}


void FPGA::SetTrigLev(TrigSource::E, int16)
{

}


void FPGA::SetTShift(int)
{

}


char const *FPGA::GetTShiftString(int16, char [20])
{
    return "";
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


void FPGA::SetRShift(Channel::E, int16)
{

}


void FPGA::SetModeCouple(Channel::E, ModeCouple::E)
{

}


void FPGA::EnableChannelFiltr(Channel::E, bool)
{

}


void FPGA::WriteToHardware(uint8 *const, uint8, bool)
{

}


void FPGA::SetNumberMeasuresForGates(int)
{

}


void FPGA::SetDeltaTShift(int16)
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


void FPGA::SetTBase(TBase::E)
{

}


void FPGA::SetTrigPolarity(TrigPolarity::E)
{

}


bool FPGA::RangeDecrease(Channel::E)
{
    return false;
}


void FPGA::TBaseDecrease()
{

}


void FPGA::TBaseIncrease()
{

}


void FPGA::ClearData()
{

}
