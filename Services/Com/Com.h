/* =====================================================================================================================
 *  File        : Com.h
 *  Layer       : Service
 *  ECU         : STM32F103C6T6
 *  Purpose     : Config type and Api for COM module
 *  Depends     :
 * ===================================================================================================================*/


#ifndef COM_COM_H_
#define COM_COM_H_

#include <Std_Types.h>
#include <ComStack_Types.h>

/* -------------------------- Signal Config ------------------------- */
// Signal ID type
typedef uint16 Com_SignalIdType;

// I-PDU ID type
typedef uint16 Com_IpduIdType;

/*
 * Signal endianness
 */
typedef enum
{
	COM_BIG_ENDIAN = 0,
	COM_LITTLE_ENDIAN
} Com_SignalEndianessTypes;

/*
 * Signal data type
 */
typedef enum
{
	COM_SIGNAL_UINT8	= 0,
	COM_SIGNAL_UINT16	,
	COM_SIGNAL_UINT32	,
	COM_SIGNAL_SINT8	,
	COM_SIGNAL_SINT16	,
	COM_SIGNAL_SINT32
} Com_SignalTypeEnum;

/*
 *  Signal configuration structure
 */
typedef struct
{
	Com_SignalIdType			SignalId;
	Com_IpduIdType				Ipduid;
	Com_SignalTypeEnum			SignalType;
	uint8						BitPosition;
	uint8						BitSize;
	Com_SignalEndianessTypes	Endianness;
} Com_SignalConfigType;

/* ------------------------ I-PDU Configuration --------------------*/
/*
 * Rx I-PDU configuration
 */
typedef struct
{
	Com_IpduIdType				IpduId;
	uint16						PduLength;
} Com_RxIpduConfigType;

/*
 * Tx I-PDU configuration
 */
typedef struct
{
	Com_IpduIdType				IpduId;
	uint16						PduLength;
} Com_TxIpduConfigType;

/* --- COM global configuration --------*/
typedef struct
{
	const Com_SignalConfigType*			SignalConfig;
	uint16								NumSignals;

	const Com_RxIpduConfigType*			RxIpduConfig;
	uint16								NumRxIpdu;

	const Com_TxIpduConfigType*			TxIpduConfig;
	uint16								NumTxIpdu;
} Com_ConfigType;

// Initialize Com module
void Com_Init(const Com_ConfigType* ConfigPtr);

/*
 * Indication of a received I-PDU from PduR
 */
void Com_RxIndication(
		PduIdType RxPduId,
		const PduInfoType* PduInfoPtr
);

/*
 * Transmission confirmation
 */
void Com_TxConfirmation(PduIdType TxPduId);

/*
 * Send a signal
 */
Std_ReturnType Com_SendSignal(
		Com_SignalIdType	SignalId,
		const void* SignalDataPtr
);

/* ------------------------ Com Parameter config--------------------*/

// COM Ids
#define COM_SIGNAL_ID_SPEED			((Com_SignalIdType)0U)
#define COM_SIGNAL_ID_DISTANCE		((Com_SignalIdType)1U)
#define COM_SIGNAL_ID_OBSTACLE		((Com_SignalIdType)2u)

// IPDU IDs
#define COM_IPDU_ID_TX_VEHICLE		((Com_IpduIdType)0U)
#define COM_IPDU_ID_RX_SENSOR		((Com_IpduIdType)0U)

extern const Com_ConfigType			Com_Config;

#endif /* COM_COM_H_ */
