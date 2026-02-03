/* =====================================================================================================================
 *  File        : ObstacleDetection_Internal.h
 *  Layer       : Application
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Notes       :
 * ===================================================================================================================*/


#ifndef SWC_OBSTACLEDETECTION_OBSTACLEDETECTION_INTERNAL_H_
#define SWC_OBSTACLEDETECTION_OBSTACLEDETECTION_INTERNAL_H_

#include "ObstacleDetection_Cfg.h"
#include "ObstacleDetection_Types.h"

/* ============================================
 * Internal state machine definitions
 * ============================================*/
// Internal state of obstacle detection logic
typedef enum
{
	OBSTACLE_INT_STATE_INIT		= 0u,
	OBSTACLE_INT_STATE_CLEAR,
	OBSTACLE_INT_STATE_DETECTED
} ObstacleDetection_InternalStateType;

/* ============================================
 * Internal data structure
 * ============================================*/
// Internal runtime data of ObstacleDetection SWC
typedef struct
{
	ObstacleDetection_InternalStateType	State;
	ObstacleDistance_cmType				LastDistanceCm;
	ObstracleMeasurementStatusType		LastMeasurementStatus;
	uint8								InvalidMeasurementCounter;
} ObstacleDetection_InternalDataType;

/* ============================================
 * Internal Variable
 * ============================================*/
// Internal runtime data instance
extern ObstacleDetection_InternalDataType ObstacleDetection_InternalData;

/* ============================================
 * Internal Helper Function Prototypes
 * ============================================*/
// Validate distance measurement
ObstracleMeasurementStatusType ObstacleDetection_ValidateDistance(ObstacleDistance_cmType DistanceCm);

// Update obstacle state machine
void ObstacleDetection_UpdateState(ObstacleDistance_cmType DistanceCm);

// Handle invalid measurement behavior
void ObstacleDetection_HandleInvalidMeasurement(void);

#endif /* SWC_OBSTACLEDETECTION_OBSTACLEDETECTION_INTERNAL_H_ */
