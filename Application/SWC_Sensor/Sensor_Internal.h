/* =====================================================================================================================
 *  File        : Sensor_Internal.h
 *  Layer       : Application
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Notes       :
 * ===================================================================================================================*/


#ifndef SWC_SENSOR_SENSOR_INTERNAL_H_
#define SWC_SENSOR_SENSOR_INTERNAL_H_

#include "Sensor_Types.h"

// Internal state
typedef struct
{
	Sensor_StatusType				Status;
	Sensor_DistanceCmType			LastDistance;
	Sensor_MeasurementStatusType	LastMeasurement;
	uint8							TimeoutCounter;
} Sensor_InternalDataType;

// Internal Api
void Sensor_TriggerPulse(void);
Std_ReturnType Sensor_ReadEcho(Sensor_DistanceCmType* DistanceCm);
Sensor_MeasurementStatusType Sensor_ValidateDistance(Sensor_DistanceCmType DistanceCm);

#endif /* SWC_SENSOR_SENSOR_INTERNAL_H_ */
