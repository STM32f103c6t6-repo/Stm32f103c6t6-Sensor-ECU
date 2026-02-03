/* =====================================================================================================================
 *  File        : SysteamApp.h
 *  Layer       : Application
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Notes       :
 * ===================================================================================================================*/


#ifndef SYSTEMAPP_H_
#define SYSTEMAPP_H_

/* ============================================
 * Includes
 * ============================================*/
#include "Std_Types.h"

/* ============================================================
 *  Configuration Macros
 * ============================================================ */
// Enable/Disable debug logging for systemapp
#define SYSTEM_DEBUG_ENABLE			(STD_ON)

/* ============================================================
 *  Public API Prototype
 * ============================================================ */
// Initialize System application layer
void SystemApp_Init(void);

// Periodic main function of System Application
void SystemApp_MainFunction(void);

// Update system tick counter
void SystemApp_Tick1ms(void);


#endif /* SYSTEMAPP_H_ */
