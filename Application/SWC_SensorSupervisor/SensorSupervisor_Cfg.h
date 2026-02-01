/* =====================================================================================================================
 *  File        : SensorSupervisor_Cfg.h
 *  Layer       : Application
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Depends     :
 * ===================================================================================================================*/


#ifndef SWC_SENSORSUPERVISOR_SENSORSUPERVISOR_CFG_H_
#define SWC_SENSORSUPERVISOR_SENSORSUPERVISOR_CFG_H_

#include "SensorSupervisor_Types.h"

/* ============================================
 * General configuration
 * ============================================*/
// enable/disable sensorsupervisor module
#define SENSOR_SUPERVISOR_ENABLE				(STD_ON)

// enable/disable sensor data validation
#define SENSOR_SUPERVISOR_DATA_VALIDATION		(STD_ON)

/* ============================================
 * Distance Configuration
 * ============================================*/
// obstacle detection threshold
#define SENSOR_SUPERVISOR_OBSTACLE_THRESHOLD_MM	(300u)

// value used to indicate invalid or unavailable distance
#define SENSOR_SUPERVISOR_INVALID_DISTANCE		((Rte_DistanceType)0xFFFFu)

// minimum valid distance
#define SENSOR_SUPERVISOR_MIN_VALID_DISTANCE_MM	(20u)

// maximum valid distance
#define SENSOR_SUPERVISOR_MAX_VALID_DISTANCE_MM	(4000u)

/* ============================================
 * Timing configuration
 * ============================================*/
// Runnable period
#define SENSOR_SUPERVISOR_RUNNABLE_PERIOD_MS	(10ms)

// Number of consecutive invalid samples
#define SENSOR_SUPERVISOR_MAX_INVALID_COUNT		(5u)

/* ============================================
 * Default state
 * ============================================*/
// Default obstacle state at startup
#define SENSOR_SUPERVISOR_DEFAULT_OBSTACLE_STATE	(SENSOR_SUPERVISOR_NO_OBSTACLE)

// Default sensor state of startup
#define SENSOR_SUPERVISOR_DEFAULT_SENSOR_STATE		(SENSOR_SUPERVISOR_SENSOR_OK)

/* ============================================
 * Debug configuration
 * ============================================*/
// Enable Debug hooks
#define SENSOR_SUPERVISOR_DEBUG		(STD_OFF)


#endif /* SWC_SENSORSUPERVISOR_SENSORSUPERVISOR_CFG_H_ */
