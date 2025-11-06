/* =====================================================================================================================
 *  File        : EcuM.c
 *  Layer       : Services
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : write Api for init system
 *  Depends     :
 * ===================================================================================================================*/

#include "EcuM.h"
#include <string.h>

/* ==============================
 *          LOCAL STATE
 * ============================== */
static volatile EcuM_StateType	s_state = ECUM_STATE_UNINIT;
static const EcuM_ConfigType*	s_cfg = NULL_PTR;

/* ==============================
 *      INTERNAL UTILITIES
 * ============================== */
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
static inline Std_ReturnType ECUM_DET_FAIL(uint8 api, uint8 err)
{
	ECUM_DET_REPORT(api,err);
	return E_NOT_OK;
}
#else
static inline Std_ReturnType ECUM_DET_FAIL(uint8 api, uint8 err)
{
	(void)api; (void)err;
	return E_NOT_OK;
}
#endif

static inline Std_ReturnType call_init_hook(EcuM_InitHookType hook)
{
	if(hook == NULL_PTR) return E_OK;
	return hook();
}

static inline void call_void_hook(EcuM_VoidHookType hook)
{
	if(hook != NULL_PTR){ hook();}
}

/* ==============================
 *            APIS
 * ============================== */
void EcuM_Init(const EcuM_ConfigType* CfgPtr)
{
	if(CfgPtr == NULL_PTR || CfgPtr->Hooks == NULL_PTR){
		(void)ECUM_DET_FAIL(ECUM_API_ID_INIT, ECUM_E_PARAM_POINTER);
		return;
	}
	if(s_state != ECUM_STATE_UNINIT){
		(void)ECUM_DET_FAIL(ECUM_API_ID_INIT, ECUM_E_ALREADY_INIT);
		return ;
	}

	s_cfg = CfgPtr;

	//MCU
	s_state = ECUM_STATE_STARTUP_ONE;
	if(call_init_hook(s_cfg->Hooks->Mcu_InitHook) != E_OK) {
		s_state = ECUM_STATE_SHUTDOWN;
		return ;
	}

	// Port & Comm base
	s_state = ECUM_STATE_STARTUP_TWO;
	if(call_init_hook(s_cfg->Hooks->Port_InitHook) != E_OK) {s_state = ECUM_STATE_SHUTDOWN; return;}
	if(call_init_hook(s_cfg->Hooks->Uart_InitHook) != E_OK) {s_state = ECUM_STATE_SHUTDOWN; return;}
	if(call_init_hook(s_cfg->Hooks->UartIf_InitHook) != E_OK) {s_state = ECUM_STATE_SHUTDOWN; return;}

	// Logger & Det
	if(call_init_hook(s_cfg->Hooks->Logger_InitHook) != E_OK){ s_state = ECUM_STATE_SHUTDOWN;return;}
	if(call_init_hook(s_cfg->Hooks->Det_InitHook) != E_OK){ s_state = ECUM_STATE_SHUTDOWN;return;}

	// Optional drivers
	if(call_init_hook(s_cfg->Hooks->Gpt_InitHook) != E_OK){ s_state = ECUM_STATE_SHUTDOWN;return;}
	if(call_init_hook(s_cfg->Hooks->Icu_InitHook) != E_OK){ s_state = ECUM_STATE_SHUTDOWN;return;}
	if(call_init_hook(s_cfg->Hooks->Can_InitHook) != E_OK){ s_state = ECUM_STATE_SHUTDOWN;return;}
	if(call_init_hook(s_cfg->Hooks->Com_InitHook) != E_OK){ s_state = ECUM_STATE_SHUTDOWN;return;}

	// Application
	if(call_init_hook(s_cfg->Hooks->App_InitHook) != E_OK){ s_state = ECUM_STATE_SHUTDOWN;return;}

	// Done
	s_state = ECUM_STATE_RUN;
}

void EcuM_DeInit(void)
{
	if(s_state == ECUM_STATE_UNINIT){
		(void)ECUM_DET_FAIL(ECUM_API_ID_DEINIT, ECUM_E_UNINIT);
		return;
	}

	// Deinit
	if (s_cfg != NULL_PTR && s_cfg->Hooks != NULL_PTR){
		// App
		call_void_hook(s_cfg->Hooks->App_DeInitHook);

		// Stack/comm
		call_void_hook(s_cfg->Hooks->Com_DeInitHook);
		call_void_hook(s_cfg->Hooks->Can_DeInitHook);

		// Timmer/Input capture
		call_void_hook(s_cfg->Hooks->Icu_DeInitHook);
		call_void_hook(s_cfg->Hooks->Gpt_DeInitHook);

		// Services
		call_void_hook(s_cfg->Hooks->Det_DeInitHook);
		call_void_hook(s_cfg->Hooks->Logger_DeInitHook);

		// Comm base
		call_void_hook(s_cfg->Hooks->UartIf_DeInitHook);
		call_void_hook(s_cfg->Hooks->Uart_DeInitHook);

		// Io/Mcu
		call_void_hook(s_cfg->Hooks->Port_DeInitHook);
		call_void_hook(s_cfg->Hooks->Mcu_DeInitHook);
	}

	s_state = ECUM_STATE_SHUTDOWN;
}

EcuM_StateType EcuM_GetState(void)
{
	return s_state;
}

Std_ReturnType EcuM_GoToSleep(void)
{
	if(s_state != ECUM_STATE_RUN) {
		return ECUM_DET_FAIL(ECUM_API_ID_GOTOSLEEP, ECUM_E_INVALID_STATE);
	}

	if(s_cfg != NULL_PTR && s_cfg->Hooks != NULL_PTR){
		call_void_hook(s_cfg->Hooks->PreSleepHook);
	}
	s_state = ECUM_STATE_SLEEP;
	return E_OK;
}

Std_ReturnType EcuM_Wakeup(void){
	if(s_state != ECUM_STATE_SLEEP){
		return ECUM_DET_FAIL(ECUM_API_ID_WAKEUP, ECUM_E_INVALID_STATE);
	}

	if(s_cfg != NULL_PTR && s_cfg->Hooks != NULL_PTR){
		call_void_hook(s_cfg->Hooks->PostWakeupHook);
	}

	s_state = ECUM_STATE_RUN;
	return E_OK;
}
