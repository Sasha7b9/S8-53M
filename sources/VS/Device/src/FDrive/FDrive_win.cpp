#include "defines.h"
#include "FDrive/FDrive.h"


bool FDrive::isConnected = false;


void FDrive::Init()
{

}


void FDrive::GetNumDirsAndFiles(const char *, int *, int *)
{

}


bool FDrive::GetNameDir(const char *, int, char *, StructForReadDir *)
{
    return false;
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
