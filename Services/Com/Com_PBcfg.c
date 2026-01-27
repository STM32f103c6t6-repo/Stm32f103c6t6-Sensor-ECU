/* =====================================================================================================================
 *  File        : Com_PBcfg.c
 *  Layer       : Service
 *  ECU         : STM32F103C6T6
 *  Purpose     : Config for Com
 *  Depends     :
 * ===================================================================================================================*/

#include "Com.h"

// IPDU Buffer

const Com_SignalConfigType	Com_SignalConfigList[] =
{
		{
			COM_SIGNAL_ID_SPEED,
			COM_IPDU_ID_TX_VEHICLE,
			COM_SIGNAL_UINT16,
			0U,
			16U,
			COM_LITTLE_ENDIAN
		},

		{
			COM_SIGNAL_ID_DISTANCE,
			COM_IPDU_ID_RX_SENSOR,
			COM_SIGNAL_UINT16,
			0U,
			16U,
			COM_LITTLE_ENDIAN
		}
};

// Rx IPDU Config
const Com_RxIpduConfigType Com_RxIpduConfigList[] =
{
		{
			COM_IPDU_ID_RX_SENSOR,
			8U
		}
};

// Tx IPDU Config
const Com_TxIpduConfigType Com_TxIpduConfigList[] =
{
		{
			COM_IPDU_ID_TX_VEHICLE,
			8U
		}
};

// Global COM Config
const Com_ConfigType Com_Config =
{
		.SignalConfig	= Com_SignalConfigList,
		.NumSignals		= (uint16)(sizeof(Com_SignalConfigList) / sizeof(Com_SignalConfigType)),

		.RxIpduConfig	= Com_RxIpduConfigList,
		.NumRxIpdu		= (uint16)(sizeof(Com_RxIpduConfigList) / sizeof(Com_RxIpduConfigType)),

		.TxIpduConfig	= Com_TxIpduConfigList,
		.NumTxIpdu		= (uint16)(sizeof(Com_RxIpduConfigList) / sizeof(Com_TxIpduConfigType)),
};

