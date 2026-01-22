/* =====================================================================================================================
 *  File        : CanIf.h
 *  Layer       : Abstraction
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Provide API send/receiver Can driver for services/applications
 *  Notes       :
 * ===================================================================================================================*/


#ifndef CANIF_CANIF_H_
#define CANIF_CANIF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Std_Types.h"
#include "ComStack_Types.h"
#include "Can.h"

/* =====================================================================================================================
 *  Type definitions
 * ===================================================================================================================*/
/*
 * Can Identifier type used in Canif
 * Standard CAN : 11bits
 * Extended CAN : 29 bits
 */

typedef uint32 CanIf_CanIdType;

/*
 * Hardware object handle type
 * Used to identify Tx/Rx mailbox in CAN driver
 */
typedef uint8 CanIf_HohType;

/*
 * Mapping between upper layer PDU and CAN hw
 */

typedef struct
{
	PduIdType			TxPduId;
	CanIf_CanIdType		CanId;
	CanIf_HohType		Hoh;
} CanIf_TxPduConfigType;

/*
 * Mapping between received CAN ID  and upper layer PDU
 */

typedef struct
{
	PduIdType			RxPduId;
	CanIf_CanIdType		CanId;
} CanIf_RxPduConfigType;

/*
 * Global CanIf configuration
 */
typedef struct
{
	CanIf_TxPduConfigType*			TxPduConfig;
	uint8							NumTxPdu;
	CanIf_RxPduConfigType*			RxPduConfig;
	uint8							NumRxPdu;
} CanIf_ConfigType;

/* =====================================================================================================================
 *  SPI prototypes
 * ===================================================================================================================*/
void CanIf_Init(const CanIf_ConfigType* CanConfigPtr);

Std_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);

void CanIf_TxConfirmation(PduIdType TxPduId);

void CanIf_RxIndication( PduIdType RxPduId, const PduInfoType* PduInfoPtr);

#ifdef __cplusplus
}
#endif

#endif /* CANIF_CANIF_H_ */
