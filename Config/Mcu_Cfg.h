/* =====================================================================================================================
 *  File        : Mcu_Cfg.h
 *  Layer		: MCAL
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Configure clock & system parameters for MCU layer (MCAL/Mcu)
 *  Notes       :
 * ===================================================================================================================*/

#ifndef MCU_CFG_H_
#define MCU_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include<stdint.h>

/* =====================================================================================================================
 *  Target clock and parameter.
 *
 * - HSE: 8MHz
 * - AHB(HCLK): 72MHz
 * - APB1(PCLK1): 36MHz
 * - APB2(PCLK2): 72MHz
 * - ADC: 12MHz
 * ===================================================================================================================*/
#define MCU_CFG_HSE_ENABLE				(1u) //1: use HSE as PLL source, 0: use HSI
#define MCU_CFG_HSE_FREQ_HZ				(8000000u) //Hz
#define MCU_CFG_HSE_FREQ_HZ             (8000000u)    /* Hz */

#define MCU_CFG_SYSCLK_FREQ_HZ			(72000000u) // Hz
#define MCU_CFG_AHB_FREQ_HZ				(72000000u) // Hz
#define MCU_CFG_APB1_FREQ_HZ			(36000000u) // Hz
#define MCU_CFG_APB2_FREQ_HZ			(72000000u) // Hz
#define MCU_CFG_ADC_FREQ_HZ				(12000000u) // Hz (PCLK2 /6)

/* =====================================================================================================================
 *  PLL & prescaler
 *
 * - SYSCLK = (HSE or HSI/2) *PLL_MUL
 * - AHB 	= SYSCLK/AHB_PRESC
 * - APB1	= AHB/APB1_PRESC
 * - APB2	= AHB/APB2_PRESC
 * - ADC	= APB2/ADC_PRESC
 * ===================================================================================================================*/
#define MCU_CFG_PLL_SOURCE_HSE			(1u) // 1: HSE, 0: HSI/2
#define MCU_CFG_PLL_MUL					(9u) // 8MHZ*9 = 72MHz

#define MCU_CFG_AHB_PRESC				(1u) // 1,2,4,8,16,64,128,256,512
#define MCU_CFG_APB1_PRESC				(1u) //1 or 2...16
#define MCU_CFG_APB2_PRESC				(1u)
#define MCU_CFG_ADC_PRESC				(6u)

/* =====================================================================================================================
 * SysTick & timebase
 * create 1ms for scheduler/timeout
 * ===================================================================================================================*/
#define MCU_CFG_SYSTICK_HZ				(1000u) // 1000Hz -> 1ms
#define MCU_CFG_SYSTICK_SOURCE_HCLK		(1u)	// 1: HCLK, 0: HCLK/8


/* =====================================================================================================================
 * Priority of interrupt
 * - Group: 2-bit preemption, 2-bit subpriority
 * - Priority: ICU higher UART log.
 * ===================================================================================================================*/
#define MCU_CFG_NVIC_PRIGROUP			(0x05u)	//NVIC_PriorityGroup_2

#define MCU_PRIO_TIM2_ICU_PREEMPT		(1u)
#define MCU_PRIO_TIM2_ICU_SUB			(0u)

#define MCU_PRIO_CAN_TX_PREEMPT			(2u)
#define MCU_PRIO_CAN_TX_SUB				(0u)
#define MCU_PRIO_CAN_RX0_PREEMPT		(2u)
#define MCU_PRIO_CAN_RX0_SUB			(1u)

#define	MCU_PRIO_USART1_PREEMPT			(3u)
#define	MCU_PRIO_USART1_SUB				(0u)

#define	MCU_PRIO_SYSTICK_PREEMPT		(3u)
#define	MCU_PRIO_SYCTICK_SUB			(1u)

/* =====================================================================================================================
 * Watchdog
 * - Enable IWDG for fail-safe
 * ===================================================================================================================*/
#define MCU_CFG_IWDG_ENABLE				(0u)	// 1: enable, 0: disable
#define MCU_CFG_IWDG_PRESC				(64u)	// Prescaler: 4...256
#define MCU_CFG_IWDFG_TIMEOUT			(1000u)	// Timeout (ms)

/* =====================================================================================================================
 * Check config compile-time
 * ===================================================================================================================*/
#if(MCU_CFG_SYSCLK_FREQ_HZ != (MCU_CFG_HSE_FREQ_HZ*MCU_CFG_PLL_MUL))
#if (MCU_CFG_PLL_SOURCE_HSE == 1u)
#error "SYSCLK != HSE * PLL_MUL — check MCU_CFG_PLL_MUL or HSE_FREQ_HZ"
#endif //MCU_CFG_PLL_SOURCE_HSE
#endif //MCU_CFG_SYSCLK_FREQ_HZ

#if(MCU_CFG_APB1_FREQ_HZ > 36000000u)
#error"APB1 clock > 36MHz - Invalid for STM32F1 (CAN on APB1)!"
#endif

#if (MCU_CFG_ADC_FREQ_HZ > 14000000u)
#error "ADC clock > 14MHz - exceeds datasheet recommendations!"
#endif

/* =====================================================================================================================
 * API choose the config
 * ===================================================================================================================*/
typedef enum
{
	MCU_CLOCK_PROFILE_DEFAULT =0u
} Mcu_ClockProfileType;

// Parameter for config

typedef struct
{
	uint32_t sysclk_hz;
	uint32_t hclk_hz;
	uint32_t pclk1_hz;
	uint32_t pclk2_hz;
	uint32_t adcclk_hz;
	uint32_t systick_hz;
}Mcu_ClockInfoType;

#ifdef __cplusplus
}
#endif


#endif /* MCU_CFG_H_ */
