/* =====================================================================================================================
 *  File        : UartIf.h
 *  Layer       : Abstraction
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Provide API send/receiver Uart for services/applications
 *  Notes       :
 * ===================================================================================================================*/

#ifndef UARTIF_UARTIF_H_
#define UARTIF_UARTIF_H_

#if __cplusplus
extern "C" {
#endif

#include "Std_Types.h"
#include "Uart.h"
#include "Uart_Cfg.h"
#include "Det.h"

/* =====================================================================================================================
 *  Version & IDs
 * ===================================================================================================================*/
#define UARTIF_VENDOR_ID					(0x00u)
#define UARTIF_MODULE_ID					(0x00u)
#define UARTIF_INSTANCE_ID					(0x00u)

#define UARTIF_AR_RELEASE_MAJOR_VERSION		(4u)
#define UARTIF_AR_RELEASE_MINOR_VERSION		(3u)
#define UARTIF_AR_RELEASE_PATCH_VERSION		(1u)

#define UARTIF_SW_MAJOR_VERSION				(1u)
#define UARTIF_SW_MINOR_VERSION				(0u)
#define UARTIF_SW_PATCH_VERSION				(0u)

/* =====================================================================================================================
 *  DEVELOPMENT SWITCHES
 * ===================================================================================================================*/
#ifndef UARTIF_DEV_ERROR_DETECT
#define UARTIF_DEV_ERROR_DETECT				STD_OFF
#endif

#ifndef UARTIF_DEFAULT_CRLF
#define UARTIF_DEFAULT_CRLF					1u
#endif

/* =====================================================================================================================
 *  API ID
 * ===================================================================================================================*/
#define UARTIF_API_ID_INIT					(0x00u)
#define UARTIF_API_ID_DEINIT				(0x01u)
#define UARTIF_API_ID_WRITE					(0x02u)
#define UARTIF_API_ID_WRITECHAR				(0x03u)
#define UARTIF_API_ID_WRITELINE				(0x04u)
#define UARTIF_API_ID_READ					(0x05u)
#define UARTIF_API_ID_READ_NONBLOCK			(0x06u)
#define UARTIF_API_ID_REG_RXCB				(0x07u)
#define UARTIF_API_ID_REG_TXCB				(0x08u)
#define UARTIF_API_ID_MAINFUNCTION			(0x09u)
#define UARTIF_API_ID_ISTXBUSY				(0x0Au)
#define UARTIF_API_ID_ISINIT				(0x0Bu)

/* ==============================
 *         DET ERROR CODES
 * ============================== */
#define UARTIF_E_UNINIT						(0x01u)
#define UARTIF_E_PARAM_POINTER				(0x02u)
#define UARTIF_E_PARAM_CHANNEL				(0x03u)
#define UARTIF_E_BUSY						(0x04u)
#define UARTIF_E_TIMEOUT					(0x05u)
#define UARTIF_E_NULL_CALLBACK				(0x06u)

#if(UARTIF_DEV_ERROR_DETECT == STD_ON)
#define UARTIF_DET_REPORT(_apiId, _err)\
	Det_ReportError(UARTIF_MODULE_ID,UARTIF_INSTANCE_ID,(_apiId),(_err));
#else
#define UARTIF_DET_REPORT(_apiID, _err) ((void)0);
#endif

/* =====================================================================================================================
 *  TYPES
 * ===================================================================================================================*/

// Callback when has new data
typedef void (*UartIf_RxIndicationType) (const uint8* DataPtr, uint16 Length);

//Callback when complete transmitted
typedef void (*UartIf_TxConfirmationType) (void);

// Config UART channel
typedef struct
{
	Uart_ChannelType		ChannelId;
	uint32					Baudrate;
	boolean					UseDma;
} UartIf_ChannelCfgType;

typedef struct
{
	const UartIf_ChannelCfgType* Channels;
	uint8						 ChannelCount;
	uint8						 DefaultChannelIndex;
} UartIf_ConfigType;

/* =====================================================================================================================
 *  API
 * ===================================================================================================================*/
// Init Interface
void UartIf_Init(const UartIf_ConfigType* CfgPtr);
void UartIf_DeInit(void); // Clear state to uninit
boolean UartIf_IsInitialized(void); // Check init state
Std_ReturnType UartIf_Write(const uint8* DataPtr, uint16 Length);
Std_ReturnType UartIf_WriteChar(uint8 ch); //send a char
Std_ReturnType UartIf_WriteLine(const char* CStr); // send a string
Std_ReturnType UartIf_Read(uint8* BufPtr, uint16 BufSize, uint16* OutLen); // Read data
Std_ReturnType UartIf_ReadNonBlocking(uint8* BufPtr, uint16 BufSize, uint16* OutLen); // Read non-blocking
Std_ReturnType UartIf_RegisterRxIndication(UartIf_RxIndicationType RxCb); //Register Callback Rx
Std_ReturnType UartIf_RegisterTxConfirmation(UartIf_TxConfirmationType TxCb); //Register Callback Tx
boolean UartIf_IsTxBusy(void); //check Tx busy
void UartIf_MainFunction(void);

/* =====================================================================================================================
 *  OPTIONAL MULTI-CHANNEL
 * ===================================================================================================================*/
Std_ReturnType UartIf_WriteCh(Uart_ChannelType Channel, const uint8* DataPtr, uint16 Length);
Std_ReturnType UartIf_ReadCh(Uart_ChannelType Channel, uint8* BufPtr, uint16 BufSize, uint16* OutLen);
Std_ReturnType UartIf_RegisterRxIndicationCh(Uart_ChannelType Channel,UartIf_RxIndicationType RxCb);
Std_ReturnType UartIf_RegisterTxConfirmationCh(Uart_ChannelType Channel,UartIf_TxConfirmationType TxCb);
boolean UartIf_IsTxBusyCh(Uart_ChannelType Channel);

/* =========================================================
 * 	 Global configuration
 * =======================================================*/
extern const UartIf_ConfigType UartIf_Config;

#ifdef __cplusplus
}
#endif


#endif /* UARTIF_UARTIF_H_ */
