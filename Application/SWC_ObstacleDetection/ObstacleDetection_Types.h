/* =====================================================================================================================
 *  File        : ObstacleDetection_Types.h
 *  Layer       : Application
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Notes       :
 * ===================================================================================================================*/

#ifndef SWC_OBSTACLEDETECTION_OBSTACLEDETECTION_TYPES_H_
#define SWC_OBSTACLEDETECTION_OBSTACLEDETECTION_TYPES_H_

#include "Std_Types.h"

/* ============================================
 * Types Definitions
 * ============================================*/
// Distance value measured by ultrasonic sensor
typedef uint16 ObstacleDistance_cmType;

// Obstacle detection status
typedef boolean ObstacleDetectedType;

// Quality of distance measurement
typedef enum
{
	OBSTACLE_MEASUREMENT_VALID	= 0u,
	OBSTACLE_MEASUREMENT_INVALID,
	OBSTACLE_MEASUREMENT_TIMEOUT
} ObstracleMeasurementStatusType;

// Internal obstracle state
typedef enum
{
	OBSTACLE_STATE_CLEAR	= 0u,
	OBSTACLE_STATE_DETECTED
} ObstracleStateType;

/* ============================================
 * Signal group type
 * ============================================*/
typedef struct
{
	ObstacleDistance_cmType			Distance_cm;
	ObstacleDetectedType			ObstacleDetected;
	ObstracleMeasurementStatusType	MeasurementStatus;
} ObstacleDetection_SignalGroupType;

#endif /* SWC_OBSTACLEDETECTION_OBSTACLEDETECTION_TYPES_H_ */
