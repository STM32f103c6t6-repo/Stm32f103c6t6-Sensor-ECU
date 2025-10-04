/* =====================================================================================================================
 *  File        : Port.h
 *  Layer       : MCAL
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : declare API for Port driver (config pin GPIO + AFIO remap) follow Port_Cfg.h
 *  Notes       :
 * ===================================================================================================================*/

#ifndef PORT_PORT_H_
#define PORT_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* =========================================================
 *  Version
 * =======================================================*/
#define PORT_AR_MAJOR_VERSION		(1u)
#define PORT_AR_MINOR_VERSION		(1u)
#define PORT_AR_PATCH_VERSION		(1u)

#include <stdint.h>
#include <stdbool.h>
#include "Port_Types.h"

/* =========================================================
 *  API
 * =======================================================*/

/**
* @brief Initialize Port driver: enable GPIO/AFIO clock, configure each pin according to Port_ConfigPins[] array,
* 		 set AFIO remap (CAN1 PB8/PB9, SWJ no-JTAG…), and set initial ODR level.
* @param ConfigPtr Pointer to total configuration (Port_Config) – usually declared extern in Port_Cfg.h/.c
*/
void Port_Init(const Port_ConfigType* ConfigPtr);

/**
* @brief reset direction for a pin
* @param Pin		Pin code
* @param Direction	PORT_PIN_IN/PORT_PIN_OUT
*/
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction);

/**
* @brief Refresh returns all pins to their original configuration (useful if another module has changed).
* 		 Do not touch pins marked “immutable”.
*/
void Port_RefreshPortDirection(void);

/**
* @brief change mode of a pin
* @param Pin		Pin code
* @param Mode		New mode
*/
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode);

/**
* @brief get information version of module port
*/
void Port_GetVersionInfo(Port_VersionInfoType* versioninfo);

/**
* @brief set logic level for pin output in Port driver
*/
void Port_WritePin(Port_PinType Pin, Port_PinLevelType Level);

/**
* @brief Read current logic level of pin
*/
Port_PinLevelType Port_ReadPin(Port_PinType Pin);

#ifdef __cplusplus
}
#endif

#endif /* PORT_PORT_H_ */
