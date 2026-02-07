/* =====================================================================================================================
 *  File        : ObstacleDetection_Cfg.h
 *  Layer       : Application
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Depends     :
 * ===================================================================================================================*/


#ifndef SWC_SENSOR_SENSOR_CFG_H_
#define SWC_SENSOR_SENSOR_CFG_H_

// Trigger pulse with (us)
#define SENSOR_TRIG_PULSE_US			(10U)

// Max echo timeout (us) ~4m
#define SENSOR_ECHO_TIMEOUT_US			(25000U)

// Distance limits
#define SENSOR_MIN_DISTANCE				(2U)
#define SENSOR_MAX_DISTANCE				(400U)

// Task period
#define SENSOR_MAINFUNCTION_PERIOD_MS	(10U)

#endif /* SWC_SENSOR_SENSOR_CFG_H_ */
