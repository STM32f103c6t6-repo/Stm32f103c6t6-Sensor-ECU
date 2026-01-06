/* =====================================================================================================================
 *  File        : GPT_Cfg.h
 *  Layer       : MCAL
 *  ECU         : STM32F103C6T6
 *  Purpose     : Config GPT driver
 *  Notes       :
 * ===================================================================================================================*/

#ifndef GPT_CFG_H_
#define GPT_CFG_H_

#include "Gpt_Types.h"

#define GPT_CHANNEL_ID		 	0
#define GPT_MODE			 	GPT_MODE_CONTINUOUS
#define GPT_TIMER_ID 		 	TIM2ID
#define GPT_TIMER_PRESCALE	 	71
#define GPT_CHANNEL_COUNT		1
#define GPT_TICK_FREQUENCY		1000000

#endif /* GPT_CFG_H_ */
