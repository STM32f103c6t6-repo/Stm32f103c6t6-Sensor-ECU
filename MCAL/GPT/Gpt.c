/* =====================================================================================================================
 *  File        : Gpt.c
 *  Layer       : MCAL
 *  ECU         : STM32F103C6T6
 *  Purpose     : Write API for Gpt driver.
 *  Notes       :
 * ===================================================================================================================*/
#include "Gpt.h"

static const Gpt_ConFigType* Gpt_ConfigPtr = NULL_PTR ;

static inline TIM_TypeDef* prv_GetTim(const Gpt_ChannelConfigType* chCfg )
{
	switch(chCfg->TimerId)
	{
	case TIM1ID : return  TIM1;
	case TIM2ID : return  TIM2;
	case TIM3ID : return  TIM3;
	case TIM4ID : return  TIM4;
	default: return (TIM_TypeDef*) 0;
	}
}

static inline void prv_EnTim(const Gpt_ChannelConfigType* chCfg )
{
	switch(chCfg->TimerId)
	{
	case TIM1ID :
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
		break;
	case TIM2ID :
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
		break;
	case TIM3ID :
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
		break;
	case TIM4ID :
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
		break;
	default: return;
	}
}

/*
 * Gpt_Init
 * - Enable clock for timer
 * - Config prescaler
 * - Reset Counter
 * - Start timer
 */
void Gpt_Init(const Gpt_ConFigType* ConfigPtr)
{
	Gpt_ConfigPtr = ConfigPtr;

	/*
	 * Get config channel 0
	 */
	const Gpt_ChannelConfigType* chCfg = &ConfigPtr -> ChannelConfig[0];
	TIM_TypeDef* Timer = prv_GetTim(chCfg);
	if(!Timer) return;

	/*
	 * Enable clock for timer
	 */
	prv_EnTim(chCfg);

	/*
	 * Disable timer
	 */
	Timer->CR1 &= ~(TIM_CR1_CEN);

	/*
	 * Prescale
	 */
	Timer->PSC = chCfg->Prescale-1;

	/*
	 *  Auto reload max
	 */
	Timer->ARR = 0xFFFFFFFF;

	/*
	 * Reset counter
	 */
	Timer->CNT = 0;

	/*
	 * Enable Timer
	 */
	Timer->CR1 |= TIM_CR1_CEN;
}

/*
 * Gpt Deinit
 */

void Gpt_DeInit(void)
{
	TIM_TypeDef* Timer = prv_GetTim(&(Gpt_ConfigPtr->ChannelConfig[0]));
	Timer->CR1 &= ~(TIM_CR1_CEN);
}

/* =========================
 * Gpt_StartTimer
 * =========================
 * continuous timer:
 * - Timer already running
 * - Not load Value
 */
void Gpt_StartTimer(Gpt_ChannelType Channel, Gpt_ValueType Value)
{
	(void) Channel;
	(void) Value;
}

/*
 * Gpt Stop timer
 */

void Gpt_StopTimer(Gpt_ChannelType Channel)
{
	TIM_TypeDef* Timer = prv_GetTim(&(Gpt_ConfigPtr->ChannelConfig[Channel]));
	Timer->CR1 &= ~(TIM_CR1_CEN);
}

/* =========================
 * Gpt_GetTimeElapsed
 * =========================
 * return tick count
 * 1 tick = 1 µs
 */
Gpt_ValueType Gpt_GetTimeElapsed(Gpt_ChannelType Channel)
{
	TIM_TypeDef* Timer = prv_GetTim(&(Gpt_ConfigPtr->ChannelConfig[Channel]));
	return ((Gpt_ValueType)Timer->CNT);
}

/* =========================
 * Gpt_GetTimeRemaining
 * ========================= */
Gpt_ValueType Gpt_GetTimeRemaining(Gpt_ChannelType Channel)
{
	TIM_TypeDef* Timer = prv_GetTim(&(Gpt_ConfigPtr->ChannelConfig[Channel]));
	return ((Gpt_ValueType)Timer->ARR) - ((Gpt_ValueType)Timer->CNT);
}
