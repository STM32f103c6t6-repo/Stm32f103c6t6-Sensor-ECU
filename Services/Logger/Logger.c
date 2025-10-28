/* =====================================================================================================================
 *  File        : Logger.c
 *  Layer       : Service
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     :
 *  Notes       :
 * ===================================================================================================================*/

#include "Logger.h"
#include "Logger_Cfg.h"

#if (LOGGER_CFG_ENABLE == 1)

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/* ==============================
 *       LOCAL STATE
 * ============================== */
static bool 				s_inited	= FALSE;
static Logger_LevelType		s_level		= LOGGER_CFG_DEFAULT_LEVEL;
#if(LOGGER_CFG_ENABLE_TAG_FILTER == 1)
static Logger_TagMaskType	s_tagMask	= 0xFFFFFFFFu;
#endif

static Logger_OutputFnType		s_outWrite	= NULL_PTR;
static Logger_OutputLineFnType	s_outWriteLine = NULL_PTR;

#if (LOGGER_CFG_ENABLE_TIMESTAMP_MS == 1)
static uint32 (*s_getTimeMs)(void) = NULL_PTR;
#endif

/* ==============================
 *       INTERNAL HELPERS
 * ============================== */
#if( LOGGER_CFG_DEV_ERROR_DETECT == STD_ON )
#define LOGGER_DET_CHK_INIT(api) \
	do{ if( s_inited == FALSE ) { LOGGER_DET_REPORT((api), LOGGER_E_UNINIT); return E_NOT_OK;}} while(0)
#else
#define LOGGER_DET_CHK_INIT(api) do { (void)(api);} while(0);
#endif

// Map Level -> short string
static const char* prv_LevelStr(Logger_LevelType lv)
{
	switch(lv)
	{
		case LOG_LEVEL_ERROR : return "ERROR";
		case LOG_LEVEL_WARN : return "WARN";
		case LOG_LEVEL_INFO : return "INFO";
		case LOG_LEVEL_DEBUG : return "DEBUG";
		case LOG_LEVEL_OFF :
		default:				return "OFF";
	}
}

// Build prefix into buf. return length written
static uint16 prv_BuidPrefix(char* buf, uint16 bufsize, Logger_LevelType lv, uint32 tagMask)
{
#if(LOGGER_CFG_PREFIX_ENABLE == 0)
	(void) buf; (void)bufsize; (void)lv; (void)tagMask;
	return 0u;
#else
	uint16 used = 0u;
#if (LOGGER_CFG_ENABLE_TIMESTAMP_MS == 1)
	if(s_getTimeMs != NULL_PTR){
		uint32 ms = s_getTimeMs();
		int n = snprintf(&buf[used], (bufsize > used) ? (bufsize - used) : 0, "[%lu]", (unsigned long)ms);
		if(n > 0) used = (uint16)((used +(uint16)n) < bufsize ? (used + (uint16)n): bufsize);
	}
#endif

#ifdef LOGGER_CFG_PREFIX_ECU_STR
	{
		int n = snprintf(&buf[used], (bufsize > used) ? (bufsize - used) : 0, "[%s]",LOGGER_CFG_PREFIX_ECU_STR );
		if (n > 0) used = (uint16)((used + (uint16)n) < bufsize ? (used+(uint16)n) : bufsize);
	}
#endif

#if (LOGGER_CFG_PREFIX_SHOW_LEVEL == 1)
	{
		const char* s = prv_LevelStr(lv);
		int n = snprintf(&buf[used], (bufsize > used) ? (bufsize - used) : 0, "[%s]",s );
		if(n > 0) used = (uint16)((used + (uint16)n) < bufsize ? (used + (uint16)n) : bufsize);
	}
#endif

#if (LOGGER_CFG_PREFIX_SHOW_TAG == 1)
	{
		unsigned long t = (unsigned long) tagMask;
		int n = snprintf(&buf[used], (bufsize > used) ? (bufsize - used) : 0, "[%08lx]",t );
		if(n > 0) used = (uint16)((used + (uint16)n) < bufsize ? (used + (uint16)n) : bufsize);
	}
#endif

#if (LOGGER_SPACE_AFTER_PREFIX == 1)
	if(used + 1u < bufsize) {buf[used++] = ' ';}
#endif
	return used;
#endif
}

//Push a raw buffer to backend; choose Write/WriteLine appropriately */
static Std_ReturnType prv_WriteLineRaw(const char* cstr)
{
	if(s_outWriteLine != NULL_PTR)
	{
		return s_outWriteLine(cstr);
	}

	if(s_outWrite != NULL_PTR)
	{
		uint16 len = (uint16) strlen(cstr);
		if(s_outWrite((const uint8*)cstr,len) != E_OK) return E_NOT_OK;
#if(LOGGER_CFG_CRLF_STYLE == 1)
		const uint8 crlf[2] = {'\r','\n'};
		return s_outWrite(crlf,2u);
#else
	const uint8 lf[1] = {'\n'};
	return s_outWrite(lf,1u);
#endif
	}
	return E_NOT_OK;
}

/* ==============================
 *       PUBLIC APIS
 * ============================== */
void Logger_Init(const Logger_ConfigType* Cfg)
{
	if(Cfg == NULL_PTR){
#if (LOGGER_CFG_DEV_ERROR_DETECT == STD_ON)
		LOGGER_DET_REPORT(LOGGER_API_ID_INIT, LOGGER_E_PARAM_POINTER);
#endif
		s_inited = FALSE;
		return;
	}

	s_level		= Cfg->defaultLevel;
#if (LOGGER_CFG_ENABLE_TAG_FILTER == 1)
	s_tagMask	= Cfg->enableTagsMask;
#endif
	s_outWrite	= Cfg->outWrite;
	s_outWriteLine = Cfg->outWriteLine;
#if (LOGGER_CFG_ENABLE_TIMESTAMP_MS == 1)
	s_getTimeMs	= Cfg->getTimeMs;
#endif
	s_inited	= TRUE;
}

void Logger_Deinit(void)
{
	s_inited = FALSE;
	s_outWrite = NULL_PTR;
	s_outWriteLine = NULL_PTR;
#if(LOGGER_CFG_ENABLE_TIMESTAMP_MS == 1)
	s_getTimeMs = NULL_PTR;
#endif
}

bool Logger_IsInitialized(void){
	return s_inited;
}

Std_ReturnType Logger_SetLevel(Logger_LevelType level)
{
	LOGGER_DET_CHK_INIT(LOGGER_API_ID_SETLEVEL);
	if(level > LOG_LEVEL_DEBUG) {
#if(LOGGER_CFG_DEV_ERROR_DETECT == STD_ON)
		LOGGER_DET_REPORT(LOGGER_API_ID_SETLEVEL, LOGGER_E_PARAM_LEVEL);
#endif
		return E_NOT_OK;
	}
	s_level = level;
	return E_OK;
}

Logger_LevelType Logger_GetLevel(void)
{
	return s_level;
}

#if (LOGGER_CFG_ENABLE_TAG_FILTER == 1)
void Logger_SetEnableTags(Logger_TagMaskType mask)
{
	s_tagMask = mask;
}
Logger_TagMaskType Logger_GetEnableTags(void)
{
	return s_tagMask;
}
#endif

Std_ReturnType Logger_Write(const uint8* data, uint16 len)
{
	LOGGER_DET_CHK_INIT(LOGGER_API_ID_WRITE);
	if((data == NULL_PTR) || (len == 0u))
	{
#if(LOGGER_CFG_DEV_ERROR_DETECT == STD_ON)
		LOGGER_DET_REPORT(LOGGER_API_ID_WRITE, LOGGER_E_PARAM_POINTER);
#endif
		return E_NOT_OK;
	}
	if(s_outWrite == NULL_PTR) return E_NOT_OK;
	return s_outWrite(data,len);
}

Std_ReturnType Logger_WriteLine(const char* cstr)
{
	LOGGER_DET_CHK_INIT(LOGGER_API_ID_WRITELINE);
	if(cstr == NULL_PTR)
	{
#if(LOGGER_CFG_DEV_ERROR_DETECT == STD_ON)
		LOGGER_DET_REPORT(LOGGER_API_ID_WRITELINE, LOGGER_E_PARAM_POINTER);
#endif
		return E_NOT_OK;
	}
	return prv_WriteLineRaw(cstr);
}

Std_ReturnType Logger_Logf(Logger_LevelType level, uint32 tagMask, const char* fmt,...)
{
	LOGGER_DET_CHK_INIT(LOGGER_API_ID_LOGF);

	// Filter by level
	if(level == LOG_LEVEL_OFF || level > LOG_LEVEL_DEBUG) return E_OK;
	if(level > s_level) return E_OK;
#if (LOGGER_CFG_ENABLE_TAG_FILTER == 1)
	if((s_tagMask & tagMask) == 0u) return E_OK;
#else
	(void)tagMask;
#endif
	if((fmt == NULL_PTR) || (s_outWrite == NULL_PTR && s_outWriteLine == NULL_PTR))
	{
#if (LOGGER_CFG_DEV_ERROR_DETECT == STD_ON)
		LOGGER_DET_REPORT(LOGGER_API_ID_LOGF, LOGGER_E_PARAM_POINTER);
#endif
		return E_NOT_OK;
	}

	char prefix[64];
	uint16 pLen = prv_BuidPrefix(prefix, (uint16)sizeof(prefix), level, tagMask);
	char msg[LOGGER_CFG_FMT_BUF_SIZE];
	va_list ap;
	va_start(ap,fmt);
	int m = vsniprintf(msg,sizeof(msg),fmt,ap);
	va_end(ap);

	if(m < 0) return E_NOT_OK;
	uint16 mLen = (uint16)((m >= (int)sizeof(msg)) ? (sizeof(msg)-1u) : (uint16)m);

	// Send: prefix + message + newline
	if(pLen > 0u && s_outWrite != NULL_PTR)
	{
		if(s_outWrite((const uint8*)prefix,pLen) != E_OK)  return E_NOT_OK;
	}

	if(s_outWriteLine != NULL_PTR)
	{
		return s_outWriteLine(msg);
	} else if(s_outWrite != NULL_PTR){
		if(s_outWrite((const uint8*)msg,mLen) != E_OK)  return E_NOT_OK;
#if (LOGGER_CFG_CRLF_STYLE == 1)
		const uint8 crlf[2] = {'\r','\n'};
		return s_outWrite(crlf,2u);
#else
		const uint8 lf[1] = {'\n'};
		return s_outWrite(lf,1u);
#endif
	}
	return E_NOT_OK;
}

Std_ReturnType Logger_Printf(const char* fmt, ...)
{
	LOGGER_DET_CHK_INIT(LOGGER_API_ID_PRINTF);
	if((fmt == NULL_PTR) || (s_outWrite == NULL_PTR && s_outWriteLine == NULL_PTR)){
#if(LOGGER_CFG_DEV_ERROR_DETECT == STD_ON)
		LOGGER_DET_REPORT(LOGGER_API_ID_PRINTF, LOGGER_E_PARAM_POINTER);
#endif
		return E_NOT_OK;
	}

	char buf[LOGGER_CFG_FMT_BUF_SIZE];
	va_list ap;
	va_start(ap,fmt);
	int n = vsniprintf(buf,sizeof(buf),fmt,ap);
	va_end(ap);

	if( n<0 ) return E_NOT_OK;
	uint16 len = (uint16)((n >= (int)sizeof(buf)) ? (sizeof(buf)-1) : (uint16)n);

	if(s_outWriteLine != NULL_PTR)
	{
		return s_outWriteLine(buf);
	} else if (s_outWrite != NULL_PTR){
		if(s_outWrite((const uint8*)buf, len) != E_OK) return E_NOT_OK;
#if(LOGGER_CFG_CRLF_STYLE == 1)
		const uint8 crlf[2] = {'\r','\n'};
		return s_outWrite(crlf,2u);
#else
		const uint8 lf[1] = {'\n'};
		return s_outWrite(lf,1u);
#endif
	}
	return E_NOT_OK;
}

Std_ReturnType Logger_HexDump(Logger_LevelType level, uint32 tagMask, const uint8* data, uint16 len)
{
	LOGGER_DET_CHK_INIT(LOGGER_API_ID_HEXDUMP);

	if((data == NULL_PTR) || (len == 0u)){
#if(LOGGER_CFG_DEV_ERROR_DETECT == STD_ON)
		LOGGER_DET_REPORT(LOGGER_API_ID_HEXDUMP, LOGGER_E_PARAM_POINTER);
#endif
		return E_NOT_OK;
	}

	//Filter
	if(level == LOG_LEVEL_OFF || level > s_level) return E_OK;
#if(LOGGER_CFG_ENABLE_TAG_FILTER == 1)
	if((s_tagMask & tagMask) == 0u) return E_OK;
#endif
	//Header
	(void)Logger_Logf(level, tagMask, "HexDump len=%u", (unsigned)len);

	char line[LOGGER_CFG_FMT_BUF_SIZE];
	uint16 offset = 0u;

	while(offset < len)
	{
		uint16 chunk = (uint16)((len - offset) > 16u ? 16u : (len - offset));
		int pos = 0;
		pos += snprintf(&line[pos],(sizeof(line) > (size_t)pos) ? (sizeof(line) - pos) : 0, "%04X", (unsigned)offset);

		for(uint16 i = 0; i < chunk; i++){
			pos += snprintf(&line[pos],(sizeof(line) > (size_t)pos) ? (sizeof(line) - pos) : 0, "%02X", (unsigned)data[offset+i]);
		}
		for(uint16 i = chunk; i< 16u;i++){
			pos += snprintf(&line[pos],(sizeof(line) > (size_t)pos) ? (sizeof(line) - pos) : 0, "   ");
		}

		//ASCII part
		pos += snprintf(&line[pos],(sizeof(line) > (size_t)pos) ? (sizeof(line) - pos) :0, " |");
		for(uint16 i = 0 ; i < chunk;i++)
		{
			uint8 c = data[offset + i];
			line[pos++] = ((c >= 32u && c< 127u) ? (char)c : '.');
			if((size_t)pos >= sizeof(line)) break;
		}
		if((size_t)pos < sizeof(line)) line[pos++] = '|';
		if((size_t)pos < sizeof(line)) line[pos] = '\0';

		(void)Logger_Logf(level, tagMask, "%s", line);
		offset = (uint16)(offset + chunk);
	}
	return E_OK;

}

#endif
