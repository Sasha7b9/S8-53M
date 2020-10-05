#include "defines.h"
#include "common/Log.h"
#include "SCPI.h"
#include "Utils/Strings.h"
#include "Utils/GlobalFunctions.h"
#include <ctype.h>
#include <cstring>
#include <cstdlib>



struct StateProcessing { enum E
{
    WAIT,
    SAVE_SYMBOLS
};};

static int FindNumSymbolsInCommand(const uint8 *buffer);

static const int SIZE_BUFFER = 100;
// ���� ��������� ����� ������
static uint8 bufData[SIZE_BUFFER];
// ��������� �� ������ ��������� ���� ������
static int pointer = 0;



void SCPI::AddNewData(const uint8 *data, uint length)
{
    /*
    uint8 *temp = (uint8 *)malloc(length + 1);
    memcpy(temp, data, length);
    temp[length] = 0;

    int count = 0;

    for (int i = 0; i < length; i++)
    {
        if (data[i] == ':')
        {
            ++count;
        }
    }

    if(data[1] != 'D')
    {
        data[1] = data[1];
    }

    free(temp);
    */

    std::memcpy(&bufData[pointer], data, length);
    pointer += length;

label_another:

    for (int i = 0; i < pointer; i++)
    {
        bufData[i] = static_cast<uint8>(toupper(static_cast<int8>(bufData[i])));

        if (bufData[i] == 0x0d || bufData[i] == 0x0a)
        {
            uint8 *pBuffer = bufData;
            while (*pBuffer == ':' || *pBuffer == 0x0d || *pBuffer == 0x0a)
            {
                ++pBuffer;
            }

            SCPI::ParseNewCommand(pBuffer);
            if (i == pointer - 1)
            {
                pointer = 0;                // ���� ����� ���� - �������
                return;
            }
            else                            // ���� � ������ ���� ���� ������
            {
                uint8 *pBuf = bufData;
                for (++i; i < pointer; i++)
                {
                    *pBuf = bufData[i];   // �������� �� � ������
                    ++pBuf;
                    pointer = pBuf - bufData;
                }
                goto label_another;         // � ��������� ����� ��� ��� //-V2505
            }
        }
    }
}



void SCPI::ParseNewCommand(uint8 *buf)
{
    static const StructCommand commands[] =
    {
    {"*IDN ?",      SCPI::COMMON::IDN},
    {"*IDN?",       SCPI::COMMON::IDN},
    {"RUN",         SCPI::COMMON::RUN},
    {"STOP",        SCPI::COMMON::STOP},
    {"*RST",        SCPI::COMMON::RESET},
    {"AUTOSCALE",   SCPI::COMMON::AUTOSCALE}, 
    {"REQUEST ?",   SCPI::COMMON::REQUEST},

    {"DISPLAY",     ProcessDISPLAY},       // ������� ������ ��� ������ �����, ����� ����������.
    {"DISP",        ProcessDISPLAY},       // ��� ����� ��� ����������� ��������.

    {"CHANNEL1",    ProcessCHANNEL},
    {"CHAN1",       ProcessCHANNEL},

    {"CHANNEL2",    ProcessCHANNEL},
    {"CHAN2",       ProcessCHANNEL},

    {"TRIGGER",     ProcessTRIG},
    {"TRIG",        ProcessTRIG},

    {"TBASE",       ProcessTBASE},
    {"TBAS" ,       ProcessTBASE},

    {"KEY",         SCPI::CONTROL::KEY},
    {"GOVERNOR",    SCPI::CONTROL::GOVERNOR},
    {0}
    };
    
    SCPI::ProcessingCommand(commands, buf);
}



void SCPI::ProcessingCommand(const StructCommand *commands, uint8 *buffer) 
{
    int sizeNameCommand = FindNumSymbolsInCommand(buffer);
    if (sizeNameCommand == 0) 
    {
        return;
    }
    for (int i = 0; i < sizeNameCommand; i++)
    {
        buffer[i] = static_cast<uint8>(toupper(static_cast<int8>(buffer[i])));
    }
    int numCommand = -1;
    char *name = 0;
    do 
    {
        numCommand++;   
        name = commands[numCommand].name;
    } while (name != 0 && (!EqualsStrings(reinterpret_cast<char*>(buffer), name, sizeNameCommand)));

    if (name != 0) 
    {
        commands[numCommand].func(buffer + sizeNameCommand + 1);
    }
    else
    {
        SCPI_SEND(":COMMAND ERROR");
    }
}



static int FindNumSymbolsInCommand(const uint8 *buffer)
{
    int pos = 0;
    while ((buffer[pos] != ':') && (buffer[pos] != ' ') && (buffer[pos] != '\x0d'))
    {
        pos++;
    }
    return pos;
}



bool SCPI::FirstIsInt(const uint8 *buffer, int *value, int min, int max)
{
    Word param;
    if (GetWord(buffer, &param, 0))
    {
        char *n = reinterpret_cast<char *>(std::malloc(static_cast<uint>(param.numSymbols + 1)));
        std::memcpy(n, param.address, static_cast<uint>(param.numSymbols)); //-V575
        n[param.numSymbols] = '\0';
        bool res = String2Int(n, value) && *value >= min && *value <= max;
        std::free(n);
        return res;
    }
    return false;
}
