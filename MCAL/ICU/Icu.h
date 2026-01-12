/*
 * Icu.h
 *
 *  Created on: Jan 10, 2026
 *      Author: vhip
 */

#ifndef ICU_ICU_H_
#define ICU_ICU_H_

#if __cplusplus
extern "C" {
#endif

#include "stm32f103xx_regs.h"
#include "Icu_Types.h"

// we only use TIMER2
#define ICU_GET_TIMER(ch)		(TIM2)

#define ICU_MAX_CHANNELS		(3U)

Std_ReturnType Icu_Init(const Icu_ConfigType* ConfigPtr);
Std_ReturnType Icu_StartSignalMeasurement(Icu_ChannelType Channel);
uint32 Icu_GetTimeElapsed(Icu_ChannelType Channel);
void Icu_StopSignalMeasurement(Icu_ChannelType Channel);

#if __cplusplus
}
#endif

#endif /* ICU_ICU_H_ */
