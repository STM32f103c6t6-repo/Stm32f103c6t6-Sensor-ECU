/* =====================================================================================================================
 *  File        : Port_Types.h
 *  Layer       : MCAL (Port)
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Define data structure and macro for Port driver (GPIO + AFIO)
 *  Notes       :
 * ===================================================================================================================*/

#ifndef PORT_PORT_TYPES_H_
#define PORT_PORT_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------------------------
 *	Version
 * --------------------------------------------------------- */
#define PORT_TYPES_AR_MAJOR_VERSION		(1u)
#define PORT_TYPES_AR_MINOR_VERSION		(0u)
#define PORT_TYPES_AR_PATCH_VERSION		(0u)

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t Port_PinType;

/* ---------------------------------------------------------
 *	Type logic level and pin direction
 * --------------------------------------------------------- */
typedef enum
{
	PORT_PIN_LEVEL_LOW = 0u,
	PORT_PIN_LEVEL_HIGH = 1u
} Port_PinLevelType;

typedef enum
{
	PORT_PIN_IN = 0,
	PORT_PIN_OUT
} Port_PinDirectionType;

/* ---------------------------------------------------------
 *	Type mode GPIO for stm32f1
 * --------------------------------------------------------- */
typedef enum
{
	PORT_MODE_INPUT_ANALOG = 0,
	PORT_MODE_INPUT_FLOATING,
	PORT_MODE_INPUT_PU,			// input pull-up
	PORT_MODE_INPUT_PD,			// input pull-down

	PORT_MODE_OUTPUT_PP_2M,		// output push-pull 2MHz
	PORT_MODE_OUTPUT_PP_10M,	// output push-pull 10MHz
	PORT_MODE_OUTPUT_PP_50M,	// output push-pull 50MHz

	PORT_MODE_OUTPUT_OD_2M,		// output open-drain 2MHz
	PORT_MODE_OUTPUT_OD_10M,	// output open-drain 10MHz
	PORT_MODE_OUTPUT_OD_50M,	// output open-drain 50MHz

	PORT_MODE_AF_PP_2M,			// Alternate func push-pull 2MHz
	PORT_MODE_AF_PP_10M,		// Alternate func push-pull 10MHz
	PORT_MODE_AF_PP_50M,		// Alternate func push-pull 50MHz

	PORT_MODE_AF_OD_2M,			// Alternate func open-drain 2MHz
	PORT_MODE_AF_OD_10M,		// Alternate func open-drain 10MHz
	PORT_MODE_AF_OD_50M,		// Alternate func open-drain 50MHz
} Port_PinModeType;

/* ---------------------------------------------------------
 *	config struct pin and AFIO remap
 * --------------------------------------------------------- */
typedef enum
{
	PORT_INIT_LOW = 0,
	PORT_INIT_HIGH
} Port_InitLevelType;

typedef struct
{
	Port_PinType 		pin;			// Pin code: PORTA_PIN(x)
	Port_PinModeType	mode;			// Mode/speed/pull
	Port_InitLevelType	initLevel;		// Init Level
} Port_PinConfigType;

/* Enable/Disable remap in AFIO_MAPR*/
typedef struct
{
	uint8_t Remap_CAN;			// 0: RX-PA11, TX-PA12 | RX-PB8, TX-PB9
	uint8_t	Remap_USART1;		// 0:PA9/PA10 | 1: PB6/PB7
	uint8_t Remap_TIM2;			// 0: no remap (TIM2_CH1 in AP0)
	uint8_t Swj_cfg_disable;	// 1: Disable JTAG, Enable SWD
}Port_AfioRemapConfigType;

/* Config for Port driver*/
typedef struct
{
	const Port_PinConfigType* 	pins;
	uint32_t					pinCount;
	Port_AfioRemapConfigType	afio;
} Port_ConfigType;

/* ---------------------------------------------------------
 *	Version infor
 * --------------------------------------------------------- */
typedef struct
{
	uint16_t	vendorID;
	uint16_t	moduleID;
	uint16_t	sw_major_version;
	uint16_t	sw_minor_version;
	uint16_t	sw_patch_verion;
} Port_VersionInfoType;

/* ---------------------------------------------------------
 *	Driver state
 * --------------------------------------------------------- */
typedef enum
{
	PORT_UNINIT	= 0u,
	PORT_INIT
} Port_StatusType;

#ifdef __cplusplus
}
#endif
#endif /* PORT_PORT_TYPES_H_ */
