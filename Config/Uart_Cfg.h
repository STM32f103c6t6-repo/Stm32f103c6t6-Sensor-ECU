/* =====================================================================================================================
 *  File        : Uart_Cfg.h
 *  Layer       : MCAL (Config)
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Config UART (USART) for MCAL/Uart driver – using for log/debug
 *  Notes       : - Header config for Uart.c will include and apply USARTx.
 * ===================================================================================================================*/

#ifndef UART_CFG_H_
#define UART_CFG_H_

#ifdef __cplusplus
extern "C"{
#endif

/* =====================================================================================================================
 *  Select instance  and transmission characteristics
 *  - Sensor ECU: use USART1 (PA9/PA10).
 *  - 115200, not flow-control, interrupt mode
 * ===================================================================================================================*/
#define UART_CFG_INSTANCE_USART1			(1u) 	/* 1: enable USART1*/
#define UART_CFG_INSTANCE_USART2			(0u)

/* =====================================================================================================================
 *  Parameter config for port log
 * ===================================================================================================================*/
#define UART1_CFG_BAUDRATE					(115200)	// bps
#define UART1_CFG_WORD_LENGTH				(8u)		// 8 or 9
#define UART1_CFG_STOP_BITS					(1u)		// 1 or 2
#define UART1_CFG_PARITY_ENABLE				(0u)		// 0: no parity, 1: enable
#define UART1_CFG_PARITY_ODD				(0u)		// 0: even, 1: odd
#define UART1_CFG_HW_FLOW_CTRL				(0u)		// 0: None, 1: RTS/CTS
#define UART1_CFG_OVERSAMPLING_16			(1u)		// 1:OS16, 0: OS8

/*Config Mode activity */
#define UART1_CFG_MODE_TX_ENABLE			(1u)
#define UART1_CFG_MODE_RX_ENABLE			(1u)
#define UART1_CFG_USE_INTERRUPTS			(1u)
#define UART1_CFG_USE_DMA_TX				(0u)
#define UART1_CFG_USE_DMA_RX				(0u)

/* =====================================================================================================================
 * Ring buffer & timing
 * - TX/RX ring buffer for Logger to print non-blocking.
 * - Timeout for send/recv in ms (if driver supports).
 * ===================================================================================================================*/
#define UART1_CFG_TX_BUFFER_SIZE			(256u)		//Bytes
#define UART1_CFG_RX_BUFFER_SIZE			(128u)		//Bytes
#define UART1_CFG_TX_TIMEOUT_MS				(20u)		// Using for API Blocking
#define UART1_CFG_RX_TIMEOUT_MS				(20u)

/* =====================================================================================================================
 * Mapping logger & diagnostic channel (optional)
 * - Allows UartIf/Logger to know which port to use as "LOG_PORT"
 * ===================================================================================================================*/
#define UART_CFG_LOG_PORT					(1u)		//1:USART1, 2:USART2
#define UART_CFG_DIAG_PORT					(1u)

/* =====================================================================================================================
 * Data structure config runtime
 * - if use config in struct, enable UART_CFG_USE_STRUCT = 1
 * ===================================================================================================================*/
#define UART_CFG_USE_STRUCT					(1u)


#if (UART_CFG_USE_STRUCT == 1u)
//parity mode
typedef enum {
	UART_PARITY_NONE = 0,
	UART_PARITY_EVEN,
	UART_PARITY_ODD
} Uart_ParityModeType;

// Flow control
typedef enum
{
	UART_FLOW_NONE = 0,
	UART_FLOW_RTS_CTS
} Uart_Flow_CtrlType;

/*Config for each instance */
typedef struct
{
	uint32_t baurate;
	uint8_t wordlength;		// 8 or 9
	uint8_t	stopBits;		//1 or 2
	Uart_ParityModeType	parity;
	Uart_Flow_CtrlType flow;

	uint8_t	TxEnable;		//enable Tx
	uint8_t RxEnable;		//enable Rx
	uint8_t	useInterrupts;	// 1: IRQ, 0:polling
	uint8_t	useDmaTx;		//1:DMA Tx
	uint8_t useDmaRx;		//1: DMA Rx

	uint16_t txBufSize;		// ring buffer size
	uint16_t rxBufSize;
	uint16_t txTimeoutMs;	//timeout for API blocking
	uint16_t rxTimeoutMs;
} Uart_InstanceConfigType;

// Config all instance
typedef struct
{
	uint8_t useUsart1;
	uint8_t useUsart2;

	uint8_t logPort;
	uint8_t diagPort;

	Uart_InstanceConfigType usart1;
	Uart_InstanceConfigType usart2;
} Uart_ConfigType;

extern const Uart_ConfigType Uart_Config;

#endif //UART_CFG_USE_STRUCT

/* =====================================================================================================================
 * Check compile-time
 * ===================================================================================================================*/
#if (UART_CFG_INSTANCE_USART1 == 0u) && (UART_CFG_LOG_PORT == 1u)
#error "UART_CFG_INSTANCE_USART1 = 0 but UART_CFG_LOG_PORT = USART1 -> Enable USART1."
#endif

#if (UART1_CFG_WORD_LENGTH != 8u) && (UART1_CFG_WORD_LENGTH != 9u)
#error "UART1_CFG_WORD_LENGTH always has value 8 or 9."
#endif

#if (UART1_CFG_STOP_BITS != 1u) && (UART1_CFG_STOP_BITS != 2u)
#error "UART1_CFG_STOP_BITS always has value 1 or 2."
#endif

#if (UART1_CFG_PARITY_ENABLE == 0u)
//No parity
#else
#if(UART1_CFG_PARITY_ODD != 0u) && (UART1_CFG_PARITY_ODD != 1u)
#error "UART1_CFG_PARITY_ODD always has value 0 or 1 when enable parity."
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* UART_CFG_H_ */
