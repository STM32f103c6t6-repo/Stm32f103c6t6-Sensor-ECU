/* =====================================================================================================================
 *  File        : Rte_Cfg.h
 *  Layer       : RTE
 *  ECU         : STM32F103C6T6
 *  Purpose     :
 *  Depends     :
 * ===================================================================================================================*/

#ifndef RTE_CFG_H_
#define RTE_CFG_H_

#include "Rte_Types.h"

/* ============================================
 * General config
 * ============================================ */
#define RTE_NUM_SWCS			2u
#define RTE_NUM_RUNNABLES		2u

/* ============================================
 * Runnable ids
 * ============================================ */
#define	RTE_RUNNABLE_SENSOR		((Rte_RunnableIdType)0x10u)
#define RTE_RUNNABLE_SENSOR		((Rte_RunnableIdType)0x11u)

/* ============================================
 * PORT DEFINITIONS
 * ============================================ */
// Sensor SWC Ports
#define RTE_PORT_SENSOR_OUT		((Rte_PortIdType)0x20u)

// Control SWC Ports
#define RTE_PORT_CONTROL_IN		((Rte_PortIdType)0x21u)

/* ============================================
 * INIT Values
 * ============================================ */
#define RTE_INIT_DISTANCE_VALUE			(0u)
#define RTE_INIT_OBSTACLE_STATE_VALUE	(0u)

#endif /* RTE_CFG_H_ */
