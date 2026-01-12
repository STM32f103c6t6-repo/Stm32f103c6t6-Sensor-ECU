/* =====================================================================================================================
 *  File        : Can_Type.h
 *  Layer       : MCAL
 *  ECU         : STM32F103C6T6
 *  Purpose     : Define data structure and macro for CAN driver
 *  Notes       :
 * ===================================================================================================================*/


#ifndef CAN_CAN_TYPES_H_
#define CAN_CAN_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Std_Types.h"
#include "stm32f103xx_regs.h"

#ifndef E_OK
typedef uint8		Std_ReturnType;
#define E_OK		((Std_ReturnType)0x00u)
#define E_NOT_OK	((Std_ReturnType)0x01u)
#endif

// Can ID type
typedef uint32 Can_IdType;

// HW transmit handle
typedef uint8 Can_HwHandleType;

// Controller state
typedef enum
{
	CAN_CS_UNINIT	= 0,
	CAN_CS_STOPPED,
	CAN_CS_STARTED,
	CAN_CS_SLEEP
} Can_ControllerStateType;

// PDU definition
typedef struct
{
	Can_IdType Id;
	uint8 length;
	uint8* sdu;
	Can_HwHandleType swpduHandle;
} Can_PduType;

// Baudrate config
typedef struct
{
	uint16 precscale;
	uint8 sjw;
	uint8 tseg1;
	uint8 tseg2;
} Can_BaudrateConfigType;

// Controller config
typedef struct
{
	uint8 ControllerId;
	uint32 baseAddress;
	const Can_BaudrateConfigType baudrate;
} Can_ControllerConfigType;

typedef struct
{
	const Can_ControllerConfigType* ControllerConfig;
	uint8 numControllers;
} Can_ConfigType;

#ifdef __cplusplus
}
#endif


#endif /* CAN_CAN_TYPES_H_ */
