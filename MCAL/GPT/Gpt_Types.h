/* =====================================================================================================================
 *  File        : Gpt_Type.h
 *  Layer       : MCAL
 *  ECU         : STM32F103C6T6
 *  Purpose     : Define data structure and macro for Gpt driver (General Purpose Timer)
 *  Notes       :
 * ===================================================================================================================*/


#ifndef GPT_GPT_TYPES_H_
#define GPT_GPT_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Std_Types.h"
#include "stm32f103xx_regs.h"

#define TIM1ID			1
#define TIM2ID			2
#define TIM3ID			3
#define TIM4ID			4
#define TIM5ID			5
#define TIM6ID			6
#define TIM7ID			7
#define TIM8ID			8

typedef uint8 	Gpt_ChannelType;
typedef uint32	Gpt_ValueType;

typedef enum
{
	GPT_MODE_ONESHOT = 0,
	GPT_MODE_CONTINUOUS
} Gpt_ModeTye;

typedef struct
{
	Gpt_ChannelType ChannelId;
	Gpt_ModeTye 	Mode;
	uint32			TickFrequency;
	uint8			TimerId;
	uint16			Prescale;
} Gpt_ChannelConfigType;

typedef struct
{
	const Gpt_ChannelConfigType* ChannelConfig;
	uint8	ChannelCount;
} Gpt_ConFigType;

#ifdef __cplusplus
}
#endif

#endif /* GPT_GPT_TYPES_H_ */
