/* =====================================================================================================================
 *  File        : Sensor_Types.h
 *  Layer       : Application
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Notes       :
 * ===================================================================================================================*/


#ifndef SWC_SENSOR_SENSOR_TYPES_H_
#define SWC_SENSOR_SENSOR_TYPES_H_

#include "Std_Types.h"

// Distance type
typedef uint16 Sensor_DistanceCmType;

// Sensor status
typedef enum
{
	SENSOR_STATUS_UNINIT	 = 0,
	SENSOR_STATUS_OK,
	SENSOR_STATUS_NO_ECHO,
	SENSOR_STATUS_TIMEOUT,
	SENSOR_STATUS_HW_ERROR
} Sensor_StatusType;

typedef enum
{
	SENSOR_MEAS_INVALID = 0,
	SENSOR_MEAS_VALID
} Sensor_MeasurementStatusType;

#endif /* SWC_SENSOR_SENSOR_TYPES_H_ */
