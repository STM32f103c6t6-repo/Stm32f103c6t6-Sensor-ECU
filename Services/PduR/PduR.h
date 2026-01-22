/* =====================================================================================================================
 *  File        : PduR.h
 *  Layer       : Services
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Depends     :
 * ===================================================================================================================*/


#ifndef PDUR_PDUR_H_
#define PDUR_PDUR_H_

#include "Std_Types.h"
#include "ComStack_Types.h"

typedef enum {
	PDUR_MODULE_CANIF,
	PDUR_MODULE_APP
} PduR_ModuleType;

typedef struct {
	PduR_ModuleType SrcModule;
	PduIdType		SrcPduId;
	PduR_ModuleType	DstModule;
	PduIdType		DstPduId;
} PduR_RxRouteType;

typedef struct {
	PduR_ModuleType SrcModule;
	PduIdType		SrcPduId;
	PduR_ModuleType	DstModule;
	PduIdType		DstPduId;
} PduR_TxRouteType;

typedef struct {
	const PduR_RxRouteType*	RxRoutes;
	uint8					NumRxRoutes;
	const PduR_TxRouteType*	TxRoutes;
	uint8					NumTxRoutes;
} PduR_ConfigTypes;

void PduR_Init(PduR_ConfigTypes* ConfigPtr);

void PduR_CanIfRxIndication(
	PduIdType	RxPduId,
	const PduInfoType* PduInfoPtr
);

void PduR_CanIfTxConfirmation(
	PduIdType	TxPduId
);

Std_ReturnType PduR_ComTransmit(
	PduIdType	TxPduId,
	const PduInfoType* PduInfoPtr
);

// Rx PDU IDs from CanIf
#define PDUR_CANIF_RX_PDU_SENSOR_DISTANCE		((PduIdType)0)

// Rx PDU IDs to App
#define PDUR_APP_RX_PDU_SENSOR_DISTANCE			((PduIdType)0)

// Number of Rx routes
#define PDUR_NUM_RX_ROUTE						(1u)

// Tx Pdu ids from app
#define PDUR_APP_TX_PDU_STOP_MOTOR				((PduIdType)0)

// Tx Pdu ids to CanIf
#define PDUR_CANIF_TX_PDU_STOP_MOTOR			((PduIdType)0)

// Number of Tx routes
#define PDUR_NUM_TX_ROUTES						(1u)

#endif /* PDUR_PDUR_H_ */
