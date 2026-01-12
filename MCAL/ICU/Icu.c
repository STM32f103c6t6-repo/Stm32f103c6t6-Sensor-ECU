/* =====================================================================================================================
 *  File        : Icu.c
 *  Layer       : MCAL
 *  ECU         : STM32F103C6T6
 *  Purpose     : Apply Icu driver for STM32F1
 *  Depends     :
 * ===================================================================================================================*/

#include "Icu.h"

// Private Macro
#define ICU_NOT_INITIALIZED		0U
#define ICU_INITIALIZED			1U

// Driver state
static uint8 Icu_InitState	= ICU_NOT_INITIALIZED;

// Timestamp buffers
static uint32 Icu_RiseTime[ICU_MAX_CHANNELS];
static uint32 Icu_FallTime[ICU_MAX_CHANNELS];
static uint32 Icu_PulseWidth[ICU_MAX_CHANNELS];

// Measurement state
static uint8 Icu_MeasurementDone[ICU_MAX_CHANNELS];

// Store config pointer
static const Icu_ConfigType* Icu_ConfigPtr = NULL_PTR;

/* =================== PRIVATE FUNCTIONS =================== */
/*
 * Get TIM instance from channel config
 */
static TIM_TypeDef* Icu_GetTimer(Icu_ChannelType Channel)
{
	(void) Channel;
	return TIM2;
}

/* enable/disable NVIC IRQ for channel */
static void prv_EnableIrq(IRQn_Type irqn, bool enable)
{
	irqn = TIM2_IRQn;

	volatile uint32* ISER = (uint32*)0xE000E100UL;
	volatile uint32* ICER = (uint32*)0xE000E180UL;
	volatile uint32* ICPR = (uint32*)0xE000E280UL;

	if(irqn < 0) return;
	uint32 n   = (uint32)irqn;
	uint32 idx = n >> 5;           // /32
	uint32 bit = n & 0x1FU;        // %32
	uint32 mask = (1UL << bit);

	if (enable) {
		ICPR[idx] = mask;            // clear pending before enable
		ISER[idx] = mask;            // enable
	} else {
		ICER[idx] = mask;            // disable
		ICPR[idx] = mask;            // clear pending
	}
}


static void Icu_HwInit(Icu_ChannelType Channel)
{
	TIM_TypeDef* TIMx = Icu_GetTimer(Channel);

	// Enable TIM2 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	// Prescaler: 772MHZ / 72 = 1MHZ
	TIMx->PSC = 72-1;

	// Auto reload max
	TIMx->ARR = 0xFFFFFFFF;

	//Configure CH1 as input, mapped to TI1
	TIMx->CCMR1 &= ~TIM_CCMR1_CC1S;
	TIMx->CCMR1 |= TIM_CCMR1_CC1S_1;

	// Capture on rising edge initially
	TIMx->CCER &= ~(TIM_CCER_CC1P);

	// Enable Capture
	TIMx->CCER |= TIM_CCER_CC1E;

	// Enable interrupt
	TIMx->DIER |= TIM_DIER_CC1IE;

	// Enable counter
	TIMx->CR1 |= TIM_CR1_CEN;

	// Enable NVIC for TIM2
	prv_EnableIrq(TIM2_IRQn, TRUE);
}

/* =================== ICU API FUNCTION =================== */

Std_ReturnType Icu_Init(const Icu_ConfigType* ConfigPtr)
{
	if(ConfigPtr == NULL_PTR) return E_NOT_OK;

	Icu_ConfigPtr = ConfigPtr;

	// reset counter
	for(uint8 ch =0; ch < ConfigPtr->numsChannel; ch++)
	{
		Icu_RiseTime[ch] = 0;
		Icu_FallTime[ch] = 0;
		Icu_PulseWidth[ch] = 0;
		Icu_MeasurementDone[ch] = 0;

		Icu_HwInit(ch);
	}

	Icu_InitState = ICU_INITIALIZED;

}

Std_ReturnType Icu_StartSignalMeasurement(Icu_ChannelType Channel)
{
	if(Icu_InitState != ICU_INITIALIZED)	return E_NOT_OK;

	Icu_MeasurementDone[Channel] = 0;

	// Reset timer counter
	Icu_GetTimer(Channel)->CNT = 0;

	// Capture rising edge first
	Icu_GetTimer(Channel)->CCER &= ~(TIM_CCER_CC1P);

}

uint32 Icu_GetTimeElapsed(Icu_ChannelType Channel)
{
	if( Icu_MeasurementDone[Channel] == 1U)
	{
		return Icu_PulseWidth[Channel];
	}

	return 0;
}

void Icu_StopSignalMeasurement(Icu_ChannelType Channel)
{
	(void)Channel;

	// Disable capture interrupt
	Icu_GetTimer(Channel)->DIER &= TIM_DIER_CC1IE;
}

/* =================== ICU INTERRUPT SERVICE =================== */

void TIM2_IRQHandler(void)
{
	if(TIM2->SR & TIM_SR_CC1IF)
	{
		static uint8 edgeState = 0;

		if( edgeState == 0)
		{
			// Rising edge detected
			Icu_RiseTime[0] = TIM2->CCR1;

			// Switch to falling edge
			TIM2->CCER = TIM_CCER_CC1P;
			edgeState = 1;
		} else {
			// Falling edge detected
			Icu_FallTime[0] = TIM2->CCR1;

			if(Icu_FallTime[0] >= Icu_RiseTime[0])
			{
				Icu_PulseWidth[0] = Icu_FallTime[0] - Icu_RiseTime[0];
			} else {
				Icu_PulseWidth[0] = (0xFFFFFFFF - Icu_RiseTime[0]) + Icu_FallTime[0];
			}

			Icu_MeasurementDone[0] = 1;

			// prepare for next measurement
			TIM2->CCER &= ~(TIM_CCER_CC1P);
			edgeState = 0;
		}

		// Clear interrupt flag
		TIM2->SR &= ~TIM_SR_CC1IF;
	}
}
