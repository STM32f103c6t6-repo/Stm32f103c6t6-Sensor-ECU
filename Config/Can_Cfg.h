/* =====================================================================================================================
 *  File        : Can_Cfg.h
 *  Layer       : MCAL
 *  ECU         : STM32F103C6T6
 *  Purpose     : Config parameter for Can driver.
 *  Notes       :
 * ===================================================================================================================*/


#ifndef CAN_CFG_H_
#define CAN_CFG_H_

#if __cplusplus
extern "C" {
#endif

#include "Can_Types.h"
#include "stm32f103xx_regs.h"

#define CAN_CTL_NUM						1
#define CAN_CTL0_ID						0

#define CAN_SYNC_SEGMENT_CTL0			1
#define CAN_BIT_SEGMENT_1_CTL0			13
#define CAN_BIT_SEGMENT_2_CTL0			4
#define CAN_PREC_SCALE_CTL0				4

#if __cplusplus
}
#endif

#endif /* CAN_CFG_H_ */
