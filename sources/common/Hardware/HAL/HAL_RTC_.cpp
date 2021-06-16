// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include <stm32f4xx_hal.h>


#define VALUE_FOR_RTC 0x644


#define RTC_CLOCK_SOURCE_LSE
// #define RTC_CLOCK_SOURCE_LSI


#ifdef RTC_CLOCK_SOURCE_LSI
#define RTC_ASYNCH_PREDIV 0x7f
#define RTC_SYNCH_PREDIV 0x0130
#endif


#ifdef RTC_CLOCK_SOURCE_LSE
#define RTC_ASYNCH_PREDIV 0x7f
#define RTC_SYNCH_PREDIV 0x00ff
#endif


static RTC_HandleTypeDef handleRTC =
{
    RTC,
    {
        RTC_HOURFORMAT_24,          // HourFormat
        RTC_ASYNCH_PREDIV,          // AsynchPrediv
        RTC_SYNCH_PREDIV,           // SynchPrediv
        RTC_OUTPUT_DISABLE,         // OutPut
        RTC_OUTPUT_POLARITY_HIGH,   // OutPutPolarity
        RTC_OUTPUT_TYPE_OPENDRAIN   // OutPutType
    }
};


void *HAL_RTC::handle = &handleRTC;


void HAL_RTC::Init()
{
    __HAL_RCC_RTC_ENABLE();

    RCC_OscInitTypeDef oscIS;
    RCC_PeriphCLKInitTypeDef periphClkIS;

    HAL_PWR_EnableBkUpAccess();

    oscIS.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
    oscIS.PLL.PLLState = RCC_PLL_NONE;
    oscIS.LSEState = RCC_LSE_ON;
    oscIS.LSIState = RCC_LSI_OFF;

    if (HAL_RCC_OscConfig(&oscIS) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    periphClkIS.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    periphClkIS.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if (HAL_RCCEx_PeriphCLKConfig(&periphClkIS) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    if (HAL_RTC_Init(reinterpret_cast<RTC_HandleTypeDef *>(handle)) != HAL_OK)
    {
        ERROR_HANDLER();
    }

//    if (HAL_RTCEx_BKUPRead(reinterpret_cast<RTC_HandleTypeDef *>(&handle), RTC_BKP_DR0) != VALUE_FOR_RTC)
//    {
//        if (SetTimeAndData(11, 11, 11, 11, 11, 11))
//        {
//            HAL_RTCEx_BKUPWrite(reinterpret_cast<RTC_HandleTypeDef *>(&handle), RTC_BKP_DR0, VALUE_FOR_RTC);
//        }
//    }
}


void HAL_RTC::Test()
{
    RTC_TimeTypeDef stimestructureget;

    HAL_RTC_GetTime(&handleRTC, &stimestructureget, RTC_FORMAT_BIN);

    LOG_WRITE("%d", stimestructureget.Seconds);
}


PackedTime HAL_RTC::GetPackedTime()
{
    PackedTime time;

    RTC_TimeTypeDef isTime;
    
    if (HAL_RTC_GetTime(reinterpret_cast<RTC_HandleTypeDef *>(&handle), &isTime, FORMAT_BIN) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    time.hours = isTime.Hours;
    time.minutes = isTime.Minutes;
    time.seconds = isTime.Seconds;

    RTC_DateTypeDef isDate;

    if (HAL_RTC_GetDate(reinterpret_cast<RTC_HandleTypeDef *>(&handle), &isDate, FORMAT_BIN) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    time.year = isDate.Year;
    time.month = isDate.Month;
    time.day = isDate.Date;

    return time;
}


bool HAL_RTC::SetTimeAndData(int8 day, int8 month, int8 year, int8 hours, int8 minutes, int8 seconds)
{
    RTC_DateTypeDef dateStruct =
    {
        RTC_WEEKDAY_MONDAY,
        (uint8)(month),
        (uint8)(day),
        (uint8)(year)
    };

    if (HAL_RTC_SetDate(reinterpret_cast<RTC_HandleTypeDef *>(&handle), &dateStruct, FORMAT_BIN) != HAL_OK)
    {
        ERROR_HANDLER();

        return false;
    };

    RTC_TimeTypeDef timeStruct =
    {
        (uint8)(hours),
        (uint8)(minutes),
        (uint8)(seconds),
        RTC_HOURFORMAT_24,
        RTC_DAYLIGHTSAVING_NONE,
        RTC_STOREOPERATION_SET
    };

    if (HAL_RTC_SetTime(reinterpret_cast<RTC_HandleTypeDef *>(&handle), &timeStruct, FORMAT_BIN) != HAL_OK)
    {
        ERROR_HANDLER();

        return false;
    };

    return true;
}
