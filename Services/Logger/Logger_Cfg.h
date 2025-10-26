/* =====================================================================================================================
 *  File        : Logger_Cfg.h
 *  Layer       : Service
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Set compile-time for logger service
 *  Depends     :
 * ===================================================================================================================*/

#ifndef LOGGER_LOGGER_CFG_H_
#define LOGGER_LOGGER_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Logger.h"

/* Enable/Disable module logger */
#define LOGGER_CFG_ENABLE				(1u)

/* Enable error DEV through Det */
#define LOGGER_CFG_DEV_ERROR_DETECT		STD_ON

/* Level log default */
#define LOGGER_CFG_DEFAULT_LEVEL		(LOG_LEVEL_INFO)

/* Enable timestamp [ms] from prefix */
#define LOGGER_CFG_ENABLE_TIMESTAMP_MS	(0u)

/* Internal buffer size */
#define LOGGER_CFG_FMT_BUF_SIZE			(128u)

/* Filter follow Tag - if 0: skip filter */
#define LOGGER_CFG_ENABLE_TAG_FILTER	(1u)

// prefix option display

#ifndef LOGGER_CFG_PREFIX_ENABLE
#define LOGGER_CFG_PREFIX_ENABLE		(1u)
#endif

#ifndef LOGGER_CFG_PREFIX_ECU_STR
#define LOGGER_CFG_PREFIX_ECU_STR		"SENSOR"
#endif

#ifndef LOGGER_CFG_PREFIX_SHOW_TAG
#define LOGGER_CFG_PREFIX_SHOW_TAG		(1u)
#endif

#ifndef LOGGER_CFG_PREFIX_SHOW_LEVEL
#define LOGGER_CFG_PREFIX_SHOW_LEVEL	(1u)
#endif

#ifndef LOGGER_CFG_CRLF_STYLE
#define LOGGER_CFG_CRLF_STYLE	(1u)
#endif

#ifndef LOGGER_CFG_BACKEND_NONBLOCKING
#define LOGGER_CFG_BACKEND_NONBLOCKING	(1u)
#endif

#ifndef LOGGER_CFG_MAX_LINE_LEN
#define LOGGER_CFG_MAX_LINE_LEN	(1u)
#endif

#ifndef LOGGER_SPACE_AFTER_PREFIX
#define LOGGER_SPACE_AFTER_PREFIX	(1u)
#endif

#ifdef __cplusplus
}
#endif
#endif /* LOGGER_LOGGER_CFG_H_ */
