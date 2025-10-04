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

#include <stdint.h>
#include "Port_Types.h"

/* ---------------------------------------------------------
 *	Convenient Pin & Port Encoding
 *	- High nible: index port (A=0, B=1 , C=2 ,...)
 *	- Low nible: pin number  (0....15)
 * --------------------------------------------------------- */
#define PORT_ENCODE_PIN(port_letter, pin_num)		(((uint8_t)((port_letter) - 'A') << 4 ) | ((uint8_t)(pin_num) & 0x0Fu))
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

/* =========================================================
 *  Service IDs (for DET/Logger)
 * =======================================================*/
#define PORT_SID_INIT					(0x00u)
#define PORT_SID_SETPINDIRECTION		(0x01u)
#define PORT_SID_REFRESHPORTDIRECTION	(0x02u)
#define PORT_SID_SETPINMODE				(0x03u)
#define PORT_SID_GETVERSIONINFO			(0x04u)

/* =========================================================
 *  DET Error code
 * =======================================================*/
#define PORT_E_UNINIT					(0x01u)		// Call API when Port not init
#define PORT_E_PARAM_POINTER			(0x02u)		// Null pointer
#define PORT_E_PARAM_PIN				(0x03u)		// Invalid pin/not in config table
#define PORT_E_PARAM_INVALID_MODE		(0x04u)		// Invalid mode on STM32F1
#define PORT_E_DIRECTION_UNCHANGEABLE	(0x05u)
#define PORT_E_MODE_UNCHANGEABLE		(0x06u)

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
