/* =====================================================================================================================
 *  File        : Rte.h
 *  Layer       : RTE
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Depends     :
 * ===================================================================================================================*/


#ifndef RTE_H_
#define RTE_H_

#include "Std_Types.h"
#include "Rte_Types.h"
#include "Com.h"

/* =====================================================================================================================
 *  RTE Init/DeInit
 * ===================================================================================================================*/

// Initialize RTE module
void Rte_Init(void);

// Deinitialize RTE module
void Rte_Deinit(void);

/* =====================================================================================================================
 *  Sender/Receiver APIs
 * ===================================================================================================================*/

/* Sensor Nodes API */
// Write distance value from sensor
Std_ReturnType	Rte_Write_Distance(Rte_DistanceType Distance);

// Write obstacle state
Std_ReturnType	Rte_Write_ObstacleState(Rte_ObstacleStateType State);

/* Motor Node APIs */
// Distance value in Motor Control
Std_ReturnType	Rte_Read_Distance(Rte_DistanceType* Distance );

// Read obstacle state
Std_ReturnType	Rte_Read_ObstacleState(Rte_ObstacleStateType* State);

/* =====================================================================================================================
 *  Client/Server APIs
 * ===================================================================================================================*/

// Stop motor immediately
Std_ReturnType	Rte_Call_StopMotor(void);

// Start motor
Std_ReturnType	Rte_Call_StartMotor(void);

/* =====================================================================================================================
 *  Runnable Entity Prototypes
 * ===================================================================================================================*/

// Runnable for sensor acquisition
void Rte_Runnable_Sensor(void);

// Runnable for motor control logic
void Rte_Runnable_MotorControl(void);

/* =====================================================================================================================
 *  Mode management APIs
 * ===================================================================================================================*/

// Get current system mode
Rte_SystemModeType Rte_GetSystemMode(void);

// Set system mode
void Rte_SetSystemMode(Rte_SystemModeType Mode);

/* =====================================================================
 *  Signal Status APIs (optional but AUTOSAR-like)
 * ===================================================================== */

// Get signal validity status
Rte_SignalStatusType Rte_GetSignalStatus(Com_SignalIdType SignalId);

/* =====================================================================================================================
 *  Rte Instance Handling
 * ===================================================================================================================*/

// Get RTE instance ID
Rte_InstanceIdType Rte_GetInstanceId(void);


#endif /* RTE_H_ */
