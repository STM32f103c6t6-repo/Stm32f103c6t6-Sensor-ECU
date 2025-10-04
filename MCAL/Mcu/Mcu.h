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

#include <stdint.h>
#include <stdbool.h>
#include "Mcu_Cfg.h"
#include "Mcu_Types.h"

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

Std_ReturnType Mcu_Init(Mcu_ClockProfileType profile);

/**
 * @brief  init Clock fllow profile
 */
Std_ReturnType Mcu_InitClock(Mcu_ClockProfileType profile);

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
Std_ReturnType Mcu_SetNvicPriorityGrouping(uint32_t prigroup_value);

/**
 * @brief  set priority for some IRQ
 * @param  irqn           IRQ number
 * @param  preemptPrio    preemption level (0 = high)
 * @param  subPrio        subpriority lelvel
 * @return E_OK/E_NOT_OK
 */

Std_ReturnType Mcu_SetIrqPriority(uint32_t irqn, uint8_t PreemptPrio, uint8_t subPrio);

/**
 * @brief  Enable SysTick to create ticks at the desired frequency (e.g. 1000 Hz).
 * @param  tick_hz SysTick frequency, source follow Mcu_Cfg.h
 * @return E_OK/E_NOT_OK
 */
Std_ReturnType Mcu_Set_SysTickHZ(uint32_t tick_hz);

/**
 * @brief  delay ms follow SysTick.
 */
void Mcu_DelayMs(uint32_t ms);

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

#ifdef __cplusplus
}
#endif

#endif /* MCU_MCU_H_ */
