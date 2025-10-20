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


#include "Port_Types.h"

/* ---------------------------------------------------------
 *	Convenient Pin & Port Encoding
 *	- High nible: index port (A=0, B=1 , C=2 ,...)
 *	- Low nible: pin number  (0....15)
 * --------------------------------------------------------- */
#define PORT_ENCODE_PIN(port_letter, pin_num)		((()((port_letter) - 'A') << 4 ) | (()(pin_num) & 0x0Fu))
#define PORTA_PIN(n)		PORT_ENCODE_PIN('A', (n))
#define PORTB_PIN(n)		PORT_ENCODE_PIN('B', (n))
#define PORTC_PIN(n)		PORT_ENCODE_PIN('C', (n))

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

#define PORT_PIN_HCSR04_TRIG		PORTA_PIN(1)
#define PORT_PIN_HCSR04_ECHO		PORTA_PIN(0)

/* =====================================================================================================================
 *  List connfig Pin
 *  - This array will declare in Port_Cfg.c
 * ===================================================================================================================*/
/*Array config for each pin.*/
extern const Port_PinConfigType Port_configPins[];

/*Number of element in array*/
extern const uint32 Port_ConfigPins_Count;

/*Config AFIO remap*/
extern const Port_AfioRemapConfigType Port_AfioRemapCfg;

/*Config all*/
extern const Port_ConfigType Port_Config;

#ifdef __cplusplus
}
#endif

#endif /* PORT_CFG_H_ */
