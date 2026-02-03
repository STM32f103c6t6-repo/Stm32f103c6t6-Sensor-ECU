/* =====================================================================================================================
 *  File        : ObstacleDetection.c
 *  Layer       : Application
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Depends     :
 * ===================================================================================================================*/

#include "ObstacleDetection.h"
#include "ObstacleDetection_Internal.h"
#include "ObstacleDetection_Cfg.h"
#include "Rte.h"
#include "Logger.h"
#include "LogTags.h"

/* ============================================================
 *  Internal Runtime Data Definition
 * ============================================================ */
ObstacleDetection_InternalDataType	ObstacleDetection_InternalData;

/* ============================================
 * API function prototypes
 * ============================================*/

// Initialize Obstacle detection software component
void ObstacleDetection_Init(void)
{
	ObstacleDetection_InternalData.State						= OBSTACLE_INT_STATE_INIT;
	ObstacleDetection_InternalData.LastDistanceCm				= 0U;
	ObstacleDetection_InternalData.LastMeasurementStatus		= OBSTACLE_MEASUREMENT_INVALID;
	ObstacleDetection_InternalData.InvalidMeasurementCounter	= 0U;

#if (OBSTACLE_DETECTION_DEBUG_ENABLE == STD_ON)
	LOG_INFO(LOG_TAG_SWC_OBTACLE,"ObstacleDetection initialized");
#endif

}

// Periodic runnable for obstacle detection logic
void ObstacleDetection_MainFunction(void)
{
	Std_ReturnType	RteStatus;
	ObstacleDistance_cmType			DistanceCm;
	ObstracleMeasurementStatusType	MeasurementStatus;

	if(ObstacleDetection_InternalData.State != OBSTACLE_INT_STATE_INIT)
	{
		return;
	}

	// Read input signal
	RteStatus = Rte_Read_Distance(&DistanceCm);

	if(RteStatus != E_OK)
	{
		ObstacleDetection_HandleInvalidMeasurement();
		return ;
	}

	// validate measurement
	MeasurementStatus = ObstacleDetection_ValidateDistance(DistanceCm);
	ObstacleDetection_InternalData.LastMeasurementStatus = MeasurementStatus;

	if(MeasurementStatus != OBSTACLE_MEASUREMENT_VALID)
	{
		ObstacleDetection_HandleInvalidMeasurement();
		return;
	}

	// Reset invalid counter or valid data
	ObstacleDetection_InternalData.InvalidMeasurementCounter	= 0U;
	ObstacleDetection_InternalData.LastDistanceCm	= DistanceCm;

	// Update state machine
	ObstacleDetection_UpdateState(DistanceCm);

}

/* ============================================
 * Internal Helper Function Prototypes
 * ============================================*/
// Validate distance measurement
ObstracleMeasurementStatusType ObstacleDetection_ValidateDistance(ObstacleDistance_cmType DistanceCm)
{
	if( (DistanceCm < OBSTACLE_DETECTION_MIN_VALID_DISTANCE_CM) || (DistanceCm > OBSTACLE_DETECTION_MAX_VALID_DISTANCE_CM))
	{
		return OBSTACLE_MEASUREMENT_INVALID;
	}

	return OBSTACLE_MEASUREMENT_VALID;
}

// Update obstacle state machine
void ObstacleDetection_UpdateState(ObstacleDistance_cmType DistanceCm)
{
	switch (ObstacleDetection_InternalData.State)
	{
	case OBSTACLE_INT_STATE_CLEAR:
		if(DistanceCm < OBSTACLE_DETECTION_DISTANCE_THRESHOLD_CM)
		{
			ObstacleDetection_InternalData.State = OBSTACLE_INT_STATE_DETECTED;
		}
		break;

	case OBSTACLE_INT_STATE_DETECTED:
		if(DistanceCm > (OBSTACLE_DETECTION_DISTANCE_THRESHOLD_CM + OBSTACLE_DETECTION_HYSTERESIS_CM))
		{
			ObstacleDetection_InternalData.State = OBSTACLE_INT_STATE_CLEAR;
		}
		break;
	default:
		ObstacleDetection_InternalData.State = OBSTACLE_INT_STATE_CLEAR;
		break;
	}
}

// Handle invalid measurement behavior
void ObstacleDetection_HandleInvalidMeasurement(void)
{
	 ObstacleDetection_InternalData.InvalidMeasurementCounter++;

	 if(ObstacleDetection_InternalData.InvalidMeasurementCounter >= OBSTACLE_DETECTION_MAX_INVALID_ID_COUNT)
	 {
#if (OBSTACLE_DETECTION_FAIL_SAFE_MODE == STD_ON)
		 ObstacleDetection_InternalData.State = OBSTACLE_INT_STATE_DETECTED;
#endif
	 }
}


