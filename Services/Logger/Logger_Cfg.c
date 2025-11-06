/* =====================================================================================================================
 *  File        : Logger_Cfg.c
 *  Layer       : Service
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Config Logger
 *  Depends     :
 * ===================================================================================================================*/
#include "Logger.h"
#include "Logger_Cfg.h"
#include "UartIf.h"

extern Std_ReturnType UartIf_Write(const uint8* DataPtr, uint16 Length);
extern Std_ReturnType UartIf_WriteLine(const char* CStr);

#if (LOGGER_CFG_ENABLE_TIMESTAMP_MS == 1)
static uint32 My_GetMs(void){return 0u;}
#endif

const Logger_ConfigType Logger_Config = {
		.defaultLevel		= LOGGER_CFG_DEFAULT_LEVEL,
		.enableTagsMask		= 0xFFFFFFFFu,
		.outWrite			= UartIf_Write,
		.outWriteLine		= UartIf_WriteLine,
#if (LOGGER_CFG_ENABLE_TIMESTAMP_MS == 1)
		.getTimeMs			= My_GetMs
#endif
};

