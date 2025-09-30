/* =====================================================================================================================
 *  File        : Port_Cfg.h
 *  Layer       : MCAL (Config)
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : declare pin (pin mapping, mode/speed/pull) & AFIO remap cho Port driver
 *  Notes       : - Header config for Port.c read and apply to RCC/APB2, GPIOx, AFIO_MAPR.
 * ===================================================================================================================*/

#ifndef PORT_CFG_H_
#define PORT_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include<stdint.h>

/* =====================================================================================================================
 *  Data structure for config pin:
 *  - Mode	: input/output/analog + Pushpull/opendrain
 *  - Speed	: 2/10/50 MHz (apply for output/AF)
 *  - Pull	: none/pull-up/pull-down
 * ===================================================================================================================*/

typedef enum
{
	PORT_MODE_INPUT_ANALOG = 0,
	PORT_MODE_INPUT_FLOATING,
	PORT_MODE_INPUT_PU,					//input pull-up
	PORT_MODE_INPUT_PD,					//input pull_down

	PORT_MODE_OUTPUT_PP_2M,				//output push-pull 2M
	PORT_MODE_OUTPUT_PP_10M,			//output push-pull 10M
	PORT_MODE_OUTPUT_PP_50M,			//output push-pull 50M

	PORT_MODE_OUTPUT_OD_2M,				//output open-drain 2M
	PORT_MODE_OUTPUT_OD_10M,			//output open-drain 10M
	PORT_MODE_OUTPUT_OD_50M,			//output open-drain 50M

	PORT_MODE_AF_PP_2M,					//alternate function push-pull 2M
	PORT_MODE_AF_PP_10M,				//alternate function push-pull 10M
	PORT_MODE_AF_PP_50M,				//alternate function push-pull 50M

	PORT_MODE_AF_OP_2M,					//alternate function open-drain 2M
	PORT_MODE_AF_OP_10M,				//alternate function open-drain 10M
	PORT_MODE_AF_OP_50M,				//alternate function open-drain 50M
} Port_PinModeType;

//Initialization logic level

typedef enum
{
	PORT_INIT_LOW = 0,
	PORT_INIT_HIG
} Port_InitLevelType;

/* Port code + Pin config */
#define PORT_ENCODE_PIN(port_letter,pin_num)			(((uint8_t)((port_letter) - "A") << 4 ) | ((uint8_t)(pin_num) & 0x0F))
#define PORT_A_PIN(n)	PORT_ENCODE_PIN('A', (n))
#define PORT_B_PIN(n)	PORT_ENCODE_PIN('B', (n))
#define PORT_C_PIN(n)	PORT_ENCODE_PIN('C', (n))

/* Config for 1 pin*/
typedef struct
{
	uint8_t				Pin;
	Port_PinModeType	Mode;
	Port_InitLevelType	Initlevel;
}Port_PinConfigType;

/* Enable/Disable remap in AFIO_MAPR*/
typedef struct
{
	uint8_t Remap_CAN;			// 0: RX-PA11, TX-PA12 | RX-PB8, TX-PB9
	uint8_t	Remap_USART1;		// 0:PA9/PA10 | 1: PB6/PB7
	uint8_t Remap_TIM2;			// 0: no remap (TIM2_CH1 in AP0)
	uint8_t Swj_cfg_disable;	// 1: Disable JTAG, Enable SWD
}Port_AfioRemapConfigType;

/*Config for list pin + AFIO remap*/
typedef struct
{
	const Port_PinConfigType* Pins;
	uint32_t				  PinCount;
	Port_AfioRemapConfigType  Afio;
}Port_Config;

/* =====================================================================================================================
 *  Pin mapping for Sensor_ECU:
 *  - UART1 : 	PA9 -TX, PA10-RX
 * 	- CAN: 		PA12-Tx, PA11-Rx
 * 	- HC-SR04: 	PA1-Trigger(OUT_PP_2M), PA0-Echo(TIM2_CH1)
 * ===================================================================================================================*/
#define PORT_PIN_UART1_TX		PORTA_PIN(9)
#define PORT_PIN_UART1_RX		PORTA_PIN(10)

#define PORT_PIN_CAN1_TX		PORTA_PIN(12)
#define PORT_PIN_CAN1_RX		PORTA_PIN(11)

#define PORT_PIN_HCSR04_TRG		PORTA_PIN(1)
#define PORT_PIN_HCSR04_TRG		PORTA_PIN(0)

/* =====================================================================================================================
 *  List connfig Pin
 *  - This array will declare in Port_Cfg.c
 * ===================================================================================================================*/
/*Array config for each pin.*/
extern const Port_PinConfigType Port_configPins[];

/*Number of element in array*/
extern const uint32_t Port_ConfigPins_Count;

/*Config AFIO remap*/
extern const Port_AfioRemapConfigType Port_AfioRemapCfg;

/*Config all*/
extern const Port_ConfigType Port_Config;

#ifdef __cplusplus
}
#endif

#endif /* PORT_CFG_H_ */
