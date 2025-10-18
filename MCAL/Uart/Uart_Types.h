/* =====================================================================================================================
 *  File        : Uart_Types.h
 *  Layer       : MCAL (UART)
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Define data structure for Uart driver
 *  Notes       :
 * ===================================================================================================================*/

#ifndef UART_UART_TYPES_H_
#define UART_UART_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------------------------
 *	Version
 * --------------------------------------------------------- */
#define UART_TYPES_AR_MAJOR_VERSION		(1u)
#define UART_TYPES_AR_MINOR_VERSION		(0u)
#define UART_TYPES_AR_PATCH_VERSION		(0u)

#include <stdint.h>
#include <stdbool.h>

#ifndef E_OK
typedef uint8_t		Std_ReturnType;
#define E_OK		((Std_ReturnType)0x00u)
#define E_NOT_OK	((Std_ReturnType)0x01u)
#endif

/* ---------------------------------------------------------
 *	Instance/Channel
 * --------------------------------------------------------- */
typedef enum
{
	UART_CH1 = 0u, /* USART1 */
	UART_CH2,
	UART_CH3,
	UART_CH_COUNT
} Uart_ChannelType;

/* Driver state*/
typedef enum
{
	UART_UNINIT	= 0u,
	UART_INIT
} Uart_DriverStatusType;

/* ---------------------------------------------------------
 *	Parameter trans
 * --------------------------------------------------------- */
typedef enum
{
	UART_WORDLEN_8B = 8u,
	UART_WORDLEN_9B = 9u
} Uart_WordLengthType;

typedef enum
{
	UART_STOPBITS_1 = 1u,
	UART_STOPBITS_2 = 2u
}Uart_StopBitsType;

//parity mode
typedef enum {
	UART_PARITY_NONE = 0,
	UART_PARITY_EVEN,
	UART_PARITY_ODD
} Uart_ParityModeType;

// Flow control
typedef enum
{
	UART_FLOW_NONE = 0u,
	UART_FLOW_RTS_CTS
} Uart_Flow_CtrlType;

typedef enum
{
	UART_OVERSAMPLING_16 = 0u,
	UART_OVERSAMPLING_8
} Uart_OversamplingType;

/* Method of transmission */
typedef enum
{
	UART_XFER_POLLING = 0u, //blocking/polling
	UART_XFER_INTERRUPT,	// use IRQ and ring buffer
	UART_XFER_DMA
} Uart_TransferModeType;

/* Enable/Disable Tx/Rx */
typedef struct
{
	uint8_t txEnable;
	uint8_t rxEnable;
} Uart_DirectionCfgType;

/* ---------------------------------------------------------
 *	Ring buffer & Callback
 * --------------------------------------------------------- */
/* State/Type of error*/
typedef enum
{
	UART_ERR_NONE			= 0x00u,
	UART_ERR_OVERRUN		= 0x01u,	// ORE
	UART_ERR_NOISE			= 0x02u,	// NF
	UART_ERR_FRAMING		= 0x03u,
	UART_ERR_PARITY			= 0x04u,
} Uart_ErrorFlagType;

typedef struct
{
	uint32_t	txBytes;
	uint32_t	rxBytes;
	uint32_t	txIrqCount;
	uint32_t	rxIrqCount;
	uint32_t	rxOverrunCount;
	uint32_t	framingErrCount;
	uint32_t	parityErrCount;
} Uart_StatsType;

/* Prototype callbacks*/
typedef void (*Uart_TxCompleteNotifType)(Uart_ChannelType ch);
typedef void (*Uart_RxCompleteNotifType)(Uart_ChannelType ch, uint16_t data);
typedef void (*Uart_ErrorNotifyType)(Uart_ChannelType ch, Uart_ErrorFlagType errFlags);

/* Callback register follow instance*/
typedef struct
{
	Uart_TxCompleteNotifType onTxEmptyOrCplt;
	Uart_RxCompleteNotifType onRxChar;
	Uart_ErrorNotifyType	 onError;
} Uart_CallbacksType;

/* ---------------------------------------------------------
 *	Config per-instance & all
 * --------------------------------------------------------- */
/* Config for each instance */
typedef struct
{
	uint32_t 				baurate;
	uint8_t 				wordlength;		// 8 or 9
	uint8_t					stopBits;		//1 or 2
	Uart_ParityModeType		parity;
	Uart_Flow_CtrlType 		flow;

	Uart_DirectionCfgType	dir;
	Uart_TransferModeType	transMode;

	uint8_t					TxEnable;		//enable Tx
	uint8_t 				RxEnable;		//enable Rx
	uint8_t					useInterrupts;	// 1: IRQ, 0:polling
	uint8_t					useDmaTx;		//1:DMA Tx
	uint8_t 				useDmaRx;		//1: DMA Rx

	uint16_t 				txBufSize;		// ring buffer size
	uint16_t 				rxBufSize;
	uint16_t 				txTimeoutMs;	//timeout for API blocking
	uint16_t 				rxTimeoutMs;

	Uart_CallbacksType	cbs;
} Uart_ChannelConfigType;

// Config all instance
typedef struct
{
	uint8_t useCh1;
	uint8_t useCh2;
	uint8_t useCh3;

	Uart_ChannelType logPort;
	Uart_ChannelType diagPort;

	Uart_ChannelConfigType usart1;
	Uart_ChannelConfigType usart2;
	Uart_ChannelConfigType usart3;
} Uart_ConfigType;

/* ---------------------------------------------------------
 *	Handle runtime
 * --------------------------------------------------------- */
typedef struct
{
	volatile uint16_t 	head;
	volatile uint16_t 	tail;
	uint16_t			size;
	uint8_t*			buf;
} Uart_RingBufferType;

typedef struct
{
	Uart_DriverStatusType	status;
	Uart_StatsType			stats;

	Uart_RingBufferType		txRb;
	Uart_RingBufferType		rxRb;

	uint8_t					parityEnable;
	uint8_t					wordLen9b;
} Uart_ChannelHandleType;

/* ---------------------------------------------------------
 *	Version info
 * --------------------------------------------------------- */
typedef struct
{
	uint16_t	vendorID;
	uint16_t	moduleID;
	uint16_t	sw_major_version;
	uint16_t	sw_minor_version;
	uint16_t	sw_patch_verion;
} Uart_VersionInfoType;

#ifdef __cplusplus
}
#endif


#endif /* UART_UART_TYPES_H_ */
