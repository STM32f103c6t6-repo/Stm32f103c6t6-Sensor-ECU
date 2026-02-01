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
 * Static variable
 * ============================================*/
// Store last valid distance value
static Rte_DistanceType	SensorSupervisor_LastDistance = SENSOR_SUPERVISOR_SENSOR_INVALID;

// Current sensor status
static SensorSupervisor_SensorStatusType SensorSupervisor_Status = SENSOR_SUPERVISOR_STATUS_INIT;

/* ============================================
 * Local Function
 * ============================================*/
// Validate distance value
static boolean SensorSupervisor_IsDistanceValid(Rte_DistanceType Distance)
{
	if((Distance >= SENSOR_SUPERVISOR_MIN_VALID_DISTANCE_MM) && (Distance <= SENSOR_SUPERVISOR_MAX_VALID_DISTANCE_MM))
	{
		return TRUE;
	}
	return FALSE;
}
