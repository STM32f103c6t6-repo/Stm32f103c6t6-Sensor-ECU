/* =====================================================================================================================
 *  File        : PduR.c
 *  Layer       : Service
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Notes       :
 * ===================================================================================================================*/

#include "PduR.h"
#include "CanIf.h"

static PduR_ConfigTypes* PduR_ConfigPtr = NULL_PTR;

void PduR_Init(PduR_ConfigTypes* ConfigPtr)
{
	PduR_ConfigPtr = ConfigPtr;
}

void PduR_CanIfRxIndication(
	PduIdType	RxPduId,
	const PduInfoType* PduInfoPtr
)
{
	uint8 i;
	const PduR_RxRouteType* route;

	if(PduR_ConfigPtr == NULL_PTR || PduInfoPtr == NULL_PTR)
	{
		return;
	}

	// Find route
	for(i = 0; i < PduR_ConfigPtr->NumRxRoutes;i++)
	{
		route = &PduR_ConfigPtr->RxRoutes[i];

		if(route->SrcModule == PDUR_MODULE_CANIF && route->SrcPduId == RxPduId)
		{
			// Route to COM
			if(route->DstModule == PDUR_MODULE_APP)
			{
//				App_RxIndication(route->DstPduId,PduInfoPtr);
			}
			break;
		}
	}

}

void PduR_CanIfTxConfirmation( PduIdType	TxPduId )
{
	uint8 i;
	const PduR_TxRouteType* route;

	if(PduR_ConfigPtr == NULL_PTR) return;

	for(i = 0;i < PduR_ConfigPtr->NumTxRoutes;i++)
	{
		route = &PduR_ConfigPtr->TxRoutes[i];

		if((route->DstModule == PDUR_MODULE_CANIF) && (route->DstPduId == TxPduId))
		{
			if(route->SrcModule == PDUR_MODULE_APP)
			{
//				App_TxConfirmation(route->SrcPduId);
			}
			break;
		}
	}
}

Std_ReturnType PduR_ComTransmit(
	PduIdType	TxPduId,
	const PduInfoType* PduInfoPtr
)
{
	uint8 i;
	const PduR_TxRouteType* route;

	if((PduR_ConfigPtr == NULL_PTR)||(PduInfoPtr == NULL_PTR)) return E_NOT_OK;

	for( i = 0; i < PduR_ConfigPtr->NumTxRoutes; i++)
	{
		route = &PduR_ConfigPtr->TxRoutes[i];
		if((route->SrcModule == PDUR_MODULE_APP) || (route->SrcPduId == TxPduId))
		{
			if(route->DstModule == PDUR_MODULE_CANIF)
			{
				return CanIf_Transmit(route->DstPduId, PduInfoPtr);
			}
		}
	}
	return E_NOT_OK;
}
