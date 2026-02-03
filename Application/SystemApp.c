/* =====================================================================================================================
 *  File        : SystemApp.c
 *  Layer       : Application
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Depends     :
 * ===================================================================================================================*/

#include "SystemApp.h"

/* ============================================
 * Includes - AUTOSAR Services
 * ============================================*/
#include "EcuM.h"
#include "Det.h"
#include "Rte.h"

/* ============================================
 * Includes - Application SWCs
 * ============================================*/
#include "ObstacleDetection.h"
#include "SensorSupervisor.h"

/* ============================================================
 *  Local Variables
 * ============================================================ */
// System tick counter
static uint32 SystemApp_TickMs = 0U;

/* ============================================================
 *  Public API Implementation
 * ============================================================ */
// Initialize System Application
void SystemApp_Init(void)
{
	// RTE Init
	Rte_Init();

	// SWC Init
	ObstacleDetection_Init();
	SensorSupervisor_Init();
}

// Periodic main function of System Application
void SystemApp_MainFunction(void)
{

	// Check if it's time to run cyclic SWCs
	if((SystemApp_TickMs % 10U) == 0U)
	{
		// Obstacle detection logic
		ObstacleDetection_MainFunction();

		// Sensor supervision & decision
		SensorSupervisor_Runnable_10ms();
	}
}

// Update system tick counter
void SystemApp_Tick1ms(void)
{
	SystemApp_TickMs++;
}





