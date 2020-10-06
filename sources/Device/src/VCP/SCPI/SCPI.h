#pragma once
#include "Globals.h"
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
    if(CLIENT_VCP_IS_CONNECTED)                         \
    {                                                   \
        VCP::SendFormatStringAsynch(__VA_ARGS__);       \
    }                                                   \
    if (CLIENT_LAN_IS_CONNECTED)                        \
    {                                                   \
        TCPSocket_SendFormatString(__VA_ARGS__);        \
    }

#define ENTER_PARSE_FUNC(funcName)                      \
void funcName(pUCHAR buffer)                      \
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
    static void AddNewData(pUCHAR buffer, uint length);

private:
    static void ParseNewCommand(uint8 *buffer);   // \todo Временно. Потом доделать
    static void ProcessingCommand(const StructCommand *commands, uint8 *buffer);
    static bool FirstIsInt(pUCHAR buffer, int *value, int min, int max);
    static void ProcessDISPLAY(pUCHAR buffer);
    static void ProcessCHANNEL(pUCHAR buffer);
    static void ProcessTRIG(pUCHAR buffer);
    static void ProcessTBASE(pUCHAR buffer);

    class COMMON
    {
    public:
        static void IDN(pUCHAR buffer);
        static void RUN(pUCHAR buffer);
        static void STOP(pUCHAR buffer);
        static void RESET(pUCHAR buffer);
        static void AUTOSCALE(pUCHAR buffer);
        static void REQUEST(pUCHAR buffer);
    };

    class CONTROL
    {
    public:
        static void KEY(pUCHAR buffer);
        static void GOVERNOR(pUCHAR buffer);
    };

    class CHANNEL
    {
    public:
        static void INPUT(pUCHAR buffer);
        static void COUPLE(pUCHAR buffer);
        static void FILTR(pUCHAR buffer);
        static void INVERSE(pUCHAR buffer);
        static void RANGE(pUCHAR buffer);
        static void OFFSET(pUCHAR buffer);
        static void FACTOR(pUCHAR buffer);
    };

    class DISPLAY
    {
    public:
        static void AUTOSEND(pUCHAR buffer);
        static void MAPPING(pUCHAR buffer);
        static void ACCUM(pUCHAR buffer);
        static void ACCUM_NUMBER(pUCHAR buffer);
        static void ACCUM_MODE(pUCHAR buffer);
        static void ACCUM_CLEAR(pUCHAR buffer);
        static void AVERAGE(pUCHAR buffer);
        static void AVERAGE_NUMBER(pUCHAR buffer);
        static void AVERAGE_MODE(pUCHAR buffer);
        static void MINMAX(pUCHAR buffer);
        static void FILTR(pUCHAR buffer);
        static void FPS(pUCHAR buffer);
        static void WINDOW(pUCHAR buffer);
        static void GRID(pUCHAR buffer);
        static void GRID_TYPE(pUCHAR buffer);
        static void GRID_BRIGHTNESS(pUCHAR buffer);
    };

    class TBASE
    {
    public:
        static void RANGE(pUCHAR buffer);
        static void OFFSET(pUCHAR buffer);
        static void SAMPLING(pUCHAR buffer);
        static void PEACKDET(pUCHAR buffer);
        static void TPOS(pUCHAR buffer);
        static void SELFRECORDER(pUCHAR buffer);
        static void FUNCTIMEDIV(pUCHAR buffer);
    };

    class TRIGGER
    {
    public:
        static void MODE(pUCHAR buffer);
        static void SOURCE(pUCHAR buffer);
        static void POLARITY(pUCHAR buffer);
        static void INPUT(pUCHAR buffer);
        static void FIND(pUCHAR buffer);
        static void OFFSET(pUCHAR buffer);
    };
};


/** @}
 */
