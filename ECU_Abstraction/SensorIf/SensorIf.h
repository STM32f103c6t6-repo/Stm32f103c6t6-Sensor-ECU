/* =====================================================================================================================
 *  File        : SensorIf.h
 *  Layer       : Abstraction
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Provide API send/receiver Can driver for services/applications
 *  Notes       :
 * ===================================================================================================================*/
#ifndef SENSORIF_SENSORIF_H_
#define SENSORIF_SENSORIF_H_

#include "SensorIf_Types.h"

/* ============================================================
 *  Configuration Macros
 * ============================================================ */
// Maximum measurable distance for ultrasonic sensor (cm)
#define SENSORIF_MAX_DISTANCE_CM		(400U)

// Minimum measurable distance
#define SENSORIF_MIN_DISTANCE_CM		(20U)

// Timeout echo reception
#define SENSORIF_ECHO_TIMOUT_US			(30000U)

/* ============================================================
 *  Public API Prototype
 * ============================================================ */
// initialize sensor interface module
void SensorIf_Init(void);

// Deinitialize Sensor Interface module
void SensorIf_DeInit(void);

// Trigger a distance measurement
SensorIf_StatusType	SensorIf_TriggerMeasurement(void);

// Read distance measurement result
SensorIf_StatusType	SensorIf_ReadMeasurement( SensorIf_MeasurementType* Measurement );

// Perform complete synchronous distance measurement
SensorIf_StatusType	SensorIf_GetDistanceCm(SensorIf_DistanceCmType* DistanceCm);

// Periodic processing function
void SensorIf_Mainfunction(void);

// Check whether SensorIf is initialized
boolean SensorIf_IsInitialized(void);

#endif /* SENSORIF_SENSORIF_H_ */
