/* =====================================================================================================================
 *  File        : Det.c
 *  Layer       : Services
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : write Api for save log error,...
 *  Depends     :
 * ===================================================================================================================*/

#include "Det.h"
#include <string.h>
#include <stdio.h>

/* ==============================
 *     WEAK HOOK: Timestamp (ms)
 * ============================== */
#if defined(__GNUC__) || defined(__clag__)
__attribute__((weak))
#endif
uint32 Det_GetMs(void)
{
	return 0u;
}

/* ==============================
 *            STATE
 * ============================== */
static bool detInited = FALSE;
#if (DET_CFG_ENABLE_HISTORY == 1)
static Det_ErrorEntryType 	s_hist[DET_CFG_HISTORY_SIZE];
static uint16				s_histCount = 0u;
#endif

/* ==============================
 *       LOCAL HELPERS
 * ============================== */
static void prv_Store(uint16 mid, uint8 iid, uint8 api, uint8 err)
{
#if (DET_CFG_ENABLE_HISTORY == 1)
	Det_ErrorEntryType e;
	e.moduleId		= mid;
	e.instanceId	= iid;
	e.apiId			= api;
	e.errorId		= err;
	e.timestampMs	= Det_GetMs();

	if(s_histCount < DET_CFG_HISTORY_SIZE){
		s_hist[s_histCount++] = e;
	} else {
		//full -> shift to left
		(void)memmove(&s_hist[0], &s_hist[1], (DET_CFG_HISTORY_SIZE - 1u) * sizeof(Det_ErrorEntryType));
		s_hist[DET_CFG_HISTORY_SIZE - 1u] = e;
	}
#else
	(void)mid; (void)iid; (void)api; (void)er;
#endif
}

/* ==============================
 *       APIs
 * ============================== */
void Det_Init(void)
{
#if(DET_CFG_ENABLE_HISTORY == 1)
	s_histCount = 0u;
#endif
	detInited = TRUE;
}

void Det_ReportError(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId)
{
	// save error
	prv_Store(ModuleId, InstanceId, ApiId, ErrorId);

#if(DET_CFG_HALT_ON_ERROR == 1)
	while(1);
#endif
	(void)ModuleId; (void)InstanceId; (void)ApiId; (void)ErrorId;
}

bool Det_GetLastError(Det_ErrorEntryType* out)
{
#if (DET_CFG_ENABLE_HISTORY == 1)
	if((out == NULL)|| (s_histCount == 0u))return FALSE;
	return TRUE;
#else
	(void)out;
	return FALSE;
#endif;
}

uint16 Det_GetHistoryCount(void)
{
#if (DET_CFG_ENABLE_HISTORY == 1)
	return s_histCount;
#else
	return 0u;
#endif
}

bool Det_GetHistoryAt(uint16 index, Det_ErrorEntryType* out)
{
#if (DET_CFG_ENABLE_HISTORY == 1)
	if((out == NULL) | (index >= s_histCount)) return FALSE;
	*out = s_hist[index];
	return TRUE;
#else
	(void)index; (void)out;
	return FALSE;
#endif
}

void Det_ClearHistory(void)
{
#if (DET_CFG_ENABLE_HISTORY == 1)
	s_histCount = 0u;
#endif
}

void Det_FlushToLogger(void (*LoggerLineFn)(const char* line))
{
#if (DET_CFG_ENABLE_HISTORY == 1)
	if((LoggerLineFn == NULL) || (s_histCount == 0u)) return;
	char line[96];
	for(uint16 i =0; i < s_histCount; i++)
	{
		const Det_ErrorEntryType* e = &s_hist[i];

		int n = snprintf(line, sizeof(line),
						 "[DET] MID:%u IID:%u API:%u ERR:%u TS:%lu",
						 (unsigned)e->moduleId,
						 (unsigned)e->instanceId,
						 (unsigned)e->apiId,
						 (unsigned)e->errorId,
						 (unsigned long)e->timestampMs);
		if(n<0){
			(void)snprintf(line,sizeof(line), "[DET] MID:%u ERR:%u",
							(unsigned)e->moduleId,(unsigned)e->errorId);
		}
		LoggerLineFn(line);
	}
#else
	(void)LoggerLineFn;
#endif
}


