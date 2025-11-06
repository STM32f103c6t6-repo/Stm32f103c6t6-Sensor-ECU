/* =====================================================================================================================
 *  File        : Config.c
 *  Layer       :
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Config Ecu, mcu, uart,.... for project
 *  Depends     :
 * ===================================================================================================================*/

#include "Port_Cfg.h"
#include "Uart_Cfg.h"
#include "Mcu_Cfg.h"

// Config for Mcu
const Mcu_ConfigType Mcu_Config = {
		.pllSource		= MCU_CLOCK_SRC_HSE,
		.pllMul			= MCU_PLL_MUL_9,

		.ahbPresc		= MCU_AHB_DIV_1,
		.apb1Presc		= MCU_AHB_DIV_2,
		.apb2Presc		= MCU_AHB_DIV_1,

		.adcPresc		= 6u,

		.flashLatency	= MCU_FLASH_LATENCY_2WS,

		.systickSrc		= MCU_SYSTICK_CLKSRC_HCLK,
		.systickHz		= 1000u,

		.nvicPrigroup	= MCU_NVIC_PRIGROUP_4_4,
		.iwdg			= { .enable = 0u,
							.prescaler = 64u,
							.timeoutMs = 1000u
							}
};

// Config for Uart
const Uart_ConfigType Uart_Config = {
		.useCh1				= 1u,
		.useCh2				= 0u,
		.useCh3				= 0u,

		.logPort			= UART_CH1,
		.diagPort			= UART_CH1,

		.usart1				= {
				.baurate 		= 115200u,
				.wordlength		= UART_WORDLEN_8B,
				.stopBits		= UART_STOPBITS_1,
				.parity			= UART_PARITY_NONE,
				.flow			= UART_FLOW_NONE,

				.dir			= { .txEnable = 1u, .rxEnable = 1u},
				.transMode		= UART_XFER_INTERRUPT,

				.TxEnable		= 1u,
				.RxEnable		= 1u,
				.useInterrupts	= 1u,
				.useDmaTx		= 0u,
				.useDmaRx		= 0u,

				.txBufSize	 	= 256u,
				.rxBufSize		= 256u,
				.txTimeoutMs	= 10u,
				.rxTimeoutMs	= 10u,

				.cbs			= NULL_PTR
		},

		.usart2 = {0},
		.usart3 = {0}
};

//Config for Port
static const Port_PinConfigType s_PortPins[] = {
		//Uart for log
		{ .pin = PORT_PIN_UART1_TX, .mode = PORT_MODE_AF_PP_10M, .initLevel = PORT_INIT_HIGH},
		{ .pin = PORT_PIN_UART1_RX, .mode = PORT_MODE_INPUT_FLOATING, .initLevel = PORT_INIT_LOW},

		// Can
		{ .pin = PORT_PIN_CAN1_TX, .mode = PORT_MODE_AF_PP_10M, .initLevel = PORT_INIT_HIGH},
		{ .pin = PORT_PIN_CAN1_RX, .mode = PORT_MODE_INPUT_FLOATING, .initLevel = PORT_INIT_LOW},

		//HSRC04
		{ .pin = PORT_PIN_HCSR04_TRIG, .mode = PORT_MODE_OUTPUT_PP_2M, .initLevel = PORT_INIT_LOW},
		{ .pin = PORT_PIN_HCSR04_ECHO, .mode = PORT_MODE_INPUT_FLOATING, .initLevel = PORT_INIT_LOW}
};

static const Port_AfioRemapConfigType s_AfioCfg = {
		.Remap_CAN = 0u,
		.Remap_USART1 = 0u,
		.Remap_TIM2	= 0u,
		.Swj_cfg_disable = 1u
};

const Port_ConfigType Port_Config = {
		.pins = s_PortPins,
		.pinCount = (uint32)(sizeof(s_PortPins) / sizeof(Port_PinConfigType)),
		.afio	= s_AfioCfg
};
