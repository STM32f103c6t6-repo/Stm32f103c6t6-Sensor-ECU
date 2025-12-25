/* =====================================================================================================================
 *  File        : Gpt.h
 *  Layer       : MCAL
 *  ECU         : STM32F103C6T6
 *  Purpose     : Define API for GPT module.
 *  Notes       :
 * ===================================================================================================================*/


#ifndef GPT_GPT_H_
#define GPT_GPT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Gpt_Types.h"

void Gpt_Init(const Gpt_ConFigType* ConfigPtr);
void Gpt_DeInit(void);

void Gpt_StartTimer(Gpt_ChannelType Channel, Gpt_ValueType Value);
void Gpt_StopTimer(Gpt_ChannelType Channel);

// Read current counter
Gpt_ValueType Gpt_GetTimeElapsed(Gpt_ChannelType Channel);

//remain value to ARR
Gpt_ValueType Gpt_GetTimeRemaining(Gpt_ChannelType Channel);

#ifdef __cplusplus
}
#endif

#endif /* GPT_GPT_H_ */
