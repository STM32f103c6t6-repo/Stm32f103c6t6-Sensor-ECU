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

#include "Uart_Types.h"

/* =====================================================================================================================
 *  Select instance  and transmission characteristics
 *  - Sensor ECU: use USART1 (PA9/PA10).
 *  - 115200, not flow-control, interrupt mode
 * ===================================================================================================================*/
#define UART_CFG_INSTANCE_USART1			(1u) 	/* 1: enable USART1*/
#define UART_CFG_INSTANCE_USART2			(0u)
#define UART_CFG_INSTANCE_USART3			(0u)

/* =====================================================================================================================
 *  Parameter config for port log
 * ===================================================================================================================*/
#define UART1_CFG_BAUDRATE					(115200)			// bps
#define UART1_CFG_WORD_LENGTH				UART_WORDLEN_8B		// 8 or 9
#define UART1_CFG_STOP_BITS					UART_STOPBITS_1		// 1 or 2
#define UART1_CFG_PARITY_ENABLE				(0u)				// 0: no parity, 1: enable
#define UART1_CFG_PARITY_ODD				(0u)				// 0: even, 1: odd
#define UART1_CFG_PARTY						UART_PARITY_NONE
#define UART1_CFG_HW_FLOW_CTRL				UART_FLOW_NONE		// 0: None, 1: RTS/CTS
#define UART1_CFG_OVERSAMPLING_16			(1u)				// 1:OS16, 0: OS8
#define UART1_CFG_TRANS_MODE				UART_XFER_INTERRUPT

/*Config Mode activity */
#define UART1_CFG_DIR_TX_ENABLE				(1u)
#define UART1_CFG_DIR_RX_ENABLE				(1u)
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
#define UART1_CFG_RX_BUFFER_SIZE			(256u)		//Bytes
#define UART1_CFG_TX_TIMEOUT_MS				(20u)		// Using for API Blocking
#define UART1_CFG_RX_TIMEOUT_MS				(20u)

/* =====================================================================================================================
 * Mapping logger & diagnostic channel (optional)
 * - Allows UartIf/Logger to know which port to use as "LOG_PORT"
 * ===================================================================================================================*/
#define UART_CFG_LOG_PORT					UART_CH1		//1:USART1, 2:USART2
#define UART_CFG_DIAG_PORT					UART_CH1

/* =====================================================================================================================
 * Data structure config runtime
 * - if use config in struct, enable UART_CFG_USE_STRUCT = 1
 * ===================================================================================================================*/
#define UART_CFG_USE_STRUCT					(1u)

/* =====================================================================================================================
 * Check compile-time
 * ===================================================================================================================*/
#if (UART_CFG_INSTANCE_USART1 == 0u) && (UART_CFG_LOG_PORT == UART_CH1)
#error "UART_CFG_INSTANCE_USART1 = 0 but UART_CFG_LOG_PORT = USART1 -> Enable USART1."
#endif

#if (UART1_CFG_WORD_LENGTH != UART_WORDLEN_8B) && (UART1_CFG_WORD_LENGTH != UART_WORDLEN_9B)
#error "UART1_CFG_WORD_LENGTH always has value 8 or 9."
#endif

#if (UART1_CFG_STOP_BITS != UART_STOPBITS_1) && (UART1_CFG_STOP_BITS != UART_STOPBITS_2)
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
