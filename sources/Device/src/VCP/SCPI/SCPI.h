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
void funcName(const uint8 *buffer)                      \
{                                                       \
    static const StructCommand commands[] =             \
    {

#define LEAVE_PARSE_FUNC                                \
        {0}                                             \
    };                                                  \
    SCPI::ProcessingCommand(commands, buffer);          \
}



struct StructCommand
{
    char        *name;
    pFuncpCU8   func;
};



class SCPI
{
public:
    static void AddNewData(const uint8 *buffer, uint length);

private:
    static void ParseNewCommand(uint8 *buffer);   // \todo Временно. Потом доделать
    static void ProcessingCommand(const StructCommand *commands, const uint8 *buffer);
    static bool FirstIsInt(const uint8 *buffer, int *value, int min, int max);
    static void ProcessDISPLAY(const uint8 *buffer);
    static void ProcessCHANNEL(const uint8 *buffer);
    static void ProcessTRIG(const uint8 *buffer);
    static void ProcessTBASE(const uint8 *buffer);

    class COMMON
    {
    public:
        static void IDN(const uint8 *buffer);
        static void RUN(const uint8 *buffer);
        static void STOP(const uint8 *buffer);
        static void RESET(const uint8 *buffer);
        static void AUTOSCALE(const uint8 *buffer);
        static void REQUEST(const uint8 *buffer);
    };

    class CONTROL
    {
    public:
        static void KEY(const uint8 *buffer);
        static void GOVERNOR(const uint8 *buffer);
    };

    class CHANNEL
    {
    public:
        static void INPUT(uint8 *buffer);
        static void COUPLE(uint8 *buffer);
        static void FILTR(uint8 *buffer);
        static void INVERSE(uint8 *buffer);
        static void RANGE(uint8 *buffer);
        static void OFFSET(uint8 *buffer);
        static void FACTOR(uint8 *buffer);
    };

    class DISPLAY
    {
    public:
        static void AUTOSEND(const uint8 *buffer);
        static void MAPPING(const uint8 *buffer);
        static void ACCUM(const uint8 *buffer);
        static void ACCUM_NUMBER(const uint8 *buffer);
        static void ACCUM_MODE(const uint8 *buffer);
        static void ACCUM_CLEAR(const uint8 *buffer);
        static void AVERAGE(const uint8 *buffer);
        static void AVERAGE_NUMBER(const uint8 *buffer);
        static void AVERAGE_MODE(const uint8 *buffer);
        static void MINMAX(const uint8 *buffer);
        static void FILTR(const uint8 *buffer);
        static void FPS(const uint8 *buffer);
        static void WINDOW(const uint8 *buffer);
        static void GRID(const uint8 *buffer);
        static void GRID_TYPE(const uint8 *buffer);
        static void GRID_BRIGHTNESS(const uint8 *buffer);
    };

    class TBASE
    {
    public:
        static void RANGE(uint8 *buffer);
        static void OFFSET(uint8 *buffer);
        static void SAMPLING(uint8 *buffer);
        static void PEACKDET(uint8 *buffer);
        static void TPOS(uint8 *buffer);
        static void SELFRECORDER(uint8 *buffer);
        static void FUNCTIMEDIV(uint8 *buffer);
    };

    class TRIGGER
    {
    public:
        static void MODE(uint8 *buffer);
        static void SOURCE(uint8 *buffer);
        static void POLARITY(uint8 *buffer);
        static void INPUT(uint8 *buffer);
        static void FIND(uint8 *buffer);
        static void OFFSET(uint8 *buffer);
    };
};


/** @}
 */
