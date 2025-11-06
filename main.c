/* =====================================================================================================================
 *  File        : main.c
 *  Layer       :
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Entry point
 *  Depends     :
 * ===================================================================================================================*/

#include "Std_Types.h"
#include "EcuM.h"
#include "UartIf.h"
#include "Logger.h"

extern const EcuM_ConfigType EcuM_Config;

#ifndef LOG_TAG_SYSTEM
#define LOG_TAG_SYSTEM (0x00000001u)
#endif

int main(void)
{
	EcuM_Init(&EcuM_Config);

	LOG_INFO(LOG_TAG_SYSTEM, "=== Sensor ECU boot ok ===");
	LOG_DEBUG(LOG_TAG_SYSTEM, "UART log is ready.");
	LOG_WARN(LOG_TAG_SYSTEM, "this is demo warning.");

	for(;;)
	{
		UartIf_MainFunction();
	}
}

