/* =====================================================================================================================
 *  File        : Dio_Types.h
 *  Layer       : MCAL
 *  ECU         : (STM32F103C6T6)
 *  Purpose     : Define data structure/enum/struct using by MCAL/DIO
 *  Notes       :
 * ===================================================================================================================*/
#ifndef DIO_DIO_TYPES_H_
#define DIO_DIO_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "Std_Types.h"
#include "Port_Types.h"
#include "stm32f103xx_regs.h"

typedef Port_PinLevelType Dio_ChannelState;				// pin state
typedef Port_PinType Dio_ChannelType;					// each channel is a GPIO pin
typedef GPIO_TypeDef Dio_PortType;						// each port as GPIOA, GPIOB,...
typedef uint32 Dio_PortLevelType;						// each port 16 bit

/*
 * Struct of all config DIO
 */
typedef struct {
	Dio_ChannelType ChannelID;
	Dio_PortType* PortID;
} Dio_ChannelGroupLevel;

#ifdef __cplusplus
}
#endif

#endif /* DIO_DIO_TYPES_H_ */
