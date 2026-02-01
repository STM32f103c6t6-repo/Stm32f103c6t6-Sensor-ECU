/* =====================================================================================================================
 *  File        : SensorSupervisor_Types.h
 *  Layer       : Application
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Notes       :
 * ===================================================================================================================*/

#ifndef SWC_SENSORSUPERVISOR_SENSORSUPERVISOR_TYPES_H_
#define SWC_SENSORSUPERVISOR_SENSORSUPERVISOR_TYPES_H_

#include "Std_Types.h"
#include "Rte_Types.h"

/* ============================================
 * Basic Definitions
 * ============================================*/
// Distance type used internally by SensorSupervisor
typedef Rte_DistanceType SensorSupervisor_DistanceType;

/* ============================================
 * Sensor status
 * ============================================*/
// Sensor healthy status
typedef enum
{
	SENSOR_SUPERVISOR_SENSOR_OK		= 0u,
	SENSOR_SUPERVISOR_SENSOR_TIMEOUT,
	SENSOR_SUPERVISOR_SENSOR_INVALID,
	SENSOR_SUPERVISOR_SENSOR_ERROR
} SensorSupervisor_SensorStatusType;

/* ============================================
 * Obstacle decision state
 * ============================================*/
// Obstacle decision result
typedef enum
{
	SENSOR_SUPERVISOR_NO_OBSTACLE	= 0u,
	SENSOR_SUPERVISOR_OBSTACLE_DETECTED,
	SENSOR_SUPERVISOR_OBSTACLE_UNKNOWN
} SensorSupervisor_ObstacleDecisionType;

/* ============================================
 * Supervisor state machine
 * ============================================*/
// Internal state of SensorSupervisor
typedef enum
{
	SENSOR_SUPERVISOR_STATUS_INIT	= 0u,
	SENSOR_SUPERVISOR_STATUS_RUNNING,
	SENSOR_SUPERVISOR_STATUS_ERROR
} SensorSupervisor_StateType;

/* ============================================
 * Configuration structure
 * ============================================*/
// Config parameters for SensorSupervisor
typedef struct
{
	SensorSupervisor_DistanceType	ObstacleThresholdMm;
	SensorSupervisor_DistanceType	MaxValidDistanceMm;
	SensorSupervisor_DistanceType	MinValidDistanceMm;
} SensorSupervisor_ConfigType;

/* ============================================
 * Runtime context structure
 * ============================================*/
// Runtime context for SensorSupervisor SWC
typedef struct
{
	SensorSupervisor_StateType				CurrentState;
	SensorSupervisor_SensorStatusType		SensorStatus;
	SensorSupervisor_DistanceType			LastDistance;
	SensorSupervisor_ObstacleDecisionType	ObstacleDecistion;
} SensorSupervisor_ContextType;

#endif /* SWC_SENSORSUPERVISOR_SENSORSUPERVISOR_TYPES_H_ */
