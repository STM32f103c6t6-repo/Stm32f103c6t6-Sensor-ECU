/* =====================================================================================================================
 *  File        : Icu_Types.h
 *  Layer       : MCAL
 *  ECU         : STM32F103C6T6
 *  Purpose     : Define data structure and macro for ICU
 *  Notes       :
 * ===================================================================================================================*/


#ifndef ICU_ICU_TYPES_H_
#define ICU_ICU_TYPES_H_

#if __cplusplus
extern "C" {
#endif

#include "Std_Types.h"
#include "stm32f103xx_regs.h"

#ifndef E_OK
typedef uint8		Std_ReturnType;
#define E_OK		((Std_ReturnType)0x00u)
#define E_NOT_OK	((Std_ReturnType)0x01u)
#endif

typedef uint8 Icu_ChannelType;

typedef enum {
	ICU_MODE_SIGNNAL_EDGE_DETECT,
	ICU_MODE_SIGNNAL_MEASUREMENT
} Icu_MeasurementModeType;

typedef enum {
	ICU_RISING_EDGE,
	ICU_FALLING_EDGE,
	ICU_BOTH_EDGES
} Icu_ActivationType;

typedef struct
{
	uint8 ChannelID;
	TIM_TypeDef* Timer;
	Icu_ChannelType IcuChannel;
	Icu_ActivationType DefaultEdge;
} Icu_ChannelConfigType;

typedef struct
{
	uint8 numsChannel;
	Icu_ChannelConfigType channels;
} Icu_ConfigType;

typedef enum
{
	TIM2_IRQn = 28,
	TIM3_IRQn = 29,
} IRQn_Type;

#if __cplusplus
}
#endif

#endif /* ICU_ICU_TYPES_H_ */
