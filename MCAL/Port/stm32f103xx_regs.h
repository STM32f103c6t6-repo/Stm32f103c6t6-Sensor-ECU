/* =====================================================================================================================
 *  File   : stm32f103xx_regs.h
 *  Layer  : MCAL / Common (Register Map)
 *  ECU    : Sensor_ECU (STM32F103C6T6)
 *  Role   : Define base address & struct register for RCC/AFIO/GPIO/EXTI/TIM/USART/CAN
 *  Note   :
 * ===================================================================================================================*/

#ifndef COMMON_STM32F103XX_REGS_H_
#define COMMON_STM32F103XX_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <Std_Types.h>

#define __vo 							volatile

/* =========================================================
 *  Bus base addresses
 * =======================================================*/
#define PERIPH_BASE					(0x40000000UL)
#define APB1PERIPH_BASE				(PERIPH_BASE + 0x00000UL)
#define APB2PERIPH_BASE				(PERIPH_BASE + 0x10000UL)
#define AHBPERIPH_BASE				(PERIPH_BASE + 0x20000UL)

/* =========================================================
 *  RCC/FLASH/AFIO
 * =======================================================*/
#define RCC_BASE					(AHBPERIPH_BASE  + 0x1000UL)
#define FLASH_R_BASE				(AHBPERIPH_BASE  + 0x2000UL)
#define AFIO_BASE					(APB2PERIPH_BASE + 0x1000UL)

typedef struct
{
	__vo uint32 CR;			//0x00
	__vo uint32 CFGR;			//0x04
	__vo uint32 CIR;			//0x08
	__vo uint32 APB2RSTR;		//0x0C
	__vo uint32 APB1RSTR;		//0x10
	__vo uint32 AHBENR;		//0x14
	__vo uint32 APB2ENR;		//0x18
	__vo uint32 APB1ENR;		//0x1C
	__vo uint32 BDCR;			//0x20
	__vo uint32 CSR;			//0x24
} RCC_TypeDef;

typedef struct
{
	__vo uint32 ACR;			//0x00
	__vo uint32 KEYR;			//0x04
	__vo uint32 OPTKEYR;		//0x08
	__vo uint32 SR;			//0x0C
	__vo uint32 CR;			//0x10
	__vo uint32 AR;			//0x14
	__vo uint32 RESERVED;		//0x18
	__vo uint32 OBR;			//0x1C
	__vo uint32 WRPR;			//0x20
} FLASH_TypeDef;

typedef struct
{
	__vo uint32 EVCR;			//0x00
	__vo uint32 MAPR;			//0x04
	__vo uint32 EXTIR[4];		//0x08...0x14
	__vo uint32 MAPR2;		//0x1C
} AFIO_TypeDef;

#define RCC						((RCC_TypeDef*)		RCC_BASE)
#define FLASH_R					((FLASH_TypeDef*)	FLASH_R_BASE)
#define AFIO					((AFIO_TypeDef*)	AFIO_BASE)

/* ---- RCC CR bits ---- */
#define	RCC_CR_HSION			(1UL << 0)
#define	RCC_CR_HSIRDY			(1UL << 1)
#define	RCC_CR_HSEON			(1UL << 16)
#define	RCC_CR_HSERDY			(1UL << 17)
#define	RCC_CR_PLLON			(1UL << 24)
#define	RCC_CR_PLLRDY			(1UL << 25)

/* ---- RCC CFGR fields ---- */
/* SW[1:0] */
#define RCC_CFGR_SW_Pos				0U
#define RCC_CFGR_SW_HSI				(0x0UL << ECC_CFGR_SW_Pos)
#define RCC_CFGR_SW_HSE				(0x1UL << ECC_CFGR_SW_Pos)
#define RCC_CFGR_SW_PLL				(0x2UL << ECC_CFGR_SW_Pos)
/* SWS[3:2] */
#define RCC_CFGR_SWS_Pos			2U
#define RCC_CFGR_SWS_HSI			(0x0UL << ECC_CFGR_SWS_Pos)
#define RCC_CFGR_SWS_HSE			(0x1UL << ECC_CFGR_SWS_Pos)
#define RCC_CFGR_SWS_PLL			(0x2UL << ECC_CFGR_SWS_Pos)
/* HPRE[7:4], PPRE1[10:8], PPRE2[13:11], ADCPRE[15:14] */
#define RCC_CFGR_HPRE_Pos			4U
#define RCC_CFGR_PPRE1_Pos			8U
#define RCC_CFGR_PPRE2_Pos			11U
#define RCC_CFGR_ADCPRE_Pos			14U
/* PLLSRC[16], PLLXTPRE[17], PLLMUL[21:18] */
#define RCC_CFGR_PLLSRC				(1UL << 16)
#define RCC_CFGR_PLLXTPRE			(1UL << 17)
#define RCC_CFGR_PLLMUL_Pos			18U
#define RCC_CFGR_PULLMUL_Msk		(0xFUL << RCC_CFGR_PLLMUL_Pos)
/* ---- RCC ENR bits ---- */
#define RCC_APB2ENR_AFIOEN			(1UL << 0)
#define RCC_APB2ENR_IOPAEN			(1UL << 2)
#define RCC_APB2ENR_IOPBEN			(1UL << 3)
#define RCC_APB2ENR_IOPCEN			(1UL << 4)
#define RCC_APB2ENR_USART1EN		(1UL << 14)
#define RCC_APB2ENR_TIM1EN			(1UL << 11)


#define RCC_APB1ENR_TIM2EN			(1UL << 0)
#define RCC_APB1ENR_TIM3EN			(1UL << 1)
#define RCC_APB1ENR_TIM4EN			(1UL << 2)
#define RCC_APB1ENR_CAN1EN			(1UL << 25)

/* ---- FLASH ACR ---- */
#define FLASH_ACR					(*(__vo uint32*)(0x40022000UL))
#define FLASH_ACR_LATENCY_Pos		0U
#define FLASH_ACR_PRFTBE_Pos		4U

/* IWDG registers */
#define IWDG_KR						(*(__vo uint32*)(0x40003000UL))
#define IWDG_PR						(*(__vo uint32*)(0x40003004UL))
#define IWDG_RLR					(*(__vo uint32*)(0x40003008UL))
#define IWDG_SR						(*(__vo uint32*)(0x4000300CUL))

/* ---- AFIO MAPR helpers ---- */
/* CAN_REMAP[14:13]: 10b → CAN RX=PB8, TX=PB9 */
#define AFIO_MAPR_CAN_REMAP_Pos		13U
#define AFIO_MAPR_CAN_REMAP_Msk		(3UL << AFIO_MAPR_CAN_REMAP_Pos)
#define AFIO_MAPR_CAN_REMAP_NO		(0UL << AFIO_MAPR_CAN_REMAP_Pos)	//PA11/PA12
#define AFIO_MAPR_CAN_REMAP_PB9_8	(2UL << AFIO_MAPR_CAN_REMAP_Pos)	//PB8,PB9

/* SWJ_CFG[26:24]: 010b → Disable JTAG, Enable SWD */
#define AFIO_MAPR_SWJ_CFG_Pos		24U
#define AFIO_MAPR_SWJ_CFG_Msk		(7U << AFIO_MAPR_SWJ_CFG_Pos)
#define AFIO_MAPR_SWJ_JTAGDIS_SWD	(2U << AFIO_MAPR_SWJ_CFG_Pos)

/* =========================================================
 *  GPIO
 * =======================================================*/
#define GPIOA_BASE					(APB2PERIPH_BASE + 0x0800UL)
#define GPIOB_BASE					(APB2PERIPH_BASE + 0x0C00UL)
#define GPIOC_BASE					(APB2PERIPH_BASE + 0x1000UL)

typedef struct
{
	__vo uint32 CRL;
	__vo uint32 CRH;
	__vo uint32 IDR;
	__vo uint32 ODR;
	__vo uint32 BSRR;
	__vo uint32 BRR;
	__vo uint32 LCKR;
} GPIO_TypeDef;

#define GPIOA						((GPIO_TypeDef*)GPIOA_BASE)
#define GPIOB						((GPIO_TypeDef*)GPIOB_BASE)
#define GPIOC						((GPIO_TypeDef*)GPIOC_BASE)

/* ---- GPIO mode encoding (F1 CRL/CRH) ----
   each pin: 4 bit = [CNF(1:0) | MODE(1:0)]
   OUTPUT mode: MODE=01:2MHz,10:10MHz,11:50MHz; CNF=00:PP, 01:OD, 10:AF-PP, 11:AF-OD
   INPUT mode:  MODE=00; CNF=01:Floating, 10:PU/PD (ODR=1→PU, ODR=0→PD), 00:Analog
*/
#define GPIO_MODE_INPUT				(0x0U)
#define GPIO_MODE_OUT_2MHz			(0x2U)
#define GPIO_MODE_OUT_10MHz			(0x1U)
#define GPIO_MODE_OUT_50MHz			(0x3U)

#define GPIO_CNF_IN_ANALOG			(0x0U)
#define GPIO_CNF_IN_FLOATING		(0x1U)
#define GPIO_CNF_IN_PUPD			(0x2U)

#define GPIO_CNF_OUT_PP				(0x0U)
#define GPIO_CNF_OUT_OD				(0x1U)
#define GPIO_CNF_AF_PP				(0x2U)
#define GPIO_CNF_AF_OD				(0x3U)

/*Helper: Code 4-bit for 1 pin*/
#define GPIO_CFG4(mode,cnf)			(((uint32)(cnf)<<2) | ((uint32)(mode ) & 0x3U))
/*write on CRx*/
#define GPIO_CRx_POS(pin)			(((pin)%8U) * 4U)
#define GPIO_SET_CRL(cfg,pin)		do{ uint32 v=GPIOA->CRL; (void)v;}while(0)

/* =========================================================
 *  EXTI
 * =======================================================*/
#define EXTI_BASE					(APB2PERIPH_BASE +0x0400UL)
typedef struct
{
	__vo uint32 IMR;				//0x00
	__vo uint32 EMR;				//0x04
	__vo uint32 RTSR;				//0x08
	__vo uint32 FTSR;				//0x0C
	__vo uint32 SWIER;			//0x10
	__vo uint32 PR;				//0x14
} EXTI_TypeDef;

/* =========================================================
 *  TIM (TIM1-4; TIM2 dùng cho ICU HC-SR04)
 * =======================================================*/
#define TIM1_BASE					(APB2PERIPH_BASE +0x2C00UL)
#define TIM2_BASE					(APB1PERIPH_BASE +0x0000UL)
#define TIM3_BASE					(APB1PERIPH_BASE +0x0400UL)
#define TIM4_BASE					(APB1PERIPH_BASE +0x0800UL)

typedef struct
{
	__vo uint32 CR1;		//0x00
	__vo uint32 CR2;		//0x04
	__vo uint32 SMCR;		//0x08
	__vo uint32 DIER;		//0x0C
	__vo uint32 SR;		//0x10
	__vo uint32 EGR;		//0x14
	__vo uint32 CCMR1;	//0x18
	__vo uint32 CCMR2;	//0x1C
	__vo uint32 CCER;		//0x20
	__vo uint32 CNT;		//0x24
	__vo uint32 PSC;		//0x28
	__vo uint32 ARR;		//0x2C
	__vo uint32 RCR;		//0x30
	__vo uint32 CCR1;		//0x34
	__vo uint32 CCR2;		//0x38
	__vo uint32 CCR3;		//0x3C
	__vo uint32 CCR4;		//0x40
	__vo uint32 BDTR;		//0x44
	__vo uint32 DCR;		//0x48
	__vo uint32 DMAR;		//0x4C
} TIM_TypeDef;

#define TIM1				((TIM_TypeDef*) TIM1_BASE)
#define TIM2				((TIM_TypeDef*) TIM2_BASE)
#define TIM3				((TIM_TypeDef*) TIM3_BASE)
#define TIM4				((TIM_TypeDef*) TIM4_BASE)

#define TIM_CR1_CEN			(1UL << 0)
#define TIM_DIER_CC1IE		(1UL << 1)
#define TIM_CCER_CC1E		(1UL << 0)
#define TIM_CCER_CC1P		(1UL << 1)

/* CCMR1 */
#define TIM_CCMR1_CC1S_Pos		0U	//01:IC1 mapped on TI1
#define TIM_CCMR1_IC1F_Pos		4U	// input filter
#define TIM_CCMR1_IC1PCS_Pos	2U
#define TIM_CCMR1_CC1S			(3UL << 0 )
#define TIM_CCMR1_CC1S_0		(0U << 0)
#define TIM_CCMR1_CC1S_1		(1U << 0)
#define TIM_CCMR1_CC1S_2		(2U << )
#define TIM_CCMR1_CC1S_3		(3U << 0)

/* SR */
#define TIM_SR_CC1IF			(1UL << 1)

/* =========================================================
 *  USART (USART1 for log)
 * =======================================================*/
#define USART1_BASE				(APB2PERIPH_BASE + 0x3800UL)

typedef struct
{
	__vo uint32 SR;
	__vo uint32 DR;
	__vo uint32 BRR;
	__vo uint32 CR1;
	__vo uint32 CR2;
	__vo uint32 CR3;
	__vo uint32 GTPR;
} USART_TypeDef;

#define USART1					((USART_TypeDef*)USART1_BASE)

#define USART1EN				(1UL << 14)
#define USART2EN				(1UL << 17)
#define USART3EN				(1UL << 18)

/* Some bit Position */
#define USART_CR1_SBK						0
#define USART_CR1_RWU						1
#define USART_CR1_RE						2
#define USART_CR1_TE						3
#define USART_CR1_IDLETE					4
#define USART_CR1_RXNEIE					5
#define USART_CR1_TCIE						6
#define USART_CR1_TXEIE						7
#define USART_CR1_PEIE						8
#define USART_CR1_PS						9
#define USART_CR1_PCE						10
#define USART_CR1_WAKE						11
#define USART_CR1_M							12
#define USART_CR1_UE						13

#define USART_CR2_ADD   					0
#define USART_CR2_LBDL   					5
#define USART_CR2_LBDIE   					6
#define USART_CR2_LBCL   					8
#define USART_CR2_CPHA   					9
#define USART_CR2_CPOL   					10
#define USART_CR2_CLKEN   					11
#define USART_CR2_STOP   					12
#define USART_CR2_LINEN   					14

#define USART_CR3_EIE   					0
#define USART_CR3_IREN		   				1
#define USART_CR3_IRLP		   				2
#define USART_CR3_HDSEL		   				3
#define USART_CR3_NACK		   				4
#define USART_CR3_SCEN		   				5
#define USART_CR3_DMAR   					6
#define USART_CR3_DMAT		   				7
#define USART_CR3_RTSE   					8
#define USART_CR3_CTSE		   				9
#define USART_CR3_CTSIE		   				10

/*
 * Bit position definitions USART_SR
 */
#define USART_SR_PE   					0
#define USART_SR_FE   					1
#define USART_SR_NE   					2
#define USART_SR_ORE   					3
#define USART_SR_IDLE  					4
#define USART_SR_RXNE   				5
#define USART_SR_TC   					6
#define USART_SR_TXE   					7
#define USART_SR_LBD   					8
#define USART_SR_CTS   					9

/*
 * Bit position definition SPI_SR
 */
#define SPI_SR_RXNE				0
#define SPI_SR_TXE				1
#define SPI_SR_CHSIDE			2
#define SPI_SR_UDR				3
#define SPI_SR_CRCERR			4
#define SPI_SR_MODF				5
#define SPI_SR_OVR				6
#define SPI_SR_BSY				7

/* =========================================================
 *  CAN (bxCAN)
 * =======================================================*/
#define CAN1_BASE				(APB1PERIPH_BASE + 0x6400UL)

typedef struct {
	/* master*/
	__vo uint32 MCR;		//0x00
	__vo uint32 MSR;		//0x04
	__vo uint32 TSR;		//0x08
	__vo uint32 RF0R;		//0x0C
	__vo uint32 RF1R;		//0x10
	__vo uint32 IER;		//0x14
	__vo uint32 ESR;		//0x18
	__vo uint32 BTR;		//0x1C
	uint32 RESERVED0[88];	//0x20...0x17F
	/* Tx mailbox */
	struct
	{
		__vo uint32	TIR;
		__vo uint32	TDTR;
		__vo uint32	TDLR;
		__vo uint32	TDHR;
	} sTxMailBox[3];
	/* Rx FIFOs */
	struct
	{
		__vo uint32	RIR;
		__vo uint32	RDTR;
		__vo uint32	RDLR;
		__vo uint32	RDHR;
	} sFIFOMailBox[2];
	uint32 RESERVED1[12];
	__vo uint32	FMR;
	__vo uint32	FM1R;
	uint32 RESERVED2;
	__vo uint32	FS1R;
	uint32 RESERVED3;
	__vo uint32	FFA1R;
	uint32 RESERVED4;
	__vo uint32	FA1R;
	uint32 RESERVED5[8];
	struct
	{
		__vo uint32 FR1;
		__vo uint32 FR2;
	} sFilterRegister[14];
} CAN_TypeDef;

#define CAN1					((CAN_TypeDef*) CAN1_BASE)

#define CAN_MCR_INRQ			(1UL << 0)
#define CAN_MSR_INAK			(1UL << 0)
#define CAN_BTR_SILM			(1UL << 31)
#define CAN_BTR_LBKM			(1UL << 30)

#define CAN_MCR_SLEEP			(1UL << 1)	// bit CAN sleep
#define CAN_TSR_TME0			(1UL << 26) // bit mailbox 0
#define CAN_TI0R_STID_Pos		(21UL)
#define CAN_TI0R_EXID_Pos		(3UL)
#define CAN_TSR_RQCP0			(1UL << 0) // bit mailbox 0
#define CAN_RF0R_FMP0			(1UL << 0)
#define CAN_RI0R_IDE			(1UL << 2)

/* Tx mailbox TIR: IDE/RTR/ID */
#define CAN_TI_TXRQ				(1UL << 0)
#define CAN_TI_RTR				(1UL << 1)
#define CAN_TI_IDE				(1UL << 2)

/* Rx FIFO RIR: IDE/RTR/ID */
#define CAN_RI_RTR				(1UL << 1)
#define CAN_RI_IDE				(1UL << 2)

/* position of CAN BTR */
#define CAN_BTR_SJW_Pos				(24U)
#define CAN_BTR_TS2_Pos				(20U)
#define CAN_BTR_TS1_Pos				(16U)

/* =========================================================
 *  Core (SysTick/SCB/NVIC)
 * =======================================================*/
#define SYST_CSR				(*(__vo uint32*)0xE000E010UL)
#define SYST_RVR				(*(__vo uint32*)0xE000E014UL)
#define SYST_CVR				(*(__vo uint32*)0xE000E018UL)
#define SYST_CALIB				(*(__vo uint32*)0xE000E01CUL)
#define SCB_AIRCR				(*(__vo uint32*)0xE000ED0CUL)
#define NVIC_IPR_BASE			((__vo uint8*)0xE000E400UL)


#ifdef __cplusplus
}
#endif

#endif /* COMMON_STM32F103XX_REGS_H_ */
