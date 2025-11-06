/* =====================================================================================================================
 *  File        : Mcu.h
 *  Layer       : MCAL
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Declare API, data structure for MCAL/Mcu (clock/reset/NVIC/SysTick/IWDG)
 *  Notes       : - Read config from Sensor_ECU/Config/Mcu_Cfg.h
 * ===================================================================================================================*/

#ifndef MCU_MCU_H_
#define MCU_MCU_H_

#ifdef __cplusplus
extern "C" {
#endif

/* =========================================================
 *  Version & include
 * =======================================================*/
#define MCU_AR_MAJOR_VERSION		(1u)
#define MCU_AR_MINOR_VERSION		(0u)
#define MCU_AR_PATCH_VERSION		(0u)


#include "Mcu_Types.h"

/* =========================================================
 *  Service ID (for DET/Logger when trace error)
 * =======================================================*/
#define MCU_SID_INIT					(0x00u)
#define MCU_SID_INITCLOCK				(0x01u)
#define MCU_SID_DISTRIBUTE_PLLCLOCK		(0x02u)
#define MCU_SID_GET_PLLSTATUS			(0x03u)
#define MCU_SID_GET_CLOCKINFO			(0x04u)
#define MCU_SID_SETSYSTICKHZ			(0x05u)
#define MCU_SID_DELAYMS					(0x06u)
#define MCU_SID_PERFORMRESET			(0x07u)
#define MCU_SID_SETNVICGROUPING			(0x08u)
#define MCU_SID_SETIRQPRIORITY			(0x09u)
#define MCU_SID_ENABLEIWDG				(0x0Au)
#define MCU_SID_KICKIWDG				(0x0Bu)

/* =========================================================
 *  DET error
 * =======================================================*/
#define MCU_E_UNINIT					(0x01u)	// Call API when Mcu is not Init
#define MCU_E_PARAM_POINTER				(0x01u) // Null pointer
#define MCU_E_TIMEOUT					(0x03u) // timeout
#define MCU_E_PARAM_CONFIG				(0x04u)	// Config is error

/* =====================================================================================================================
 * Count ticks
 * ===================================================================================================================*/
extern volatile	uint32		s_systickTicks;

extern Mcu_ClockInfoType	s_clkInfo;

/* =========================================================
 *  From Mcu_Cfg.h:
 *  - Mcu_ClockProfileType:	choose profile clock.
 *  - Mcu_ClockInfoType: information frequency for print log.
 * =======================================================*/

/* =========================================================
 *  Main API (in sync with the sequence in EcuM)
 * =======================================================*/

/**
 * @brief  init MCU: clock, PLL, prescaler, FLASH wait states.
 * @param  profile  Config clock (MCU_CLOCK_PROFILE_DEFAULT…)
 * @return E_OK/E_NOT_OK
 */

Std_ReturnType Mcu_Init(Mcu_ConfigType *profile);

/**
 * @brief  get state of PLL
 */

Mcu_PllStatusType Mcu_GetPllStatus(void);

/**
 * @brief  trans system to clock PLL when clocked PLL
 * @return E_OK if success
 */
Std_ReturnType Mcu_DistributePllClock(void);

/**
 * @brief  Config NVIC priority group follow Mcu_Cfg.h
 * @return E_OK if success
 */
Std_ReturnType Mcu_SetNvicPriorityGrouping(uint32 prigroup_value);

/**
 * @brief  set priority for some IRQ
 * @param  irqn           IRQ number
 * @param  preemptPrio    preemption level (0 = high)
 * @param  subPrio        subpriority lelvel
 * @return E_OK/E_NOT_OK
 */

Std_ReturnType Mcu_SetIrqPriority(uint32 irqn, uint8 PreemptPrio, uint8 subPrio);

/**
 * @brief  Enable SysTick to create ticks at the desired frequency (e.g. 1000 Hz).
 * @param  tick_hz SysTick frequency, source follow Mcu_Cfg.h
 * @return E_OK/E_NOT_OK
 */
Std_ReturnType Mcu_Set_SysTickHZ(uint32 tick_hz);

/**
 * @brief  delay ms follow SysTick.
 */
void Mcu_DelayMs(uint32 ms);

/**
 * @brief  get information frequency.
 * @param  out  pointer struct get infomation;
 * @return  E_NOT_OK.
 */
Std_ReturnType Mcu_GetClockInfo(Mcu_ClockInfoType* out);

/**
 * @brief  Watchdog (IWDG reload)
 */
void Mcu_KickIwdg(void);

/**
 * @brief  system reset (NVIC_SystemReset)
 */
void MCu_PerformReset(void);

/* =========================================================
 *  Status query
 * =======================================================*/
Mcu_StatusType Mcu_GetStatus(void);

/* =========================================================
 *  Hooks/Callbacks
 * =======================================================*/
/* call when PLL Locked */
void MCu_PllLockedHook(void);

/* Call when timeout error init clock for write DET/Logger*/
void Mcu_ClockInitErrorHook(void);

/* =========================================================
 * 	 Global configuration
 * =======================================================*/
extern const Mcu_ConfigType Mcu_Config;

#ifdef __cplusplus
}
#endif

#endif /* MCU_MCU_H_ */
