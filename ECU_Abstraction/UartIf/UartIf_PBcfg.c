/* =====================================================================================================================
 *  File        : UartIf_PBcfg.c
 *  Layer       :
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Config UartIf
 *  Depends     :
 * ===================================================================================================================*/
#include "UartIf.h"

static const UartIf_ChannelCfgType UartIf_Channels[] = {
		{ .ChannelId = UART_CH1, .Baudrate = 115200u, .UseDma = FALSE }
};

// Config for Uart Interface
const UartIf_ConfigType UartIf_Config ={
		.Channels = UartIf_Channels,
		.ChannelCount = 1u,
		.DefaultChannelIndex = 0u
};
