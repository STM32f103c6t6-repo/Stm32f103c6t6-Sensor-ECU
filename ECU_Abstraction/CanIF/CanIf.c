/* =====================================================================================================================
 *  File        : CanIF.c
 *  Layer       : Abstraction
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Apply CANIF declare API for STM32F1
 *  Depends     :
 * ===================================================================================================================*/

#include "CanIf.h"
#include "Can.h"

static const CanIf_ConfigType* CanIf_CfgPtr = NULL_PTR;
static bool CanIf_Inited = FALSE;

void CanIf_Init(const CanIf_ConfigType* ConfigPtr)
{
	CanIf_CfgPtr = ConfigPtr;
	CanIf_Inited = TRUE;

}

Std_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
{
	CanIf_TxPduConfigType* txCfg;
	Can_PduType canPdu;

	if((CanIf_CfgPtr == NULL) || (TxPduId >= CanIf_CfgPtr->NumTxPdu))
	{
		return E_NOT_OK;
	}

	txCfg = &CanIf_CfgPtr->TxPduConfig[TxPduId];

	canPdu.Id			= txCfg->CanId;
	canPdu.sdu			= PduInfoPtr->SduDataPtr;
	canPdu.length		= PduInfoPtr->SduLength;
	canPdu.swpduHandle	= TxPduId;

	return Can_Write(txCfg->Hoh, &canPdu);
}

void CanIf_TxConfirmation(PduIdType TxPduId)
{

}

void CanIf_RxIndication( PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{
	if(RxPduId >= CanIf_CfgPtr->NumRxPdu) return;

}
