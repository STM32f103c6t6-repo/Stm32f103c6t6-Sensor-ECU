/* =====================================================================================================================
 *  File        : Com.c
 *  Layer       : Services
 *  ECU         : STM32F103C6T6
 *  Purpose     : Config Api Com
 *  Depends     :
 * ===================================================================================================================*/

#include "Com.h"
#include "PduR.h"
#include <string.h>

static const Com_ConfigType* Com_ConfigPtr = NULL_PTR;

static uint8 Com_TxBuffer[8];


// Initialize Com module
void Com_Init(const Com_ConfigType* ConfigPtr)
{
	Com_ConfigPtr	= ConfigPtr;
}

Std_ReturnType Com_TriggerTransmit(Com_IpduIdType Ipduid);

/*
 * Indication of a received I-PDU from PduR
 */
void Com_RxIndication(
		PduIdType RxPduId,
		const PduInfoType* PduInfoPtr
)
{
	uint16 i;
	const Com_SignalConfigType* sigCfg;

	if(Com_ConfigPtr == NULL_PTR || PduInfoPtr == NULL_PTR) return;

	// Unpack signal from PDU
	for(i = 0; i < Com_ConfigPtr->NumSignals; i++)
	{
		sigCfg = &Com_ConfigPtr->SignalConfig[i];

		if(sigCfg->Ipduid == (Com_IpduIdType)RxPduId)
		{
			if(sigCfg->SignalType == COM_SIGNAL_UINT16)
			{
				uint16 value;
				if(sigCfg->Endianness == COM_LITTLE_ENDIAN)
				{
					value = PduInfoPtr->SduDataPtr[(sigCfg->BitPosition)/8];
					value |= (uint16)(PduInfoPtr->SduDataPtr[(sigCfg->BitPosition)/8+1] << 8);
				} else {
					value = (uint16)(PduInfoPtr->SduDataPtr[(sigCfg->BitPosition)/8] << 8);
					value |= PduInfoPtr->SduDataPtr[(sigCfg->BitPosition)/8+1];
				}
				// Call back
//				App_SignalIndication(sigCfg->SignalId,&value);
			}
		}
	}
}

/*
 * Transmission confirmation
 */
void Com_TxConfirmation(PduIdType TxPduId)
{
	(void)TxPduId;
}

/*
 * Send a signal
 */
Std_ReturnType Com_SendSignal(
		Com_SignalIdType	SignalId,
		const void* SignalDataPtr
)
{
	uint16 i;
	const Com_SignalConfigType* sigCfg;

	if((Com_ConfigPtr == NULL_PTR) || (SignalDataPtr == NULL_PTR)) return E_NOT_OK;

	// Find signal config
	for (i = 0; i < Com_ConfigPtr->NumSignals; i++)
	{
		sigCfg = &Com_ConfigPtr->SignalConfig[i];

		if(sigCfg->SignalId == SignalId)
		{
			// clear buffer
			memset(Com_TxBuffer, 0,sizeof(Com_TxBuffer));

			//Pack signal
			if(sigCfg->SignalType == COM_SIGNAL_SINT8)
			{
				Com_TxBuffer[sigCfg->BitPosition / 8] = *(const uint8*)SignalDataPtr;
			} else if (sigCfg->SignalType == COM_SIGNAL_SINT16)
			{
				uint16 value = *(const uint16*)SignalDataPtr;

				Com_TxBuffer[sigCfg->BitPosition/8]		= (uint8)(value&0xFF);
				Com_TxBuffer[sigCfg->BitPosition/8 + 1]	= (uint8)(value >> 8);
			}

			// trigger transmit
			return Com_TriggerTransmit(sigCfg->Ipduid);
		}
	}
	return E_NOT_OK;
}

// trigger transmit

Std_ReturnType Com_TriggerTransmit(Com_IpduIdType Ipduid)
{
	PduInfoType		pduInfo;

	pduInfo.SduDataPtr		= Com_TxBuffer;
	pduInfo.SduLength		= 8u;

	return PduR_ComTransmit((PduIdType)Ipduid, &pduInfo);
}
