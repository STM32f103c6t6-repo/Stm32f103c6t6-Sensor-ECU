/* =====================================================================================================================
 *  File        : Icu_Cfg.h
 *  Layer		: MCAL
 *  ECU         : Sensor_ECU
 *  Purpose     : Configure Icu HCSR04 (MCAL/Mcu)
 *  Notes       :
 * ===================================================================================================================*/


#ifndef ICU_CFG_H_
#define ICU_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Icu_Types.h"
#include "stm32f103xx_regs.h"

#define ICU_CHANNEL_ECHO		0
#define ICU_TIMER_TIM2			TIM2
#define ICU_CHANNEL_1			1
#define ICU_CNT_CHANNEL			1

#ifdef __cplusplus
}
#endif

#endif /* ICU_CFG_H_ */
