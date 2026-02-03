/* =====================================================================================================================
 *  File        : ObstacleDetection_Cfg.h
 *  Layer       : Application
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Depends     :
 * ===================================================================================================================*/


#ifndef SWC_OBSTACLEDETECTION_OBSTACLEDETECTION_CFG_H_
#define SWC_OBSTACLEDETECTION_OBSTACLEDETECTION_CFG_H_

#include "ObstacleDetection_Types.h"

/* ============================================
 * Obstacle Detection Threshold configuration
 * ============================================*/
// Distance threshold to declare obstacle detected
#define OBSTACLE_DETECTION_DISTANCE_THRESHOLD_CM		(30u)

// Hysteresis value to avoid chattering
#define OBSTACLE_DETECTION_HYSTERESIS_CM				(5u)

/* ============================================
 * Sensor valid range configuration
 * ============================================*/
// Minimum valid distance reported by sensor
#define OBSTACLE_DETECTION_MIN_VALID_DISTANCE_CM		(2u)

// Maximum valid distance reported by sensor
#define OBSTACLE_DETECTION_MAX_VALID_DISTANCE_CM		(400u)

/* ============================================
 * Timeout & Error Handling Configuration
 * ============================================*/
// Maximum number of consecutive invalid measurements
#define OBSTACLE_DETECTION_MAX_INVALID_ID_COUNT			(3u)

// Behavior when sensor data invalid
#define OBSTACLE_DETECTION_FAIL_SAFE_MODE				(STD_ON)

/* ============================================
 * CAN/Communication behavior configuration
 * ============================================*/
// Enable sending distance value over CAN
#define OBSTACLE_DETECTION_SEND_DISTANCE				(STD_ON)

// Enable sending measurement status over CAN
#define OBSTACLE_DETECTION_SEND_MEAS_STATUS				(STD_ON)

/* ============================================
 * Debug & Trace configuration
 * ============================================*/
// Enable debug logging for this SWC
#define OBSTACLE_DETECTION_DEBUG_ENABLE					(STD_ON)

#endif /* SWC_OBSTACLEDETECTION_OBSTACLEDETECTION_CFG_H_ */
