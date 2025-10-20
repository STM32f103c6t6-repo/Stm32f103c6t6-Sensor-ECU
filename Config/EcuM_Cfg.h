/* =====================================================================================================================
 *  File        : EcuM_Cfg.h
 *  Layer       : Services (Config)
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Config EcuM (ECU State Manager) – Defines module initialization order, run mode, timeout, global options
 *  Notes       : - Header config for Services/EcuM/EcuM.c
 * ===================================================================================================================*/
#ifndef ECUM_CFG_H_
#define ECUM_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Std_Types.h"

/* =========================================================
 * Version Config
 * =======================================================*/
#define ECUM_CFG_AR_MAJOR_VERSION		(1u)
#define ECUM_CFG_AR_MINOR_VERSION		(0u)
#define ECUM_CFG_AR_PATCH_VERSION		(0u)

/* =========================================================
 * Global Options
 * - Enable/disable auxiliary services and default behavior
 * =======================================================*/
#define ECUM_CFG_ENABLE_DET					(1u)	//1: enable Default Error Tracer
#define ECUM_CFG_ENABLE_LOGGER				(1u)	//1: enable logger (UART) in EcuM
#define ECUM_CFG_ENABLE_WATCHDOG			(0u)	//1: ECU wiil enable IWDG
#define ECUM_CFG_FAILSAFE_STOP_ON_COM_LOSS	(1u)	//1:enter safe state if COM is lost according to COM_Spec policy

/*When build Release, we can put value to 0 */
#ifndef ECUM_CFG_BOOT_LOG_LEVEL
#define ECUM_CFG_BOOT_LOG_LEVEL				(3u) /* 0...5 reference LogLevel_t in Loglevels.h*/
#endif

/* =========================================================
 * Run mode/startup target
 * =======================================================*/
typedef enum
{
	ECUM_RUNMODE_DEFAULT	= 0u, //mode run default
	ECUM_RUNMODE_SAFE,
	ECUM_RUNMODE_COUNT
}EcuM_RunModeType;

/* =========================================================
 * Declare module IDs & initialization order
 * - EcuM will init follow this order
 * - DeInit in reverse
 * =======================================================*/
typedef enum
{
	/* MCAL */
	ECUM_MOD_MCU = 0,	//Clock,SysTick, NVic group
	ECUM_MOD_PORT,		//Pin Mux/AFIO remap
	ECUM_MOD_DIO,		// I/O driver
	ECUM_MOD_GPT,		//Timebase for ICU/Timeout
	ECUM_MOD_ICU,		//Capture Echo HC-SR04
	ECUM_MOD_UART,		//UART log/debug
	ECUM_MOD_CAN,		//bxCAN

	/*ECU Abstraction*/
	ECUM_MOD_UARTIF,	//Uart interface
	ECUM_MOD_CANIF,		//Can interface

	/*Services*/
	ECUM_MOD_DET,		//Default Error Tracer
	ECUM_MOD_LOGGER,	//Logger service
	ECUM_MOD_PDUR,		//PDU Router
	ECUM_MOD_COM,		//Com

	/*RTE*/
	ECUM_MOD_RTE,		//RTE writing

	/*Application*/
	ECUM_MOD_APP,		// SWCs start

	ECUM_MOD_COUNT
} EcuM_ModuleId;


/* Board order init */
extern const EcuM_ModuleId EcuM_StartUpOrder[];
extern const uint32	EcuM_StartUpOrder_Count;

/* Board order deinit */
extern const EcuM_ModuleId EcuM_ShutDownOrder[];
extern const uint32	EcuM_ShutDownOrder_Count;

/* =========================================================
 * Timeout and startup monitoring
 * - ms units
 * =======================================================*/
#define ECUM_CFG_TIMEOUT_MCU_INIT_MS		(50u)
#define ECUM_CFG_TIMEOUT_PORT_INIT_MS		(10u)
#define ECUM_CFG_TIMEOUT_UART_INIT_MS		(20u)
#define ECUM_CFG_TIMEOUT_CAN_INIT_MS		(50u)
#define ECUM_CFG_TIMEOUT_COM_STACK_MS		(100u)
#define ECUM_CFG_TIMEOUT_RTE_MS				(20u)
#define ECUM_CFG_TIMEOUT_APP_MS				(50u)

// Sum of time for entire ECU Startup
#define ECUM_CFG_TIMEOUT_STARTUP_BUDGET_MS	(500u)

/* =========================================================
 * Wake up sources/ shutdown targets
 * =======================================================*/
typedef enum
{
	ECUM_SHUTDOWN_TARGET_RESET = 0u,			//System reset (NVIC_SystemReset).
	ECUM_SHUTDOWN_TARGET_OFF,				//Power off
	ECUM_SHUTDOWN_TARGET_SLEEP				//Enter Stop/Standby
}EcuM_ShutdownTargetType;

typedef uint32 EcuM_WakeupSourceType;
#define ECU_WAKEUP_NONE			((EcuM_WakeupSourceType)0u)
#define ECU_WAKEUP_CAN_TX		((EcuM_WakeupSourceType)(1u << 0))
#define ECU_WAKEUP_TIMER		((EcuM_WakeupSourceType)(1u << 1))
#define ECU_WAKEUP_EXTI			((EcuM_WakeupSourceType)(1u << 2))

/* =========================================================
 * Compile-time hints
 * =======================================================*/
#if (ECUM_CFG_ENABLE_LOGGER == 1u)
#ifndef UART_CFG_LOG_PORT
#error "EcuM/Logger request UART_CFG_LOG_PORT in Uart_Cfg.h"
#endif
#endif

/*CAN stack define order: Mcu->Port->CAN->Canif->Pdur->Com*/
#if defined(ECUM_ENFORCE_CAN_DEP) && (ECUM_ENFORCE_CAN_DEP == 1u)

#endif

/* =========================================================
 * Parameter runtime
 * - EcuM.c will write this information
 * =======================================================*/
typedef struct
{
	EcuM_RunModeType 		runModeSelected;
	uint32				startupBudgetMs;
	uint8					detEnable;
	uint8					loggerEnable;
	uint8					watchdogEnable;
} EcuM_InfoType;

/* =========================================================
 * Config API (EcuM.c will call)
 * =======================================================*/
typedef struct
{
	EcuM_RunModeType			defaultRunMode;
	EcuM_ShutdownTargetType		defaultShutdownTarget;
	EcuM_WakeupSourceType		defaultWakeupSources;

	/*order init/deinit*/
	const EcuM_ModuleId*		startOrder;
	uint32					startOrderCount;
	const EcuM_ModuleId*		stopOrder;
	uint32					stopOrderCount;

	/* Timeout budgets */
	uint32 					timeoutMcuInitMs;
	uint32 					timeoutPortInitMs;
	uint32 					timeoutUartInitMs;
	uint32 					timeoutCanInitMs;
	uint32 					timeoutComStackInitMs;
	uint32 					timeoutRteMs;
	uint32 					timeoutAppMs;
	uint32 					timeoutStartupBudgetMs;

	/*Options*/
	uint8						enableDet;
	uint8						enableLogger;
	uint8						enableWatchdog;
	uint8						failsafeStopOnComLoss;

} EcuM_ConfigType;

/*config it in EcuM_Cfg.c*/
extern const EcuM_ConfigType EcuM_Config;

#ifdef __cplusplus
}
#endif

#endif /* ECUM_CFG_H_ */
