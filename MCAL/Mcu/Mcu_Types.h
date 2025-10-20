/* =====================================================================================================================
 *  File        : Mcu_Types.h
 *  Layer       : MCAL
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Define data structure/enum/struct dùng bởi MCAL/Mcu (clock/reset/NVIC/SysTick/IWDG)
 *  Notes       : - Contains no register access; just shared type definitions.
 * ===================================================================================================================*/
#ifndef MCU_MCU_TYPES_H_
#define MCU_MCU_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/* =====================================================================================================================
 * Version & include
 * ===================================================================================================================*/
#define MCU_TYPE_AR_MAJOR_VERSION		(1u)
#define MCU_TYPE_AR_MINOR_VERSION		(0u)
#define MCU_TYPE_AR_PATCH_VERSION		(0u)

#include "Std_Types.h"

#ifndef E_OK
typedef uint8 Std_ReturnType;
#define E_OK		((Std_ReturnType)0x00u)
#define E_NOT_OK	((Std_ReturnType)0x01u)
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
	uint32 sysclk_hz;
	uint32 hclk_hz;
	uint32 pclk1_hz;
	uint32 pclk2_hz;
	uint32 adcclk_hz;
	uint32 systick_hz;
}Mcu_ClockInfoType;

/* =====================================================================================================================
 * Clock & choose PLL
 * ===================================================================================================================*/
typedef enum
{
	MCU_CLOCK_SRC_HSE = 0u,			// HSE into PLL
	MCU_CLOCK_SRC_HSI_DIV2			// HSI/2 into PLL
}Mcu_ClockSourceType;

/* PLL state */
typedef enum
{
	MCU_PLL_UNLOCKED	= 0u,
	MCU_PLL_LOCKED,
	MCU_PLL_STATUS_UNDEFINED
}Mcu_PllStatusType;

/*prameter to describe clock*/
typedef struct
{
	uint32 sysclk_hz;		//SYSCLK after PLL
	uint32 hclk_hz;		//AHB(HCLK)
	uint32 pclk1_hz;		//APB1
	uint32 pclk2_hz;		//APB2
	uint32 adcclk_hz;		//Adc clock
} Mcu_ClockRawType;

/* =====================================================================================================================
 * Flash latency / wait state (reference RM0008)
 * ===================================================================================================================*/
typedef enum
{
	MCU_FLASH_LATENCY_0WS = 0u, // <= 24 MHz
	MCU_FLASH_LATENCY_1WS = 1u, // <= 48 MHz
	MCU_FLASH_LATENCY_2WS = 2u, // <= 72 MHz
}Mcu_FlashLatencyType;

/* =====================================================================================================================
 * Prescaler & PLL parameter
 * ===================================================================================================================*/
typedef enum
{
	MCU_AHB_DIV_1			= 1u,
	MCU_AHB_DIV_2			= 2u,
	MCU_AHB_DIV_4			= 4u,
	MCU_AHB_DIV_8			= 8u,
	MCU_AHB_DIV_16			= 16u,
	MCU_AHB_DIV_64			= 64u,
	MCU_AHB_DIV_128			= 128u,
	MCU_AHB_DIV_256			= 256u,
	MCU_AHB_DIV_512			= 512u,
}Mcu_AhbPrescalerType;

typedef enum
{
	MCU_APB_DIV_1			= 1u,
	MCU_APB_DIV_2			= 2u,
	MCU_APB_DIV_4			= 4u,
	MCU_APB_DIV_8			= 8u,
	MCU_APB_DIV_16			= 16u
}Mcu_ApbPrescalerType;

/*PLL multiplier*/
typedef enum
{
	MCU_PLL_MUL_2 = 2u,
	MCU_PLL_MUL_3 = 3u,
	MCU_PLL_MUL_4 = 4u,
	MCU_PLL_MUL_5 = 5u,
	MCU_PLL_MUL_6 = 6u,
	MCU_PLL_MUL_7 = 7u,
	MCU_PLL_MUL_8 = 8u,
	MCU_PLL_MUL_9 = 9u,
	MCU_PLL_MUL_10 = 10u,
	MCU_PLL_MUL_11 = 11u,
	MCU_PLL_MUL_12 = 12u,
	MCU_PLL_MUL_13 = 13u,
	MCU_PLL_MUL_14 = 14u,
	MCU_PLL_MUL_15 = 15u,
	MCU_PLL_MUL_16 = 16u,
} Mcu_PllMulType;

/* =====================================================================================================================
 * SysTick & NVIC
 * ===================================================================================================================*/
/* SysTick : HCLK or HCLK/8 (follow hardware cortex-m3 */
typedef enum
{
	MCU_SYSTICK_CLKSRC_HCLK_DIV8 =0u,
	MCU_SYSTICK_CLKSRC_HCLK
} Mcu_SysTickClksrcType;

typedef enum
{
	MCU_NVIC_PRIGROUP_7_1	= 0x03u,	// 7 bits preempt, 1bit sub.
	MCU_NVIC_PRIGROUP_6_2	= 0x04u,
	MCU_NVIC_PRIGROUP_5_3	= 0x05u,	// 5 preempts, 3 sub
	MCU_NVIC_PRIGROUP_4_4	= 0x06u,
	MCU_NVIC_PRIGROUP_3_5	= 0x07u,
} Mcu_NvicPrigroupType;

typedef struct
{
	uint8 preemptPrio;
	uint8 subPrio;
}Mcu_IrqPriorityType;

/* =====================================================================================================================
 * Reset source
 * ===================================================================================================================*/
typedef enum
{
	MCU_RESET_UNDEFINED		= 0u,
	MCU_RESET_POWER_ON,				//POR/PDR
	MCU_RESET_PIN,					//NRST
	MCU_RESET_SOFTWARE,				//SW
	MCU_RESET_IWDG,					//Independent WDG
	MCU_RESET_WWDG,					//Window WDG
	MCU_RESET_LOW_POWER				//Low-power reset
} Mcu_ResetReasonType;

/*bit mask reset flag of RCC_CSR for Mcu.c to decode fast*/
typedef struct
{
	uint8 por;	//Power-on/PDR
	uint8 pin;	//NRST pin
	uint8 sw;		//Software
	uint8 iwdg;	//independent watchdog
	uint8 wwdg;	//window watchdog
	uint8 lpw;	//low-power reset
} Mcu_ResetFlagsType;

/* Decode result reset for logger in*/
typedef struct
{
	Mcu_ResetReasonType	reason;
	Mcu_ResetFlagsType	rawFlags;
} Mcu_ResetInfoType;

/* =====================================================================================================================
 * Driver state
 * ===================================================================================================================*/
typedef enum
{
	MCU_UNINIT	= 0,
	MCU_INIT
} Mcu_StatusType;

/* working state*/
typedef struct
{
	Mcu_StatusType 		init;			// check init
	Mcu_PllStatusType 	pll;			// pll state
	Mcu_ResetInfoType	resetInfo;
	Mcu_ClockRawType	clocks;			// frequence
	Mcu_ClockInfoType	exportedInfo;
} Mcu_StateSnapshotType;

/* =====================================================================================================================
 * Config IWDG
 * ===================================================================================================================*/
typedef struct
{
	uint8		enable;			// 1: enable IWDG
	uint16	prescaler;		// 4...256
	uint16	timeoutMs;		// Timeout Target
} Mcu_IwdgConfigType;

/* =====================================================================================================================
 * Overall configuration
 * ===================================================================================================================*/
typedef struct
{
	/* source & pll*/
	Mcu_ClockSourceType		pllSource;
	Mcu_PllMulType			pllMul;

	/* prescaler bus*/
	Mcu_AhbPrescalerType	ahbPresc;
	Mcu_AhbPrescalerType	apb1Presc;
	Mcu_AhbPrescalerType	apb2Presc;

	/* ADC prescaler */
	uint8					adcPresc;

	/* Flash Latency with SYSCLK*/
	Mcu_FlashLatencyType	flashLatency;

	/* SysTick */
	Mcu_SysTickClksrcType	systickSrc;
	uint32				systickHz;

	/* NVIC default group */
	Mcu_NvicPrigroupType	nvicPrigroup;

	/* IWDG */
	Mcu_IwdgConfigType		iwdg;
} Mcu_Configtype;

#ifdef __cplusplus
}
#endif

#endif /* MCU_MCU_TYPES_H_ */
