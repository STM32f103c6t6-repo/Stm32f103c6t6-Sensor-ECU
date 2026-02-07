/* =====================================================================================================================
 *  File        : SensorIf.c
 *  Layer       : Abstraction
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Apply CANIF declare API for STM32F1
 *  Depends     :
 * ===================================================================================================================*/

#include "SensorIf.h"
#include "Dio.h"
#include "Gpt.h"
#include "Gpt_Cfg.h"
#include "Port_Cfg.h"

#define SENSORIF_US_TO_CM(us)		((us)/58U)

/* ============================================================
 *  Local types
 * ============================================================ */
// Internal state machine
typedef enum
{
	SENSORIF_STATE_UNINIT			= 0U,
	SENSORIF_STATE_IDLE,
	SENSORIF_STATE_TRIGGERED,
	SENSORIF_STATE_WAIT_ECHO_START,
	SENSORIF_STATE_WAIT_ECHO_END,
	SENSORIF_STATE_DONE,
	SENSORIF_STATE_ERROR
} SensorIf_InternalStateType;

/* ============================================================
 *  Local variables
 * ============================================================ */
static boolean SensorIf_Initialized		= FALSE;
static SensorIf_InternalStateType	SensorIf_State = SENSORIF_STATE_UNINIT;
static SensorIf_MeasurementType SensorIf_LastMeasurement;

// Timing
static uint32 EchoStartTick 	= 0U;
static uint32 EchoEndTick		= 0U;
static uint32 TimeoutTick		= 0U;

/* ============================================================
 *	MCAL Abstraction
 * ============================================================ */
// These functions represent MCAL access
static void SensorIf_Mcal_SetTriggerHigh(void)
{
	Dio_WriteChannel(PORT_PIN_HCSR04_TRIG, TRUE);
}

static void SensorIf_Mcal_SetTriggerLow(void)
{
	Dio_WriteChannel(PORT_PIN_HCSR04_TRIG, FALSE);
}

static boolean SensorIf_Mcal_ReadEchoPin(void)
{
	return Dio_ReadChannel(PORT_PIN_HCSR04_ECHO);
}

static uint32 SensorIf_Mcal_GetMicroTick(void)
{
	return Gpt_GetTimeElapsed(GPT_CHANNEL_ID);
}

/* ============================================================
 *  Public API Prototype
 * ============================================================ */
// initialize sensor interface module
void SensorIf_Init(void)
{
	SensorIf_Initialized = TRUE;
	SensorIf_State	= SENSORIF_STATE_IDLE;

	SensorIf_LastMeasurement.DistanceCm = 0u;
	SensorIf_LastMeasurement.Status	= SENSORIF_MEAS_INVALID;
}

// Deinitialize Sensor Interface module
void SensorIf_DeInit(void)
{
	SensorIf_Initialized = FALSE;
	SensorIf_State	= SENSORIF_STATE_UNINIT;
}

// Trigger a distance measurement
SensorIf_StatusType	SensorIf_TriggerMeasurement(void)
{
	if((SensorIf_Initialized == FALSE) || (SensorIf_State != SENSORIF_STATE_IDLE))
	{
		return SENSORIF_STATUS_NOT_READY;
	}

	// send trigger pulse
	SensorIf_Mcal_SetTriggerHigh();
	// delay 10us
	SensorIf_Mcal_SetTriggerLow();

	TimeoutTick	= SensorIf_Mcal_GetMicroTick();
	SensorIf_State = SENSORIF_STATE_TRIGGERED;
	return SENSORIF_STATUS_OK;
}

// Read distance measurement result
SensorIf_StatusType	SensorIf_ReadMeasurement( SensorIf_MeasurementType* Measurement )
{
	if(Measurement == NULL_PTR) return SENSORIF_STATUS_INVALID;

	if(SensorIf_State != SENSORIF_STATE_DONE) return SENSORIF_STATUS_NOT_READY;

	*Measurement = SensorIf_LastMeasurement;

	SensorIf_State = SENSORIF_STATE_IDLE;

	return SENSORIF_STATUS_OK;
}

// Perform complete synchronous distance measurement
SensorIf_StatusType	SensorIf_GetDistanceCm(SensorIf_DistanceCmType* DistanceCm)
{
	SensorIf_MeasurementType	Meas;
	SensorIf_StatusType			Ret;

	if(DistanceCm == NULL_PTR) return SENSORIF_STATUS_INVALID;

	Ret = SensorIf_TriggerMeasurement();
	if(Ret != SENSORIF_STATUS_OK) return Ret;

	// Blocking wait
	while(SensorIf_State != SENSORIF_STATE_DONE)
	{
		SensorIf_Mainfunction();
	}

	Ret = SensorIf_ReadMeasurement(&Meas);
	if(Ret == SENSORIF_STATUS_OK)
	{
		*DistanceCm	= Meas.DistanceCm;
	}

	return Ret;
}

// Periodic processing function
void SensorIf_Mainfunction(void)
{
	uint32 CurrentTick;

	if(SensorIf_Initialized == FALSE)
	{
		return;
	}

	CurrentTick = SensorIf_Mcal_GetMicroTick();

	switch(SensorIf_State)
	{
	case SENSORIF_STATE_TRIGGERED:
		SensorIf_State = SENSORIF_STATE_WAIT_ECHO_START;
		break;

	case SENSORIF_STATE_WAIT_ECHO_START:
		if(SensorIf_Mcal_ReadEchoPin() == TRUE)
		{
			EchoStartTick = CurrentTick;
			SensorIf_State = SENSORIF_STATE_WAIT_ECHO_END;
		} else if ((CurrentTick - TimeoutTick ) > SENSORIF_ECHO_TIMOUT_US) {
			SensorIf_State = SENSORIF_STATE_ERROR;
		}
		break;
	case SENSORIF_STATE_WAIT_ECHO_END:
		if(SensorIf_Mcal_ReadEchoPin() == FALSE)
		{
			EchoEndTick = CurrentTick;

			uint32 PulseWidth = EchoEndTick - EchoStartTick;

			SensorIf_LastMeasurement.DistanceCm = (SensorIf_DistanceCmType) SENSORIF_US_TO_CM(PulseWidth);

			SensorIf_LastMeasurement.Status = SENSORIF_MEAS_INVALID;

			SensorIf_State = SENSORIF_STATE_DONE;
		}

		break;
	case SENSORIF_STATE_ERROR:
		SensorIf_LastMeasurement.Status = SENSORIF_MEAS_TIMEOUT;
		SensorIf_State = SENSORIF_STATE_IDLE;
		break;

	default:
		break;
	}
}

// Check whether SensorIf is initialized
boolean SensorIf_IsInitialized(void);
