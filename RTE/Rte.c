/* =====================================================================================================================
 *  File        : Rte.c
 *  Layer       : Rte
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Depends     :
 * ===================================================================================================================*/

#include "Rte.h"

/*===================== Local Types ============================*/
// Internal RTE signal buffer
typedef struct
{
	uint8						UpDated;
	Rte_SignalStatusType		Status;
	uint32						Data;
} Rte_InternalSignalType;

/*===================== Local Variable ============================*/
// Internal Buffer for signals
static Rte_InternalSignalType	Rte_Signal_Distance;
static Rte_InternalSignalType	Rte_Signal_Obstacle;

// System mode
static Rte_SystemModeType		Rte_SystemMode	= RTE_MODE_INIT;

// RTE instance ID
static Rte_InstanceIdType		Rte_InstanceId = 0u;

/*===================== Local Functions ============================*/
static void Rte_ClearSignal(Rte_InternalSignalType* sig) {
	sig -> UpDated = 0u;
	sig -> Status  = RTE_SIGNAL_INVALID;
	sig -> Data	   = 0u;
}

/* =====================================================================================================================
 *  RTE Init/DeInit
 * ===================================================================================================================*/

// Initialize RTE module
void Rte_Init(void)
{
	Rte_ClearSignal(&Rte_Signal_Distance);
	Rte_ClearSignal(&Rte_Signal_Obstacle);

	Rte_SystemMode = RTE_MODE_NORMAL;
}

// Deinitialize RTE module
void Rte_Deinit(void)
{
	Rte_SystemMode = RTE_MODE_INIT;
}

/* =====================================================================================================================
 *  Sender/Receiver APIs
 * ===================================================================================================================*/

/* Sensor Nodes API */
// Write distance value from sensor
Std_ReturnType	Rte_Write_Distance(Rte_DistanceType Distance)
{
	Std_ReturnType Ret;

	Ret = Com_SendSignal(RTE_SIGNAL_DISTANCE, &Distance);

	if(Ret == E_OK)
	{
		Rte_Signal_Distance.Data	= Distance;
		Rte_Signal_Distance.Status	= RTE_SIGNAL_VALID;
		Rte_Signal_Distance.UpDated	= 1u;
	}

	return Ret;
}

// Write obstacle state
Std_ReturnType	Rte_Write_ObstacleState(Rte_ObstacleStateType State)
{
	Std_ReturnType Ret;

	Ret = Com_SendSignal(RTE_SIGNAL_OBSTACLE, &State);
	if(Ret == E_OK)
	{
		Rte_Signal_Obstacle.Data	= State;
		Rte_Signal_Obstacle.Status	= RTE_SIGNAL_VALID;
		Rte_Signal_Obstacle.UpDated	= 1u;
	}

	return Ret;
}

/* Motor Node APIs */
// Distance value in Motor Control
Std_ReturnType	Rte_Read_Distance(Rte_DistanceType* Distance )
{
	if((Distance == NULL_PTR) || (Rte_Signal_Distance.Status != RTE_SIGNAL_VALID))
	{
		return RTE_E_NO_DATA;
	}

	*Distance	= (Rte_DistanceType) Rte_Signal_Distance.Data;
	Rte_Signal_Distance.UpDated	= 0u;

	return RTE_E_OK;
}

// Read obstacle state
Std_ReturnType	Rte_Read_ObstacleState(Rte_ObstacleStateType* State)
{
	if((State == NULL_PTR) || (Rte_Signal_Obstacle.Status != RTE_SIGNAL_VALID))
	{
		return RTE_E_NO_DATA;
	}

	*State = (Rte_ObstacleStateType) Rte_Signal_Obstacle.Data;
	Rte_Signal_Obstacle.UpDated = 0u;

	return RTE_E_OK;
}

/* =====================================================================================================================
 *  Client/Server APIs
 * ===================================================================================================================*/

// Stop motor immediately
Std_ReturnType	Rte_Call_StopMotor(void)
{
	Rte_SpeedType speed = 0u;
	return Com_SendSignal(RTE_SIGNAL_SPEED, &speed);
}

// Start motor
Std_ReturnType	Rte_Call_StartMotor(void)
{
	Rte_SpeedType speed = 100u;
	return Com_SendSignal(RTE_SIGNAL_SPEED, &speed);
}

/* =====================================================================================================================
 *  Runnable Entity Prototypes
 * ===================================================================================================================*/

// Runnable for sensor acquisition
void Rte_Runnable_Sensor(void)
{
	Rte_DistanceType	Distance;

	Distance = 250u; // Test

	(void)Rte_Write_Distance(Distance);

	if(Distance < 300u)
	{
		(void)Rte_Write_ObstacleState(RTE_OBSTACLE_DETECT);
	} else {
		(void)Rte_Write_ObstacleState(RTE_OBSTACLE_NOT_DETECT);
	}
}

// Runnable for motor control logic
void Rte_Runnable_MotorControl(void)
{
	Rte_DistanceType Distance;

	if(Rte_Read_Distance(&Distance) == RTE_E_OK)
	{
		if(Distance < 300u)
		{
			(void) Rte_Call_StopMotor();
		} else {
			(void) Rte_Call_StartMotor();
		}
	}
}

/* =====================================================================================================================
 *  Mode management APIs
 * ===================================================================================================================*/

// Get current system mode
Rte_SystemModeType Rte_GetSystemMode(void)
{
	return Rte_SystemMode;
}

// Set system mode
void Rte_SetSystemMode(Rte_SystemModeType Mode)
{
	Rte_SystemMode = Mode;
}

/* =====================================================================
 *  Signal Status APIs (optional but AUTOSAR-like)
 * ===================================================================== */

// Get signal validity status
Rte_SignalStatusType Rte_GetSignalStatus(Com_SignalIdType SignalId)
{
	switch (SignalId)
	{
	case RTE_SIGNAL_DISTANCE: return Rte_Signal_Distance.Status;
	case RTE_SIGNAL_OBSTACLE: return Rte_Signal_Obstacle.Status;
	default: return RTE_SIGNAL_INVALID;
	}
}

/* =====================================================================================================================
 *  Rte Instance Handling
 * ===================================================================================================================*/

// Get RTE instance ID
Rte_InstanceIdType Rte_GetInstanceId(void)
{
	return Rte_InstanceId;
}
