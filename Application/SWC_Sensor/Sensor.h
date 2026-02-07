/* =====================================================================================================================
 *  File        : Sensor.h
 *  Layer       : Application
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Notes       :
 * ===================================================================================================================*/

#ifndef SWC_SENSOR_SENSOR_H_
#define SWC_SENSOR_SENSOR_H_

#include "Sensor_Types.h"

// Init sensor SWC
void Sensor_Init(void);

// Periodic runnable
void Sensor_MainFunction(void);

// Get last sensor status
Sensor_StatusType Sensor_GetStatus(void);

#endif /* SWC_SENSOR_SENSOR_H_ */
