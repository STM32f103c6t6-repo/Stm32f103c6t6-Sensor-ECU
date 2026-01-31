/* =====================================================================================================================
 *  File        : RTE_Types.h
 *  Layer       : RTE
 *  ECU         : (STM32F103C6T6)
 *  Purpose     :
 *  Notes       :
 * ===================================================================================================================*/

#ifndef RTE_TYPES_H_
#define RTE_TYPES_H_

#include "Std_Types.h"
#include "Com.h"

#ifndef RTE_E_OK
#define RTE_E_OK		((Std_ReturnType)0x00u)
#endif

#ifndef RTE_E_NOT_OK
#define RTE_E_NOT_OK	((Std_ReturnType)0x01u)
#endif

#ifndef RTE_E_INVALID
#define RTE_E_INVALID	((Std_ReturnType)0x03u)
#endif

#ifndef RTE_E_NO_DATA
#define RTE_E_NO_DATA	((Std_ReturnType)0x04u)
#endif

#define RTE_SIGNAL_DISTANCE	COM_SIGNAL_ID_DISTANCE
#define RTE_SIGNAL_OBSTACLE	COM_SIGNAL_ID_OBSTACLE
#define RTE_SIGNAL_SPEED	COM_SIGNAL_ID_SPEED

/* ============================================
 * Application Data types
 * ============================================*/

// Distance measured by ultrasonic sensor
typedef uint16 Rte_DistanceType;

// Vehicle speed command or feedback
typedef uint16 Rte_SpeedType;

// Motor control command
typedef enum
{
	RTE_MOTOR_STOP	= 0u,
	RTE_MOTOR_RUN
} Rte_MotorCommandType;

// Obstacle detection state
typedef enum
{
	RTE_OBSTACLE_NOT_DETECT		= 0u,
	RTE_OBSTACLE_DETECT
} Rte_ObstacleStateType;

/* ============================================
 * RTE Signal status
 * ============================================ */

// Signal update state
typedef enum
{
	RTE_SIGNAL_INVALID		= 0u,
	RTE_SIGNAL_VALID
} Rte_SignalStatusType;

/* ============================================
 * RTE Port Abstraction Types
 * ============================================ */
// Sender/Receiver Port IDs
typedef uint16 Rte_PortIdType;

// Sender/Receiver port types
typedef struct
{
	void*						DataPtr;
	uint16						DataSize;
	Rte_SignalStatusType		Status;
} Rte_SrPortType;

/* ============================================
 * Runnable Context types
 * ============================================ */

// Runnable execution context
typedef enum
{
	RTE_CONTEXT_INIT = 0,
	RTE_CONTEXT_PERIODIC,
	RTE_CONTEXT_EVENT
} Rte_RunableContextType;

typedef uint16 Rte_RunnableIdType;

/* ============================================
 * RTE Mode Declarations
 * ============================================ */

// System operating mode
typedef enum
{
	RTE_MODE_INIT	= 0,
	RTE_MODE_NORMAL,
	RTE_MODE_ERROR
} Rte_SystemModeType;

/* ============================================
 * RTE Handle Types
   ============================================ */
typedef uint8 Rte_InstanceIdType;


#endif /* RTE_TYPES_H_ */
