/* =====================================================================================================================
 *  File        : ObstacleDetection.c
 *  Layer       : Application
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Depends     :
 * ===================================================================================================================*/

#include "Sensor.h"
#include "Sensor_Cfg.h"
#include "Sensor_Internal.h"
#include "Rte.h"
#include "SensorIf.h"

// Internal Data
static Sensor_InternalDataType Sensor_InternalData;

// Init
void Sensor_Init(void)
{
	Sensor_InternalData.Status			= SENSOR_STATUS_UNINIT;
	Sensor_InternalData.LastDistance	= 0U;
	Sensor_InternalData.LastMeasurement	= SENSOR_MEAS_INVALID;
	Sensor_InternalData.TimeoutCounter	= 0U;
}

// Periodic runnable
void Sensor_MainFunction(void)
{
	Sensor_DistanceCmType	DistanceCm;
	Std_ReturnType			Ret;

	// Trigger HC-SR04
	Sensor_TriggerPulse();

	// Read Echo
	Ret = Sensor_ReadEcho(&DistanceCm);

	if(Ret != E_OK)
	{
		Sensor_InternalData.Status = SENSOR_STATUS_TIMEOUT;
		Sensor_InternalData.TimeoutCounter++;
		return;
	}

	// Validate distance
	if(Sensor_ValidateDistance(DistanceCm) != SENSOR_MEAS_VALID)
	{
		Sensor_InternalData.Status = SENSOR_STATUS_NO_ECHO;
		return;
	}

	// Valid measurement
	Sensor_InternalData.Status			= SENSOR_STATUS_OK;
	Sensor_InternalData.LastDistance	= DistanceCm;
	Sensor_InternalData.LastMeasurement	= SENSOR_MEAS_VALID;
	Sensor_InternalData.TimeoutCounter	= 0U;

	// Send Data to RTE
	(void)Rte_Write_Distance(DistanceCm);
}

// Get last sensor status
Sensor_StatusType Sensor_GetStatus(void)
{
	return Sensor_InternalData.Status;
}

// Internal Api
void Sensor_TriggerPulse(void)
{
	(void)SensorIf_TriggerMeasurement();
}

Std_ReturnType Sensor_ReadEcho(Sensor_DistanceCmType* DistanceCm)
{
	SensorIf_MeasurementType		Meas;
	SensorIf_StatusType				IfStatus;

	if(DistanceCm == NULL_PTR) return E_NOT_OK;

	IfStatus = SensorIf_ReadMeasurement(&Meas);

	if(IfStatus != SENSORIF_STATUS_OK) return E_NOT_OK;
	if(Meas.Status != SENSORIF_MEAS_VALID) return E_NOT_OK;

	*DistanceCm = (Sensor_DistanceCmType)Meas.DistanceCm;

	return E_OK;
}

Sensor_MeasurementStatusType Sensor_ValidateDistance(Sensor_DistanceCmType DistanceCm)
{
	if( (DistanceCm > SENSORIF_MAX_DISTANCE_CM) || (DistanceCm < SENSORIF_MIN_DISTANCE_CM))
	{
		return SENSOR_MEAS_INVALID;
	}

	return SENSOR_MEAS_VALID;
}





