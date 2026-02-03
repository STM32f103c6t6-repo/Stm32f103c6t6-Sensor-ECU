/* =====================================================================================================================
 *  File        : ObstacleDetection.h
 *  Layer       : Application
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Notes       :
 * ===================================================================================================================*/


#ifndef SWC_OBSTACLEDETECTION_OBSTACLEDETECTION_H_
#define SWC_OBSTACLEDETECTION_OBSTACLEDETECTION_H_

#include "ObstacleDetection_Types.h"

/* ============================================
 * API function prototypes
 * ============================================*/

// Initialize Obstacle detection software component
void ObstacleDetection_Init(void);

// Periodic runnable for obstacle detection logic
void ObstacleDetection_MainFunction(void);

#endif /* SWC_OBSTACLEDETECTION_OBSTACLEDETECTION_H_ */
