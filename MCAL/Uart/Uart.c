/* =====================================================================================================================
 *  File        : Uart.c
 *  Layer       : MCAL (UART)
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Apply UART driver for STM32F1
 *  Depends     : Uart_Types.h, Uart_Cfg.h/.c
 * ===================================================================================================================*/


#include "Uart.h"
#include "Uart_Types.h"
#include "Uart_Cfg.h"
#include "Mcu.h"
#include "stm32f103xx_regs.h"

/* Version */
#define UART_VENDOR_ID					(0u)
#define UART_MODULE_ID					(0x22u)
#define UART_SW_MAJOR_VERSION			(1u)
#define UART_SW_MINOR_VERSION			(0u)
#define UART_SW_PATCH_VERSION			(0u)

/* Driver state*/
static Uart_ConfigType*	s_cfg	= NULL_PTR;
static Uart_ChannelHandleType s_handle[UART_CH_COUNT];

#if (UART_CFG_ENABLE_ASYNC_APIS == 1u)
uint8  Uart1_TxBuf[UART1_CFG_TX_BUFFER_SIZE];
uint16 Uart1_TxBufSize = UART1_CFG_TX_BUFFER_SIZE;

uint8  Uart1_RxBuf[UART1_CFG_RX_BUFFER_SIZE];
uint16 Uart1_RxBufSize = UART1_CFG_RX_BUFFER_SIZE;
#endif

/* ---------------------------------------------------------
 *	Mapping & utilities
 * --------------------------------------------------------- */
static inline USART_TypeDef* prv_GetRegs(Uart_ChannelType ch)
{
	switch (ch)
	{
	case UART_CH1: return USART1;
	case UART_CH2: return (USART_TypeDef*) 0;
	case UART_CH3: return (USART_TypeDef*) 0;
	default: return (USART_TypeDef*) 0;
	}
}

static inline sint32 prv_GetIrqNum(Uart_ChannelType ch)
{
	switch (ch) {
	case UART_CH1: return 37;
	case UART_CH2: return 38;
	case UART_CH3: return 39;
	default:	   return -1;
	}
}

static inline uint32 prv_CalcBRR(uint32 pclk_hz ,uint32 baud, Uart_OversamplingType os)
{
	(void) os;

	if( os == UART_OVERSAMPLING_16)
	{
		uint32 usartdiv_x100 = (pclk_hz*25)/(4*baud);
		uint32 mantissa = usartdiv_x100/100;
		uint32 fraction = (((usartdiv_x100 - mantissa*100)*16)+50)/100;

		if(fraction == 16)
		{
			mantissa += 1;
			fraction = 0U;
		}

		return (uint32) (mantissa << 4) | fraction;
	} else {
		/* Todo: because stm32f1 only has Over sample 16*/
		return 0U;
	}
}

static inline void prv_RbInit( Uart_RingBufferType* rb, uint8* buf, uint16 size)
{
	rb->buf = buf;
	rb->size = size;

	rb->head = 0;
	rb->tail = 0;

	(void) rb; (void)buf; (void)size;
}

static inline uint16 prv_RbUsed(const Uart_RingBufferType* rb)
{
	uint16 h = rb -> head;
	uint16 t = rb -> tail;
	if( h >= t){
		 return (uint16) (h-t);
	} else {
		return (uint16) (rb->size - (t-h));
	}
}

static inline uint16 prv_RbFree(const Uart_RingBufferType* rb)
{
	return (uint16)((rb->size - 1U) - prv_RbUsed(rb));
}

static inline bool prv_RbPush(Uart_RingBufferType* rb, uint8 b )
{
	uint16 next = rb->head+1U;
	if(next == rb->size) next = 0U;

	if( next == rb->tail ) return FALSE;

	rb->buf[rb->head] = b;

	rb->head = next;

	return TRUE;
}

static inline bool prv_RbPop(Uart_RingBufferType* rb, uint8* out)
{
	if( rb->tail == rb->head) return FALSE;

	*out = rb->buf[rb->tail];
	uint16 next = rb->tail + 1U;
	if(next == rb->size) next = 0U;

	rb->tail = next;
	return TRUE;
}

static inline uint32 prv_GetTickMs(void)
{
	return s_systickTicks;
}

static inline bool prv_WaitUntil(bool (*cond)(void*), void* arg, uint32 timeoutMs)
{
	if(timeoutMs == 0)
	{
		return cond(arg);
	}

	const uint32 start = prv_GetTickMs();

	while(1)
	{
		if(cond(arg))
		{
			return TRUE;
		}

		if((uint32)((prv_GetTickMs() - start) >= timeoutMs ) )
		{
			return FALSE;
		}
	}
}

static void prv_ClearAndReportError(Uart_ChannelType ch, USART_TypeDef* regs, uint32 sr)
{
	// Clear by read SR before read DR
	(void) regs->SR;
	volatile uint32 dummy = regs->DR;
	(void) dummy;

	if(sr & UART_SR_ORE) { s_handle[ch].stats.rxOverrunCount++; }
	if(sr & UART_SR_PE) { s_handle[ch].stats.parityErrCount++; }
	if(sr & UART_SR_FE) { s_handle[ch].stats.framingErrCount++; }
}

/* enable clock bus for USART*/
static void prv_EnableClock(Uart_ChannelType ch)
{
	switch(ch)
	{
	case UART_CH1: RCC->APB2ENR |= USART1EN; return;
	case UART_CH2: RCC->APB1ENR |= USART2EN; return;
	case UART_CH3: RCC->APB1ENR |= USART3EN; return;
	default: return;
	}

}

/* enable/disable NVIC IRQ for channel */
static void prv_EnableIrq(Uart_ChannelType ch, bool enable)
{
	sint32 irqn = prv_GetIrqNum(ch);

	volatile uint32* ISER = (uint32*)0xE000E100UL;
	volatile uint32* ICER = (uint32*)0xE000E180UL;
	volatile uint32* ICPR = (uint32*)0xE000E280UL;

	if(irqn < 0) return;
	uint32 n   = (uint32)irqn;
	uint32 idx = n >> 5;           // /32
	uint32 bit = n & 0x1FU;        // %32
	uint32 mask = (1UL << bit);

	if (enable) {
		ICPR[idx] = mask;            // clear pending before enable
		ISER[idx] = mask;            // enable
	} else {
		ICER[idx] = mask;            // disable
		ICPR[idx] = mask;            // clear pending
	}
}

/*Start Tx if idle */
static inline void prv_KickTxIfIdle(Uart_ChannelType ch, USART_TypeDef* regs)
{
	if((regs->CR1 & (1U << USART_CR1_TXEIE)) == 0u)
	{
		if(prv_RbUsed(&s_handle[ch].txRb) > 0)
		{
			regs -> CR1 |= (1<< USART_CR1_TXEIE);
		}
	}
}

/* ---------------------------------------------------------
 *	API Implementation
 * --------------------------------------------------------- */
Std_ReturnType Uart_Init(const Uart_ConfigType* cfg)
{
	if(cfg == NULL_PTR) { return E_NOT_OK;}
	s_cfg = (Uart_ConfigType*)cfg;

	for(uint32 i = 0 ; i < (uint32)UART_CH_COUNT; i++)
	{
		s_handle[i].stats = (Uart_StatsType) {0};
#if(UART_CFG_ENABLE_STATS == 1u)
		s_handle[i].stats = (Uart_StatsType){0};
#endif
		s_handle[i].txRb = (Uart_RingBufferType){0};
		s_handle[i].rxRb = (Uart_RingBufferType){0};
		s_handle[i].parityEnable = 0;
		s_handle[i].wordLen9b = 0;
	}

	Mcu_ClockInfoType clk; (void) clk;
	(void) Mcu_GetClockInfo(&clk);

	if(cfg->useCh1)
	{
		USART_TypeDef* regs = prv_GetRegs(UART_CH1);
		if(!regs){ return E_NOT_OK;}

		prv_EnableClock(UART_CH1);
		regs->CR1 &= ~(1 << USART_CR1_UE);

		uint32 brr = prv_CalcBRR(clk.pclk2_hz, cfg->usart1.baurate, UART_OVERSAMPLING_16);
		regs->BRR = brr;

		uint32 cr1 =0;

		/*WordLength*/
		if(cfg->usart1.wordlength == UART_WORDLEN_9B)
		{
			cr1 |= (1 << USART_CR1_M );
			s_handle[UART_CH1].wordLen9b = 1;
		} else {
			cr1 &= ~(1 << USART_CR1_M );
			s_handle[UART_CH1].wordLen9b = 0;
		}

		/*Parity*/
		if(cfg->usart1.parity != UART_PARITY_NONE )
		{
			cr1 |= USART_CR1_PCE;

			s_handle[UART_CH1].parityEnable = 1;
			if(cfg->usart1.parity == UART_PARITY_ODD)
			{
				cr1 |= (1 << USART_CR1_PS);
			} else {
				cr1 &= ~(1 << USART_CR1_PS);
			}
		}

		/*Direction*/
		if(cfg->usart1.TxEnable) cr1 |= (1 << USART_CR1_TE);
		if(cfg->usart1.RxEnable) cr1 |= (1 << USART_CR1_RE);

		regs->CR1 = cr1;

		uint32 cr2 = regs->CR2 & ~(0b11 << USART_CR2_STOP);
		/*stop bit*/
		if(cfg->usart1.stopBits == UART_STOPBITS_2)
		{
			cr2 |= (0b10 << USART_CR2_STOP);
		}
		regs->CR2 = cr2;

		/*Flow Control*/
		uint32 cr3 =0;
		if(cfg->usart1.flow == UART_FLOW_RTS_CTS) { cr3 |= (( 1 << USART_CR3_RTSE ) | ( 1 << USART_CR3_CTSE));}
		if(cfg->usart1.transMode == UART_XFER_INTERRUPT) { cr3 |= ( 1 << USART_CR3_EIE);}
		regs->CR3 = cr3;

#if (UART_CFG_ENABLE_ASYNC_APIS == 1)
		if(cfg->usart1.transMode == UART_XFER_INTERRUPT)
		{
//			extern uint8 Uart1_TxBuf[]; uint16 Uart1_TxBufSize;
//			extern uint8 Uart1_RxBuf[]; uint16 Uart1_RxBufSize;
			prv_RbInit(&s_handle[UART_CH1].txRb, Uart1_TxBuf, Uart1_TxBufSize);
			prv_RbInit(&s_handle[UART_CH1].rxRb, Uart1_RxBuf, Uart1_RxBufSize);

			/*enable RXNEIE*/
			regs->CR1 |= (1<<USART_CR1_RXNEIE);

			/*enable NVIC*/
			prv_EnableIrq(UART_CH1, TRUE);
		}
#endif
		/* clear pending error*/
		(void)regs->SR; (void) regs->DR;

		/*enable UE*/
		regs -> CR1 |= (1 << USART_CR1_UE);
		s_handle[UART_CH1].status = UART_INIT;
	}
	return E_OK;
}

void Uart_Deinit(void)
{
	if(s_cfg && s_cfg->useCh1)
	{
		USART_TypeDef* regs = prv_GetRegs(UART_CH1);
		if(regs)
		{
#if (UART_CFG_ENABLE_ASYNC_APIS == 1u)
			/*Diable IRQ NVIC*/
			prv_EnableIrq(UART_CH1, FALSE);

			/*Disable RXNEIE/TXEIE/TCIE/PEIE*/
			regs->CR1 &= ~((1 << USART_CR1_RXNEIE) | (1 << USART_CR1_TXEIE) | ( 1 << USART_CR1_TCIE));
			regs->CR3 &= ~(1 << USART_CR3_EIE);
#endif
			regs->CR1 &= ~(1 << USART_CR1_UE);
		}
		s_handle[UART_CH1].status = UART_UNINIT;
	}
}

/* ---------------------------------------------------------
 *	Blocking API
 * --------------------------------------------------------- */
#if (UART_CFG_ENABLE_BLOCKING_APIS == 1u)
Std_ReturnType Uart_Write(Uart_ChannelType ch, const uint8* data, uint16 len, uint32 timeoutMs)
{
	USART_TypeDef* regs = prv_GetRegs(ch);
	if( !regs || s_handle[ch].status != UART_INIT || data == NULL_PTR) return E_NOT_OK;

	uint32 t0 = prv_GetTickMs();

	for( uint16 i = 0; i < len; i++)
	{
		// wait TXE
		while( ((regs -> SR) & (1 << USART_SR_TXE)) == 0u)
		{
			if (prv_GetTickMs() - t0 >= timeoutMs) return E_NOT_OK;
		}

		regs->DR = data[i];
#if (UART_CFG_ENABLE_STATS == 1)
		s_handle[ch].stats.txBytes++;
#endif
	}

	// wait TC complete
	while( ((regs->SR) & (1 << USART_SR_TC)) == 0u )
	{
		if(prv_GetTickMs() - t0 >= timeoutMs) return E_NOT_OK;
	}

	return E_OK;
}

Std_ReturnType Uart_Read(Uart_ChannelType ch, uint8* data, uint16 len, uint32 timeoutMs)
{
	USART_TypeDef* regs = prv_GetRegs(UART_CH1);
	if( !regs || s_handle[ch].status != UART_INIT || data == NULL_PTR) return E_NOT_OK;

	uint32 t0 = prv_GetTickMs();

	for( uint16 i = 0; i < len; i++)
	{
		//wait RXNE
		while( ((regs -> SR) & (USART_SR_RXNE)) == 0)
		{
			uint32 sr = regs -> SR;
			if( sr & (USART_SR_ORE | USART_SR_FE | USART_SR_PE ) )
			{
				prv_ClearAndReportError(ch, regs, sr);
			}
			if(prv_GetTickMs() - t0 >= timeoutMs) return E_NOT_OK;
		}
		uint8 temp = (uint8) (regs->DR);
		data[i] = temp;
#if(UART_CFG_ENABLE_STATS == 1)
		s_handle[ch].stats.rxBytes++;
#endif
	}
	return E_OK;
}

Std_ReturnType Uart_PutChar(Uart_ChannelType ch, uint8 byte, uint32 timeoutMs)
{
	return Uart_Write(ch, &byte, 1u, timeoutMs);
}

Std_ReturnType Uart_GetChar(Uart_ChannelType ch, uint8* outByte, uint32 timeoutMs)
{
	return Uart_Read(ch, outByte, 1u, timeoutMs);
}

#endif //UART_CFG_ENABLE_BLOCKING_APIS

/* =========================================================
 * Async APIs (IRQ)
 * =======================================================*/
#if( UART_CFG_ENABLE_ASYNC_APIS == 1u)
Std_ReturnType Uart_WriteAsync(Uart_ChannelType ch, const uint8* data, uint16 len)
{
	USART_TypeDef* regs = prv_GetRegs(ch);
	if( !regs || s_handle[ch].status != UART_INIT || data == NULL_PTR) return E_NOT_OK;

	const Uart_ChannelConfigType* temp = (ch == UART_CH1)?(&s_cfg->usart1):NULL_PTR;
	if(!temp || temp->transMode != UART_XFER_INTERRUPT) return E_NOT_OK;

	/* Push to Tx buffer*/
	for(int i = 0; i < len; i++)
	{
		if(!prv_RbPush(&s_handle[ch].txRb, data[i]))
		{
			return E_NOT_OK;
		}
	}

	prv_KickTxIfIdle(ch, regs);
	return E_OK;
}

uint16 Uart_ReadAsync(Uart_ChannelType ch, uint8* data, uint16 len)
{
	if(data == NULL_PTR) return 0u;
	if(s_handle[ch].status == UART_UNINIT) return 0u;

	uint16 cnt = 0;
	while(cnt < len)
	{
		uint8 b;
		if(!prv_RbPop(&s_handle[ch].rxRb, &b)) break;
		data[cnt++] = b;
	}
	return cnt;
}

Std_ReturnType Uart_FlushTx(Uart_ChannelType ch, uint32 timeoutMs)
{
	USART_TypeDef* regs = prv_GetRegs(ch);
	if( !regs || s_handle[ch].status != UART_INIT ) return E_NOT_OK;

	uint32 t0 = prv_GetTickMs();
	while((prv_RbUsed(&s_handle[ch].txRb) != 0u) || (((regs->SR)&(1 << USART_SR_TC)) == 0u))
	{
		if((prv_GetTickMs() - t0) >= timeoutMs) return E_NOT_OK;
	}
	return E_OK;
}
#endif //UART_CFG_ENABLE_ASYNC_APIS

/* =========================================================
 * Control & Callbacks
 * =======================================================*/
Std_ReturnType Uart_SetBaudrate(Uart_ChannelType ch, uint32 baud)
{
	USART_TypeDef* regs = prv_GetRegs(ch);
	if( !regs || s_handle[ch].status != UART_INIT || baud == 0) return E_NOT_OK;

	if( (((regs->SR)&(1 << USART_SR_TC)) == 0u) || (((regs->SR)&(1 << USART_SR_TXE)) == 0u)) return E_NOT_OK;

	Mcu_ClockInfoType clk;
	if(Mcu_GetClockInfo(&clk) != E_OK) return E_NOT_OK;
	uint32 pclk_hz = (ch == UART_CH1) ? (clk.pclk2_hz) : (clk.pclk1_hz);
	regs->BRR = prv_CalcBRR(pclk_hz, baud, UART_OVERSAMPLING_16);
	return E_OK;
}

Std_ReturnType Uart_SetDirection(Uart_ChannelType ch, Uart_DirectionCfgType dir)
{
	USART_TypeDef* regs = prv_GetRegs(ch);
	if( !regs || s_handle[ch].status != UART_INIT ) return E_NOT_OK;

	uint32 cr1 = regs->CR1;
	if(!dir.txEnable)
	{
		regs->CR1 &= (~((1<<USART_CR1_TXEIE)|(1<<USART_CR1_TCIE)));
	}

	cr1 = (regs -> CR1 & ~((1<<USART_CR1_TE)|(1<<USART_CR1_RE)));
	if(dir.txEnable) cr1 |= (1 << USART_CR1_TE);
	if(dir.rxEnable) cr1 |= (1 << USART_CR1_RE);

	regs->CR1 |= cr1;
#if (UART_CFG_ENABLE_ASYNC_APIS)
	if(!dir.rxEnable) regs->CR1 &= ~(1<<USART_CR1_TXEIE);
	else 			  regs->CR1 |= (1<<USART_CR1_TXEIE);
#endif //UART_CFG_ENABLE_ASYNC_APIS
	return E_OK;
}

Std_ReturnType Uart_RegisterCallbacks(Uart_ChannelType ch, const Uart_CallbacksType* cbs)
{
	if(s_handle[UART_CH1].status == UART_UNINIT) return E_NOT_OK;
	if(!cbs) return E_NOT_OK;

	if(ch == UART_CH1)
	{
		(void)cbs;
		return E_OK;
	}
	return E_NOT_OK;
}

#if(UART_CFG_ENABLE_STATS == 1u)
Std_ReturnType Uart_GetStats(Uart_ChannelType ch, Uart_StatsType* out)
{
	if(!out || s_handle[ch].status != UART_INIT) return E_NOT_OK;
	*out = s_handle[ch].stats;
	return E_OK;
}

void Uart_ClearStats(Uart_ChannelType ch)
{
	if(s_handle[ch].status != UART_INIT) return;
	s_handle[ch].stats = (Uart_StatsType){0};
}
#endif //UART_CFG_ENABLE_STATS

/* =========================================================
 * Stats & Version
 * =======================================================*/
void Uart_GetVersionInfo(Uart_VersionInfoType* vi)
{
	if(!vi) return;
	vi->vendorID = UART_VENDOR_ID;
	vi->moduleID = UART_MODULE_ID;
	vi->sw_major_version = UART_SW_MAJOR_VERSION;
	vi->sw_minor_version = UART_SW_MINOR_VERSION;
	vi->sw_patch_verion = UART_SW_PATCH_VERSION;
}

/* =========================================================
 * IRQ entry point
 * =======================================================*/
void Uart_IrqHandler(Uart_ChannelType ch)
{
#if (UART_CFG_ENABLE_ASYNC_APIS == 1u)
	USART_TypeDef* regs = prv_GetRegs(ch);
	if( !regs || s_handle[ch].status != UART_INIT ) return;
	uint32 sr = regs->SR;

	//Error
	if(sr & ((1 << USART_SR_ORE) | (1 << USART_SR_PE) | (1 << USART_SR_FE)))prv_ClearAndReportError(ch, regs, sr);

	//RXNE
	if(sr & (1 << USART_SR_RXNE))
	{
		uint8 b = (uint8)(regs->DR & 0xFFu);
		(void)prv_RbPush(&s_handle[ch].rxRb, b);
#if (UART_CFG_ENABLE_STATS ==1)
		s_handle[ch].stats.txBytes++;
		s_handle[ch].stats.txIrqCount++;
#endif
		if(ch == UART_CH1 && s_cfg->usart1.cbs.onRxChar) s_cfg->usart1.cbs.onRxChar(ch,b);
	}

	//TXE
	if((sr & (1 << USART_SR_TXE)) && (regs -> CR1 & USART_CR1_TXEIE) )
	{
		uint8 b;
		if(prv_RbPop(&s_handle[ch].txRb, &b))
		{
			regs->DR = b;
#if (UART_CFG_ENABLE_STATS == 1)
			s_handle[ch].stats.rxBytes++;
			s_handle[ch].stats.rxIrqCount++;
#endif
		} else {
			regs->CR1 &= ~USART_CR1_TXEIE;
			regs->CR1 |= USART_CR1_TCIE;
		}
	}

	if((sr & (1 << USART_SR_TC)) && (regs->CR1 & (1<<USART_CR1_TCIE)))
	{
		regs -> CR1 &= ~(1 << USART_CR1_TXEIE);
		if(ch == UART_CH1 && s_cfg->usart1.cbs.onTxEmptyOrCplt)
		{
			s_cfg->usart1.cbs.onTxEmptyOrCplt(ch);
		}
	}

#else
	(void)ch;
#endif
}

bool Uart_IsTxBusy(Uart_ChannelType ch)
{
	USART_TypeDef* regs = prv_GetRegs(ch);
	if(!regs) return FALSE;

	if(s_handle[ch].status != UART_INIT) return FALSE;

#if(UART_CFG_ENABLE_ASYNC_APIS == 1u)
	if(prv_RbUsed(&s_handle[ch].txRb) != 0u)
	{
		return TRUE;
	}

	if((regs->SR &(1U << USART_SR_TC)) == 0U)
	{
		return TRUE;
	}

	return FALSE;
#else
	return ((regs->SR &(1U << USART_SR_TC)) == 0U) ? TRUE : FALSE;
#endif
}


