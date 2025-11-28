/* =====================================================================================================================
 *  File        : Mcu.c
 *  Layer       : MCAL
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Aply driver MCU: clock/reset/NVIC/SysTick/IWDG
 *  Depends     : - Sensor_ECU/Config/Mcu_Cfg.h
 *                - AutosarShared/include/mcal/stm32f103xx_regs.h (RCC/GPIO/AFIO/USART/CAN base)
 *  Notes       :
 * ===================================================================================================================*/

#include "Mcu.h"
#include "Mcu_Cfg.h"
#include "stm32f103xx_regs.h"
#include "Mcu_Types.h"

/*SYST_CSR bits*/
#define SYST_CSR_ENABLE_Pos			0U
#define SYST_CSR_TICKINT_Pos		0U
#define SYST_CSR_CLKSOURCE_Pos		0U

/* SCB AIRRCR (priority grouping & system reset )*/
#define SCB_AIRCR_VECTKEY_Pos		16U
#define SCB_AIRCR_VECTKEY			(0x5FAUL << SCB_AIRCR_VECTKEY_Pos)
#define SCB_AIRCR_PRIGROUP_Pos 		8U
#define SCB_AIRCR_SYSRESETREQ_Pos	2U

/* IWDG registers */
#define IWDG_KR_KEY_ENABLE			(0xCCCCUL)
#define IWDG_KR_KEY_RELOAD			(0xAAAAUL)
#define IWDG_KR_KEY_WRITE			(0x5555UL)

/* Static state */
__vo	uint32		s_systickTicks 	= 0;
static Mcu_StatusType		s_mcuStatus		= MCU_UNINIT;
static Mcu_PllStatusType	s_pllStatus		= MCU_PLL_STATUS_UNDEFINED;
Mcu_ClockInfoType	s_clkInfo		= {0};

static void prv_FlashSetLatencyAndPreFetch(Mcu_FlashLatencyType Flash_Latency);
static uint32 prv_EncodeAhbPrescaler(uint32 div);
static uint32 prv_EncodeApbPrescaler(uint32 div);
static uint32 prv_EncodeAdcPrescaler(uint32 div);
static uint32 prv_EncodePllMul(uint32 mul);

/* Timeout (avoid hang)*/
static inline bool prv_WaitFlagSet(__vo const uint32* reg, uint32 mask, uint32 loops)
{
	while(((*reg) & mask) == 0)
	{
		if(loops-- == 0U) return FALSE;
	}
	return TRUE;
}

Std_ReturnType Mcu_Init(const Mcu_ConfigType *profile)
{
//	(void) profile;

	/* Enable HSE*/
#if (MCU_CFG_PLL_SOURCE_HSE == MCU_CLOCK_SRC_HSE)
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
	prv_FlashSetLatencyAndPreFetch(profile->flashLatency);

	/* config prescaler bus */
	uint32 cfgr = RCC->CFGR;

	/* AHB prescaler (HPRE) - bits 7:4 */
	cfgr &= ~(0xFUL << 4);
	cfgr |= (prv_EncodeAhbPrescaler(profile->ahbPresc) << 4);

	/* APB1 prescaler (PPRE1) - bits 10:8 */
	cfgr &= ~(0x7UL << 8);
	cfgr |= (prv_EncodeApbPrescaler(profile->apb1Presc) << 8);

	/* APB2 prescaler (PPRE2) - bits 13:11 */
	cfgr &= ~(0x7UL << 11);
	cfgr |= (prv_EncodeApbPrescaler(profile->apb2Presc) << 11);

	/* ADC prescaler (ADCPRE) - bits 15:14 */
	cfgr &= ~(0x3UL << 14);
	cfgr |= (prv_EncodeAdcPrescaler(profile->adcPresc) << 14);

	/* Config PLL source & multiplier */
#if (MCU_CFG_PLL_SOURCE_HSE == MCU_CLOCK_SRC_HSE)
	cfgr |= (1UL << 16);		// PLLSRC = HSE
	cfgr &= ~(1UL << 17);		// PLLXTPRE = HSE not devided
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

Std_ReturnType Mcu_DistributePllClock(void)
{
	if(s_pllStatus != MCU_PLL_LOCKED) return E_NOT_OK;

	/* SW(1:0) = 10b -> SYSCLK = PLL */
	uint32 cfgr = RCC -> CFGR;
	cfgr &= ~(0x3UL << 0);
	cfgr |=  (0x2UL << 0);
	RCC->CFGR = cfgr;

	/*wait SWS (3:2) = 10b */
	for( uint32 t = 0; t < 1000000U;t++)
	{
		if(((RCC->CFGR >>2) & 0x3UL) == 0x2UL)
		{
			/*Update info for other modules to query*/
			s_clkInfo.sysclk_hz 	= MCU_CFG_SYSCLK_FREQ_HZ;
			s_clkInfo.hclk_hz		= MCU_CFG_AHB_FREQ_HZ;
			s_clkInfo.pclk1_hz		= MCU_CFG_APB1_FREQ_HZ;
			s_clkInfo.pclk2_hz		= MCU_CFG_APB2_FREQ_HZ;
			s_clkInfo.adcclk_hz		= MCU_CFG_ADC_FREQ_HZ;
			s_clkInfo.systick_hz	= MCU_CFG_SYSTICK_HZ;

			s_mcuStatus = MCU_INIT;
			return E_OK;
		}
	}

	return E_NOT_OK;
}

Mcu_PllStatusType Mcu_GetPllStatus(void)
{
	return s_pllStatus;
}

Std_ReturnType Mcu_SetNvicPriorityGrouping(uint32 prigroup_value)
{
	/*Write AIRCR with VECTKEY*/
	uint32 reg = SCB_AIRCR;
	reg &= ~(0x7UL << SCB_AIRCR_PRIGROUP_Pos);
	reg = (reg & ~(0xFFFFUL << 16)) | SCB_AIRCR_VECTKEY;
	reg |= ((prigroup_value & 0x7UL) << SCB_AIRCR_PRIGROUP_Pos);
	SCB_AIRCR = reg;
	return E_OK;
}

Std_ReturnType Mcu_SetIrqPriority(uint32 irqn, uint8 preemptPrio, uint8 subPrio)
{
	(void)subPrio;
	if(irqn < 0){ return E_NOT_OK;} //Do not configure system exceptions here.

	NVIC_IPR_BASE[(uint32)irqn] = (uint8)(preemptPrio << 4); // 4 MSB
	return E_OK;
}

Std_ReturnType Mcu_SetSysTickHz(uint32 tick_hz)
{
	uint32 hclk 	= (MCU_CFG_SYSTICK_SOURCE_HCLK == 1u) ? MCU_CFG_AHB_FREQ_HZ : (MCU_CFG_AHB_FREQ_HZ / 8u);
	uint32 reload = (hclk / tick_hz) -1U;
	if(reload > 0x00FFFFFFUL) return E_NOT_OK;

	/* Reset Systick */
	SYST_CSR = 0U;
	SYST_RVR = reload;
	SYST_CVR = 0U;

	/* Enable: CLKSOURCE follow config, TICKINT on*/
	uint32 csr = (1UL << SYST_CSR_TICKINT_Pos) | (1UL << SYST_CSR_ENABLE_Pos);
	if(MCU_CFG_SYSTICK_SOURCE_HCLK == 1u) { csr |= (1UL << SYST_CSR_CLKSOURCE_Pos);}
	SYST_CSR	= csr;

	s_clkInfo.systick_hz = tick_hz;
	return E_OK;
}

/* Tick increase each 1ms ( if set 1000Hz) - Port enter startip: put pointer vector SysTick_Handler to this function*/
void SysTick_Handler(void)
{
	s_systickTicks++;
}

void Mcu_DelayMs(uint32 ms)
{
	uint32 start = s_systickTicks;
	while ((s_systickTicks - start) < ms){}
}

Std_ReturnType Mcu_GetClockInfo(Mcu_ClockInfoType* out)
{
	if(out == NULL_PTR){ return E_NOT_OK; }
	*out = s_clkInfo;
	return (s_mcuStatus == MCU_INIT) ? E_OK : E_NOT_OK;
}

Std_ReturnType Mcu_EnableIwdg(void)
{
#if (MCU_CFG_IWDG_ENABLE == 1u)

	/*Hardware: LSI source ~40kHz; presc = 4..256; timeout ~= (RLR+1) before / 40kHz
	Using Mcu_Cfg.h: MCU_CFG_IWDG_PRESC, MCU_CFG_IWDG_TIMEOUT_MS */
	// 1. Unlock write
	IWDG_KR = IWDG_KR_KEY_WRITE;

	// 2. Prescaler.
	// Map prescaler: hardware PR[2:0]: 0:4,1:8,2:16,3:32,4:64,5:128,6:256
	uint32 pr_val = 0U;
	switch(MCU_CFG_IWDG_PRESC)
	{
	case 4: 	pr_val = 0; break;
	case 8: 	pr_val = 1; break;
	case 16: 	pr_val = 2; break;
	case 32:	pr_val = 3; break;
	case 64:	pr_val = 4; break;
	case 128:	pr_val = 5; break;
	case 256:	pr_val = 6; break;
	default: 	pr_val = 4; break; //default 64
	}
	IWDG_PR = pr_val;

	// 3. calculate reload from timeout
	// LSI ~ 40kHz -> tick = presc / 4000 s. RLR = timeout_s / tick -1

	uint32 lsi_hz = 40000U;
	uint32 ticks	= (uint32)(((uint64_t)MCU_CFG_IWDG_TIMEOUT_MS * (uint64_t)lsi_hz) / (1000ULL * (uint64_t)MCU_CFG_IWDG_PRESC));
	if(ticks == 0U) { ticks = 1U;}
	if(ticks > 0x0FFFU) {ticks = 0x0FFFU;}
	IWDG_RLR = (ticks & 0x0FFFU);

	// 4. Start
	IWDG_KR = IWDG_KR_KEY_ENABLE;

	// 5. Reload now
	IWDG_KR = IWDG_KR_KEY_RELOAD;

	return E_OK;
#else
	return E_OK;
#endif
}

void Mcu_KickIwdg(void)
{
#if (MCU_CFG_IWDG_ENABLE == 1u)
	IWDG_KR = IWDG_KR_KEY_RELOAD;
#endif
}

/* System Reset */
void MCu_PerformReset(void)
{
	/*write AIRCR with VECTKEY and SYSRESETREQ = 1*/
	SCB_AIRCR = SCB_AIRCR_VECTKEY | (SCB_AIRCR &  (0x7UL << SCB_AIRCR_PRIGROUP_Pos)) | (1UL << SCB_AIRCR_SYSRESETREQ_Pos);
	while(1){}; //wait reset
}

Mcu_StatusType Mcu_GetStatus(void)
{
	return s_mcuStatus;
}

/*weak hooks */
__attribute__((weak)) void MCu_PllLockedHook(void) {}
__attribute__((weak)) void Mcu_ClockInitErrorHook(void) {}

/* Local helpers impl */
static void prv_FlashSetLatencyAndPreFetch(Mcu_FlashLatencyType Flash_Latency)
{

	uint32 acr = FLASH_ACR;
	acr &= ~(0x7UL << FLASH_ACR_LATENCY_Pos);
	acr |= (Flash_Latency << FLASH_ACR_LATENCY_Pos);
	acr |= (1UL << FLASH_ACR_PRFTBE_Pos); //enable prefetch
	FLASH_ACR = acr;
}

static uint32 prv_EncodeAhbPrescaler(uint32 div)
{
	/* HPRE encoding: 0xxx: /1, 1000:/2,1001:/4,1010:/8,1011:/16,1100:/64,1101:/128,1110:/256,1111:/512 */
	switch(div)
	{
	case 1:			return 0x0U;
	case 2:			return 0x8U;
	case 4:			return 0x9U;
	case 8:			return 0xAU;
	case 16:		return 0xBU;
	case 64:		return 0xCU;
	case 128:		return 0xDU;
	case 256:		return 0xEU;
	case 512:		return 0xFU;
	default:		return 0x0U;
	}
}

static uint32 prv_EncodeApbPrescaler(uint32 div)
{
	/* PPRE encoding: 0xx:/1, 100:/2,101:/4,110:/8,111:/16 */
	switch(div)
	{
	case 1: 		return 0x0U;
	case 2: 		return 0x4U;
	case 4: 		return 0x5U;
	case 8: 		return 0x6U;
	case 16: 		return 0x7U;
	default:		return 0x0U;
	}
}

static uint32 prv_EncodeAdcPrescaler(uint32 div)
{
	/* ADCPRE bits: 00:/2, 01:/4, 10:/6, 11:/8 */
	switch(div)
	{
	case 2:			return 0x0U;
	case 4:			return 0x1U;
	case 6:			return 0x2U;
	case 8:			return 0x3U;
	default:		return 0x2U; //default /6 for 72 MHz -> 12MHz
	}
}

static uint32 prv_EncodePllMul(uint32 mul)
{
	/* PLLMUL bits (21:18): 0000:x2 ... 0111:x9, 1000:x10 ... 1110:x16 (F1 classic) */
	if( mul < 2U)  { mul = 2U; }
	if( mul > 16U) { mul = 16U;}
	return (mul - 2U);
}



