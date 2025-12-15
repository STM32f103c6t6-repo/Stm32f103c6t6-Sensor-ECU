/* =====================================================================================================================
 *  File        : Dio.h
 *  Layer       : MCAL
 *  ECU         : (STM32F103C6T6)
 *  Purpose     : Declare API, data structure for MCAL/Dio
 *  Notes       :
 * ===================================================================================================================*/


#ifndef DIO_DIO_H_
#define DIO_DIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Std_Types.h"
#include "Dio_Types.h"
#include "Port.h"

/*
 * init DIO module
 */
void Dio_Init(void);

/*
 * Read state of a Pin
 */
Dio_ChannelState Dio_ReadChannel(Dio_ChannelType pinID);

/*
 * Write state of a Pin
 */
void Dio_WriteChannel(Dio_ChannelType pinID, Dio_ChannelState Level);

/*
 * Flip State of Pin
 */
Dio_ChannelState Dio_FlipChannel(Dio_ChannelType pinID);

/*
 * Read all a Port
 */
Dio_PortLevelType Dio_ReadPort(Dio_ChannelType pinID);

/*
 * Write all a Port
 */
Dio_PortLevelType Dio_ReadPort(Dio_ChannelType pinID);

#ifdef __cplusplus
}
#endif


#endif /* DIO_DIO_H_ */
