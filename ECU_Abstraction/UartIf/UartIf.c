/* =====================================================================================================================
 *  File        : UartIf.c
 *  Layer       : Abstraction
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Apply UART declare API for STM32F1
 *  Depends     :
 * ===================================================================================================================*/

#include "UartIf.h"
#include "Uart.h"
#include <string.h>

#ifndef UARTIF_MCAL_WRITE
#define UARTIF_MCAL_WRITE(_ch, _buf, _len)			Uart_WriteAsync((_ch), (_buf), (_len))
#endif

#ifndef UARTIF_MCAL_REGISTER_CALLBACKS
#define UARTIF_MCAL_REGISTER_CALLBACKS(_ch, _pcbs)	((void)(_ch), (void)(_pcbs),(void)0)
#endif

#ifndef UARTIF_MCAL_ISTXBUSY
#define UARTIF_MCAL_ISTXBUSY(_ch)					Uart_IsTxBusy((_ch))
#endif

#ifndef UARTIF_RX_RING_SIZE
#define UARTIF_RX_RING_SIZE		(256u)
#endif

/* =====================================================================================
 *     Config and State
 * ===================================================================================== */
typedef struct {
	volatile uint16 head;
	volatile uint16 tail;
	uint8			data[UARTIF_RX_RING_SIZE];
} UartIf_RxRingType;

// Config init
static const UartIf_ConfigType* UartIf_CfgPtr = NULL_PTR;
// Init state
static bool UartIf_Inited = FALSE;

// Callback for application or logger
static UartIf_RxIndicationType		UartIf_RxCb_Default = NULL_PTR;
static UartIf_TxConfirmationType 	UartIf_TxCb_Default = NULL_PTR;

// Ring-buffer for default channel
static UartIf_RxRingType UartIf_RxRing_Default = {0u,0u};

/* =====================================================================================
 *     RING-BUFFER HELPERS
 * ===================================================================================== */
static inline uint8 ring_is_empty(const UartIf_RxRingType* rb)
{
	return (rb->head == rb->tail);
}

static inline uint8 ring_is_full(const UartIf_RxRingType* rb)
{
	return ((uint16)((rb->head + 1u) % UARTIF_RX_RING_SIZE) == rb->tail);
}

static inline void ring_push(UartIf_RxRingType* rb, uint8 byte)
{
	uint16 next = (uint16)((rb->head +1u) % UARTIF_RX_RING_SIZE);
	if(next == rb->tail)
	{
		//full
		rb->tail = (uint16)((rb->tail + 1u) % UARTIF_RX_RING_SIZE);
	}
	rb->data[rb->head] = byte;
	rb->head = next;
}

static inline uint16 ring_pop_many(UartIf_RxRingType* rb, uint8* buf, uint16 maxlen)
{
	uint16 cnt = 0u;
	while((cnt < maxlen) && !ring_is_empty(rb))
	{
		buf[cnt++] = rb->data[rb->tail];
		rb->tail = (uint16)((rb->tail + 1u) % UARTIF_RX_RING_SIZE);
	}
	return cnt;
}

/* =====================================================================================
 *     WRAPPER for CALLBACK of MCAL
 * ===================================================================================== */
static void UartIf_McalRxCb(Uart_ChannelType ch, uint16 data)
{
	(void)ch;
	ring_push(&UartIf_RxRing_Default, (uint8)(data & 0xFFu));
}

static void UartIf_McalTxCb(Uart_ChannelType ch)
{
	(void)ch;
	if(UartIf_TxCb_Default != NULL_PTR)
	{
		UartIf_TxCb_Default();
	}
}

static void UartIf_McalErrCb(Uart_ChannelType ch, Uart_ErrorFlagType err)
{
	(void) ch; (void) err;
}

/* =====================================================================================
 *     Initial/Clear
 * ===================================================================================== */
void UartIf_Init(const UartIf_ConfigType* CfgPtr)
{
#if(UARTIF_DEV_ERROR_DETECT == STD_ON)
	if(CfgPtr == NULL_PTR)
	{
		UARTIF_DET_REPORT(UARTIF_API_ID_INIT, UARTIF_E_PARAM_POINTER);
		return;
	}
#endif

	UartIf_CfgPtr = CfgPtr;
	UartIf_Inited = TRUE;

	//reset ring/callback runtime
	UartIf_RxRing_Default.head 	= 0u;
	UartIf_RxRing_Default.tail 	= 0u;
	UartIf_RxCb_Default		   	= NULL_PTR;
	UartIf_TxCb_Default			= NULL_PTR;

	// Register callback with MCAL
	Uart_CallbacksType cbs;
	cbs.onTxEmptyOrCplt = UartIf_McalTxCb;
	cbs.onRxChar		= UartIf_McalRxCb;
	cbs.onError			= UartIf_McalErrCb;

	if((CfgPtr != NULL_PTR) && (CfgPtr->ChannelCount > 0u))
	{
		Uart_ChannelType defCh = CfgPtr->Channels[CfgPtr->DefaultChannelIndex].ChannelId;
		UARTIF_MCAL_REGISTER_CALLBACKS(defCh,&cbs);
	}
}

void UartIf_DeInit(void)
{
#if (UARTIF_DEV_ERROR_DETECT == STD_ON)
	if(UartIf_Inited == FALSE){
		UARTIF_DET_REPORT(UARTIF_API_ID_DEINIT, UARTIF_E_UNINIT)
	}
#endif
	UartIf_Inited = FALSE;
	UartIf_CfgPtr = NULL_PTR;
	UartIf_RxCb_Default = NULL_PTR;
	UartIf_TxCb_Default = NULL_PTR;
}

bool UartIf_IsInitialized(void)
{
	return UartIf_Inited;
}

/* =====================================================================================
 *     Transfer/Receiver
 * ===================================================================================== */
Std_ReturnType UartIf_Write(const uint8* DataPtr, uint16 Length)
{
#if(UARTIF_DEV_ERROR_DETECT == STD_ON )
	if(UartIf_Inited == FALSE){
		UARTIF_DET_REPORT(UARTIF_API_ID_WRITE, UARTIF_E_UNINIT);
		return E_NOT_OK;
	}
	if( (DataPtr == NULL_PTR) || (Length == 0u) ){
		UARTIF_DET_REPORT(UARTIF_API_ID_WRITE, UARTIF_E_PARAM_POINTER);
		return E_NOT_OK;
	}
	if((UartIf_CfgPtr == NULL_PTR) | (UartIf_CfgPtr->ChannelCount == 0u)){
		UARTIF_DET_REPORT(UARTIF_API_ID_WRITE, UARTIF_E_PARAM_CHANNEL);
		return E_NOT_OK;
	}
#endif
	const Uart_ChannelType ch = UartIf_CfgPtr->Channels[UartIf_CfgPtr->DefaultChannelIndex].ChannelId;

	if(UARTIF_MCAL_ISTXBUSY(ch)){
#if (UARTIF_DEV_ERROR_DETECT == STD_ON)
		UARTIF_DET_REPORT(UARTIF_API_ID_WRITE, UARTIF_E_BUSY);
#endif
		return E_NOT_OK;
	}
	return UARTIF_MCAL_WRITE(ch, DataPtr, Length);
}

Std_ReturnType UartIf_WriteChar(uint8 ch8)
{
	return UartIf_Write(&ch8, 1u);
}

Std_ReturnType UartIf_WriteLine(const char* CStr)
{
#if(UARTIF_DEV_ERROR_DETECT == STD_ON)
	if(UartIf_Inited == FALSE)
	{
		UARTIF_DET_REPORT(UARTIF_API_ID_WRITELINE, UARTIF_E_UNINIT);
		return E_NOT_OK;
	}
	if(CStr == NULL_PTR)
	{
		UARTIF_DET_REPORT(UARTIF_API_ID_WRITELINE, UARTIF_E_PARAM_POINTER);
		return E_NOT_OK;
	}
#endif
	Std_ReturnType ret = UartIf_Write((const uint8*) CStr, (uint16)strlen(CStr));
	if(ret != E_OK) return ret;
#if (UARTIF_DEFAULT_CRLF == 1)
	static const uint8 crlf[2] = {'r','n'};
	ret = UartIf_Write(crlf,2u);
#else
	static const uint8 lf[1] = {'\n'};
	ret = UartIf_Write(lf,1u);
#endif
	return ret;
}

Std_ReturnType UartIf_Read(uint8* BufPtr, uint16 BufSize, uint16* OutLen)
{
#if(UARTIF_DEV_ERROR_DETECT == STD_ON)
	if(UartIf_Inited == FALSE)
	{
		UARTIF_DET_REPORT(UARTIF_API_ID_READ,UARTIF_E_UNINIT);
		return E_NOT_OK;
	}
	if((BufPtr == NULL_PTR) || (OutLen == NULL_PTR) || (BufSize == 0u))
	{
		UARTIF_DET_REPORT(UARTIF_API_ID_READ, UARTIF_E_PARAM_POINTER);
		return E_NOT_OK;
	}
#endif
	*OutLen = ring_pop_many(&UartIf_RxRing_Default, BufPtr, BufSize);
	return (*OutLen > 0u) ? E_OK : E_NOT_OK;
}

Std_ReturnType UartIf_ReadNonBlocking(uint8* BufPtr, uint16 BufSize, uint16* OutLen)
{
	return UartIf_Read(BufPtr, BufSize, OutLen);
}

/* =====================================================================================
 * 		Register CALLBACK
 * ===================================================================================== */
Std_ReturnType UartIf_RegisterRxIndication(UartIf_RxIndicationType RxCb)
{
#if(UARTIF_DEV_ERROR_DETECT == STD_ON)
	if(UartIf_Inited == FALSE)
	{
		UARTIF_DET_REPORT(UARTIF_API_ID_REG_RXCB,UARTIF_E_UNINIT);
		return E_NOT_OK;
	}
	if(RxCb == NULL_PTR)
	{
		UARTIF_DET_REPORT(UARTIF_API_ID_REG_RXCB, UARTIF_E_NULL_CALLBACK);
		return E_NOT_OK;
	}
#endif
	UartIf_RxCb_Default = RxCb;
	return E_OK;
}

Std_ReturnType UartIf_RegisterTxConfirmation(UartIf_TxConfirmationType TxCb)
{
#if(UARTIF_DEV_ERROR_DETECT == STD_ON)
	if(UartIf_Inited == FALSE)
	{
		UARTIF_DET_REPORT(UARTIF_API_ID_REG_TXCB,UARTIF_E_UNINIT);
		return E_NOT_OK;
	}
	if(TxCb == NULL_PTR)
	{
		UARTIF_DET_REPORT(UARTIF_API_ID_REG_TXCB, UARTIF_E_NULL_CALLBACK);
		return E_NOT_OK;
	}
#endif
	UartIf_TxCb_Default = TxCb;
	return E_OK;
}

/* =====================================================================================
 * 		State Tx
 * ===================================================================================== */
bool UartIf_IsTxBusy(void)
{
#if(UARTIF_DEV_ERROR_DETECT == STD_ON)
	if(UartIf_Inited == FALSE)
	{
		UARTIF_DET_REPORT(UARTIF_API_ID_ISTXBUSY,UARTIF_E_UNINIT);
		return E_NOT_OK;
	}
	if((UartIf_CfgPtr == NULL_PTR) || (UartIf_CfgPtr -> ChannelCount == 0u))
	{
		UARTIF_DET_REPORT(UARTIF_API_ID_ISTXBUSY, UARTIF_E_PARAM_CHANNEL);
		return E_NOT_OK;
	}
#endif
	const Uart_ChannelType ch = UartIf_CfgPtr->Channels[UartIf_CfgPtr->DefaultChannelIndex].ChannelId;
	return (bool) UARTIF_MCAL_ISTXBUSY(ch);
}

void UartIf_MainFunction(void)
{
#if(UARTIF_DEV_ERROR_DETECT == STD_ON)
	if(UartIf_Inited == FALSE)
	{
		UARTIF_DET_REPORT(UARTIF_API_ID_MAINFUNCTION,UARTIF_E_UNINIT);
		return ;
	}
#endif
	if(UartIf_RxCb_Default != NULL_PTR)
	{
		uint8 chunk[32];
		uint16 got = ring_pop_many(&UartIf_RxRing_Default, chunk, (uint16)sizeof(chunk));
		if(got > 0u)
		{
			UartIf_RxCb_Default(chunk,got);
		}
	}
}

/* =====================================================================================================================
 *  OPTIONAL MULTI-CHANNEL
 * ===================================================================================================================*/
Std_ReturnType UartIf_WriteCh(Uart_ChannelType Channel, const uint8* DataPtr, uint16 Length)
{
#if(UARTIF_DEV_ERROR_DETECT == STD_ON)
	if(UartIf_Inited == FALSE)
	{
		UARTIF_DET_REPORT(UARTIF_API_ID_WRITE,UARTIF_E_UNINIT);
		return E_NOT_OK;
	}
	if((DataPtr == NULL_PTR) || (Length == 0u))
	{
		UARTIF_DET_REPORT(UARTIF_API_ID_WRITE, UARTIF_E_PARAM_POINTER);
		return E_NOT_OK;
	}
#endif
	if(UARTIF_MCAL_ISTXBUSY(Channel)){
#if(UARTIF_DEV_ERROR_DETECT == STD_ON)
		UARTIF_DET_REPORT(UARTIF_API_ID_WRITE,UARTIF_E_BUSY);
#endif
		return E_NOT_OK;
	}
	return UARTIF_MCAL_WRITE(Channel,DataPtr,Length);
}

Std_ReturnType UartIf_ReadCh(Uart_ChannelType Channel, uint8* BufPtr, uint16 BufSize, uint16* OutLen)
{
	(void)Channel;
	return UartIf_Read(BufPtr, BufSize, OutLen);
}

Std_ReturnType UartIf_RegisterRxIndicationCh(Uart_ChannelType Channel,UartIf_RxIndicationType RxCb)
{
	(void)Channel;
	return UartIf_RegisterRxIndication(RxCb);
}

Std_ReturnType UartIf_RegisterTxConfirmationCh(Uart_ChannelType Channel,UartIf_TxConfirmationType TxCb)
{
	(void)Channel;
	return UartIf_RegisterTxConfirmation(TxCb);
}

bool UartIf_IsTxBusyCh(Uart_ChannelType Channel)
{
	return (bool)UARTIF_MCAL_ISTXBUSY(Channel);
}
