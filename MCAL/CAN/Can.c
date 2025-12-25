/* =====================================================================================================================
 *  File        : Can.c
 *  Layer       : MCAL
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Depends     : - API for Can driver
 *  Notes       :
 * ===================================================================================================================*/
#include "Can.h"

static const Can_ConfigType* Can_ConfigPtr;
static Can_ControllerStateType Can_State;
static Can_PduType Can_TxPduPending;
static bool Can_TxPending = FALSE;
static uint8 Can_RxBuffer[8];

__attribute__((weak)) void Can_TxConfirmation( Can_HwHandleType Hth)
{
	(void)Hth;
}

__attribute__((weak)) void Can_RxIndication( Can_HwHandleType Hrh, const Can_PduType* PduInfo)
{
	(void)Hrh;
	(void)PduInfo;
}


void Can_Init(const Can_ConfigType* Config)
{
	Can_ConfigPtr = Config;

	// Enable Can clock
	RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;

	//Enter init mode
	CAN1->MCR |= CAN_MCR_INRQ;
	while(!(CAN1->MSR & CAN_MSR_INAK));

	// Config bit timing
	CAN1->BTR =
			((Config->ControllerConfig[0].baudrate->sjw - 1U ) << CAN_BTR_SJW_Pos)|
			((Config->ControllerConfig[0].baudrate->tseg2 - 1U ) << CAN_BTR_TS2_Pos)|
			((Config->ControllerConfig[0].baudrate->tseg1 - 1U ) << CAN_BTR_TS1_Pos)|
			((Config->ControllerConfig[0].baudrate->precscale - 1U ));
	/* Leave init mode */
	CAN1->MCR &= ~CAN_MCR_INRQ;
	while(CAN1->MSR & CAN_MSR_INAK);

	Can_State = CAN_CS_STOPPED;
}

Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Mode)
{
	if(Mode == CAN_CS_STARTED)
	{
		CAN1->MCR &= ~CAN_MCR_SLEEP;
		Can_State = CAN_CS_STARTED;
		return E_OK;
	}

	if(Mode == CAN_CS_STOPPED)
	{
		CAN1->MCR |= CAN_MCR_INRQ;
		Can_State = CAN_CS_STOPPED;
		return E_OK;
	}

	return E_NOT_OK;
}

Std_ReturnType Can_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo)
{
	if((CAN1->TSR & CAN_TSR_TME0) == 0U)
	{
		return E_NOT_OK;
	}

	Can_TxPduPending = *PduInfo;
	Can_TxPending = TRUE;

	CAN1->sTxMailBox[0].TIR = (PduInfo->Id << CAN_TI0R_STID_Pos);
	CAN1->sTxMailBox[0].TDTR = PduInfo->length;

	CAN1->sTxMailBox[0].TDLR =
			(PduInfo->sdu[3] << 24)|
			(PduInfo->sdu[2] << 16)|
			(PduInfo->sdu[1] << 8)|
			(PduInfo->sdu[0]);

	CAN1->sTxMailBox[0].TIR |= CAN_TI_TXRQ;

	return E_OK;
}

void Can_MainFunction_Tx(void)
{
	if(Can_TxPending == FALSE)
	{
		return;
	}

	if(CAN1->TSR & CAN_TSR_RQCP0)
	{
		// Clear flag
		CAN1->TSR |= CAN_TSR_RQCP0;

		Can_TxPending = FALSE;

		// Notify upper layer
		Can_TxConfirmation(0);
	}
}

void Can_MainFunction_Rx(void)
{
	if((CAN1->RF0R & CAN_RF0R_FMP0) == 0U)
	{
		return;
	}

	Can_PduType RxPdu;
	RxPdu.Id =
		(CAN1->sFIFOMailBox[0].RIR & CAN_RI0R_IDE) ?
		(CAN1->sFIFOMailBox[0].RIR >> CAN_TI0R_EXID_Pos):
		(CAN1->sFIFOMailBox[0].RIR >> CAN_TI0R_STID_Pos);

	RxPdu.length = CAN1->sFIFOMailBox[0].RDTR & 0x0FU;
	RxPdu.sdu = Can_RxBuffer;

	Can_RxBuffer[0] = (CAN1->sFIFOMailBox[0].RDLR >> 0) & 0xFF;
	Can_RxBuffer[1] = (CAN1->sFIFOMailBox[0].RDLR >> 8) & 0xFF;
	Can_RxBuffer[2] = (CAN1->sFIFOMailBox[0].RDLR >> 16) & 0xFF;
	Can_RxBuffer[3] = (CAN1->sFIFOMailBox[0].RDLR >> 24) & 0xFF;

	// Release FIFO
	CAN1->RF0R = CAN_RF0R_FMP0;

	Can_RxIndication(0, &RxPdu);
}
