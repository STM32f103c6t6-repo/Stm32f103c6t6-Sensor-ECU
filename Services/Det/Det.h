/* =====================================================================================================================
 *  File        : LOGGER
 *  Layer       : Services
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Development Error Tracer
 *  Depends     :
 * ===================================================================================================================*/

#ifndef DET_DET_H_
#define DET_DET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Std_Types.h"

/* ==============================
 *      VERSION & IDENTIFIERS
 * ============================== */

#define DET_VENDOR_ID                     	(1u)
#define DET_MODULE_ID                     	(0x00u)
#define DET_INSTANCE_ID                   	(0x00u)

#define DET_AR_RELEASE_MAJOR_VERSION		(1u)
#define DET_AR_RELEASE_MINOR_VERSION		(0u)
#define DET_AR_RELEASE_PATCH_VERSION		(0u)

#define DET_SW_MAJOR_VERSION				(1u)
#define DET_SW_MINOR_VERSION				(0u)
#define DET_SW_PATCH_VERSION				(0u)

/* ==============================
 *      BUILD CONFIG SWITCHES
 * ============================== */
#ifndef DET_CFG_ENABLE_HISTORY
#define DET_CFG_ENABLE_HISTORY				(1u)
#endif

#ifndef DET_CFG_HISTORY_SIZE
#define DET_CFG_HISTORY_SIZE				(1u)
#endif

#ifndef DET_CFG_HALT_ON_ERROR
#define DET_CFG_HALT_ON_ERROR				(0u)
#endif

/* ==============================
 *      TYPES
 * ============================== */
typedef struct {
	uint16 moduleId;
	uint8 instanceId;
	uint8 apiId;
	uint8 errorId;
	uint32 timestampMs;
} Det_ErrorEntryType;

/* ==============================
 *              API
 * ============================== */
// initial DET
void Det_Init(void);

// API for module notify Error
void Det_ReportError(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId);

// get last log error
boolean Det_GetLastError(Det_ErrorEntryType* out);

// get count number of history error
uint16 Det_GetHistoryCount(void);

// Get version
boolean Det_GetHistoryAt(uint16 index, Det_ErrorEntryType* out);
// delete history Error
void Det_ClearHistory(void);

// Push all history to backend logging
void Det_FlushToLogger(void (*LoggerLineFn)(const char* line));

#ifdef __cplusplus
}
#endif


#endif /* DET_DET_H_ */
