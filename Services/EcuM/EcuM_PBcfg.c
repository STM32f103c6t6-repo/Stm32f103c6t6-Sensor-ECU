/* =====================================================================================================================
 *  File        : EcuM_PBcfg.c
 *  Layer       :
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Config for Ecu
 *  Depends     :
 * ===================================================================================================================*/

#include "Std_Types.h"
#include "EcuM.h"

#include "Mcu.h"
#include "Uart.h"
#include "Port.h"
#include "../../ECU_Abstraction/UartIf/UartIf.h"
#include "../Logger/Logger.h"
#include "Det.h"

extern const Mcu_ConfigType Mcu_Config;
extern const Port_ConfigType Port_Config;
extern const Uart_ConfigType Uart_Config;
extern const UartIf_ConfigType UartIf_Config;
extern const Logger_ConfigType Logger_Config;

// Config module init
static Std_ReturnType Mcu_Init_Hook(void)
{
	Mcu_Init(&Mcu_Config);
	return E_OK;
}

static Std_ReturnType Port_Init_Hook(void)
{
	Port_Init(&Port_Config);
	return E_OK;
}

static Std_ReturnType Uart_Init_Hook(void)
{
	Uart_Init(&Uart_Config);
	return E_OK;
}

static Std_ReturnType UartIf_Init_Hook(void)
{
	UartIf_Init(&UartIf_Config);
	return E_OK;
}

static Std_ReturnType Logger_Init_Hook(void)
{
	Logger_Init(&Logger_Config);
	return E_OK;
}

static Std_ReturnType Det_Init_Hook(void)
{
	Det_Init();
	return E_OK;
}

// Config deinit
static void Logger_DeInit_Hook(void)	{ Logger_Deinit(); }
static void UartIf_DeInit_Hook(void)	{ UartIf_DeInit(); }
static void Uart_DeInit_Hook(void)		{ Uart_Deinit(); }

static const EcuM_HooksType EcuM_Hooks = {
	// Startup One
	.Mcu_InitHook 		= Mcu_Init_Hook,

	// Startup Two
	.Port_InitHook 		= Port_Init_Hook,
	.Uart_InitHook 		= Uart_Init_Hook,
	.UartIf_InitHook 	= UartIf_Init_Hook,

	// Services
	.Logger_InitHook 	= Logger_Init_Hook,
	.Det_InitHook 		= Det_Init_Hook,

	// Optional driver
	.Gpt_InitHook		= NULL,
	.Icu_InitHook		= NULL,
	.Can_InitHook		= NULL,
	.Com_InitHook		= NULL,

	//App
	.App_InitHook		= NULL,

	//Deinit
	.App_DeInitHook		= NULL,
	.Com_DeInitHook		= NULL,
	.Can_DeInitHook		= NULL,
	.Icu_DeInitHook		= NULL,
	.Gpt_DeInitHook		= NULL,
	.Det_DeInitHook		= NULL,
	.Logger_DeInitHook	= Logger_DeInit_Hook,
	.UartIf_DeInitHook	= UartIf_DeInit_Hook,
	.Uart_DeInitHook	= Uart_DeInit_Hook,
	.Port_DeInitHook	= NULL,
	.Mcu_DeInitHook		= NULL,

	// Sleep/Wakeup hooks
	.PreSleepHook		= NULL,
	.PostWakeupHook		= NULL,
};

const EcuM_ConfigType EcuM_Config = {
		.Hooks			= &EcuM_Hooks,
		.Reserved		= 0u
};

