#include "defines.h"
#include "common/Utils/String_.h"
#include "FDrive/FDrive.h"


bool FDrive::isConnected = false;
bool FDrive::needOpenFileMananger = false;


void FDrive::Init()
{

}


void FDrive::GetNumDirsAndFiles(const char *, int *, int *)
{

}


String FDrive::GetNameDir(const char *, int, StructForReadDir *)
{
    return String("");
}


bool FDrive::GetNextNameDir(char *, StructForReadDir *)
{
    return false;
}


void FDrive::CloseCurrentDir(StructForReadDir *)
{

}


bool FDrive::GetNameFile(const char *, int , char *, StructForReadDir *)
{
    return false;
}


bool FDrive::GetNextNameFile(char *, StructForReadDir *)
{
    return false;
}


bool FDrive::OpenNewFileForWrite(const char *, StructForWrite *)
{
    return false;
}


bool FDrive::WriteToFile(uint8 *, int , StructForWrite *)
{
    return false;
}


bool FDrive::CloseFile(StructForWrite *)
{
    return false;
}


void FDrive::Update()
{

}
