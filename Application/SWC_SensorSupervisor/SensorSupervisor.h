/* =====================================================================================================================
 *  File        : SensorSupervisor.h
 *  Layer       : Application
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Notes       :
 * ===================================================================================================================*/


#ifndef SWC_SENSORSUPERVISOR_SENSORSUPERVISOR_H_
#define SWC_SENSORSUPERVISOR_SENSORSUPERVISOR_H_

#include "SensorSupervisor_Types.h"
#include "Rte.h"

/* =====================================================================================================================
 *  Init function
 * ===================================================================================================================*/

// Initialize SensorSupervisor SWC

void SensorSupervisor_Init(void);

/* =====================================================================================================================
 *  Runnable prototypes
 * ===================================================================================================================*/

// Periodic runnable of sensorsupervisor

void SensorSupervisor_Runnable_10ms(void);

/* =====================================================================================================================
 *  Internal API
 * ===================================================================================================================*/

// Get current sensor status
SensorSupervisor_SensorStatusType	SensorSupervisor_GetSensorStatus(void);

// Get current obstacle decision
SensorSupervisor_ObstacleDecisionType	SensorSupervisor_GetObstacleDecision(void);

// Reset SensorSupervisor state
void SensorSupervisor_Reset(void);

#endif /* SWC_SENSORSUPERVISOR_SENSORSUPERVISOR_H_ */
