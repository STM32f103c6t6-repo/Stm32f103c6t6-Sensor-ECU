/* =====================================================================================================================
 *  File        : Can.h
 *  Layer       : MCAL
 *  ECU         : (STM32F103C6T6)
 *  Purpose     : Declare API, data structure for Can driver
 *  Notes       :
 * ===================================================================================================================*/


#ifndef CAN_CAN_H_
#define CAN_CAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Can_Types.h"

void Can_Init(const Can_ConfigType* Config);
Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Mode);
Std_ReturnType Can_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo);

void Can_MainFunction_Tx(void);
void Can_MainFunction_Rx(void);

#ifdef __cplusplus
}
#endif

#endif /* CAN_CAN_H_ */
