/* =====================================================================================================================
 *  File        : Port.c
 *  Layer       : MCAL
 *  ECU         : Sensor_ECU (STM32F103C6T6)
 *  Purpose     : Apply Port driver (config GPIO + AFIO remap) for STM32F1.
 * ===================================================================================================================*/

#include "Port.h"
#include "Port_Cfg.h"
#include "stm32f103xx_regs.h"

/*	Version */
#define PORT_VENDOR_ID					(0u)
#define PORT_MODULE_ID					(0x21u)
#define PORT_SW_MAJOR_VERSION			(1u)
#define PORT_SW_MINOR_VERSION			(0u)
#define PORT_SW_PATCH_VERSION			(0u)

/*	Driver state */
static Port_StatusType s_portStatus		= PORT_UNINIT;
static const Port_ConfigType* s_cfg		= NULL_PTR;

/* Decode Pin and pin number 0..15 */
GPIO_TypeDef* prv_GetGpioFromPin(Port_PinType pin, uint8* outPinNum)
{
	uint8 portIndex	= (pin >> 4) & 0x0Fu;
	uint8 pinNum		= (pin & 0x0Fu);

	if (outPinNum) { *outPinNum = pinNum; }

	switch (portIndex)
	{
		case 0: return GPIOA;
		case 1: return GPIOB;
		case 2: return GPIOC;
		default: return GPIOA;
	}
}

/* Enable Clock for working Port */
static void prv_EnableGpioClocksFromConfig( const Port_ConfigType* cfg)
{
	uint32 en_apb2 = RCC->APB2ENR | (1UL << 0);

	for(uint32 i = 0 ; i < cfg -> pinCount; i++)
	{
		uint8 portIndex = (cfg->pins[i].pin >> 4) & 0x0Fu;

		switch(portIndex)
		{
		case 0: en_apb2 |= (1UL << 2);	break; // IOPAEN
		case 1: en_apb2 |= (1UL << 3);	break; // IOPBEN
		case 3: en_apb2 |= (1UL << 4);	break; // IOPCEN
		default:	break;
		}
	}
	RCC->APB2ENR = en_apb2;
}

/* Port_PinModeType -> Mode & CNF[1:0] + PullUp/PullDown flag*/
typedef struct
{
	uint32 modeBits;
	uint32 cnfBits;
	uint8 isInput;
	uint8 setPU;
	uint8 setPD;
} prv_ModeMap_t;

static prv_ModeMap_t prv_MapMode(Port_PinModeType m)
{
	prv_ModeMap_t r = {0,0,0,0,0};

	switch (m)
	{
	case PORT_MODE_INPUT_ANALOG		: 		r.modeBits=0b00; r.cnfBits=0b00;r.isInput=1;break;
	case PORT_MODE_INPUT_FLOATING	:		r.modeBits=0b00; r.cnfBits=0b01;r.isInput=1;break;
	case PORT_MODE_INPUT_PU			:		r.modeBits=0b00; r.cnfBits=0b10;r.isInput=1;r.setPU=1;break;
	case PORT_MODE_INPUT_PD			:		r.modeBits=0b00; r.cnfBits=0b10;r.isInput=1;r.setPD=1;break;

	case PORT_MODE_OUTPUT_PP_2M		:		r.modeBits=0b10; r.cnfBits=0b00;break;
	case PORT_MODE_OUTPUT_PP_10M	:		r.modeBits=0b01; r.cnfBits=0b00;break;
	case PORT_MODE_OUTPUT_PP_50M	:		r.modeBits=0b11; r.cnfBits=0b00;break;

	case PORT_MODE_OUTPUT_OD_2M		:		r.modeBits=0b10; r.cnfBits=0b01;break;
	case PORT_MODE_OUTPUT_OD_10M	:		r.modeBits=0b01; r.cnfBits=0b01;break;
	case PORT_MODE_OUTPUT_OD_50M	:		r.modeBits=0b11; r.cnfBits=0b01;break;

	case PORT_MODE_AF_PP_2M			:		r.modeBits=0b10; r.cnfBits=0b10;break;
	case PORT_MODE_AF_PP_10M		:		r.modeBits=0b01; r.cnfBits=0b10;break;
	case PORT_MODE_AF_PP_50M		:		r.modeBits=0b11; r.cnfBits=0b10;break;

	case PORT_MODE_AF_OD_2M			:		r.modeBits=0b10; r.cnfBits=0b11;break;
	case PORT_MODE_AF_OD_10M		:		r.modeBits=0b01; r.cnfBits=0b11;break;
	case PORT_MODE_AF_OD_50M		:		r.modeBits=0b11; r.cnfBits=0b11;break;

	default:								r.modeBits=0b00; r.cnfBits=0b01;r.isInput=1;break;
	}

	return r;
}

/* Write Mode/Cnf for 1 Pin*/
static void prv_WritePinCR(GPIO_TypeDef* GPIOx, uint8 pinNum, uint32 modeBits, uint32 cnfBits)
{
	__vo uint32* CR = (pinNum < 8) ? &GPIOx->CRL : &GPIOx->CRH;
	uint8	shift = (pinNum & 0x07) * 4u;
	uint32 val = *CR;

	val &= ~(0xFu << shift);
	val |= (((modeBits & 0x3u) | ((cnfBits & 0x3u) << 2)) << shift);
	*CR = val;
}

/* Set ODR level for input PU/PD or output init level */
void prv_WriteOdr( GPIO_TypeDef* GPIOx, uint8 pinNum, boolean high)
{
	if(high)
	{
		GPIOx->BSRR = (1UL << pinNum);	// set
	} else {
		GPIOx->BRR	= (1UL << pinNum); // reset
	}
}

/* Apply AFIO remap follow Port_AfoRemapCfg */
static void prv_ApplyAfioRemap(const Port_AfioRemapConfigType* afioCfg)
{
	uint32 mapr = AFIO -> MAPR;

	//CAN remap: CAN_REMAP[14:13] = 10b for use PB8/PB9
	mapr &= ~AFIO_MAPR_CAN_REMAP_Msk;
	if(afioCfg->Remap_CAN){
		mapr |= AFIO_MAPR_CAN_REMAP_PB9_8;
	}

	//USART1 remap
	if(afioCfg->Remap_USART1){
		mapr |= (1U << 2);
	} else {
		mapr &= ~(1U << 2);
	}


	if(afioCfg->Swj_cfg_disable){
		mapr &= ~AFIO_MAPR_SWJ_CFG_Msk;
		mapr |= (2UL << AFIO_MAPR_SWJ_CFG_Pos);
	}

	AFIO->MAPR = mapr;
}

/* Find index Pin in config table, return -1 if not found */
static sint32 prv_FindPinInConfig(Port_PinType pin)
{
	if(!s_cfg) return -1;
	for(uint32 i = 0; i < s_cfg->pinCount;i++)
	{
		if(s_cfg->pins[i].pin == pin) return (sint32) i;
	}
	return -1;
}

/* ------------------------------------------------------------------------------------------------------------------ */
/*                                                  API IMPLEMENTATION                                               */
/* ------------------------------------------------------------------------------------------------------------------ */

void Port_Init(const Port_ConfigType* ConfigPtr)
{
	if(ConfigPtr == NULL_PTR) { return; }

	s_cfg = ConfigPtr;

	// Enable clock for AFIO and all GPIO port
	prv_EnableGpioClocksFromConfig(s_cfg);

	// Apply AFIO remap
	prv_ApplyAfioRemap(&s_cfg->afio);

	// Config each pin and ODR */
	for(uint32 i = 0; i < s_cfg->pinCount ; i++)
	{
		const Port_PinConfigType* pc = &s_cfg->pins[i];

		uint8 pinNum = 0 ;
		GPIO_TypeDef* GPIOx =  prv_GetGpioFromPin(pc -> pin, &pinNum);

		prv_ModeMap_t mm = prv_MapMode(pc->mode);

		//with InputPU/PD: set ODR and write CNF = 10
		if(mm.isInput && mm.setPU) { prv_WriteOdr(GPIOx, pinNum, TRUE); }
		if(mm.isInput && mm.setPD) { prv_WriteOdr(GPIOx, pinNum, FALSE); }

		prv_WritePinCR(GPIOx, pinNum, mm.modeBits, mm.cnfBits);

		//if output/AF output, set initlevel
		if(!mm.isInput){
			prv_WriteOdr(GPIOx, pinNum, (pc->initLevel == PORT_INIT_HIGH));
		}
	}

	s_portStatus = PORT_INIT;
}

void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
	if(s_portStatus != PORT_INIT) { return; }

	sint32 idx = prv_FindPinInConfig(Pin);
	if(idx < 0) { return; }

	uint8 pinNum = 0;
	GPIO_TypeDef* GPIOx = prv_GetGpioFromPin(Pin, &pinNum);

	if(Direction == PORT_PIN_IN)
	{
		prv_WritePinCR(GPIOx, pinNum, 0b00, 0b01);
	} else
	{
		prv_WritePinCR(GPIOx, pinNum, 0b10, 0b00);
	}
}

void Port_RefreshPortDirection(void)
{
	if(s_portStatus != PORT_INIT) { return; }
	for( uint32 i = 0; i < s_cfg -> pinCount; i++)
	{
		const Port_PinConfigType* pc = &s_cfg->pins[i];
		uint8 pinNum = 0;
		GPIO_TypeDef* GPIOx = prv_GetGpioFromPin(pc->pin, &pinNum);

		prv_ModeMap_t mm = prv_MapMode(pc->mode);

		if(mm.isInput && mm.setPU){ prv_WriteOdr(GPIOx, pinNum, TRUE);}
		if(mm.isInput && mm.setPD){ prv_WriteOdr(GPIOx, pinNum, FALSE);}

		prv_WritePinCR(GPIOx, pinNum, mm.modeBits , mm.cnfBits );
	}
}

void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode)
{
	if(s_portStatus != PORT_INIT){return; }

	uint8 pinNum = 0;
	GPIO_TypeDef* GPIOx = prv_GetGpioFromPin(Pin, &pinNum);
	prv_ModeMap_t mm = prv_MapMode(Mode);

	if(mm.isInput && mm.setPU) { prv_WriteOdr(GPIOx, pinNum, TRUE); }
	if(mm.isInput && mm.setPD) { prv_WriteOdr(GPIOx, pinNum, FALSE); }

	prv_WritePinCR(GPIOx, pinNum, mm.modeBits, mm.cnfBits);
}

void Port_WritePin(Port_PinType Pin, Port_PinLevelType Level)
{
	if(s_portStatus != PORT_INIT) {return; }

	uint8 pinNum = 0;
	GPIO_TypeDef* GPIOx = prv_GetGpioFromPin(Pin, &pinNum);

	if(Level == PORT_PIN_LEVEL_HIGH)
	{
		GPIOx->BSRR = (1UL << pinNum);
	} else {
		GPIOx->BRR	= (1UL << pinNum);
	}
}

Port_PinLevelType Port_ReadPin(Port_PinType Pin)
{
	if(s_portStatus != PORT_INIT ) { return PORT_PIN_LEVEL_LOW; }

	uint8 pinNum = 0 ;
	GPIO_TypeDef* GPIOx = prv_GetGpioFromPin(Pin, &pinNum);
	return (( (GPIOx -> IDR) & (1UL << pinNum)) ? PORT_PIN_LEVEL_HIGH : PORT_PIN_LEVEL_LOW);
}

void Port_GetVersionInfo(Port_VersionInfoType* versioninfo)
{
	if(versioninfo == NULL_PTR){ return ; }
	versioninfo->vendorID 				= PORT_VENDOR_ID;
	versioninfo->moduleID				= PORT_MODULE_ID;
	versioninfo->sw_major_version		= PORT_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version		= PORT_SW_MINOR_VERSION;
	versioninfo->sw_patch_verion		= PORT_SW_PATCH_VERSION;
}
