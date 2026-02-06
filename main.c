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
#include "SystemApp.h"

extern const EcuM_ConfigType EcuM_Config;

#ifndef LOG_TAG_SYSTEM
#define LOG_TAG_SYSTEM (0x00000001u)
#endif

int main(void)
{
	// Init sequence
	EcuM_Init(&EcuM_Config);


	while(1)
	{
		SystemApp_MainFunction();
	}

	return 0;
}

