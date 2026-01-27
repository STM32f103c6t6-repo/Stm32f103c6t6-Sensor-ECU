/* =====================================================================================================================
 *  File        : PduR_PBCfg.c
 *  Layer       : Service
 *  ECU         : STM32F103C6T6
 *  Purpose     : Config for PduR
 *  Depends     :
 * ===================================================================================================================*/

#include "PduR.h"


// Config CanIf to App
static const PduR_RxRouteType PduR_RxRoutes[] = {
		{
				.SrcModule	= PDUR_MODULE_CANIF,
				.SrcPduId	= 0,			// CanIF Rx PDU ID
				.DstModule	= PDUR_MODULE_APP,
				.DstPduId	= 0				// CanIf Tx PDU ID
		}
};

// Config app to canif
static const PduR_TxRouteType PduR_TxRoutes[] = {
		{
				.SrcModule	= PDUR_MODULE_APP,
				.SrcPduId	= 0,
				.DstModule	= PDUR_MODULE_CANIF,
				.DstPduId	= 0
		}
};

const PduR_ConfigTypes PduR_Config =
{
		.RxRoutes		= PduR_RxRoutes,
		.NumRxRoutes 	= sizeof(PduR_RxRoutes) / sizeof(PduR_RxRouteType),
		.TxRoutes		= PduR_TxRoutes,
		.NumTxRoutes	= sizeof(PduR_TxRoutes) / sizeof(PduR_TxRouteType)
};


