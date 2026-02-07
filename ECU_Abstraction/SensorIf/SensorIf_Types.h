/* =====================================================================================================================
 *  File        : SensorIf_Types.h
 *  Layer       : Abstraction
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Provide API send/receiver Can driver for services/applications
 *  Notes       :
 * ===================================================================================================================*/


#ifndef SENSORIF_SENSORIF_TYPES_H_
#define SENSORIF_SENSORIF_TYPES_H_

#include "Std_Types.h"

/* ============================================================
 *  Type Definitions
 * ============================================================ */
// Distance type centimeters
typedef uint16 SensorIf_DistanceCmType;

// Raw echo pulse duration in microseconds
typedef uint32 SensorIf_EchoTimeUsType;

// Sensor measurement validity
typedef enum
{
	SENSORIF_MEASUREMENT_INVALID	= 0,
	SENSORIF_MEASUREMENT_VALID
} SensorIf_MeasurementValidityType;

typedef enum
{
    SENSORIF_MEAS_INVALID = 0,
    SENSORIF_MEAS_VALID,
    SENSORIF_MEAS_TIMEOUT
} SensorIf_MeasurementStatusType;

typedef enum
{
	SENSORIF_STATUS_OK = 0,
	SENSORIF_STATUS_NO_ECHO,
	SENSORIF_STATUS_OUT_OF_RANGE,
	SENSORIF_STATUS_HW_ERROR,
	SENSORIF_STATUS_INVALID,
	SENSORIF_STATUS_NOT_INITIALIZED,
	SENSORIF_STATUS_NOT_READY
} SensorIf_StatusType;

// Sensor type identifier
typedef enum
{
	SENSORIF_SENSOR_ULTRASONIC	= 0,
	SENSORIF_SENSOR_RESERVED
} SensorIf_SensorType;

// Complete sensor measurement data structure
typedef struct
{
	SensorIf_SensorType					SensorType;
	SensorIf_DistanceCmType				DistanceCm;
	SensorIf_EchoTimeUsType				EchoTimeUs;
	SensorIf_MeasurementValidityType	Validity;
	SensorIf_MeasurementStatusType		Status;
} SensorIf_MeasurementType;


#endif /* SENSORIF_SENSORIF_TYPES_H_ */
