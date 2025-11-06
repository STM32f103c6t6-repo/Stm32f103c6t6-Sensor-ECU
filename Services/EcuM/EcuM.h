/* =====================================================================================================================
 *  File        : EcuM - ECU State Manager
 *  Layer       : Services
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Coordinate the system initialization / shutdown sequence according to module order
 * 					- Provide ECU status (STARTUP/RUN/SLEEP/SHUTDOWN)
 * 					- Call hooks/init functions of each layer: Mcu -> Port -> Uart -> UartIf -> Logger -> Det -> (other drivers)
 *  Depends     :
 * ===================================================================================================================*/


#ifndef ECUM_ECUM_H_
#define ECUM_ECUM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Std_Types.h"
#include "ComStack_Types.h"
#include "Det.h"

/* ==============================
 *       VERSION & IDENTITIES
 * ============================== */
#define ECUM_VENDOR_ID						(0x00u)
#define ECUM_MODULE_ID						(0x10u)
#define ECUM_INSTANCE_ID					(0x00u)

#define ECUM_AR_RELEASE_MAJOR_VERSION		(4u)
#define ECUM_AR_RELEASE_MINOR_VERSION		(3u)
#define ECUM_AR_RELEASE_PATCH_VERSION		(1u)

#define ECUM_SW_MAJOR_VERSION				(1u)
#define ECUM_SW_MINOR_VERSION				(0u)
#define ECUM_SW_PATCH_VERSION				(0u)

/* ==============================
 *       BUILD-TIME SWITCHES
 * ============================== */
#ifndef ECUM_DEV_ERROR_DETECT
#define ECUM_DEV_ERROR_DETECT				STD_ON
#endif

#ifndef ECUM_ENABLE_MAINFUNCTION
#define ECUM_ENABLE_MAINFUNCTION			(1u)
#endif

/* ==============================
 *       API IDs
 *      (report DET)
 * ============================== */
#define ECUM_API_ID_INIT					(0x01u)
#define ECUM_API_ID_DEINIT					(0x02u)
#define ECUM_API_ID_GETSTATE				(0x03u)
#define ECUM_API_ID_MAINFUNCTION			(0x04u)
#define ECUM_API_ID_GOTOSLEEP				(0x05u)
#define ECUM_API_ID_WAKEUP					(0x06u)

/* ==============================
 *         DET ERROR CODES
 * ============================== */
#define ECUM_E_PARAM_POINTER				 (0x01u)
#define ECUM_E_UNINIT						 (0x02u)
#define ECUM_E_ALREADY_INIT					 (0x03u)
#define ECUM_E_INVALID_STATE				 (0x04u)

/* ==============================
 *      DET REPORT MACRO
 * ============================== */
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
#define ECUM_DET_REPORT(_api, _err)\
	Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, (_api), (_err))
#else
#define ECUM_DET_REPORT(_api,_err) ((void)0)
#endif

/* ==============================
 *            TYPES
 * ============================== */
typedef enum
{
	ECUM_STATE_UNINIT =0u,
	ECUM_STATE_STARTUP_ONE,
	ECUM_STATE_STARTUP_TWO,
	ECUM_STATE_RUN,
	ECUM_STATE_SLEEP,
	ECUM_STATE_SHUTDOWN
} EcuM_StateType;

typedef Std_ReturnType (*EcuM_InitHookType)(void);
typedef void (*EcuM_VoidHookType)(void);

// Init hook group
typedef struct
{

	EcuM_InitHookType		Mcu_InitHook;			//Start One: platform
	EcuM_InitHookType		Port_InitHook;
	EcuM_InitHookType		Uart_InitHook;
	EcuM_InitHookType		UartIf_InitHook;
	EcuM_InitHookType		Logger_InitHook;
	EcuM_InitHookType		Det_InitHook;

	EcuM_InitHookType		Gpt_InitHook;
	EcuM_InitHookType		Icu_InitHook;
	EcuM_InitHookType		Can_InitHook;
	EcuM_InitHookType		Com_InitHook;

	EcuM_InitHookType		App_InitHook;

	EcuM_VoidHookType		App_DeInitHook;
	EcuM_VoidHookType		Com_DeInitHook;
	EcuM_VoidHookType		Can_DeInitHook;
	EcuM_VoidHookType		Icu_DeInitHook;
	EcuM_VoidHookType		Gpt_DeInitHook;
	EcuM_VoidHookType		Det_DeInitHook;
	EcuM_VoidHookType		Logger_DeInitHook;
	EcuM_VoidHookType		UartIf_DeInitHook;
	EcuM_VoidHookType		Uart_DeInitHook;
	EcuM_VoidHookType		Port_DeInitHook;
	EcuM_VoidHookType		Mcu_DeInitHook;

	EcuM_VoidHookType		PreSleepHook;
	EcuM_VoidHookType		PostWakeupHook;
} EcuM_HooksType;

/*
 *  EcuM master configuration: hook groups and optional flags
 */
typedef struct
{
	const EcuM_HooksType*	Hooks;
	uint8					Reserved;
} EcuM_ConfigType;

/* ==============================
 *             API
 * ============================== */
/*
 * EcuM_init
 *	Port_InitHook, UartInitHook, UartIf_InitHook
 *	Services:	Logger_InitHook, Det_InitHook
 *	Other:		GPT/Icu/Can/Com
 *	App:		App_InitHook
 *	if hook == NULL => Skip
 */
void EcuM_Init(const EcuM_ConfigType* CfgPtr);
void EcuM_DeInit(void);

EcuM_StateType EcuM_GetState(void); // Get ECU State

Std_ReturnType EcuM_GoToSleep(void); // Get system to Sleep

Std_ReturnType EcuM_Wakeup(void); // Wake up System

/* =========================================================
 * 	 Global configuration
 * =======================================================*/
extern const EcuM_ConfigType EcuM_Config;

#ifdef __cplusplus
}
#endif

#endif /* ECUM_ECUM_H_ */
