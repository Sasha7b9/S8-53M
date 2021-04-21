// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "VCP/VCP.h"
#include "LAN/SocketTCP.h"


/** @defgroup SCPI
 *  @{
 */



#define ENTER_ANALYSIS                                  \
    Word parameter;                                     \
    if (GetWord(buffer, &parameter, 0)) {               \
        uint8 value = GetValueFromMap(map, &parameter); \
        if (value < 255) {

#define LEAVE_ANALYSIS   }                              \
        else {LOG_WRITE("Error");}                      \
    }


#define SCPI_SEND(...)                                  \
    if(VCP::connectToHost)                              \
    {                                                   \
        VCP::SendFormatStringAsynch(__VA_ARGS__);       \
    }                                                   \
    if (LAN::clientIsConnected)                         \
    {                                                   \
        TCPSocket::SendFormatString(__VA_ARGS__);       \
    }

#define ENTER_PARSE_FUNC(funcName)                      \
void funcName(puchar buffer)                            \
{                                                       \
    static const StructCommand commands[] =             \
    {

#define LEAVE_PARSE_FUNC                                \
        {0}                                             \
    };                                                  \
    SCPI::ProcessingCommand(commands, const_cast<uint8 *>(buffer));  \
}



struct StructCommand
{
    char        *name;
    pFuncpCU8   func;
};



class SCPI
{
public:
    static void AddNewData(puchar buffer, uint length);

private:
    static void ParseNewCommand(uint8 *buffer);   // \todo Временно. Потом доделать
    static void ProcessingCommand(const StructCommand *commands, uint8 *buffer);
    static bool FirstIsInt(puchar buffer, int *value, int min, int max);
    static void ProcessDISPLAY(puchar buffer);
    static void ProcessCHANNEL(puchar buffer);
    static void ProcessTRIG(puchar buffer);
    static void ProcessTBASE(puchar buffer);

    class COMMON
    {
    public:
        static void IDN(puchar buffer);
        static void RUN(puchar buffer);
        static void STOP(puchar buffer);
        static void RESET(puchar buffer);
        static void AUTOSCALE(puchar buffer);
        static void REQUEST(puchar buffer);
    };

    class CONTROL
    {
    public:
        static void KEY(puchar buffer);
        static void GOVERNOR(puchar buffer);
    };

    class CHANNEL
    {
    public:
        static void INPUT(puchar buffer);
        static void COUPLE(puchar buffer);
        static void FILTR(puchar buffer);
        static void INVERSE(puchar buffer);
        static void RANGE(puchar buffer);
        static void OFFSET(puchar buffer);
        static void FACTOR(puchar buffer);
    };

    class DISPLAY
    {
    public:
        static void AUTOSEND(puchar buffer);
        static void MAPPING(puchar buffer);
        static void ACCUM(puchar buffer);
        static void ACCUM_NUMBER(puchar buffer);
        static void ACCUM_MODE(puchar buffer);
        static void ACCUM_CLEAR(puchar buffer);
        static void AVERAGE(puchar buffer);
        static void AVERAGE_NUMBER(puchar buffer);
        static void AVERAGE_MODE(puchar buffer);
        static void MINMAX(puchar buffer);
        static void FILTR(puchar buffer);
        static void FPS(puchar buffer);
        static void WINDOW(puchar buffer);
        static void GRID(puchar buffer);
        static void GRID_TYPE(puchar buffer);
        static void GRID_BRIGHTNESS(puchar buffer);
    };

    class TBASE
    {
    public:
        static void RANGE(puchar buffer);
        static void OFFSET(puchar buffer);
        static void SAMPLING(puchar buffer);
        static void PEACKDET(puchar buffer);
        static void TPOS(puchar buffer);
        static void SELFRECORDER(puchar buffer);
        static void FUNCTIMEDIV(puchar buffer);
    };

    class TRIGGER
    {
    public:
        static void MODE(puchar buffer);
        static void SOURCE(puchar buffer);
        static void POLARITY(puchar buffer);
        static void INPUT(puchar buffer);
        static void FIND(puchar buffer);
        static void OFFSET(puchar buffer);
    };
};


/** @}
 */
