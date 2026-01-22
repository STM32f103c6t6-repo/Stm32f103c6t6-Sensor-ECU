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
#include "Gpt_Cfg.h"
#include "Can_Cfg.h"
#include "Icu_Cfg.h"

// Config for Mcu
const Mcu_ConfigType Mcu_Config = {
		.pllSource		= MCU_CFG_PLL_SOURCE_HSE,
		.pllMul			= MCU_CFG_PLL_MUL,

		.ahbPresc		= MCU_CFG_AHB_PRESC,
		.apb1Presc		= MCU_CFG_APB1_PRESC,
		.apb2Presc		= MCU_CFG_APB2_PRESC,

		.adcPresc		= MCU_CFG_ADC_PRESC,

		.flashLatency	= MCU_CFG_LATENCY,

		.systickSrc		= MCU_CFG_SYSTICK_SOURCE_HCLK,
		.systickHz		= MCU_CFG_SYSTICK_HZ,

		.nvicPrigroup	= MCU_CFG_NVIC_PRIGROUP,
		.iwdg			= { .enable = MCU_CFG_IWDG_ENABLE,
							.prescaler = MCU_CFG_IWDG_PRESC,
							.timeoutMs = MCU_CFG_IWDFG_TIMEOUT
							}
};

// Config for Uart
const Uart_ConfigType Uart_Config = {
		.useCh1				= UART_CFG_INSTANCE_USART1,
		.useCh2				= UART_CFG_INSTANCE_USART2,
		.useCh3				= UART_CFG_INSTANCE_USART3,

		.logPort			= UART_CFG_LOG_PORT,
		.diagPort			= UART_CFG_DIAG_PORT,

		.usart1				= {
				.baurate 		= UART1_CFG_BAUDRATE,
				.wordlength		= UART1_CFG_WORD_LENGTH,
				.stopBits		= UART1_CFG_STOP_BITS,
				.parity			= UART1_CFG_PARTY,
				.flow			= UART1_CFG_HW_FLOW_CTRL,

				.dir			= { .txEnable = UART1_CFG_DIR_TX_ENABLE, .rxEnable = UART1_CFG_DIR_RX_ENABLE},
				.transMode		= UART1_CFG_TRANS_MODE,

				.TxEnable		= UART1_CFG_MODE_TX_ENABLE,
				.RxEnable		= UART1_CFG_MODE_RX_ENABLE,
				.useInterrupts	= UART1_CFG_USE_INTERRUPTS,
				.useDmaTx		= UART1_CFG_USE_DMA_TX,
				.useDmaRx		= UART1_CFG_USE_DMA_RX,

				.txBufSize	 	= UART1_CFG_TX_BUFFER_SIZE,
				.rxBufSize		= UART1_CFG_RX_BUFFER_SIZE,
				.txTimeoutMs	= UART1_CFG_TX_TIMEOUT_MS,
				.rxTimeoutMs	= UART1_CFG_RX_TIMEOUT_MS,

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

// Config for Gpt
static const Gpt_ChannelConfigType s_GptChannelConfigs[] = {
		{
				.ChannelId 		= GPT_CHANNEL_ID,
				.Mode			= GPT_MODE,
				.TickFrequency	= GPT_TICK_FREQUENCY,
				.TimerId		= GPT_TIMER_ID,
				.Prescale		= GPT_TIMER_PRESCALE
		}
};

const Gpt_ConFigType Gpt_Config = {
		.ChannelConfig 	= s_GptChannelConfigs,
		.ChannelCount	= GPT_CHANNEL_COUNT
};

//Config for CAN
const const Can_BaudrateConfigType s_CanBaudrate[] = {
		{
				.precscale 	= CAN_PREC_SCALE_CTL0,
				.sjw		= CAN_SYNC_SEGMENT_CTL0,
				.tseg1		= CAN_BIT_SEGMENT_1_CTL0,
				.tseg2		= CAN_BIT_SEGMENT_2_CTL0
		}
};

static const Can_ControllerConfigType s_CanControllerConfigs[] = {
		{
				.ControllerId = CAN_CTL0_ID,
				.baseAddress = CAN1_BASE,
				.baudrate = s_CanBaudrate
		}
};

const Can_ConfigType Can_Config = {
		.ControllerConfig	= s_CanControllerConfigs,
		.numControllers		= CAN_CTL_NUM
};

// Config for ICU
static const Icu_ChannelConfigType s_IcuChannelConfigs = {
		.ChannelID = ICU_CHANNEL_ECHO,
		.Timer = ICU_TIMER_TIM2,
		.IcuChannel = ICU_CHANNEL_1,
		.DefaultEdge = ICU_RISING_EDGE
};

const Icu_ConfigType Icu_Config = {
		.channels = s_IcuChannelConfigs,
		.numsChannel = ICU_CNT_CHANNEL
};
