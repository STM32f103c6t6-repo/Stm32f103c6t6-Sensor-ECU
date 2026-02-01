/* =====================================================================================================================
 *  File        : SensorSupervisor.c
 *  Layer       : Application
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Depends     :
 * ===================================================================================================================*/


#include "SensorSupervisor.h"
#include "SensorSupervisor_Cfg.h"

/* ============================================
 * Static internal data
 * ============================================*/
// Current health status of the sensor
static SensorSupervisor_SensorStatusType		SensorSupervisor_Status;

// Current obstacle decision results
static SensorSupervisor_ObstacleDecisionType	SensorSupervisor_ObstacleDecition;

// Last valid distance value received from RTE
static Rte_DistanceType		SensorSupervisor_LastDistance;

/* =====================================================================================================================
 *  Init function
 * ===================================================================================================================*/

// Initialize SensorSupervisor SWC

void SensorSupervisor_Init(void)
{
	SensorSupervisor_Status = SENSOR_SUPERVISOR_SENSOR_INVALID;

	SensorSupervisor_ObstacleDecition = SENSOR_SUPERVISOR_OBSTACLE_UNKNOWN;

	SensorSupervisor_LastDistance	= SENSOR_SUPERVISOR_INVALID_DISTANCE;
}

/* =====================================================================================================================
 *  Runnable prototypes
 * ===================================================================================================================*/

// Periodic runnable of sensorsupervisor

void SensorSupervisor_Runnable_10ms(void)
{
	Rte_DistanceType Distance;

	// Read distance value from rte
	if(Rte_Read_Distance(&Distance) != RTE_E_OK)
	{
		// Read failed
		SensorSupervisor_Status = SENSOR_SUPERVISOR_SENSOR_ERROR;
		return;
	}

	// Store last receiver distance
	SensorSupervisor_LastDistance = Distance;

	// Valid distance range
	if((Distance < SENSOR_SUPERVISOR_MIN_VALID_DISTANCE_MM) || (Distance > SENSOR_SUPERVISOR_MAX_VALID_DISTANCE_MM))
	{
		SensorSupervisor_Status = SENSOR_SUPERVISOR_SENSOR_INVALID;
		SensorSupervisor_ObstacleDecition = SENSOR_SUPERVISOR_OBSTACLE_UNKNOWN;
		return;
	}

	SensorSupervisor_Status = SENSOR_SUPERVISOR_SENSOR_OK;

	// Obstacle decision logic
	if(Distance <= SENSOR_SUPERVISOR_OBSTACLE_THRESHOLD_MM )
	{
		SensorSupervisor_ObstacleDecition = SENSOR_SUPERVISOR_OBSTACLE_DETECTED;

		// notify other SWCs via RTE
		(void)Rte_Write_ObstacleState(RTE_OBSTACLE_DETECT);
	}

}

/* =====================================================================================================================
 *  Internal API
 * ===================================================================================================================*/

// Get current sensor status
SensorSupervisor_SensorStatusType	SensorSupervisor_GetSensorStatus(void)
{
	return SensorSupervisor_Status;
}

// Get current obstacle decision
SensorSupervisor_ObstacleDecisionType	SensorSupervisor_GetObstacleDecision(void)
{
	return SensorSupervisor_ObstacleDecition;
}

// Reset SensorSupervisor state
void SensorSupervisor_Reset(void)
{
	SensorSupervisor_Status = SENSOR_SUPERVISOR_SENSOR_INVALID;

	SensorSupervisor_ObstacleDecition = SENSOR_SUPERVISOR_OBSTACLE_UNKNOWN;

	SensorSupervisor_LastDistance	= SENSOR_SUPERVISOR_INVALID_DISTANCE;
}
