/* =====================================================================================================================
 *  File        : Mcu.c
 *  Layer       : MCAL
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Aply driver MCU: clock/reset/NVIC/SysTick/IWDG
 *  Depends     : - Sensor_ECU/Config/Mcu_Cfg.h
 *                - AutosarShared/include/mcal/stm32f103xx_regs.h (RCC/GPIO/AFIO/USART/CAN base)
 *  Notes       :
 * ===================================================================================================================*/

#include <stdint.h>
#include <stdbool.h>
#include "Mcu.h"
#include "Mcu_Types.h"
#include "stm32f103xx_regs.h"

/*SYST_CSR bits*/
#define SYST_CSR_ENABLE_Pos			0U
#define SYST_CSR_TICKINT_Pos		0U
#define SYST_CSR_CLKSOURCE_Pos		0U

/* SCB AIRRCR (priority grouping & system reset )*/
#define SCB_AIRC_VECTKEY_Pos		16U
#define SCB_AIRC_VECTKEY			(0x5FAUL << SCB_AIRC_VECTKEY_Pos)
#define SCB_AIRC_PRIGROUP_Pos		8U
#define SCB_AIRC_SYSRESETREQ_Pos	2U

/* IWDG registers */
#define IWDG_KR_KEY_ENABLE			(0xCCCCUL)
#define IWDG_KR_KEY_RELOAD			(0xAAAAUL)
#define IWDG_KR_KEY_WRITE			(0x5555UL)

/* Static state */
static __vo	uint32_t		s_systickTicks 	= 0;
static Mcu_StatusType		s_mcuStatus		= MCU_UNINIT;
static Mcu_PllStatusType	s_pllStatus		= MCU_PLL_STATUS_UNDEFINED;
static Mcu_ClockInfoType	s_clkInfo		= {0};

static void prv_FlashSetLatencyAndPreFetch(uint32_t sysclk_hz);
static uint32_t prv_EncodeAhbPrescaler(uint32_t div);
static uint32_t prv_EncodeApbPrescaler(uint32_t div);
static uint32_t prv_EncodeAdcPrescaler(uint32_t div);
static uint32_t prv_EncodePllMul(uint32_t mul);

/* Timeout (avoid hang)*/
static inline bool prv_WaitFlagSet(__vo const uint32_t* reg, uint32_t mask, uint32_t loops)
{
	while(((*reg) & mask) == 0)
	{
		if(loops-- == 0U) return false;
	}
	return true;
}

Std_ReturnType Mcu_Init(Mcu_ClockProfileType profile)
{
	(void) profile;

	/* Enable HSE*/
#if (MCU_CFG_PLL_SOURCE_HSE == 1u)
	/* Enable HSE & AFIO clock */
	RCC -> CR |= (1UL <<16); //HSEon
	if(!prv_WaitFlagSet(&RCC->CR, (1UL<<17), 1000000U)) //HSERDY
	{
		s_pllStatus	= MCU_PLL_STATUS_UNDEFINED;
		Mcu_ClockInitErrorHook();
		return E_NOT_OK;
	}
#else
	/*confirm HSI enable*/
	RCC -> CR |= (1UL <<0); //HSIon
	if(!prv_WaitFlagSet(&RCC->CR, (1UL<<1), 1000000U)) //HSERDY
	{
		s_pllStatus	= MCU_PLL_STATUS_UNDEFINED;
		Mcu_ClockInitErrorHook();
		return E_NOT_OK;
	}
#endif
	/* config Flash latency & Prefetch follow target SYSCLK */
	prv_FlashSetLatencyAndPreFetch(MCU_CFG_SYSCLK_FREQ_HZ);

	/* config prescaler bus */
	uint32_t cfgr = RCC->CFGR;

	/* AHB prescaler (HPRE) - bits 7:4 */
	cfgr &= ~(0xFUL << 4);
	cfgr |= (prv_EncodeAhbPrescaler(MCU_CFG_AHB_PRESC) << 4);

	/* APB1 prescaler (PPRE1) - bits 10:8 */
	cfgr &= ~(0x7UL << 8);
	cfgr |= (prv_EncodeApbPrescaler(MCU_CFG_APB1_PRESC) << 8);

	/* APB2 prescaler (PPRE2) - bits 13:11 */
	cfgr &= ~(0x7UL << 11);
	cfgr |= (prv_EncodeApbPrescaler(MCU_CFG_APB2_PRESC) << 11);

	/* ADC prescaler (ADCPRE) - bits 15:14 */
	cfgr &= ~(0x3UL << 14);
	cfgr |= (prv_EncodeAdcPrescaler(MCU_CFG_ADC_PRESC) << 4);

	/* Config PLL source & multiplier */
#if (MCU_CFG_PLL_SOURCE_HSE == 1u)
	cfgr |= (1UL << 16);		// PLLSRC = HSE
	cfgr &= (1UL << 17);		// PLLXTPRE = HSE not devided
#else
	cfgr &= ~(1UL << 16);
#endif
	cfgr &= ~(0xFUL << 18);
	cfgr |= (prv_EncodePllMul(MCU_CFG_PLL_MUL) << 18);

	RCC->CFGR = cfgr;

	/* Enable PLL and wait PLLRDY*/
	RCC->CR |= (1UL << 24); //PLL on
	if(!prv_WaitFlagSet(&RCC->CR, (1UL << 25), 1000000U)) //PLLRDY
	{
		s_pllStatus = MCU_PLL_STATUS_UNDEFINED;
		Mcu_ClockInitErrorHook();
		return E_NOT_OK;
	}
	s_pllStatus = MCU_PLL_LOCKED;
	MCu_PllLockedHook();

	/* swap SYSCLK to PLL*/
	return Mcu_DistributePllClock();
}

