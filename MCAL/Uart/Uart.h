/* =====================================================================================================================
 *  File        : Uart.h
 *  Layer       : MCAL (UART)
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : API Public for Uart driver (USART1/2/3)
 *  Depends     : Uart_Types.h, Uart_Cfg.h/.c
 * ===================================================================================================================*/

#ifndef UART_UART_H_
#define UART_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

/* =========================================================
 *  Version
 * =======================================================*/
#define Uart_AR_MAJOR_VERSION		(1u)
#define Uart_AR_MINOR_VERSION		(0u)
#define Uart_AR_PATCH_VERSION		(0u)

#include "Uart_Types.h"

/* =========================================================
 *  Service IDs (for DET/Logger)
 * =======================================================*/
#define UART_SID_INIT					(0x00u)
#define UART_SID_DEINIT					(0x01u)
#define UART_SID_WRITE					(0x02u)
#define UART_SID_READ					(0x03u)
#define UART_SID_WRITE_ASYNC			(0x04u)
#define UART_SID_READ_ASYNC				(0x05u)
#define UART_SID_PUTCHAR				(0x06u)
#define UART_SID_GETCHAR				(0x07u)
#define UART_SID_FLUSHTX				(0x08u)
#define UART_SID_SETBAURD				(0x09u)
#define UART_SID_SETDIRECTION			(0x0Au)
#define UART_SID_REGISTER_CALLBACK		(0x0Bu)
#define UART_SID_GETSTATS				(0x0Cu)
#define UART_SID_CLEARSTATS				(0x0Du)
#define UART_SID_GETVERSIONINFO			(0x0Eu)
#define UART_SID_IRQHANDLER				(0x0Fu)

/* =========================================================
 *  DET Error code
 * =======================================================*/
#define UART_E_UNINIT					(0x01u) 	// Call API when not init
#define UART_E_PARAM_POINTER			(0x02u)		// Null pointer
#define UART_E_PARAM_CHANNEL			(0x03u)		// Channel is invalid
#define UART_E_BUSY						(0x05u)		// Channel is busy
#define UART_E_TIMEOUT					(0x06u)		// Timout
#define UART_E_BUFFER_FULL				(0x07u)		// full ring buffer
#define UART_E_BUFFER_EMPTY				(0x08u)		// ring buffer is empty
#define UART_E_NOT_SUPPORTED			(0x07u)		// API not support current mode

/* =========================================================
 *  position of Bit Error in Sr reg
 * =======================================================*/
#define UART_SR_PE						(0x01u)
#define UART_SR_FE						(0x02u)
#define UART_SR_NE						(0x04u)
#define UART_SR_ORE						(0x08u)
#define UART_ERR_MASK					(UART_SR_PE | UART_SR_FE | UART_SR_NE | UART_SR_ORE)

/* =========================================================
 *  Baudrate config
 * =======================================================*/
#define UART1_BAUD 						115200
#define UART2_BAUD 						115200
#define UART3_BAUD 						115200

/* =========================================================
 *  Build options
 * =======================================================*/
#ifndef		UART_CFG_ENABLE_BLOCKING_APIS
#define		UART_CFG_ENABLE_BLOCKING_APIS (1u)
#endif

#ifndef		UART_CFG_ENABLE_ASYNC_APIS
#define		UART_CFG_ENABLE_ASYNC_APIS (1u)
#endif

#ifndef		UART_CFG_ENABLE_STATS
#define		UART_CFG_ENABLE_STATS (1u)
#endif

#ifndef		UART_CFG_EOL
#define		UART_CFG_EOL "\r\n"
#endif


/* =========================================================
 * 	 Global configuration
 * =======================================================*/
extern const Uart_ConfigType Uart_Config;

/* =========================================================
 * 	 API
 * =======================================================*/
/**
 * @brief  Initialization Uart driver follow Uart_Config.
 * @return E_OK/E_NOT_OK
 */
Std_ReturnType Uart_Init(const Uart_ConfigType* cfg);

/**
 * @brief  pause all channel which enable.
 */
void Uart_Deinit(void);

#if (UART_CFG_ENABLE_BLOCKING_APIS == 1u)
/**
 * @brief  Send sync buffer, wait for completion
 */
Std_ReturnType Uart_Write(Uart_ChannelType ch, const uint8* data, uint16 len, uint32 timeoutMs);

/**
 * @brief  received sync buffer, wait for completion
 */
Std_ReturnType Uart_Read(Uart_ChannelType ch, uint8* data, uint16 len, uint32 timeoutMs);

/**
 * @brief  Send a char
 */
Std_ReturnType Uart_PutChar(Uart_ChannelType ch, uint8 byte, uint32 timeoutMs);

/**
 * @brief  Received a char
 */
Std_ReturnType Uart_GetChar(Uart_ChannelType ch, uint8* outByte, uint32 timeoutMs);
#endif

#if( UART_CFG_ENABLE_ASYNC_APIS == 1u)

/**
 * @brief  Put data to TX ring buffer.
 * @return E_OK if all are filled; E_NOT_OK if buffer is out of space.
 */
Std_ReturnType Uart_WriteAsync(Uart_ChannelType ch, const uint8* data, uint16 len);

/**
 * @brief  get data from TX ring buffer.
 * @return number of bits.
 */
uint16 Uart_ReadAsync(Uart_ChannelType ch, uint8* data, uint16 len);

/**
 * @brief  request flush Tx
 */
Std_ReturnType Uart_FlushTx(Uart_ChannelType ch, uint32 timeoutMs);
#endif

/**
 * @brief  change baudrate  runtime
 */
Std_ReturnType Uart_SetBaudrate(Uart_ChannelType ch, uint32 baud);

/**
 * @brief  Enable/Disable Tx/Rx Direction
 */
Std_ReturnType Uart_SetDirection(Uart_ChannelType ch, Uart_DirectionCfgType dir);

/**
 * @brief  Register callback
 */
Std_ReturnType Uart_RegisterCallbacks(Uart_ChannelType ch, const Uart_CallbacksType* cbs);

#if(UART_CFG_ENABLE_STATS == 1u)
/**
 * @brief  Get statistical snapshots
 */
Std_ReturnType Uart_GetStats(Uart_ChannelType ch, Uart_StatsType* out);

/**
 * @brief  Get statistical snapshots
 */
void Uart_ClearStats(Uart_ChannelType ch);
#endif

/**
 * @brief  Get information version
 */
void Uart_GetVersionInfo(Uart_VersionInfoType* vi);

/**
 * @brief  IRQ entry points
 */
void Uart_IrqHandler(Uart_ChannelType ch);

/**
 * @brief  Check Tx is Busy now
 */
bool Uart_IsTxBusy(Uart_ChannelType ch);

#ifdef __cplusplus
}
#endif

#endif /* UART_UART_H_ */
