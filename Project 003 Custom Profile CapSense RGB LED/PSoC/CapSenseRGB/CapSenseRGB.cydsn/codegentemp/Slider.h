/*******************************************************************************
* File Name: Slider.h
* Version 2.20
*
* Description:
*  This file provides constants and parameter values for the CapSense CSD
*  component.
*
* Note:
*
********************************************************************************
* Copyright 2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CAPSENSE_CSD_Slider_H)
#define CY_CAPSENSE_CSD_Slider_H

#include "cytypes.h"
#include "cyfitter.h"
#include "core_cm0_psoc4.h"
#include "CyLib.h"

#include "Slider_IP.h"
#include "Slider_SenseClk.h"
#include "Slider_SampleClk.h"

/* Constants set by Customizer */
#define Slider_TOTAL_SENSOR_COUNT            (5u)
#define Slider_TOTAL_SCANSLOT_COUNT          (5u)
#define Slider_INDEX_TABLE_SIZE              (0u)

/* Define Sensors */
#define Slider_SENSOR_LINEARSLIDER0_E0__LS    (0u)
#define Slider_SENSOR_LINEARSLIDER0_E1__LS    (1u)
#define Slider_SENSOR_LINEARSLIDER0_E2__LS    (2u)
#define Slider_SENSOR_LINEARSLIDER0_E3__LS    (3u)
#define Slider_SENSOR_LINEARSLIDER0_E4__LS    (4u)

#define Slider_TOTAL_SENSOR_MASK (((Slider_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)	

/* Interrupt handler */
CY_ISR_PROTO(Slider_ISR);

/* ISR Number and Priority to work with CyLib functions */
#define Slider_ISR_NUMBER        (Slider_ISR__INTC_NUMBER)
#define Slider_ISR_PRIORITY      (Slider_ISR__INTC_PRIOR_NUM)

/***************************************
*   Condition compilation parameters
***************************************/

#define Slider_CONNECT_INACTIVE_SNS       (0u)
#define Slider_IS_COMPLEX_SCANSLOTS       (0u)
#define Slider_COMPLEX_SCANSLOTS_NUM      (0u)
#define Slider_DEDICATED_SENSORS_NUM      (0u)
#define Slider_PORT_PIN_CONFIG_TBL_ZISE   (5u)

#define Slider_IDAC_CNT                   (2u)
#define Slider_IDAC1_POLARITY             (0u)
#define Slider_IDAC1_RANGE_VALUE          (0u)
#define Slider_IDAC2_RANGE_VALUE          (0u)

#define Slider_MULTIPLE_FREQUENCY_SET     (1u)
#define Slider_FILTER_ENABLE			    (0u)
#define Slider_PRS_OPTIONS                (0u)

#define Slider_WATER_PROOF                (0u)

#define Slider_TUNING_METHOD              (2u)
#define Slider_TUNER_API_GENERATE         (0u)

#define Slider_CSHL_API_GENERATE          (1u)

#define Slider_CMOD_PREGARGE_OPTION       (1u)
#define Slider_CSH_TANK_PREGARGE_OPTION   (0u)
#define Slider_IS_SHIELD_ENABLE           (0u)
#define Slider_CSH_TANK_ENABLE            (0u)

#define Slider_SHIELD_DELAY               (0u)
#define Slider_AUTOCALIBRATION_ENABLE		(0u)

#define Slider_IS_OVERSAMPLING_EN         (1u)

#define Slider_CSD_4B_PWM_MODE			(0u)
#define Slider_CSD_4B_PWM_COUNT			(0u)

/**************************************
*    Enumerated Types and Parameters
**************************************/

/* Current source definitions */
#define Slider__IDAC_SOURCE 0
#define Slider__IDAC_SINK 1

#define Slider__IDAC_4X 0
#define Slider__IDAC_8X 1

#define Slider__PRS_NONE 0
#define Slider__PRS_8BITS 1
#define Slider__PRS_12BITS 2
#define Slider__PRS_AUTO 3

/* Connection of inactive sensors definitions */
#define Slider__GROUND 0
#define Slider__HIZ_ANALOG 1
#define Slider__SHIELD 2

/* Precharge options definitions */
#define Slider__CAPPRIOBUF 0
#define Slider__CAPPRVREF 1

/* Method of tunning */
#define Slider__TUNING_NONE 0
#define Slider__TUNING_MANUAL 1
#define Slider__TUNING_AUTO 2

/* Dead band PWM modulator options */
#define Slider__PWM_OFF 0
#define Slider__PWM_HIGH 2
#define Slider__PWM_LOW 3


#define Slider_DELAY_EXTRACYCLES_NUM			(9u)
#define Slider_GLITCH_ELIMINATION_TIMEOUT		(0u)
#define Slider_GLITCH_ELIMINATION_CYCLES_CALC	(Slider_GLITCH_ELIMINATION_TIMEOUT * CYDEV_BCLK__SYSCLK__MHZ)

#if(Slider_GLITCH_ELIMINATION_CYCLES_CALC >= Slider_DELAY_EXTRACYCLES_NUM)
	#define Slider_GLITCH_ELIMINATION_CYCLES (Slider_GLITCH_ELIMINATION_CYCLES_CALC -\
													    Slider_DELAY_EXTRACYCLES_NUM)
#else
	#define Slider_GLITCH_ELIMINATION_CYCLES (Slider_GLITCH_ELIMINATION_CYCLES_CALC)
#endif /* (Slider_GLITCH_ELIMINATION_CYCLES_CALC >= Slider_DELAY_EXTRACYCLES_NUM) */

#define Slider_INITIAL_CLK_DIVIDER			(CYDEV_BCLK__HFCLK__KHZ / CYDEV_BCLK__SYSCLK__KHZ)


/* Structure to save registers before going to sleep */
typedef struct
{
    uint8 enableState;
} Slider_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/
void Slider_Init(void);
void Slider_CsdHwBlockInit(void);
void Slider_Enable(void);
void Slider_Start(void);
void Slider_Stop(void);
void Slider_SaveConfig(void);
void Slider_Sleep(void);
void Slider_RestoreConfig(void);
void Slider_Wakeup(void);
uint32 Slider_IsBusy(void);
void Slider_ScanSensor(uint32 sensor);
void Slider_ScanWidget(uint32 widget);
void Slider_ScanEnabledWidgets(void);
uint16 Slider_ReadSensorRaw(uint32 sensor);
void Slider_WriteSensorRaw(uint32 sensor, uint16 value);
void Slider_ClearSensors(void);
void Slider_SetShieldDelay(uint32 delay);
uint32 Slider_ReadCurrentScanningSensor(void);

uint32 Slider_GetScanResolution(uint32 widget);
uint32 Slider_GetSenseClkDivider(uint32 sensor);
uint32 Slider_GetModulatorClkDivider(uint32 sensor);
uint32 Slider_GetModulationIDAC(uint32 sensor);
uint32 Slider_GetCompensationIDAC(uint32 sensor);
uint32 Slider_GetIDACRange(void);

#if (Slider_TUNING_METHOD != Slider__TUNING_NONE)
	void Slider_SetScanResolution(uint32 widget, uint32 resolution);
    void Slider_SetSenseClkDivider(uint32 sensor, uint32 senseClk);
	void Slider_SetModulatorClkDivider(uint32 sensor, uint32 modulatorClk);
	void Slider_SetModulationIDAC(uint32 sensor, uint32 modIdacValue);
	void Slider_SetCompensationIDAC(uint32 sensor, uint32 compIdacValue);
	void Slider_SetIDACRange(uint32 iDacRange);
#endif /* (Slider_TUNING_METHOD != Slider__TUNING_NONE) */


void Slider_SetDriveModeAllPins(uint32 driveMode);
void Slider_RestoreDriveModeAllPins(void);
void Slider_SetUnscannedSensorState(uint32 sensor, uint32 sensorState);
void Slider_NormalizeWidgets(uint32 widgetsNum, uint8 *dataPtr);

/***************************************
*           API Constants
***************************************/

/* PWM Resolution */
#define Slider_CNT_MSB_RESOLUTION_8_BITS  (0x00u)
#define Slider_CNT_MSB_RESOLUTION_9_BITS  (0x01u)
#define Slider_CNT_MSB_RESOLUTION_10_BITS (0x03u)
#define Slider_CNT_MSB_RESOLUTION_11_BITS (0x07u)
#define Slider_CNT_MSB_RESOLUTION_12_BITS (0x0Fu)
#define Slider_CNT_MSB_RESOLUTION_13_BITS (0x1Fu)
#define Slider_CNT_MSB_RESOLUTION_14_BITS (0x3Fu)
#define Slider_CNT_MSB_RESOLUTION_15_BITS (0x7Fu)
#define Slider_CNT_MSB_RESOLUTION_16_BITS (0xFFu)

#define Slider_ONE_CYCLE                  (0x00010000Lu)

#define Slider_WINDOW_APPEND              (0xFFFF0000Lu)
#define Slider_RESOLUTION_6_BITS          (0x003F0000Lu)
#define Slider_RESOLUTION_7_BITS          (0x007F0000Lu)
#define Slider_RESOLUTION_8_BITS          (0x00FF0000Lu)
#define Slider_RESOLUTION_9_BITS          (0x01FF0000Lu)
#define Slider_RESOLUTION_10_BITS         (0x03FF0000Lu)
#define Slider_RESOLUTION_11_BITS         (0x07FF0000Lu)
#define Slider_RESOLUTION_12_BITS         (0x0FFF0000Lu)
#define Slider_RESOLUTION_13_BITS         (0x1FFF0000Lu)
#define Slider_RESOLUTION_14_BITS         (0x3FFF0000Lu)
#define Slider_RESOLUTION_15_BITS         (0x7FFF0000Lu)
#define Slider_RESOLUTION_16_BITS         (0xFFFF0000Lu)

#define Slider_RESOLUTION_OFFSET          (16u)
#define Slider_MSB_RESOLUTION_OFFSET      (22u)
#define Slider_RESOLUTIONS_TBL_SIZE       (1u)

/* Software Status Register Bit Masks */
#define Slider_SW_STS_BUSY                (0x01u)
/* Software Control Register Bit Masks */
#define Slider_SW_CTRL_SINGLE_SCAN        (0x80u)

/* Software Control Register Bit Masks for scanning one widget */
#define Slider_SW_CTRL_WIDGET_SCAN        (0x40u)

/* Flag for complex scan slot */
#define Slider_COMPLEX_SS_FLAG            (0x80u)

/* Flag for scanning is complete */
#define Slider_NOT_SENSOR                 (0xFFFFFFFFLu)

/* Number of bits for each pin in PC register. */
#define Slider_PC_PIN_CFG_SIZE            (0x03u)

/* Number of bits for each pin in HSIOM register. */
#define Slider_HSIOM_PIN_CFG_SIZE         (0x04u)

#if (CY_PSOC4_4000)
    #define Slider_CFG_REG_TBL_SIZE       (4u)
#elif(CY_PSOC4_4100 || CY_PSOC4_4200)
    #define Slider_CFG_REG_TBL_SIZE       (5u)
#elif (CY_PSOC4_4100BL || CY_PSOC4_4200BL)
    #define Slider_CFG_REG_TBL_SIZE       (6u)
#else
    #define Slider_CFG_REG_TBL_SIZE       (8u)
#endif /* (CY_PSOC4_4000) */

/***************************************
*             Registers
***************************************/

#define Slider_CSD_ID_REG             (*(reg32 *)  Slider_CSD_FFB__CSD_ID)
#define Slider_CSD_ID_PTR             ( (reg32 *)  Slider_CSD_FFB__CSD_ID)

#define Slider_CSD_CFG_REG            (*(reg32 *)  Slider_CSD_FFB__CSD_CONFIG)
#define Slider_CSD_CFG_PTR            ( (reg32 *)  Slider_CSD_FFB__CSD_CONFIG)

#define Slider_CSD_IDAC_REG           (*(reg32 *)  Slider_IDAC1_cy_psoc4_idac__CSD_IDAC)
#define Slider_CSD_IDAC_PTR           ( (reg32 *)  Slider_IDAC1_cy_psoc4_idac__CSD_IDAC)

#define Slider_CSD_CNT_REG            (*(reg32 *) Slider_CSD_FFB__CSD_COUNTER)
#define Slider_CSD_CNT_PTR            ( (reg32 *) Slider_CSD_FFB__CSD_COUNTER)

#define Slider_CSD_STAT_REG           (*(reg32 *) Slider_CSD_FFB__CSD_STATUS)
#define Slider_CSD_STAT_PTR           ( (reg32 *) Slider_CSD_FFB__CSD_STATUS)

#define Slider_CSD_INTR_REG           (*(reg32 *) Slider_CSD_FFB__CSD_INTR)
#define Slider_CSD_INTR_PTR           ( (reg32 *) Slider_CSD_FFB__CSD_INTR)

#define Slider_CSD_INTR_SET_REG       (*(reg32 *) Slider_CSD_FFB__CSD_INTR_SET)
#define Slider_CSD_INTR_SET_PTR       ( (reg32 *) Slider_CSD_FFB__CSD_INTR_SET)

#define Slider_CSD_4B_PWM_REG         (*(reg32 *) Slider_CSD_FFB__CSD_PWM)
#define Slider_CSD_4B_PWM_PTR         ( (reg32 *) Slider_CSD_FFB__CSD_PWM)

#define Slider_CSD_TRIM1_REG          (*(reg32 *) Slider_IDAC1_cy_psoc4_idac__CSD_TRIM1)
#define Slider_CSD_TRIM1_PTR          ( (reg32 *) Slider_IDAC1_cy_psoc4_idac__CSD_TRIM1)

#define Slider_CSD_TRIM2_REG          (*(reg32 *) Slider_IDAC1_cy_psoc4_idac__CSD_TRIM2)
#define Slider_CSD_TRIM2_PTR          ( (reg32 *) Slider_IDAC1_cy_psoc4_idac__CSD_TRIM2)
	
#if (CY_PSOC4_4100M || CY_PSOC4_4200M)
	#if (Slider_CSD_FFB__CSD_NUMBER != 1u)
		#define Slider_SFLASH_CSD_TRIM1_REG   (*(reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)
		#define Slider_SFLASH_CSD_TRIM1_PTR   ( (reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)

		#define Slider_SFLASH_CSD_TRIM2_REG   (*(reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
		#define Slider_SFLASH_CSD_TRIM2_PTR   ( (reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
	#else
		#define Slider_SFLASH_CSD_TRIM1_REG   (*(reg8 *) CYREG_SFLASH_CSD1_TRIM1_CSD)
		#define Slider_SFLASH_CSD_TRIM1_PTR   ( (reg8 *) CYREG_SFLASH_CSD1_TRIM1_CSD)

		#define Slider_SFLASH_CSD_TRIM2_REG   (*(reg8 *) CYREG_SFLASH_CSD1_TRIM2_CSD)
		#define Slider_SFLASH_CSD_TRIM2_PTR   ( (reg8 *) CYREG_SFLASH_CSD1_TRIM2_CSD)		
	#endif /* (Slider_CSD_FFB__CSD_NUMBER != 1u) */
#else
	#define Slider_SFLASH_CSD_TRIM1_REG   (*(reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)
	#define Slider_SFLASH_CSD_TRIM1_PTR   ( (reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)

	#define Slider_SFLASH_CSD_TRIM2_REG   (*(reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
	#define Slider_SFLASH_CSD_TRIM2_PTR   ( (reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
#endif /* (CY_PSOC4_4100M || CY_PSOC4_4200M) */

/* Port function select */
#define Slider_PORT_SEL0_REG          	(*(reg32 *) CYREG_HSIOM_PORT_SEL0 )
#define Slider_PORT_SEL0_PTR          	( (reg32 *) CYREG_HSIOM_PORT_SEL0 )

#define Slider_PORT_SEL1_REG          	(*(reg32 *) CYREG_HSIOM_PORT_SEL1 )
#define Slider_PORT_SEL1_PTR          	( (reg32 *) CYREG_HSIOM_PORT_SEL1 )

#define Slider_PORT_SEL2_REG          	(*(reg32 *) CYREG_HSIOM_PORT_SEL2 )
#define Slider_PORT_SEL2_PTR          	( (reg32 *) CYREG_HSIOM_PORT_SEL2 )

#define Slider_PORT_SEL3_REG          	(*(reg32 *) CYREG_HSIOM_PORT_SEL3 )
#define Slider_PORT_SEL3_PTR          	( (reg32 *) CYREG_HSIOM_PORT_SEL3 )

#if (Slider_CFG_REG_TBL_SIZE > 4u)
	#define Slider_PORT_SEL4_REG          	(*(reg32 *) CYREG_HSIOM_PORT_SEL4 )
	#define Slider_PORT_SEL4_PTR          	( (reg32 *) CYREG_HSIOM_PORT_SEL4 )
#endif /* (Slider_CFG_REG_TBL_SIZE > 4u) */

#if (Slider_CFG_REG_TBL_SIZE > 5u)
	#define Slider_PORT_SEL5_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL5 )
	#define Slider_PORT_SEL5_PTR          ( (reg32 *) CYREG_HSIOM_PORT_SEL5 )
#endif /* (Slider_CFG_REG_TBL_SIZE > 5u) */

#if (Slider_CFG_REG_TBL_SIZE > 6u)
	#define Slider_PORT_SEL6_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL6 )
	#define Slider_PORT_SEL6_PTR          ( (reg32 *) CYREG_HSIOM_PORT_SEL6 )

	#define Slider_PORT_SEL7_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL7 )
	#define Slider_PORT_SEL7_PTR          ( (reg32 *) CYREG_HSIOM_PORT_SEL7 )
#endif /* (Slider_CFG_REG_TBL_SIZE > 6u) */

#if (0u == Slider_IS_M0S8IOSS_BLOCK)
	
	/* Port DR and PC registers */
	#define Slider_PORT0_DR_REG           (*(reg32 *) CYREG_PRT0_DR )
	#define Slider_PORT0_DR_PTR           ( (reg32 *) CYREG_PRT0_DR )

	#define Slider_PORT1_DR_REG           (*(reg32 *) CYREG_PRT1_DR )
	#define Slider_PORT1_DR_PTR           ( (reg32 *) CYREG_PRT1_DR )

	#define Slider_PORT2_DR_REG           (*(reg32 *) CYREG_PRT2_DR )
	#define Slider_PORT2_DR_PTR           ( (reg32 *) CYREG_PRT2_DR )

	#define Slider_PORT3_DR_REG           (*(reg32 *) CYREG_PRT3_DR )
	#define Slider_PORT3_DR_PTR           ( (reg32 *) CYREG_PRT3_DR )

	#define Slider_PORT4_DR_REG           (*(reg32 *) CYREG_PRT4_DR )
	#define Slider_PORT4_DR_PTR           ( (reg32 *) CYREG_PRT4_DR )

	/* Port DR and PC registers */
	#define Slider_PORT0_PC_REG           (*(reg32 *) CYREG_PRT0_PC )
	#define Slider_PORT0_PC_PTR           ( (reg32 *) CYREG_PRT0_PC )

	#define Slider_PORT1_PC_REG           (*(reg32 *) CYREG_PRT1_PC )
	#define Slider_PORT1_PC_PTR           ( (reg32 *) CYREG_PRT1_PC )

	#define Slider_PORT2_PC_REG           (*(reg32 *) CYREG_PRT2_PC )
	#define Slider_PORT2_PC_PTR           ( (reg32 *) CYREG_PRT2_PC )

	#define Slider_PORT3_PC_REG           (*(reg32 *) CYREG_PRT3_PC )
	#define Slider_PORT3_PC_PTR           ( (reg32 *) CYREG_PRT3_PC )

	#define Slider_PORT4_PC_REG           (*(reg32 *) CYREG_PRT4_PC )
	#define Slider_PORT4_PC_PTR           ( (reg32 *) CYREG_PRT4_PC )

#else
		
	/* Port DR and PC registers */
	#define Slider_PORT0_DR_REG           (*(reg32 *) CYREG_GPIO_PRT0_DR )
	#define Slider_PORT0_DR_PTR           ( (reg32 *) CYREG_GPIO_PRT0_DR )

	#define Slider_PORT1_DR_REG           (*(reg32 *) CYREG_GPIO_PRT1_DR )
	#define Slider_PORT1_DR_PTR           ( (reg32 *) CYREG_GPIO_PRT1_DR )

	#define Slider_PORT2_DR_REG           (*(reg32 *) CYREG_GPIO_PRT2_DR )
	#define Slider_PORT2_DR_PTR           ( (reg32 *) CYREG_GPIO_PRT2_DR )

	#define Slider_PORT3_DR_REG           (*(reg32 *) CYREG_GPIO_PRT3_DR )
	#define Slider_PORT3_DR_PTR           ( (reg32 *) CYREG_GPIO_PRT3_DR )

	#define Slider_PORT4_DR_REG           (*(reg32 *) CYREG_GPIO_PRT4_DR )
	#define Slider_PORT4_DR_PTR           ( (reg32 *) CYREG_GPIO_PRT4_DR )

	/* Port DR and PC registers */
	#define Slider_PORT0_PC_REG           (*(reg32 *) CYREG_GPIO_PRT0_PC )
	#define Slider_PORT0_PC_PTR           ( (reg32 *) CYREG_GPIO_PRT0_PC )

	#define Slider_PORT1_PC_REG           (*(reg32 *) CYREG_GPIO_PRT1_PC )
	#define Slider_PORT1_PC_PTR           ( (reg32 *) CYREG_GPIO_PRT1_PC )

	#define Slider_PORT2_PC_REG           (*(reg32 *) CYREG_GPIO_PRT2_PC )
	#define Slider_PORT2_PC_PTR           ( (reg32 *) CYREG_GPIO_PRT2_PC )

	#define Slider_PORT3_PC_REG           (*(reg32 *) CYREG_GPIO_PRT3_PC )
	#define Slider_PORT3_PC_PTR           ( (reg32 *) CYREG_GPIO_PRT3_PC )

	#define Slider_PORT4_PC_REG           (*(reg32 *) CYREG_GPIO_PRT4_PC )
	#define Slider_PORT4_PC_PTR           ( (reg32 *) CYREG_GPIO_PRT4_PC )
	
#endif /* (0u == Slider_IS_M0S8IOSS_BLOCK) */

#if (Slider_CFG_REG_TBL_SIZE > 5u)
    /* Port DR and PC registers */
	#define Slider_PORT5_DR_REG           (*(reg32 *) CYREG_GPIO_PRT5_DR )
	#define Slider_PORT5_DR_PTR           ( (reg32 *) CYREG_GPIO_PRT5_DR )

	/* Port DR and PC registers */
	#define Slider_PORT5_PC_REG           (*(reg32 *) CYREG_GPIO_PRT5_PC )
	#define Slider_PORT5_PC_PTR           ( (reg32 *) CYREG_GPIO_PRT5_PC )
#endif /* (Slider_CFG_REG_TBL_SIZE > 5u) */

#if (Slider_CFG_REG_TBL_SIZE > 6u)
    /* Port DR and PC registers */
	#define Slider_PORT6_DR_REG           (*(reg32 *) CYREG_GPIO_PRT6_DR )
	#define Slider_PORT6_DR_PTR           ( (reg32 *) CYREG_GPIO_PRT6_DR )

	/* Port DR and PC registers */
	#define Slider_PORT6_PC_REG           (*(reg32 *) CYREG_GPIO_PRT6_PC )
	#define Slider_PORT6_PC_PTR           ( (reg32 *) CYREG_GPIO_PRT6_PC )
	
    /* Port DR and PC registers */
	#define Slider_PORT7_DR_REG           (*(reg32 *) CYREG_GPIO_PRT7_DR )
	#define Slider_PORT7_DR_PTR           ( (reg32 *) CYREG_GPIO_PRT7_DR )

	/* Port DR and PC registers */
	#define Slider_PORT7_PC_REG           (*(reg32 *) CYREG_GPIO_PRT7_PC )
	#define Slider_PORT7_PC_PTR           ( (reg32 *) CYREG_GPIO_PRT7_PC )
	
#endif /* (Slider_CFG_REG_TBL_SIZE > 6u) */

#if (CY_PSOC4_4100 || CY_PSOC4_4200 || CY_PSOC4_4100BL || CY_PSOC4_4200BL)

	/*******************************************************************************
	* Cmod and Ctank registers
	* 
	* CY_PSOC4_4200:   Cmod = 4[2]; Csh = 4[3];
	* CY_PSOC4_4200BL: Cmod = 4[0]; Csh = 4[1];
	*
	*******************************************************************************/

    #define Slider_CMOD_CONNECTION_REG        (Slider_PORT_SEL4_REG)
    #define Slider_CMOD_PORT_PC_REG           (Slider_PORT4_PC_REG)
    #define Slider_CMOD_PORT_DR_REG           (Slider_PORT4_DR_REG)

    #define Slider_CTANK_CONNECTION_REG       (Slider_PORT_SEL4_REG)
    #define Slider_CTANK_PORT_PC_REG          (Slider_PORT4_PC_REG)
    #define Slider_CTANK_PORT_DR_REG          (Slider_PORT4_DR_REG)

#elif (CY_PSOC4_4100M || CY_PSOC4_4200M)
	
	/*******************************************************************************
	* Cmod and Ctank registers
	* 
	* CY_PSOC4_4200M:  Cmod = 4[2], 5[0]; Csh = 4[3], 5[1];
	*
	*******************************************************************************/
	
	#if (Slider_CSD_FFB__CSD_NUMBER != 1u)
		#define Slider_CMOD_CONNECTION_REG        (Slider_PORT_SEL4_REG)
		#define Slider_CMOD_PORT_PC_REG           (Slider_PORT4_PC_REG)
		#define Slider_CMOD_PORT_DR_REG           (Slider_PORT4_DR_REG)

		#define Slider_CTANK_CONNECTION_REG       (Slider_PORT_SEL4_REG)
		#define Slider_CTANK_PORT_PC_REG          (Slider_PORT4_PC_REG)
		#define Slider_CTANK_PORT_DR_REG          (Slider_PORT4_DR_REG)
	#else
		#define Slider_CMOD_CONNECTION_REG        (Slider_PORT_SEL5_REG)
		#define Slider_CMOD_PORT_PC_REG           (Slider_PORT5_PC_REG)
		#define Slider_CMOD_PORT_DR_REG           (Slider_PORT5_DR_REG)

		#define Slider_CTANK_CONNECTION_REG       (Slider_PORT_SEL5_REG)
		#define Slider_CTANK_PORT_PC_REG          (Slider_PORT5_PC_REG)
		#define Slider_CTANK_PORT_DR_REG          (Slider_PORT5_DR_REG)
	#endif /* (Slider_CSD_FFB__CSD_NUMBER != 1u) */

#else

	/*******************************************************************************
	* Cmod and Ctank registers
	* 
	* CY_PSOC4_4000:   Cmod = 0[4]; Csh = 0[2];
	*
	*******************************************************************************/

    #define Slider_CMOD_CONNECTION_REG        (Slider_PORT_SEL0_REG)
    #define Slider_CMOD_PORT_PC_REG           (Slider_PORT0_PC_REG)
    #define Slider_CMOD_PORT_DR_REG           (Slider_PORT0_DR_REG)

    #define Slider_CTANK_CONNECTION_REG       (Slider_PORT_SEL0_REG)
    #define Slider_CTANK_PORT_PC_REG          (Slider_PORT0_PC_REG)
    #define Slider_CTANK_PORT_DR_REG          (Slider_PORT0_DR_REG)

#endif /* (CY_PSOC4_4100 || CY_PSOC4_4200 || CY_PSOC4_4100BL || CY_PSOC4_4200BL)) */

/***************************************
*       Register Constants
***************************************/

/* Port configuration defines */
#define Slider_CSD_HSIOM_MASK                 (0x0000000FLu)
#define Slider_CSD_SENSE_PORT_MODE            (0x00000004Lu)
#define Slider_CSD_SHIELD_PORT_MODE           (0x00000005Lu)

#define Slider_CSD_PIN_MODE_MASK              (0x00000007Lu)
#define Slider_CSD_PIN_DRIVE_MASK             (0x00000001Lu)

#if (CY_PSOC4_4100 || CY_PSOC4_4200)

	/*******************************************************************************
	* Cmod and Ctank registers
	* 
	* CY_PSOC4_4200:   Cmod = 4[2]; Csh = 4[3];
	*
	*******************************************************************************/

	#define Slider_CSD_CMOD_CONNECTION_MASK   (0x00000F00Lu)
	#define Slider_CSD_CMOD_TO_AMUXBUS_A      (0x00000600Lu)
	#define Slider_CSD_CMOD_TO_AMUXBUS_B      (0x00000700Lu)
	
	#define Slider_CSD_CTANK_CONNECTION_MASK  (0x0000F000Lu)
	#define Slider_CSD_CTANK_TO_AMUXBUS_A     (0x00006000Lu)
	#define Slider_CSD_CTANK_TO_AMUXBUS_B     (0x00007000Lu)
	
	#define  Slider_CSD_CMOD_PC_MASK          (0x000001C0Lu)
	#define  Slider_CSD_CTANK_PC_MASK         (0x00000E00Lu)

	#define  Slider_CMOD_PC_STRONG_MODE       (0x00000180Lu)
	#define  Slider_CMOD_PC_HIGH_Z_MODE       (0x00000000Lu)

	#define  Slider_CTANK_PC_STRONG_MODE      (0x00000C00Lu)
	#define  Slider_CTANK_PC_HIGH_Z_MODE      (0x00000000Lu)

	#define  Slider_CMOD_DR_VSSIO             (0x00000004Lu)
	#define  Slider_CMOD_DR_VDDIO             (0x00000000Lu)
	#define  Slider_CMOD_DR_MASK              (0x00000004Lu)

	#define  Slider_CTANK_DR_VSSIO            (0x00000008Lu)
	#define  Slider_CTANK_DR_VDDIO            (0x00000000Lu)
	#define  Slider_CTANK_DR_MASK             (0x00000008Lu)
	
#elif((CY_PSOC4_4100BL) || (CY_PSOC4_4200BL)) 

	/*******************************************************************************
	* Cmod and Ctank config registers values
	*
	* CY_PSOC4_4200BL: Cmod = 4[0]; Csh = 4[1];
	*
	*******************************************************************************/
	
	#define Slider_CSD_CMOD_CONNECTION_MASK   (0x0000000FLu)
	#define Slider_CSD_CMOD_TO_AMUXBUS_A      (0x00000006Lu)
	#define Slider_CSD_CMOD_TO_AMUXBUS_B      (0x00000007Lu)
	
	#define Slider_CSD_CTANK_CONNECTION_MASK  (0x000000F0Lu)
	#define Slider_CSD_CTANK_TO_AMUXBUS_A     (0x00000060Lu)
	#define Slider_CSD_CTANK_TO_AMUXBUS_B     (0x00000070Lu)
	
	#define  Slider_CSD_CMOD_PC_MASK          (0x00000007Lu)
	#define  Slider_CSD_CTANK_PC_MASK         (0x00000038Lu)

	#define  Slider_CMOD_PC_STRONG_MODE       (0x00000006Lu)
	#define  Slider_CMOD_PC_HIGH_Z_MODE       (0x00000000Lu)

	#define  Slider_CTANK_PC_STRONG_MODE      (0x00000030Lu)
	#define  Slider_CTANK_PC_HIGH_Z_MODE      (0x00000000Lu)

	#define  Slider_CMOD_DR_VSSIO             (0x00000001Lu)
	#define  Slider_CMOD_DR_VDDIO             (0x00000000Lu)
	#define  Slider_CMOD_DR_MASK              (0x00000001Lu)

	#define  Slider_CTANK_DR_VSSIO            (0x00000002Lu)
	#define  Slider_CTANK_DR_VDDIO            (0x00000000Lu)
	#define  Slider_CTANK_DR_MASK             (0x00000002Lu)

#elif(CY_PSOC4_4100M || CY_PSOC4_4200M)

	/*******************************************************************************
	* Cmod and Ctank registers
	* 
	* CY_PSOC4_4200M:  Cmod = 4[2], 5[0]; Csh = 4[3], 5[1];
	*
	*******************************************************************************/
	
	#if (Slider_CSD_FFB__CSD_NUMBER != 1u)

		#define Slider_CSD_CMOD_CONNECTION_MASK   (0x00000F00Lu)
		#define Slider_CSD_CMOD_TO_AMUXBUS_A      (0x00000600Lu)
		#define Slider_CSD_CMOD_TO_AMUXBUS_B      (0x00000700Lu)
		
		#define Slider_CSD_CTANK_CONNECTION_MASK  (0x0000F000Lu)
		#define Slider_CSD_CTANK_TO_AMUXBUS_A     (0x00006000Lu)
		#define Slider_CSD_CTANK_TO_AMUXBUS_B     (0x00007000Lu)
		
		#define  Slider_CSD_CMOD_PC_MASK          (0x000001C0Lu)
		#define  Slider_CSD_CTANK_PC_MASK         (0x00000E00Lu)

		#define  Slider_CMOD_PC_STRONG_MODE       (0x00000180Lu)
		#define  Slider_CMOD_PC_HIGH_Z_MODE       (0x00000000Lu)

		#define  Slider_CTANK_PC_STRONG_MODE      (0x00000C00Lu)
		#define  Slider_CTANK_PC_HIGH_Z_MODE      (0x00000000Lu)

		#define  Slider_CMOD_DR_VSSIO             (0x00000004Lu)
		#define  Slider_CMOD_DR_VDDIO             (0x00000000Lu)
		#define  Slider_CMOD_DR_MASK              (0x00000004Lu)

		#define  Slider_CTANK_DR_VSSIO            (0x00000008Lu)
		#define  Slider_CTANK_DR_VDDIO            (0x00000000Lu)
		#define  Slider_CTANK_DR_MASK             (0x00000008Lu)
	
	#else
	
		#define Slider_CSD_CMOD_CONNECTION_MASK   (0x0000000FLu)
		#define Slider_CSD_CMOD_TO_AMUXBUS_A      (0x00000006Lu)
		#define Slider_CSD_CMOD_TO_AMUXBUS_B      (0x00000007Lu)
		
		#define Slider_CSD_CTANK_CONNECTION_MASK  (0x000000F0Lu)
		#define Slider_CSD_CTANK_TO_AMUXBUS_A     (0x00000060Lu)
		#define Slider_CSD_CTANK_TO_AMUXBUS_B     (0x00000070Lu)
		
		#define  Slider_CSD_CMOD_PC_MASK          (0x00000007Lu)
		#define  Slider_CSD_CTANK_PC_MASK         (0x00000038Lu)

		#define  Slider_CMOD_PC_STRONG_MODE       (0x00000006Lu)
		#define  Slider_CMOD_PC_HIGH_Z_MODE       (0x00000000Lu)

		#define  Slider_CTANK_PC_STRONG_MODE      (0x00000030Lu)
		#define  Slider_CTANK_PC_HIGH_Z_MODE      (0x00000000Lu)

		#define  Slider_CMOD_DR_VSSIO             (0x00000001Lu)
		#define  Slider_CMOD_DR_VDDIO             (0x00000000Lu)
		#define  Slider_CMOD_DR_MASK              (0x00000001Lu)

		#define  Slider_CTANK_DR_VSSIO            (0x00000002Lu)
		#define  Slider_CTANK_DR_VDDIO            (0x00000000Lu)
		#define  Slider_CTANK_DR_MASK             (0x00000002Lu)
	
	#endif /* (Slider_CSD_FFB__CSD_NUMBER != 1u) */

#else

	/*******************************************************************************
	* Cmod and Ctank registers
	* 
	* CY_PSOC4_4000:   Cmod = 0[4]; Csh = 0[2];
	*
	*******************************************************************************/

	#define Slider_CSD_CMOD_CONNECTION_MASK   (0x000F0000Lu)
	#define Slider_CSD_CMOD_TO_AMUXBUS_A      (0x00060000Lu)
	#define Slider_CSD_CMOD_TO_AMUXBUS_B      (0x00070000Lu)
	
	#define Slider_CSD_CTANK_CONNECTION_MASK  (0x00000F00Lu)
	#define Slider_CSD_CTANK_TO_AMUXBUS_A     (0x00000600Lu)
	#define Slider_CSD_CTANK_TO_AMUXBUS_B     (0x00000700Lu)
	
	#define  Slider_CSD_CMOD_PC_MASK          (0x00007000Lu)
	#define  Slider_CSD_CTANK_PC_MASK         (0x000001C0Lu)

	#define  Slider_CMOD_PC_STRONG_MODE       (0x00006000Lu)
	#define  Slider_CMOD_PC_HIGH_Z_MODE       (0x00000000Lu)

	#define  Slider_CTANK_PC_STRONG_MODE      (0x00000180Lu)
	#define  Slider_CTANK_PC_HIGH_Z_MODE      (0x00000000Lu)

	#define  Slider_CMOD_DR_VSSIO             (0x00000010Lu)
	#define  Slider_CMOD_DR_VDDIO             (0x00000000Lu)
	#define  Slider_CMOD_DR_MASK              (0x00000010Lu)

	#define  Slider_CTANK_DR_VSSIO            (0x00000004Lu)
	#define  Slider_CTANK_DR_VDDIO            (0x00000000Lu)
	#define  Slider_CTANK_DR_MASK             (0x00000004Lu)
#endif  /* (CY_PSOC4_4100 || CY_PSOC4_4200) */

#define  Slider_CMOD_PIN_NUMBER               (Slider_Cmod__0__SHIFT)
#define  Slider_CMOD_PRT_NUMBER               (Slider_Cmod__0__PORT)

#if(0u != Slider_CSH_TANK_ENABLE)
	#define  Slider_CTANK_PIN_NUMBER          (Slider_Csh_tank__0__SHIFT)
	#define  Slider_CTANK_PRT_NUMBER          (Slider_Csh_tank__0__PORT)
#endif /* (0u != Slider_CSH_TANK_ENABLE) */

/* ID register fields position */
#define Slider_CSD_ID_ID__POS                 (0x00u)
#define Slider_CSD_ID_REV__POS                (0x10u)

#define Slider_CSD_ID_ID                      (0xFFLu << Slider_CSD_ID_ID__POS)
#define Slider_CSD_ID_REV                     (0xFFLu << Slider_CSD_ID_REV__POS)

/* CSD_CONFIG register fields position */
#define Slider_CSD_CFG_DSI_SAMPLE_EN_POS      (0x00u)
#define Slider_CSD_CFG_SAMPLE_SYNC_POS        (0x01u)
#define Slider_CSD_CFG_FILTER_EN_POS          (0x03u)
#define Slider_CSD_CFG_PRS_CLEAR_POS          (0x05u)
#define Slider_CSD_CFG_PRS_SELECT_POS         (0x06u)
#define Slider_CSD_CFG_PRS_12_8_POS           (0x07u)
#define Slider_CSD_CFG_DSI_SENSE_EN_POS       (0x08u)
#define Slider_CSD_CFG_SHIELD_DELAY_POS       (0x09u)
#define Slider_CSD_CFG_SENSE_COMP_BW_POS      (0x0Bu)
#define Slider_CSD_CFG_SENSE_EN_POS           (0x0Cu)
#define Slider_CSD_CFG_REFBUF_EN_POS          (0x0Du)
#define Slider_CSD_CFG_CHARGE_IO_POS          (0x0Eu)
#define Slider_CSD_CFG_COMP_PIN_POS           (0x0Fu)
#define Slider_CSD_CFG_POLARITY_POS           (0x10u)
#define Slider_CSD_CFG_MUTUAL_CAP_POS         (0x12u)
#define Slider_CSD_CFG_SENSE_COMP_EN_POS      (0x13u)
#define Slider_CSD_CFG_REFBUF_OUTSEL_POS      (0x15u)
#define Slider_CSD_CFG_SENSE_INSEL_POS        (0x16u)
#define Slider_CSD_REFBUF_DRV_POS             (0x17u)
#define Slider_CSD_CFG_DDFTSEL_POS            (0x1Au)
#define Slider_CSD_CFG_ADFTEN_POS             (0x1Du)
#define Slider_CSD_CFG_DDFTCOMP_POS           (0x1Eu)
#define Slider_CSD_CFG_ENABLE_POS             (0x1Fu)

/* CSD_STATUS register fields position */
#define Slider_CSD_STAT_CSD_CHARGE_POS        (0x00u)
#define Slider_CSD_STAT_CSD_SENSE_POS         (0x01u)
#define Slider_CSD_STAT_COMP_OUT_POS          (0x02u)
#define Slider_CSD_STAT_SAMPLE_POS            (0x03u)

/* CSD_IDAC control register fields position */
#define Slider_CSD_IDAC1_MODE_MASK_POS		(0x08u)
#define Slider_CSD_IDAC1_MODE_FIXED_POS		(0x08u)
#define Slider_CSD_IDAC1_MODE_VARIABLE_POS	(0x08u)
#define Slider_CSD_IDAC1_MODE_DSI_POS			(0x08u)
#define Slider_CSD_IDAC1_RANGE_8X_POS			(0x0Au)
#define Slider_CSD_IDAC1_DATA_MASK_POS		(0x00u)

#define Slider_CSD_IDAC2_MODE_MASK_POS		(0x18u)
#define Slider_CSD_IDAC2_MODE_FIXED_POS		(0x18u)
#define Slider_CSD_IDAC2_MODE_VARIABLE_POS	(0x18u)
#define Slider_CSD_IDAC2_MODE_DSI_POS			(0x18u)
#define Slider_CSD_IDAC2_RANGE_8X_POS			(0x1Au)
#define Slider_CSD_IDAC2_DATA_MASK_POS		(0x10u)

/* CSD_COUNTER register fields position */
#define Slider_CSD_CNT_COUNTER_POS            (0x00u)
#define Slider_CSD_CNT_PERIOD_POS             (0x10u)

/* CSD_CONFIRG register definitions */
#define Slider_CSD_CFG_DSI_SAMPLE_EN       	(0x01Lu << Slider_CSD_CFG_DSI_SAMPLE_EN_POS)
#define Slider_CSD_CFG_SAMPLE_SYNC         	(0x01Lu << Slider_CSD_CFG_SAMPLE_SYNC_POS)
#define	Slider_CSD_CFG_FILTER_EN           	(0x01Lu << Slider_CSD_CFG_FILTER_EN_POS)
#define Slider_CSD_CFG_PRS_CLEAR           	(0x01Lu << Slider_CSD_CFG_PRS_CLEAR_POS)
#define Slider_CSD_CFG_PRS_SELECT          	(0x01Lu << Slider_CSD_CFG_PRS_SELECT_POS)
#define Slider_CSD_CFG_PRS_12_8            	(0x01Lu << Slider_CSD_CFG_PRS_12_8_POS)
#define Slider_CSD_CFG_DSI_SENSE_EN        	(0x01Lu << Slider_CSD_CFG_DSI_SENSE_EN_POS)
#define Slider_CSD_CFG_SHIELD_DELAY_MASK   	(0x03Lu << Slider_CSD_CFG_SHIELD_DELAY_POS)
#define Slider_CSD_CFG_SENSE_COMP_BW       	(0x01Lu << Slider_CSD_CFG_SENSE_COMP_BW_POS)
#define Slider_CSD_CFG_SENSE_EN            	(0x01Lu << Slider_CSD_CFG_SENSE_EN_POS)
#define Slider_CSD_CFG_REFBUF_EN           	(0x01Lu << Slider_CSD_CFG_REFBUF_EN_POS)
#define Slider_CSD_CFG_CHARGE_IO           	(0x01Lu << Slider_CSD_CFG_CHARGE_IO_POS)
#define Slider_CSD_CFG_COMP_PIN_CH2        	(0x01Lu << Slider_CSD_CFG_COMP_PIN_POS)
#define Slider_CSD_CFG_POLARITY_VDDIO      	(0x01Lu << Slider_CSD_CFG_POLARITY_POS)
#define Slider_CSD_CFG_MUTUAL_CAP          	(0x01Lu << Slider_CSD_CFG_MUTUAL_CAP_POS)
#define Slider_CSD_CFG_SENSE_COMP_EN       	(0x01Lu << Slider_CSD_CFG_SENSE_COMP_EN_POS)
#define Slider_CSD_CFG_REFBUF_OUTSEL       	(0x01Lu << Slider_CSD_CFG_REFBUF_OUTSEL_POS)
#define Slider_CSD_CFG_SENSE_INSEL         	(0x01Lu << Slider_CSD_CFG_SENSE_INSEL_POS)
#define Slider_CSD_REFBUF_DRV_MASK         	(0x03Lu << Slider_CSD_REFBUF_DRV_POS)
#define Slider_CSD_REFBUF_DRV_LOW          	(0x01Lu << Slider_CSD_REFBUF_DRV_POS)
#define Slider_CSD_REFBUF_DRV_MEDIUM       	(0x02Lu << Slider_CSD_REFBUF_DRV_POS)
#define Slider_CSD_REFBUF_DRV_HIGH         	(0x03Lu << Slider_CSD_REFBUF_DRV_POS)
#define Slider_CSD_CFG_DDFTSEL_MASK        	(0x07Lu << Slider_CSD_CFG_DDFTSEL_POS)
#define Slider_CSD_CFG_ADFTEN              	(0x01Lu << Slider_CSD_CFG_ADFTEN_POS)
#define Slider_CSD_CFG_DDFTCOMP_MASK       	(0x03Lu << Slider_CSD_CFG_DDFTCOMP_POS)
#define Slider_CSD_CFG_DDFTCOMP_REFCOMP    	(0x01Lu << Slider_CSD_CFG_DDFTCOMP_POS)
#define Slider_CSD_CFG_DDFTCOMP_SENSECOMP  	(0x02Lu << Slider_CSD_CFG_DDFTCOMP_POS)
#define Slider_CSD_CFG_ENABLE              	(0x01Lu << Slider_CSD_CFG_ENABLE_POS)

/* CSD_STATUS register definitions */
#define Slider_CSD_STAT_CSD_CHARGE         	(0x01Lu << Slider_CSD_STAT_CSD_CHARGE_POS)
#define Slider_CSD_STAT_CSD_SENSE          	(0x01Lu << Slider_CSD_STAT_CSD_SENSE_POS)
#define Slider_CSD_STAT_COMP_OUT           	(0x01Lu << Slider_CSD_STAT_COMP_OUT_POS)
#define Slider_CSD_STAT_SAMPLE             	(0x01Lu << Slider_CSD_STAT_SAMPLE_POS)

/* CSD_IDAC control register definitions */
#define Slider_CSD_IDAC1_MODE_MASK          	(0x03Lu << Slider_CSD_IDAC1_MODE_MASK_POS)
#define Slider_CSD_IDAC1_MODE_FIXED         	(0x01Lu << Slider_CSD_IDAC1_MODE_FIXED_POS)
#define Slider_CSD_IDAC1_MODE_VARIABLE      	(0x02Lu << Slider_CSD_IDAC1_MODE_VARIABLE_POS)
#define Slider_CSD_IDAC1_MODE_DSI           	(0x03Lu << Slider_CSD_IDAC1_MODE_DSI_POS)
#define Slider_CSD_IDAC1_RANGE_8X           	(0x01Lu << Slider_CSD_IDAC1_RANGE_8X_POS)
#define Slider_CSD_IDAC1_DATA_MASK          	(0xFFLu << Slider_CSD_IDAC1_DATA_MASK_POS)

#define Slider_CSD_IDAC2_MODE_MASK          	(0x03Lu << Slider_CSD_IDAC2_MODE_MASK_POS)
#define Slider_CSD_IDAC2_MODE_FIXED         	(0x01Lu << Slider_CSD_IDAC2_MODE_FIXED_POS)
#define Slider_CSD_IDAC2_MODE_VARIABLE      	(0x02Lu << Slider_CSD_IDAC2_MODE_VARIABLE_POS)
#define Slider_CSD_IDAC2_MODE_DSI           	(0x03Lu << Slider_CSD_IDAC2_MODE_DSI_POS)
#define Slider_CSD_IDAC2_RANGE_8X           	(0x01Lu << Slider_CSD_IDAC2_RANGE_8X_POS)
#define Slider_CSD_IDAC2_DATA_MASK          	(0x7FLu << Slider_CSD_IDAC2_DATA_MASK_POS)

#define Slider_CSD_IDAC2_DATA_OFFSET        	(16u)

#define Slider_CSD_IDAC1_TRIM_MASK          	(0xFFFFFFF0u)
#define Slider_CSD_IDAC2_TRIM_MASK          	(0xFFFFFF0Fu)

#define Slider_CSFLASH_TRIM_IDAC1_MASK      	(0x0Fu)
#define Slider_CSFLASH_TRIM_IDAC2_MASK      	(0xF0)

#define Slider_CSD_4B_PWM_MODE_OFFSET       	(4u)
#define Slider_CSD_4B_PWM_MODE_DEFAULT      	(Slider_CSD_4B_PWM_MODE << Slider_CSD_4B_PWM_MODE_OFFSET)

#define Slider_CSD_4B_PWM_COUNT_MASK        	(0x0000000Fu)
#define Slider_CSD_4B_PWM_MODE_MASK         	(0x00000030u)

#define Slider_CSD_IDAC_PRECHARGE_CONFIG    	(Slider_CSD_IDAC1_MODE_FIXED |\
														Slider_PRECHARGE_IDAC1_VAL)

/* CSD_COUNTER register definitions */
#define Slider_CSD_CNT_COUNTER              	(0xFFLu << Slider_CSD_CNT_COUNTER_POS )
#define Slider_CSD_CNT_PERIOD               	(0xFFLu << Slider_CSD_CNT_PERIOD_POS)

#define Slider_CSD_PRS_8_BIT                	(0x00000000u)
#define Slider_CSD_PRS_12_BIT               	(Slider_CSD_CFG_PRS_12_8)
#define	Slider_PRS_MODE_MASK                	(Slider_CSD_CFG_PRS_12_8)

/***************************************
*    Initial Parameter Constants
***************************************/

#if (0u != Slider_FILTER_ENABLE)
    #define Slider_DEFAULT_FILTER_STATE    	(Slider_CSD_CFG_FILTER_EN)
	
#else
	#define Slider_DEFAULT_FILTER_STATE    	(0u)
	
#endif /* (Slider_PRS_OPTIONS == Slider_PRS_8BITS) */

#define	Slider_DEFAULT_CSD_4B_PWM_CONFIG   	(Slider_CSD_4B_PWM_MODE_DEFAULT | Slider_CSD_4B_PWM_COUNT)

/* Set PRS */
#if (Slider_PRS_OPTIONS == Slider__PRS_8BITS)
    #define Slider_DEFAULT_MODULATION_MODE    Slider_CSD_CFG_PRS_SELECT

#elif (Slider_PRS_OPTIONS == Slider__PRS_12BITS)
    #define Slider_DEFAULT_MODULATION_MODE    (Slider_CSD_CFG_PRS_12_8 |\
														Slider_CSD_CFG_PRS_SELECT)
#else
	#define Slider_DEFAULT_MODULATION_MODE	(0u)
#endif /* (Slider_PRS_OPTIONS == Slider_PRS_8BITS) */

/* Set IDAC ranges */
#if (Slider_IDAC1_RANGE_VALUE == Slider__IDAC_8X)
    #define Slider_DEFAULT_IDAC1_RANGE    	Slider_CSD_IDAC1_RANGE_8X
    #define Slider_DEFAULT_IDAC2_RANGE    	Slider_CSD_IDAC2_RANGE_8X
#else
    #define Slider_DEFAULT_IDAC1_RANGE    	(0u)
    #define Slider_DEFAULT_IDAC2_RANGE    	(0u)
#endif /* (Slider_IDAC1_RANGE_VALUE == Slider_IDAC_RANGE_8X) */

/* Set IDAC polarity */
#if(Slider_IDAC1_POLARITY == Slider__IDAC_SINK)

    #define Slider_DEFAULT_IDAC_POLARITY   	Slider_CSD_CFG_POLARITY_VDDIO
	#define Slider_CMOD_DR_CONFIG          	Slider_CMOD_DR_VDDIO	
	#define Slider_CTANK_DR_CONFIG         	Slider_CTANK_DR_VDDIO
#else

    #define Slider_DEFAULT_IDAC_POLARITY   	(0u)
	#define Slider_CMOD_DR_CONFIG          	Slider_CMOD_DR_VSSIO	
	#define Slider_CTANK_DR_CONFIG         	Slider_CTANK_DR_VSSIO 
#endif /* (Slider_IDAC_OPTIONS == Slider_IDAC_SINK) */

#define Slider_SNS_GROUND_CONNECT          	(6u)
#define Slider_SNS_HIZANALOG_CONNECT       	(0u)

/* Inactive sensors connection configuration */
#if (Slider_CONNECT_INACTIVE_SNS == Slider__GROUND)
    #define Slider_CSD_INACTIVE_CONNECT       (Slider_SNS_GROUND_CONNECT)
#else
    #define Slider_CSD_INACTIVE_CONNECT       (Slider_SNS_HIZANALOG_CONNECT)
#endif /* Slider_CONNECT_INACTIVE_SNS == Slider__GROUND */

#if(Slider_IS_SHIELD_ENABLE)
    #define Slider_SHIELD_PORT_NUMBER Slider_Shield__PORT
    #define Slider_SHIELD_PIN_NUMBER  Slider_Shield__SHIFT
#endif /* (Slider_IS_SHIELD_ENABLE) */

/* Defining default CSD configuration according to settings in customizer. */
#define Slider_DEFAULT_CSD_CONFIG             (Slider_DEFAULT_FILTER_STATE |\
														 Slider_DEFAULT_MODULATION_MODE |\
                                                         Slider_CSD_CFG_SENSE_COMP_BW |\
                                                         Slider_DEFAULT_IDAC_POLARITY |\
                                                         Slider_CSD_CFG_REFBUF_OUTSEL |\
                                                         Slider_CSD_REFBUF_DRV_HIGH)

#define Slider_CSD_CONFIG_MASK				(Slider_CSD_CFG_FILTER_EN |\
														 Slider_CSD_CFG_PRS_SELECT |\
														 Slider_CSD_CFG_PRS_12_8 |\
														 Slider_CSD_CFG_SENSE_COMP_BW |\
														 Slider_CSD_CFG_REFBUF_EN |\
														 Slider_CSD_CFG_REFBUF_OUTSEL |\
														 Slider_CSD_CFG_POLARITY_VDDIO |\
														 Slider_CSD_CFG_REFBUF_OUTSEL  |\
														 Slider_CSD_REFBUF_DRV_MASK) 


/* Defining the default IDACs configuration according to settings in customizer. */
#if (Slider_IDAC_CNT == 1u)
	#define Slider_DEFAULT_CSD_IDAC_CONFIG    (Slider_CSD_IDAC1_MODE_VARIABLE |\
	                                                         Slider_DEFAULT_IDAC1_RANGE)
#else
	#define Slider_DEFAULT_CSD_IDAC_CONFIG    (Slider_CSD_IDAC1_MODE_VARIABLE |\
	                                                         Slider_CSD_IDAC2_MODE_FIXED |\
	                                                         Slider_DEFAULT_IDAC1_RANGE |\
	                                                         Slider_DEFAULT_IDAC2_RANGE)
#endif /* (Slider_IDAC_CNT == 1u) */

/* Defining mask intended for clearing bits related to pre-charging options. */
#define Slider_PRECHARGE_CONFIG_MASK			(Slider_CSD_CFG_REFBUF_EN |\
														 Slider_CSD_CFG_CHARGE_IO |\
														 Slider_CSD_CFG_COMP_PIN_CH2  |\
														 Slider_CSD_CFG_REFBUF_OUTSEL)

/* Cmod precharge mode configuration */
#if(Slider_CMOD_PREGARGE_OPTION == Slider__CAPPRVREF)

	#define Slider_CMOD_PRECHARGE_CONFIG 		(Slider_CSD_CFG_REFBUF_EN |\
														 Slider_CSD_CFG_COMP_PIN_CH2)
														 
#else

	#define Slider_CMOD_PRECHARGE_CONFIG 		(Slider_CSD_CFG_REFBUF_OUTSEL |\
														 Slider_CSD_CFG_REFBUF_EN |\
														 Slider_CSD_CFG_CHARGE_IO)
														 
#endif /* (Slider_CMOD_PREGARGE_OPTION == Slider__CAPPRVREF) */

/* Ctank precharge mode configuration */
#if(Slider_CSH_TANK_PREGARGE_OPTION == Slider__CAPPRVREF)
		
	#if(0u != Slider_IS_SHIELD_ENABLE)
		#define  Slider_CTANK_PRECHARGE_CONFIG	(Slider_CSD_CFG_REFBUF_EN |\
															 Slider_CSD_CFG_PRS_CLEAR |\
															 Slider_CSD_CFG_REFBUF_OUTSEL)
	#else
		#define  Slider_CTANK_PRECHARGE_CONFIG	(Slider_CSD_CFG_REFBUF_OUTSEL |\
															 Slider_CSD_CFG_PRS_CLEAR)
	#endif /* (0u != Slider_IS_SHIELD_ENABLE) */
	
#else

	#define  Slider_CTANK_PRECHARGE_CONFIG	(Slider_CSD_CFG_REFBUF_OUTSEL |\
														 Slider_CSD_CFG_REFBUF_EN |\
														 Slider_CSD_CFG_CHARGE_IO |\
														 Slider_CSD_CFG_PRS_CLEAR |\
														 Slider_CSD_CFG_COMP_PIN_CH2)
#endif /* (Slider_CSH_TANK_PREGARGE_OPTION == Slider__CAPPRIOBUF) */

#define Slider_MAX_UINT_8                     (0xFFu)
#define Slider_MAX_UINT_16                    (0xFFFFu)
#define Slider_MAX_UINT_32                    (0xFFFFFFFFLu)

/***************************************
*     Vars with External Linkage
***************************************/
/* SmartSense functions */
#if (Slider_TUNING_METHOD == Slider__TUNING_AUTO)
    extern uint8 Slider_lowLevelTuningDone;
    extern uint8 scanSpeedTbl[((Slider_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u];
    extern void Slider_AutoTune(void);
#endif /* (Slider_TUNING_METHOD == Slider__TUNING_AUTO) */

#if(Slider_PRS_OPTIONS != Slider__PRS_NONE)
    extern uint8 Slider_prescalersTuningDone;
#endif /* (Slider_PRS_OPTIONS == Slider__PRS_NONE) */

/* Global software variables */
extern volatile uint8 Slider_csdStatusVar;
extern volatile uint8 Slider_sensorIndex;
extern uint16 Slider_sensorRaw[Slider_TOTAL_SENSOR_COUNT];
extern uint8 Slider_unscannedSnsDriveMode[Slider_TOTAL_SENSOR_COUNT];
extern uint8 Slider_sensorEnableMaskBackup[Slider_TOTAL_SENSOR_MASK];
extern uint8 Slider_sensorEnableMask[Slider_TOTAL_SENSOR_MASK];

#if ((Slider_TUNING_METHOD != Slider__TUNING_NONE) || (0u != Slider_AUTOCALIBRATION_ENABLE))
	extern uint8 Slider_modulationIDAC[Slider_TOTAL_SENSOR_COUNT];
	extern uint8 Slider_compensationIDAC[Slider_TOTAL_SENSOR_COUNT];
#else
	extern const uint8 Slider_modulationIDAC[Slider_TOTAL_SENSOR_COUNT];
	extern const uint8 Slider_compensationIDAC[Slider_TOTAL_SENSOR_COUNT];
#endif /* ((Slider_TUNING_METHOD != Slider__TUNING_NONE) || (0u != Slider_AUTOCALIBRATION_ENABLE)) */

#if (Slider_TUNING_METHOD != Slider__TUNING_NONE)
	extern uint32 Slider_widgetResolution[Slider_RESOLUTIONS_TBL_SIZE];
	#if (0u != Slider_MULTIPLE_FREQUENCY_SET)
		extern uint8 Slider_senseClkDividerVal[Slider_TOTAL_SCANSLOT_COUNT];
		extern uint8 Slider_sampleClkDividerVal[Slider_TOTAL_SCANSLOT_COUNT];
	#else
		extern uint8 Slider_senseClkDividerVal;
		extern uint8 Slider_sampleClkDividerVal;
	#endif /* (0u != Slider_MULTIPLE_FREQUENCY_SET) */
#else
	extern const uint32 Slider_widgetResolution[Slider_RESOLUTIONS_TBL_SIZE];
	#if (0u != Slider_MULTIPLE_FREQUENCY_SET)
		extern const uint8 Slider_senseClkDividerVal[Slider_TOTAL_SCANSLOT_COUNT];
		extern const uint8 Slider_sampleClkDividerVal[Slider_TOTAL_SCANSLOT_COUNT];
	#else
		extern const uint8 Slider_senseClkDividerVal;
		extern const uint8 Slider_sampleClkDividerVal;
	#endif /* (0u != Slider_MULTIPLE_FREQUENCY_SET) */	
#endif /* (Slider_TUNING_METHOD != Slider__TUNING_NONE)  */

extern const uint8 Slider_widgetNumber[Slider_TOTAL_SENSOR_COUNT];

extern reg32* const Slider_prtSelTbl[Slider_CFG_REG_TBL_SIZE];
extern reg32* const Slider_prtCfgTbl[Slider_CFG_REG_TBL_SIZE];
extern reg32* const Slider_prtDRTbl[Slider_CFG_REG_TBL_SIZE];
extern reg32 * Slider_pcTable[Slider_PORT_PIN_CONFIG_TBL_ZISE];

extern const uint8  Slider_portTable[Slider_PORT_PIN_CONFIG_TBL_ZISE];
extern const uint32 Slider_maskTable[Slider_PORT_PIN_CONFIG_TBL_ZISE];
extern const uint8  Slider_pinShiftTbl[Slider_PORT_PIN_CONFIG_TBL_ZISE];

#if (0u != Slider_INDEX_TABLE_SIZE)
extern const uint8 CYCODE Slider_indexTable[Slider_INDEX_TABLE_SIZE];
#endif /* (0u != Slider_INDEX_TABLE_SIZE)) */

/***************************************************
*    Obsolete Names (will be removed in future)
***************************************************/
#define  Slider_PrescalersTuningDone         Slider_prescalersTuningDone
#define  Slider_SensorRaw	                   Slider_sensorRaw
#define  Slider_PRSResolutionTbl             Slider_prsResolutionTbl
#define  Slider_SensorEnableMask             Slider_sensorEnableMask
#define  Slider_Clk1DividerVal               Slider_senseClkDividerVal
#define  Slider_Clk2DividerVal               Slider_sampleClkDividerVal
#define  Slider_PrtSelTbl                    Slider_prtSelTbl
#define  Slider_PrtCfgTbl                    Slider_prtCfgTbl
#define  Slider_PrtDRTbl                     Slider_prtDRTbl
#define  Slider_idac1Settings				   Slider_modulationIDAC	
#define  Slider_idac2Settings                Slider_compensationIDAC

#endif /* CY_CAPSENSE_CSD_Slider_H */


 /* [] END OF FILE */
