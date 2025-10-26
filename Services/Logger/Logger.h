/* =====================================================================================================================
 *  File        : LOGGER
 *  Layer       : Services
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : write level log with tag module
 *  Depends     :
 * ===================================================================================================================*/

#ifndef LOGGER_LOGGER_H_
#define LOGGER_LOGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Std_Types.h"
#include "ComStack_Types.h"

#include "LogLevels.h"
#include "LogTags.h"

//#include "Det.h"

/* ==============================
 *        VERSION & IDs
 * ============================== */
#define LOGGER_VENDOR_ID				(0x00u)
#define LOGGER_MODULE_ID				(0x52u)
#define LOGGER_INSTANCE_ID				(0x00u)

#define LOGGER_AR_RELEASE_MAJOR_VERSION	(1u)
#define LOGGER_AR_RELEASE_MINOR_VERSION	(0u)
#define LOGGER_AR_RELEASE_PATCH_VERSION	(0u)

#define LOGGER_SW_MAJOR_VERSION			(1u)
#define LOGGER_SW_MINOR_VERSION			(0u)
#define LOGGER_SW_PATCH_VERSION			(0u)

/* ==============================
 *     BUILD-TIME SWITCHES
 * ============================== */
#ifndef LOGGER_CFG_ENABLE
#define LOGGER_CFG_ENABLE				(1u)
#endif

#ifndef LOGGER_CFG_DEV_ERROR_DETECT
#define LOGGER_CFG_DEV_ERROR_DETECT		STD_ON
#endif

// Level of log default
#ifndef LOGGER_CFG_DEFAULT_LEVEL
#define LOGGER_CFG_DEFAULT_LEVEL		(LOG_LEVEL_INFO)
#endif

// Enable prefix timestamp [ms]
#ifndef LOGGER_CFG_ENABLE_TIMESTAMP_MS
#define LOGGER_CFG_ENABLE_TIMESTAMP_MS	(0u)
#endif

// Internal Buffer size
#ifndef LOGGER_CFG_FMT_BUF_SIZE
#define LOGGER_CFG_FMT_BUF_SIZE			(128u)
#endif

// Filter Bitmask
#ifndef LOGGER_CFG_ENABLE_TAG_FILTER
#define LOGGER_CFG_ENABLE_TAG_FILTER	(1u)
#endif

/* ==============================
 *       API IDs (for DET)
 * ============================== */
#define LOGGER_API_ID_INIT				(0x01u)
#define LOGGER_API_ID_DEINIT			(0x02u)
#define LOGGER_API_ID_ISINIT			(0x03u)
#define LOGGER_API_ID_SETLEVEL			(0x04u)
#define LOGGER_API_ID_GETLEVEL			(0x05u)
#define LOGGER_API_ID_SETTAGMASK		(0x06u)
#define LOGGER_API_ID_WRITE				(0x07u)
#define LOGGER_API_ID_WRITELINE			(0x08u)
#define LOGGER_API_ID_PRINTF			(0x09u)
#define LOGGER_API_ID_LOGF				(0x0Au)
#define LOGGER_API_ID_HEXDUMP			(0x0Bu)

/* ==============================
 *       DET ERROR CODES
 * ============================== */
#define LOGGER_E_UNINIT					(0x01u)
#define LOGGER_E_PARAM_POINTER			(0x02u)
#define LOGGER_E_PARAM_LEVEL			(0x03u)
#define LOGGER_E_BUSY					(0x04u)

/* ==============================
 *       DET REPORT MACRO
 * ============================== */
#if(LOGGER_CFG_DEV_ERROR_DETECT == STD_ON)
#define LOGGER_DET_REPORT(_api,_err) \
	Det_ReportError(LOGGER_MODULE_ID,LOGGER_INSTANCE_ID,(_api),(_err));
#else
#define LOGGER_DET_REPORT ((void)0)
#endif

/* ==============================
 *            TYPES
 * ============================== */
typedef uint8 	Logger_LevelType; // Log_LEVEL_OFF/ERROR/WARN/INFO/DEBUG
typedef uint32 	Logger_TagMaskType; //Bitmask
typedef Std_ReturnType (*Logger_OutputFnType)(const uint8* data, uint16 len); // Backend function
typedef Std_ReturnType (*Logger_OutputLineFnType)(const char* sctr);
//config runtime for Logger
typedef struct
{
	Logger_LevelType 		defaultLevel;		//log level after Init
	Logger_TagMaskType 		enableTagsMask;		//Bitmask tag.
	Logger_OutputFnType		outWrite;			// out raw bytes
	Logger_OutputLineFnType outWriteLine; 		// out a line (CR/LF)
#if(LOGGER_CFG_ENABLE_TIMESTAMP_MS == 1)
	uint32					(*getTimeMs)(void);
#endif
} Logger_ConfigType;

/* ==============================
 *            API
 * ============================== */
#if (LOGGER_CFG_ENABLE == 1)

/* Init / free */
void 		Logger_Init(const Logger_ConfigType* Cfg);
void		Logger_Deinit(void);
bool		Logger_IsInitialized(void);

/* log level and Tag mask */
Std_ReturnType 		Logger_SetLevel(Logger_LevelType level);
Logger_LevelType	Logger_GetLevel(void);

#if (LOGGER_CFG_ENABLE_TAG_FILTER == 1)
void 			Logger_SetEnableTags(Logger_TagMaskType mask);
Logger_TagMaskType 		Logger_GetEnableTags(void);
#endif

/*
 * Raw output: prefixed in logger format (ECU, lvl, tag, ts) when called via Logf/Printf.
 * These two functions are output directly to the backend (no auto-prefix) – use internally if needed.
 */
Std_ReturnType Logger_Write(const uint8* data, uint16 len);
Std_ReturnType Logger_WriteLine(const char* cstr);

/* Log formatted */
Std_ReturnType Logger_Logf(Logger_LevelType level, uint32 tagMask, const char* fmt,...);

/*
 * Printf fast with current log level
 */
Std_ReturnType Logger_Printf(const char* fmt, ...);

/*
 * Dump Buffer
 */
Std_ReturnType Logger_HexDump(Logger_LevelType level, uint32 tagMask, const uint8* data, uint16 len);

#else // LOGGER_CFG_ENABLE == 0
static inline void Logger_Init(const Logger_ConfigType* cfg) {(void)Cfg;}
static inline void Logger_Deinit(void) {}
static inline bool Logger_IsInitialized(void) {return FALSE;}
static inline Std_ReturnType Logger_SetLevel(Logger_LevelType level)	{(void)level;return E_OK;}
static inline Logger_LevelType Logger_GetLevel(void) {return (Logger_LevelType)0;}
static inline Std_ReturnType Logger_Write(const uint8* d, uint16 l) {(void)d;(void)l;return E_OK;}
static inline Std_ReturnType Logger_WriteLine(const char* s) {(void)s; return E_OK;}
static inline Std_ReturnType Logger_Logf(Logger_LevelType lv, uint32 tg, const char* f, ...) {(void) lv;(void)tg;(void)f;return E_OK;}
static inline Std_ReturnType Logger_Printf(const char* f, ...) {(void)f; return E_OK;}
static inline Std_ReturnType Logger_HexDump(Logger_LevelType lv, uint32 tg, const uint8* d, uint16 l) {(void)lv;(void) tg; (void)d, (void)l; return E_OK;}
#endif //LOGGER_CFG_ENABLE

/* ==============================
 *         HELPER MACROS
 * ============================== */
#ifndef LOG_LEVEL_OFF
#define LOG_LEVEL_OFF		0u
#endif

#ifndef LOG_LEVEL_ERROR
#define LOG_LEVEL_ERROR		1u
#endif

#ifndef LOG_LEVEL_WARN
#define LOG_LEVEL_WARN		2u
#endif

#ifndef LOG_LEVEL_INFO
#define LOG_LEVEL_INFO		3u
#endif

#ifndef LOG_LEVEL_DEBUG
#define LOG_LEVEL_DEBUG		4u
#endif

/* Macro follow level tag */
#define LOG_ERROR(TAG, FMT, ...) (void)Logger_Logf(LOG_LEVEL_ERROR, (uint32)(TAG), FMT, ##__VA_ARGS__)
#define LOG_WARN(TAG, FMT, ...) (void)Logger_Logf(LOG_LEVEL_WARN, (uint32)(TAG), FMT, ##__VA_ARGS__)
#define LOG_INFO(TAG, FMT, ...) (void)Logger_Logf(LOG_LEVEL_INFO, (uint32)(TAG), FMT, ##__VA_ARGS__)
#define LOG_DEBUG(TAG, FMT, ...) (void)Logger_Logf(LOG_LEVEL_DEBUG, (uint32)(TAG), FMT, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_LOGGER_H_ */
