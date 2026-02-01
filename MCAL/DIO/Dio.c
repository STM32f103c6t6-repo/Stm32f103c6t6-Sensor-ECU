/* =====================================================================================================================
 *  File        : Dio.c
 *  Layer       : MCAL
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Depends     : - Sensor_ECU/Config/Dio_Types.h
 *                - AutosarShared/include/mcal/stm32f103xx_regs.h (RCC/GPIO/AFIO/USART/CAN base)
 *  Notes       :
 * ===================================================================================================================*/

#include "Dio.h"

static Dio_PortType* Dio_PortAddress(Dio_ChannelType pinID, uint8* outPinNum)
{
	return prv_GetGpioFromPin(pinID,outPinNum);
}

/*
 * init DIO module
 */
void Dio_Init(void)
{
	// Enable in Port Module
}

/*
 * Read state of a Pin
 */
Dio_ChannelState Dio_ReadChannel(Dio_ChannelType pinID)
{
	return Port_ReadPin(pinID);
}

/*
 * Write state of a Pin
 */
void Dio_WriteChannel(Dio_ChannelType pinID, Dio_ChannelState Level)
{
	uint8 pinNum = 0;
	Dio_PortType* PORTx =  Dio_PortAddress(pinID, &pinNum);
	prv_WriteOdr(PORTx,pinNum,(boolean)Level);
}

/*
 * Flip State of Pin
 */
Dio_ChannelState Dio_FlipChannel(Dio_ChannelType pinID)
{
	Dio_ChannelState current = Dio_ReadChannel(pinID);
	Dio_WriteChannel(pinID, !current);
	return !current;
}

/*
 * Read all a Port
 */
Dio_PortLevelType Dio_ReadPort(Dio_ChannelType pinID)
{
	uint8 temp = 0;
	Dio_PortType* GPIOx =  Dio_PortAddress(pinID, &temp);
	return (Dio_PortLevelType) GPIOx->IDR;
}

/*
 * Write all a Port
 */
void Dio_WritePort(Dio_ChannelType pinID, Dio_PortLevelType Level)
{
	uint8 temp = 0;
	Dio_PortType* GPIOx =  Dio_PortAddress(pinID, &temp);
	GPIOx->ODR = Level ;
}

